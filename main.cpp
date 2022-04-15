#include <QGuiApplication>
#include <QtQuick/QQuickView>
#include "viewmodel.h"

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);

    qmlRegisterType<ViewModel>("PbrRender", 0, 5, "ViewModel");

    QQuickView view;
    view.setResizeMode(QQuickView::SizeRootObjectToView);
    view.setSource(QUrl("qrc:/main.qml"));
//    view.rootContext()->setContextProperty("CurDirPath", QString(QDir::currentPath()));

    view.show();

    return app.exec();
}
