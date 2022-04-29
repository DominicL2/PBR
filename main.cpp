#include <QGuiApplication>
#include <QtQuick/QQuickView>
#include <QObject>
#include "viewmodel.h"

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);

    qmlRegisterType<ViewModel>("PbrRender", 0, 5, "ViewModel");

    QQuickView view;
    view.setResizeMode(QQuickView::SizeViewToRootObject);
    view.setMinimumSize(QSize(1280, 720));
    view.setMaximumSize(QSize(1280, 720));
    view.setTitle("DML Graphcis Viewer");
    view.setSource(QUrl("qrc:/main.qml"));

    view.show();

    return app.exec();
}
