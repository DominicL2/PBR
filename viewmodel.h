#ifndef VIEWMODEL_H
#define VIEWMODEL_H

#include <QObject>
#include <QtQuick/QQuickItem>
#include <QtGlobal>
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

    Q_INVOKABLE void setViewport(QVariant x, QVariant y, QVariant width, QVariant height);

signals :
    void swapChanged();

public slots :
   void sync();

private slots :
    void handleWindowChanged(QQuickWindow *quickWindow);

private :
    qreal mSwap;
    GLRenderer *glRenderer;
    GLSpace::Rectangle mViewport;
};


#endif // VIEWMODEL_H
