#include "messagestore.h"

#include <QSqlQuery>
#include <QSqlError>
#include <QFileInfo>
#include <QDir>
#include <QUuid>
#include <QLoggingCategory>

Q_LOGGING_CATEGORY(lcStore, "whisper.store")

MessageStore::MessageStore(QObject *parent)
    : QObject(parent)
{
    m_connName = QStringLiteral("whisper-store-%1")
                 .arg(QUuid::createUuid().toString(QUuid::WithoutBraces));
}

MessageStore::~MessageStore()
{
    close();
}

bool MessageStore::open(const QString &dbPath)
{
    // Make sure the parent directory exists.
    QFileInfo fi(dbPath);
    QDir().mkpath(fi.absolutePath());

    m_db = QSqlDatabase::addDatabase(QStringLiteral("QSQLITE"), m_connName);
    m_db.setDatabaseName(dbPath);
    if (!m_db.open()) {
        qCWarning(lcStore) << "Failed to open SQLite at" << dbPath
                           << ":" << m_db.lastError().text();
        return false;
    }

    // Enforce foreign keys and reasonable durability without sacrificing UX.
    QSqlQuery pragma(m_db);
    pragma.exec(QStringLiteral("PRAGMA foreign_keys = ON"));
    pragma.exec(QStringLiteral("PRAGMA journal_mode = WAL"));
    pragma.exec(QStringLiteral("PRAGMA synchronous = NORMAL"));

    return ensureSchema();
}

void MessageStore::close()
{
    if (m_db.isOpen()) m_db.close();
    // Drop the local handle BEFORE removeDatabase, otherwise Qt warns
    // "connection ... is still in use".
    m_db = QSqlDatabase();
    QSqlDatabase::removeDatabase(m_connName);
}

bool MessageStore::ensureSchema()
{
    QSqlQuery q(m_db);
    const QString create = QStringLiteral(
        "CREATE TABLE IF NOT EXISTS messages ("
        "  id        INTEGER PRIMARY KEY AUTOINCREMENT,"
        "  peer      TEXT    NOT NULL,"
        "  direction INTEGER NOT NULL,"
        "  text      TEXT    NOT NULL,"
        "  ts        INTEGER NOT NULL"
        ")");
    if (!q.exec(create)) {
        qCWarning(lcStore) << "schema create failed:" << q.lastError().text();
        return false;
    }
    if (!q.exec(QStringLiteral("CREATE INDEX IF NOT EXISTS idx_messages_peer_ts "
                               "ON messages(peer, ts)"))) {
        qCWarning(lcStore) << "index create failed:" << q.lastError().text();
        return false;
    }
    return true;
}

qint64 MessageStore::insertMessage(const QString &peer, bool incoming,
                                   const QString &text, qint64 timestampMs)
{
    if (!m_db.isOpen()) return -1;

    m_db.transaction();
    QSqlQuery q(m_db);
    q.prepare(QStringLiteral(
        "INSERT INTO messages(peer, direction, text, ts) "
        "VALUES (:peer, :dir, :text, :ts)"));
    q.bindValue(QStringLiteral(":peer"), peer);
    q.bindValue(QStringLiteral(":dir"),  incoming ? 1 : 0);
    q.bindValue(QStringLiteral(":text"), text);
    q.bindValue(QStringLiteral(":ts"),   timestampMs);
    if (!q.exec()) {
        qCWarning(lcStore) << "insert failed:" << q.lastError().text();
        m_db.rollback();
        return -1;
    }
    qint64 newId = q.lastInsertId().toLongLong();
    m_db.commit();
    return newId;
}

QVector<ChatMessage> MessageStore::loadConversation(const QString &peer, int limit)
{
    QVector<ChatMessage> out;
    if (!m_db.isOpen()) return out;

    QSqlQuery q(m_db);
    q.prepare(QStringLiteral(
        "SELECT id, peer, direction, text, ts FROM messages "
        "WHERE peer = :peer ORDER BY ts ASC, id ASC LIMIT :lim"));
    q.bindValue(QStringLiteral(":peer"), peer);
    q.bindValue(QStringLiteral(":lim"),  limit);
    if (!q.exec()) {
        qCWarning(lcStore) << "load failed:" << q.lastError().text();
        return out;
    }
    while (q.next()) {
        ChatMessage m;
        m.id        = q.value(0).toLongLong();
        m.peer      = q.value(1).toString();
        m.incoming  = (q.value(2).toInt() == 1);
        m.text      = q.value(3).toString();
        m.timestamp = q.value(4).toLongLong();
        out.append(m);
    }
    return out;
}

QStringList MessageStore::knownPeers()
{
    QStringList out;
    if (!m_db.isOpen()) return out;
    QSqlQuery q(m_db);
    if (!q.exec(QStringLiteral(
            "SELECT DISTINCT peer FROM messages ORDER BY peer ASC"))) {
        qCWarning(lcStore) << "knownPeers failed:" << q.lastError().text();
        return out;
    }
    while (q.next())
        out.append(q.value(0).toString());
    return out;
}

bool MessageStore::clearAll()
{
    if (!m_db.isOpen()) return false;
    QSqlQuery q(m_db);
    return q.exec(QStringLiteral("DELETE FROM messages"));
}
