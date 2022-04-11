#ifndef VIEWMODEL_H
#define VIEWMODEL_H

#include <QObject>
#include <QtQuick/QQuickItem>
#include "GLRenderer.h"

class ViewModel : public QQuickItem
{
    Q_OBJECT
    Q_PROPERTY(qreal swap READ swap WRITE setSwap NOTIFY swapChanged)

public:
    ViewModel();
    ~ViewModel();

    qreal swap() const {return mSwap;}
    void setSwap(qreal swap);

signals :
    void swapChanged();

public slots :
   void sync();

private slots :
    void handleWindowChanged(QQuickWindow *quickWindow);

private :
    qreal mSwap;
    GLRenderer *glRenderer;
};


#endif // VIEWMODEL_H
