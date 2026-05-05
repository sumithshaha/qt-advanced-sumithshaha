#pragma once

#include <QObject>
#include <QSqlDatabase>
#include <QString>
#include <QVector>

#include "messagemodel.h"

/**
 * MessageStore — persists chat history to SQLite.
 *
 * Schema (created on open if missing):
 *   CREATE TABLE messages (
 *       id        INTEGER PRIMARY KEY AUTOINCREMENT,
 *       peer      TEXT NOT NULL,
 *       direction INTEGER NOT NULL,   -- 0=outgoing, 1=incoming
 *       text      TEXT NOT NULL,
 *       ts        INTEGER NOT NULL    -- unix milliseconds
 *   );
 *   CREATE INDEX IF NOT EXISTS idx_messages_peer_ts ON messages(peer, ts);
 *
 * All writes are transactional. All queries use bound parameters (no string
 * concatenation) to defeat SQL injection at the API surface.
 */
class MessageStore : public QObject
{
    Q_OBJECT

public:
    explicit MessageStore(QObject *parent = nullptr);
    ~MessageStore() override;

    bool open(const QString &dbPath);
    void close();

    // Returns the autoincrement id assigned to the inserted row, or -1.
    qint64 insertMessage(const QString &peer, bool incoming,
                         const QString &text, qint64 timestampMs);

    QVector<ChatMessage> loadConversation(const QString &peer, int limit = 200);

    QStringList knownPeers();

    bool clearAll(); // for "logout & wipe"

private:
    bool ensureSchema();

    QSqlDatabase m_db;
    QString      m_connName;
};
