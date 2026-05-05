#include "settings.h"

Settings::Settings(QObject *parent)
    : QObject(parent)
    , m_s(QStringLiteral("Metropolia.TT00GD57"), QStringLiteral("Whisper"))
{
}

QString Settings::lastNickname() const
{
    return m_s.value(QStringLiteral("nickname")).toString();
}

void Settings::setLastNickname(const QString &nick)
{
    if (lastNickname() == nick) return;
    m_s.setValue(QStringLiteral("nickname"), nick);
    emit changed();
}

QString Settings::lastServerUrl() const
{
    return m_s.value(QStringLiteral("serverUrl"),
                     QStringLiteral("wss://10.0.2.2:8765")).toString();
}

void Settings::setLastServerUrl(const QString &url)
{
    if (lastServerUrl() == url) return;
    m_s.setValue(QStringLiteral("serverUrl"), url);
    emit changed();
}

bool Settings::darkMode() const
{
    return m_s.value(QStringLiteral("darkMode"), true).toBool();
}

void Settings::setDarkMode(bool on)
{
    if (darkMode() == on) return;
    m_s.setValue(QStringLiteral("darkMode"), on);
    emit changed();
}
