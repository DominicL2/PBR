#include <QGuiApplication>
//#include <QQmlApplicationEngine>
#include <QtQuick/QQuickView>
#include "viewmodel.h"

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);

    qmlRegisterType<ViewModel>("PbrRender", 1, 0, "ViewModel");
#if 0
    QQmlApplicationEngine engine;
    engine.load(QUrl(QStringLiteral("qrc:/main.qml")));
#endif
    QQuickView view;
    view.setResizeMode(QQuickView::SizeRootObjectToView);
    view.setSource(QUrl("qrc:/main.qml"));
    view.show();

    return app.exec();
}
