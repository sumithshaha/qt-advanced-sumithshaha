#pragma once

#include <QObject>
#include <QString>
#include <QSettings>

#ifdef WHISPER_QML_BUILD
#include <QtQmlIntegration>
#endif

/**
 * Settings — thin QSettings wrapper for persisting:
 *   - last-used nickname
 *   - last-used server URL
 *   - dark mode preference
 *
 * Crypto keys are NOT persisted here. We regenerate identity each session
 * for simplicity (Signal-style long-term identity persistence is a future
 * upgrade noted in the report).
 */
class Settings : public QObject
{
    Q_OBJECT
#ifdef WHISPER_QML_BUILD
    QML_ELEMENT
    QML_SINGLETON
#endif
    Q_PROPERTY(QString lastNickname READ lastNickname WRITE setLastNickname NOTIFY changed)
    Q_PROPERTY(QString lastServerUrl READ lastServerUrl WRITE setLastServerUrl NOTIFY changed)
    Q_PROPERTY(bool darkMode READ darkMode WRITE setDarkMode NOTIFY changed)

public:
    explicit Settings(QObject *parent = nullptr);

    QString lastNickname() const;
    void setLastNickname(const QString &nick);

    QString lastServerUrl() const;
    void setLastServerUrl(const QString &url);

    bool darkMode() const;
    void setDarkMode(bool on);

signals:
    void changed();

private:
    QSettings m_s;
};
