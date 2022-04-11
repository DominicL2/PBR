#ifndef MODEL_GLRENDER_H
#define MODEL_GLRENDER_H

#include <QtGui/QOpenGLFunctions>
#include <QtQuick/qquickwindow.h>
#include <QQuickItem>
#include <QSize>
#include <vector>
#include "ModelLoader.h"
#include "gldefine.h"
#include "pbrshader.h"
#include "spacedata.h"

#define GL_RENDERER_SUCCESS   (1)
#define GL_RENDERER_FAIL      (0)

using namespace std;

class GLRenderer : public QObject, protected QOpenGLFunctions
{
    Q_OBJECT
public:
    GLRenderer();
    int32_t init();
    int32_t load(string path);

    void locateLightSource(glm::vec3 pos);
    void locateViewPoint(glm::vec3 pos);

    inline void setViewPortsize(const QSize &size) {
        mViewPort = size;
    }

    inline void setWindow(QQuickWindow *window) {
        m_window = window;
    }

    bool isLoadded() {
        return mLoadded;
    }

signals:

public slots:
    void paint();

private :
    GLuint registerShader(const string text, uint32_t type);
    int32_t connectShader2Program();
    int32_t registerAttribute(SHADER_TYPE type);
    int32_t registerUniform(SHADER_TYPE type);
    int32_t createContext();
    void checkShaderError(GLuint shader, GLuint flag, bool isProgram, const string &errMsg);
    bool mLoadded;

    SHADER_TYPE         mType;
    ModelLoader         mModelLoader;
    ModelData           *mModelData;
    GLRendererContext   mContext;
    GLSpace::SpaceInfo  mSpaceInfo;
    QSize               mViewPort;
    QQuickWindow        *m_window;
};

#endif // MODEL_GLRENDER_H
