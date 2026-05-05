#include "messagemodel.h"

MessageModel::MessageModel(QObject *parent)
    : QAbstractListModel(parent)
{
}

int MessageModel::rowCount(const QModelIndex &parent) const
{
    if (parent.isValid()) return 0;
    return m_rows.size();
}

QVariant MessageModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid() || index.row() < 0 || index.row() >= m_rows.size())
        return {};
    const ChatMessage &m = m_rows.at(index.row());
    switch (role) {
    case IdRole:        return QVariant::fromValue(m.id);
    case PeerRole:      return m.peer;
    case TextRole:      return m.text;
    case TimestampRole: return QVariant::fromValue(m.timestamp);
    case IncomingRole:  return m.incoming;
    default:            return {};
    }
}

QHash<int, QByteArray> MessageModel::roleNames() const
{
    return {
        { IdRole,        "id"        },
        { PeerRole,      "peer"      },
        { TextRole,      "text"      },
        { TimestampRole, "timestamp" },
        { IncomingRole,  "incoming"  },
    };
}

void MessageModel::setActivePeer(const QString &peer)
{
    if (m_activePeer == peer) return;
    m_activePeer = peer;
    emit activePeerChanged();
}

void MessageModel::setMessages(const QVector<ChatMessage> &msgs)
{
    beginResetModel();
    m_rows = msgs;
    endResetModel();
    emit countChanged();
}

void MessageModel::appendMessage(const ChatMessage &msg)
{
    // Only append to the model if it's relevant to the active peer.
    if (msg.peer != m_activePeer) return;
    beginInsertRows(QModelIndex(), m_rows.size(), m_rows.size());
    m_rows.append(msg);
    endInsertRows();
    emit countChanged();
}

void MessageModel::clear()
{
    if (m_rows.isEmpty()) return;
    beginResetModel();
    m_rows.clear();
    endResetModel();
    emit countChanged();
}
