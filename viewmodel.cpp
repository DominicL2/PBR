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
        connect(quickWindow, &QQuickWindow::beforeSynchronizing, this, &ViewModel::sync, Qt::DirectConnection);
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
    glRenderer->setViewPortsize(QSize(1280, 720));
    // Set View port
    // Set Swap?
    // Set Winodw
}
