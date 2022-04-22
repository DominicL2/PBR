#ifndef MODEL_GLRENDER_H
#define MODEL_GLRENDER_H

#include <QtGui/QOpenGLFunctions>
#include <QtQuick/qquickwindow.h>
#include <QQuickItem>
#include <QSize>
#include <vector>
#include <unordered_map>
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
    ~GLRenderer();
    int32_t init();
    int32_t load(string path);

    void locateLightSource(glm::vec3 pos);
    void locateViewPoint(glm::vec3 pos);

    void setViewPortsize(GLSpace::Rectangle rect);

    inline void setWindow(QQuickWindow *window) {
        m_window = window;
    }

    bool isLoadded() {
        return mContextCreated;
    }

    inline glm::vec3 getViewPos() {
        return mSpaceInfo.viewPoint;
    }
    inline glm::vec3 getLightPos() {
        return mSpaceInfo.lightSource;
    }
    inline glm::vec3 getModelRotation() {
        return mModelRatation;
    }

    inline void setViewPos(glm::vec3 pos) {
        mSpaceInfo.viewPoint = pos;
    }
    inline void setLightPos(glm::vec3 pos) {
        mSpaceInfo.lightSource = pos;
    }
    inline void setModelRotation(glm::vec3 pos){
         mModelRatation = pos;
    }

    void setMaterial(string materialName);
    void getMaterialList(vector<string> *materialList);

    void setAmbient(glm::vec3 pos);
    void setDiffuse(glm::vec3 pos);
    void setSpecular(glm::vec3 pos);

    void setShiness(float val);
    float getShiness();

    glm::vec3 getAmbient();
    glm::vec3 getDiffuse();
    glm::vec3 getSpecular();
    bool loadded() const;
    void setLoadded(bool loadded);

signals:
    void sigMeshInfo(string info);
public slots:
    void paint();

private :
    GLuint registerShader(const string text, uint32_t type);
    int32_t connectShader2Program();
    int32_t registerAttribute(SHADER_TYPE type);
    int32_t registerUniform(SHADER_TYPE type);
    int32_t createContext();
    void checkShaderError(GLuint shader, GLuint flag, bool isProgram, const string &errMsg);
    void draw(const ModelData *modelData);

    bool mContextCreated;
    bool mModelLoadded;

    SHADER_TYPE         mType;
    ModelLoader         mModelLoader;
    vector<ModelData>   mModelList;
    GLRendererContext   mContext;
    GLSpace::SpaceInfo  mSpaceInfo;
    GLSpace::Rectangle  mViewportInfo;
    glm::vec3           mModelRatation;
    glm::vec3           mLengthAll;
    glm::vec3           mScale;
    QQuickWindow        *m_window;

    vector<string>                  mMaterialList;
    unordered_map<string, uint32_t> mMaterialMap;
    uint32_t                        mCurrMaterialIndex;
};

#endif // MODEL_GLRENDER_H
