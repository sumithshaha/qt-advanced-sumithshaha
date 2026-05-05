#include "usermodel.h"

UserModel::UserModel(QObject *parent)
    : QAbstractListModel(parent)
{
}

int UserModel::rowCount(const QModelIndex &parent) const
{
    if (parent.isValid()) return 0;
    return m_rows.size();
}

QVariant UserModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid() || index.row() < 0 || index.row() >= m_rows.size())
        return {};
    const UserInfo &u = m_rows.at(index.row());
    switch (role) {
    case NickRole:    return u.nick;
    case PubKeyRole:  return u.pubKeyB64;
    case OnlineRole:  return u.online;
    default:          return {};
    }
}

QHash<int, QByteArray> UserModel::roleNames() const
{
    return {
        { NickRole,   "nick"   },
        { PubKeyRole, "pubkey" },
        { OnlineRole, "online" },
    };
}

int UserModel::indexOf(const QString &nick) const
{
    for (int i = 0; i < m_rows.size(); ++i)
        if (m_rows.at(i).nick == nick) return i;
    return -1;
}

void UserModel::setUser(const UserInfo &info)
{
    int idx = indexOf(info.nick);
    if (idx >= 0) {
        m_rows[idx] = info;
        emit dataChanged(index(idx), index(idx),
                         { NickRole, PubKeyRole, OnlineRole });
    } else {
        beginInsertRows(QModelIndex(), m_rows.size(), m_rows.size());
        m_rows.append(info);
        endInsertRows();
        emit countChanged();
        emit userAdded(info.nick, info.pubKeyB64);
    }
}

void UserModel::removeUser(const QString &nick)
{
    int idx = indexOf(nick);
    if (idx < 0) return;
    // Don't actually remove — just mark offline. Lets users still see a peer
    // they were chatting with (greyed out).
    m_rows[idx].online = false;
    emit dataChanged(index(idx), index(idx), { OnlineRole });
}

void UserModel::clear()
{
    if (m_rows.isEmpty()) return;
    beginResetModel();
    m_rows.clear();
    endResetModel();
    emit countChanged();
}

QByteArray UserModel::pubKeyOf(const QString &nick) const
{
    int idx = indexOf(nick);
    return idx >= 0 ? m_rows.at(idx).pubKeyB64 : QByteArray();
}

bool UserModel::isOnline(const QString &nick) const
{
    int idx = indexOf(nick);
    return idx >= 0 ? m_rows.at(idx).online : false;
}
