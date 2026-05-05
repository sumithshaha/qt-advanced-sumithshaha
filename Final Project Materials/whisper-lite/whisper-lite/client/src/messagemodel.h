#pragma once

#include <QAbstractListModel>
#include <QString>
#include <QVector>
#include <QDateTime>

#ifdef WHISPER_QML_BUILD
#include <QtQmlIntegration>
#endif

struct ChatMessage {
    qint64    id        = 0;
    QString   peer;        // the other party (sender if incoming, recipient if outgoing)
    QString   text;
    qint64    timestamp = 0;  // unix ms
    bool      incoming  = false;
};

/**
 * MessageModel — list of chat messages for a single conversation.
 * Filtered by peer; switch peers via setActivePeer().
 *
 * Used directly as a QML ListView model:
 *
 *     ListView {
 *         model: ctrl.messages
 *         delegate: MessageDelegate {
 *             text: model.text
 *             incoming: model.incoming
 *             timestamp: model.timestamp
 *         }
 *     }
 */
class MessageModel : public QAbstractListModel
{
    Q_OBJECT
#ifdef WHISPER_QML_BUILD
    QML_ELEMENT
#endif
    Q_PROPERTY(QString activePeer READ activePeer WRITE setActivePeer NOTIFY activePeerChanged)
    Q_PROPERTY(int count READ rowCount NOTIFY countChanged)

public:
    enum Roles {
        IdRole = Qt::UserRole + 1,
        PeerRole,
        TextRole,
        TimestampRole,
        IncomingRole,
    };

    explicit MessageModel(QObject *parent = nullptr);

    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role) const override;
    QHash<int, QByteArray> roleNames() const override;

    QString activePeer() const { return m_activePeer; }
    void setActivePeer(const QString &peer);

    // Mutations
    void setMessages(const QVector<ChatMessage> &msgs);
    void appendMessage(const ChatMessage &msg);
    void clear();

signals:
    void activePeerChanged();
    void countChanged();

private:
    QString m_activePeer;
    QVector<ChatMessage> m_rows;
};
