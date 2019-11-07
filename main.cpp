#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QtQml>
#include "openglitem.h"
int main(int argc, char *argv[])
{
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    QGuiApplication app(argc, argv);
    app.setAttribute(Qt::AA_UseDesktopOpenGL);
    app.setOverrideCursor(Qt::BlankCursor);
    QSurfaceFormat format;
    format.setDepthBufferSize(32);
    format.setStencilBufferSize(8);
    format.setSamples(4);
    QSurfaceFormat::setDefaultFormat(format);
    qmlRegisterType<OpenGLItem>("OpenGLItem", 1, 0, "OpenGLItem");
    QQmlApplicationEngine engine;
    engine.load(QUrl(QStringLiteral("media/qml/MainWindow.qml")));
    return app.exec();
}
