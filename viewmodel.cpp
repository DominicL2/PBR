#include "viewmodel.h"
#include <QtQuick/qquickwindow.h>

ViewModel::ViewModel()
    : mSwap(0)
{
    connect(this, &QQuickItem::windowChanged, this, &ViewModel::handleWindowChanged);
}

ViewModel::~ViewModel()
{
    delete glRenderer;
}

void ViewModel::handleWindowChanged(QQuickWindow *quickWindow)
{
    if (quickWindow) {
        connect(quickWindow, &QQuickWindow::afterSynchronizing, this, &ViewModel::sync, Qt::DirectConnection);
    }

    quickWindow->setClearBeforeRendering(false);
}

void ViewModel::setSwap(qreal swap)
{
    if (swap != mSwap) {
        mSwap = swap;
        emit swapChanged();
        if (window()) {
            window()->update();
        }

    } else {}
}

void ViewModel::sync()
{
    if (!glRenderer) {
        glRenderer = new GLRenderer();
    } else {}

    connect(window(), &QQuickWindow::beforeRendering, glRenderer, &GLRenderer::paint, Qt::DirectConnection);
    glRenderer->setViewPortsize(mViewport);
}

void ViewModel::setViewport(QVariant x, QVariant y, QVariant width, QVariant height)
{
    mViewport = GLSpace::Rectangle(x.toInt(), y.toInt(), width.toInt(), height.toInt());
}
