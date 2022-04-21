#include <QGuiApplication>
#include <QtQuick/QQuickView>
#include <QObject>
#include "viewmodel.h"

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);

    qmlRegisterType<ViewModel>("PbrRender", 0, 5, "ViewModel");

    QQuickView view;
    view.setResizeMode(QQuickView::SizeRootObjectToView);
    view.setSource(QUrl("qrc:/main.qml"));

    view.show();

    return app.exec();
}
