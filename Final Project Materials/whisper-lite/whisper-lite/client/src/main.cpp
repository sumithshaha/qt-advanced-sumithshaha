#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQuickStyle>
#include <QLoggingCategory>
#include <QIcon>

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);
    QGuiApplication::setApplicationName(QStringLiteral("Whisper"));
    QGuiApplication::setOrganizationName(QStringLiteral("Metropolia.TT00GD57"));
    QGuiApplication::setApplicationVersion(QStringLiteral("1.0.0"));
    QGuiApplication::setWindowIcon(QIcon(QStringLiteral(":/icon.png")));

    QQuickStyle::setStyle(QStringLiteral("Material"));

    QLoggingCategory::setFilterRules(QStringLiteral(
        "whisper.*=true\n"
        "qt.qml.connections=false"));

    QQmlApplicationEngine engine;

    QObject::connect(&engine, &QQmlApplicationEngine::objectCreationFailed,
                     &app, []() { QCoreApplication::exit(-1); },
                     Qt::QueuedConnection);

    engine.loadFromModule(QStringLiteral("Whisper"), QStringLiteral("Main"));

    return app.exec();
}
