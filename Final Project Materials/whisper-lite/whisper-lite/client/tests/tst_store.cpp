// tst_store.cpp — verifies MessageStore persistence:
//   1. open() creates the schema in a fresh DB
//   2. insertMessage round-trips via loadConversation
//   3. Ordering is by timestamp ASC
//   4. Bound parameters defeat SQL injection (peer name with quotes)
//   5. knownPeers returns distinct peer list

#include <QTest>
#include <QTemporaryFile>
#include <QTemporaryDir>
#include <QDateTime>
#include "messagestore.h"

class TestStore : public QObject
{
    Q_OBJECT

private slots:
    void openCreatesSchema()
    {
        QTemporaryDir tmp;
        QVERIFY(tmp.isValid());
        const QString path = tmp.filePath(QStringLiteral("test1.db"));

        MessageStore s;
        QVERIFY(s.open(path));
        QVERIFY(QFile::exists(path));
    }

    void insertAndLoad()
    {
        QTemporaryDir tmp;
        const QString path = tmp.filePath(QStringLiteral("test2.db"));

        MessageStore s;
        QVERIFY(s.open(path));

        const qint64 t0 = QDateTime::currentMSecsSinceEpoch();
        QVERIFY(s.insertMessage(QStringLiteral("bob"), false,
                                QStringLiteral("hello"), t0)       > 0);
        QVERIFY(s.insertMessage(QStringLiteral("bob"), true,
                                QStringLiteral("hi back"), t0 + 100) > 0);
        QVERIFY(s.insertMessage(QStringLiteral("alice"), false,
                                QStringLiteral("private"), t0 + 200) > 0);

        const auto bobs = s.loadConversation(QStringLiteral("bob"));
        QCOMPARE(bobs.size(), 2);
        QCOMPARE(bobs.at(0).text, QStringLiteral("hello"));
        QCOMPARE(bobs.at(0).incoming, false);
        QCOMPARE(bobs.at(1).text, QStringLiteral("hi back"));
        QCOMPARE(bobs.at(1).incoming, true);

        const auto alices = s.loadConversation(QStringLiteral("alice"));
        QCOMPARE(alices.size(), 1);
    }

    void orderingIsByTimestamp()
    {
        QTemporaryDir tmp;
        MessageStore s;
        QVERIFY(s.open(tmp.filePath(QStringLiteral("t.db"))));

        // Insert out of timestamp order to make sure we sort, not insert-order
        s.insertMessage(QStringLiteral("p"), false, QStringLiteral("third"), 3000);
        s.insertMessage(QStringLiteral("p"), false, QStringLiteral("first"), 1000);
        s.insertMessage(QStringLiteral("p"), false, QStringLiteral("second"), 2000);

        const auto rows = s.loadConversation(QStringLiteral("p"));
        QCOMPARE(rows.size(), 3);
        QCOMPARE(rows.at(0).text, QStringLiteral("first"));
        QCOMPARE(rows.at(1).text, QStringLiteral("second"));
        QCOMPARE(rows.at(2).text, QStringLiteral("third"));
    }

    void sqlInjectionDefeated()
    {
        // If we'd built the SQL by string concatenation, this peer name
        // would let the attacker drop the table. Bound parameters mean
        // it's stored verbatim and queried verbatim.
        QTemporaryDir tmp;
        MessageStore s;
        QVERIFY(s.open(tmp.filePath(QStringLiteral("t.db"))));

        const QString evil = QStringLiteral("'; DROP TABLE messages; --");
        QVERIFY(s.insertMessage(evil, false, QStringLiteral("payload"), 100) > 0);

        // The table must still exist and the row must still be there.
        const auto rows = s.loadConversation(evil);
        QCOMPARE(rows.size(), 1);
        QCOMPARE(rows.at(0).text, QStringLiteral("payload"));

        // And a normal insert must still work.
        QVERIFY(s.insertMessage(QStringLiteral("safe"), false,
                                QStringLiteral("ok"), 200) > 0);
    }

    void knownPeers()
    {
        QTemporaryDir tmp;
        MessageStore s;
        QVERIFY(s.open(tmp.filePath(QStringLiteral("t.db"))));

        s.insertMessage(QStringLiteral("c"), false, QStringLiteral("x"), 1);
        s.insertMessage(QStringLiteral("a"), false, QStringLiteral("x"), 2);
        s.insertMessage(QStringLiteral("b"), false, QStringLiteral("x"), 3);
        s.insertMessage(QStringLiteral("a"), false, QStringLiteral("x"), 4);

        const auto peers = s.knownPeers();
        QCOMPARE(peers.size(), 3);
        QCOMPARE(peers.at(0), QStringLiteral("a"));
        QCOMPARE(peers.at(1), QStringLiteral("b"));
        QCOMPARE(peers.at(2), QStringLiteral("c"));
    }
};

QTEST_GUILESS_MAIN(TestStore)
#include "tst_store.moc"
