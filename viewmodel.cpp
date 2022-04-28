#include "viewmodel.h"
#include <QtQuick/qquickwindow.h>
#include <pthread.h>

ViewModel::ViewModel()
    : mSwap(0)
{
    connect(this, &QQuickItem::windowChanged, this, &ViewModel::handleWindowChanged);
    setLoadded(false);
}

ViewModel::~ViewModel()
{
    delete glRenderer;
}

void ViewModel::handleWindowChanged(QQuickWindow *quickWindow)
{
    if (quickWindow) {
        connect(quickWindow, &QQuickWindow::afterSynchronizing, this, &ViewModel::sync, Qt::DirectConnection);
        quickWindow->setClearBeforeRendering(false);
    } else {}
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
        connect(glRenderer, SIGNAL(sigMeshInfo(string)), this, SLOT(printMeshInfo(string)));
    } else {}
    connect(window(), &QQuickWindow::beforeRendering, glRenderer, &GLRenderer::paint, Qt::DirectConnection);
    glRenderer->setViewPortsize(mViewport);
}

void ViewModel::setViewport(QVariant x, QVariant y, QVariant width, QVariant height)
{
    mViewport = GLSpace::Rectangle(x.toInt(), y.toInt(), width.toInt(), height.toInt());
}

void ViewModel::loadGrpahicsModel(QVariant string)
{
    if (glRenderer->load(string.toString().toStdString()) == GL_RENDERER_SUCCESS) {
        emit sigModelLoadded();
        setLoadded(true);
    } else {}
}

QVector3D ViewModel::getViewPos()
{
    glm::vec3 pos = glRenderer->getViewPos();
    QVector3D ret;
    ret.setX(pos.x);
    ret.setY(pos.y);
    ret.setZ(pos.z);
    return ret;
}

QVector3D ViewModel::getLightPos()
{
    glm::vec3 pos = glRenderer->getLightPos();
    QVector3D ret;
    ret.setX(pos.x);
    ret.setY(pos.y);
    ret.setZ(pos.z);
    return ret;
}

QVector3D ViewModel::getModelRotationPos()
{
    glm::vec3 pos = glRenderer->getModelRotation();
    QVector3D ret;
    ret.setX(pos.x);
    ret.setY(pos.y);
    ret.setZ(pos.z);
    return ret;
}

QVector3D ViewModel::getAmbient()
{
    glm::vec3 pos = glRenderer->getAmbient();
    QVector3D ret;
    ret.setX(pos.x);
    ret.setY(pos.y);
    ret.setZ(pos.z);
    return ret;
}

QVector3D ViewModel::getDiffuse()
{
    glm::vec3 pos = glRenderer->getDiffuse();
    QVector3D ret;
    ret.setX(pos.x);
    ret.setY(pos.y);
    ret.setZ(pos.z);
    return ret;
}

QVector3D ViewModel::getSpecular()
{
    glm::vec3 pos = glRenderer->getSpecular();
    QVector3D ret;
    ret.setX(pos.x);
    ret.setY(pos.y);
    ret.setZ(pos.z);
    return ret;
}

void ViewModel::setViewPos(QVector3D pos)
{
    glRenderer->setViewPos(glm::vec3(pos.x(), pos.y(), pos.z()));
}

void ViewModel::setLightPos(QVector3D pos)
{
    glRenderer->setLightPos(glm::vec3(pos.x(), pos.y(), pos.z()));
}

void ViewModel::setModelRotation(QVector3D pos)
{
    glRenderer->setModelRotation(glm::vec3(pos.x(), pos.y(), pos.z()));
}

void ViewModel::setAmbient(QVector3D pos)
{
    glRenderer->setAmbient(glm::vec3(pos.x(), pos.y(), pos.z()));
}

void ViewModel::setDiffuse(QVector3D pos)
{
    glRenderer->setDiffuse(glm::vec3(pos.x(), pos.y(), pos.z()));
}

void ViewModel::setSpecular(QVector3D pos)
{
    glRenderer->setSpecular(glm::vec3(pos.x(), pos.y(), pos.z()));
}

void ViewModel::setShiness(qreal val)
{
    glRenderer->setShiness(val);
}

void ViewModel::setCurrentMaterial(QString name)
{
    glRenderer->setMaterial(name.toStdString());
    emit sigMaterialChanged();
}

QStringList ViewModel::getMaterialList()
{
    QStringList  list;
    vector<string> tmpList;
    glRenderer->getMaterialList(&tmpList);
    for (size_t i = 0; i < tmpList.size(); i++) {
        list.push_back(QString(tmpList[i].c_str()));
    }
    return list;
}

qreal ViewModel::getShiness()
{
    return glRenderer->getShiness();
}

void ViewModel::printMeshInfo(string info) {
    QString str = QString::fromStdString(info);
    setLogMsg(str);
}

void ViewModel::setShaderType(int type)
{
    glRenderer->setShdaerType((SHADER_TYPE)type);
    emit sigMaterialChanged();
}
