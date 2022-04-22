#ifndef VIEWMODEL_H
#define VIEWMODEL_H

#include <QObject>
#include <QtQuick/QQuickItem>
#include <QtGlobal>
#include <QVector3D>
#include "GLRenderer.h"

class ViewModel : public QQuickItem
{
    Q_OBJECT
    Q_PROPERTY(qreal swap READ swap WRITE setSwap NOTIFY swapChanged)
    Q_PROPERTY(QString logMsg READ getLogMsg WRITE setLogMsg NOTIFY logMsgChanged)
    Q_PROPERTY(bool loadingFlag READ getLoadingFlag WRITE setLoadingFlag NOTIFY loadingFlagChanged)

public:
    ViewModel();
    ~ViewModel();

    /* dynamic property */
    qreal swap() const {return mSwap;}
    void setSwap(qreal swap);

    void setLogMsg(QString str) {
        mLogMsg = str;
        emit logMsgChanged();
    }

    QString getLogMsg() const {
        return mLogMsg;
    }

    void setLoadingFlag(bool isLoading) {
        mIsLoading = isLoading;
        emit loadingFlagChanged();
    }

    bool getLoadingFlag() {
        return mIsLoading;
    }

    /* Invoke method */
    Q_INVOKABLE void setViewport(QVariant x, QVariant y, QVariant width, QVariant height);
    Q_INVOKABLE void loadGrpahicsModel(QVariant string);
    Q_INVOKABLE QVector3D getViewPos();
    Q_INVOKABLE QVector3D getLightPos();
    Q_INVOKABLE QVector3D getModelRotationPos();
    Q_INVOKABLE void setViewPos(QVector3D pos);
    Q_INVOKABLE void setLightPos(QVector3D pos);
    Q_INVOKABLE void setModelRotation(QVector3D pos);

    Q_INVOKABLE QVector3D getAmbient();
    Q_INVOKABLE QVector3D getDiffuse();
    Q_INVOKABLE QVector3D getSpecular();

    Q_INVOKABLE void setAmbient(QVector3D pos);
    Q_INVOKABLE void setDiffuse(QVector3D pos);
    Q_INVOKABLE void setSpecular(QVector3D pos);

    Q_INVOKABLE qreal getShiness();
    Q_INVOKABLE void setShiness(qreal pos);

    Q_INVOKABLE void setCurrentMaterial(QString name);
    Q_INVOKABLE QStringList getMaterialList();

signals :
    void sigModelLoadded();
    void sigMaterialChanged();

    void swapChanged();
    void logMsgChanged();
    void loadingFlagChanged();

public slots :
   void sync();
   void printMeshInfo(string info);

private slots :
    void handleWindowChanged(QQuickWindow *quickWindow);

private :
    qreal mSwap;
    QString mLogMsg;
    bool    mIsLoading;
    GLRenderer *glRenderer;
    GLSpace::Rectangle mViewport;
};


#endif // VIEWMODEL_H
