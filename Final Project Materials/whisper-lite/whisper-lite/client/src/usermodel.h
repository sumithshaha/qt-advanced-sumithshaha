#pragma once

#include <QAbstractListModel>
#include <QString>
#include <QVector>
#include <QByteArray>

#ifdef WHISPER_QML_BUILD
#include <QtQmlIntegration>
#endif

struct UserInfo {
    QString    nick;
    QByteArray pubKeyB64;
    bool       online = false;
};

/**
 * UserModel — the online user list. Updated by ChatController in response
 * to 'roster' (initial list) and 'presence' (incremental) server messages.
 */
class UserModel : public QAbstractListModel
{
    Q_OBJECT
#ifdef WHISPER_QML_BUILD
    QML_ELEMENT
#endif
    Q_PROPERTY(int count READ rowCount NOTIFY countChanged)

public:
    enum Roles {
        NickRole = Qt::UserRole + 1,
        PubKeyRole,
        OnlineRole,
    };

    explicit UserModel(QObject *parent = nullptr);

    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role) const override;
    QHash<int, QByteArray> roleNames() const override;

    // Mutations
    void setUser(const UserInfo &info);
    void removeUser(const QString &nick);
    void clear();

    Q_INVOKABLE QByteArray pubKeyOf(const QString &nick) const;
    Q_INVOKABLE bool isOnline(const QString &nick) const;

signals:
    void countChanged();
    void userAdded(const QString &nick, const QByteArray &pubKeyB64);

private:
    int indexOf(const QString &nick) const;
    QVector<UserInfo> m_rows;
};
