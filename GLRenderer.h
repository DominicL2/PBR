/**
 * @file GLRenderer.h
 *
 * @brief draw 3D graphics model using model manager
 *
 * @author DM Lee
 * Contact: kwoo2007@naver.com
 *
 */
#ifndef MODEL_GLRENDER_H
#define MODEL_GLRENDER_H

#include <QtGui/QOpenGLFunctions>
#include <QtQuick/qquickwindow.h>
#include <QQuickItem>
#include <QSize>
#include <vector>
#include <map>
#include "ModelManager.h"
#include "gldefine.h"
#include "pbrshader.h"
#include "spacedata.h"

#define GL_RENDERER_SUCCESS   (1)
#define GL_RENDERER_FAIL      (0)

using namespace std;

typedef enum {
    FILE_EXTENSION_OBJ = 0,
    FILE_EXTENSION_FBX,
    FILE_EXTENSION_BLEND,
    NUM_FILE_EXTENSION,
} FileExtension;

/**
 * Implementation of a gl renderer for 3D model rendering
 */
class GLRenderer : public QObject, protected QOpenGLFunctions
{
    Q_OBJECT
public:
    GLRenderer();
    ~GLRenderer();
    int32_t init();
    int32_t load(string path);

    bool isLoadded() {
        return mContextCreated;
    }

    /// Set common option
    void locateLightSource(glm::vec3 pos);
    void locateViewPoint(glm::vec3 pos);

    void setViewPortsize(GLSpace::Rectangle rect);

    inline void setWindow(QQuickWindow *window) {
        m_window = window;
    }

    inline glm::vec3 getLightColor() {
        return mSpaceInfo.lightColor;
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

    inline void setLightColor(glm::vec3 color) {
        mSpaceInfo.lightColor = color;
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

    /// Set material parameter
    void setMaterial(string name);
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

    /// choose shader type
    void setShdaerType(SHADER_TYPE type);

    void loadTexture(string path);

    inline void showAxisLine(bool isShow) {
       mIsShowOfAxisLine = isShow;
    }

signals:
    void sigMeshInfo(string info);

public slots:
    void paint();

private :
    FileExtension getFileExtension(string path);
    string loadShaderFile(string path);
    GLuint registerShader(const string text, uint32_t type);
    int32_t connectShader2Program(GLRendererContext *context);
    int32_t registerAttribute(SHADER_TYPE type);
    int32_t registerUniform(SHADER_TYPE type);
    int32_t registerUniformForPhong();
    int32_t registerUniformForCookTorrance();
    int32_t createContext();
    int32_t createmPrimitiveContext();
    void checkShaderError(GLuint shader, GLuint flag, bool isProgram, const string &errMsg);
    void draw(const ModelData *modelData);
    void drawUsingPhong(const ModelData *modelData);
    void drawUsingCookTorrance(const ModelData *modelData);
    void drawAxis();
    bool mContextCreated;
    bool mModelLoadded;
    bool mIsContextSwitching;

    SHADER_TYPE         mType;
    ModelManager        *mModelManager;
    vector<ModelData>   mModelList;
    GLRendererContext   mContext;
    GLRendererContext   mPrimitiveContext;
    GLSpace::SpaceInfo  mSpaceInfo;
    GLSpace::Rectangle  mViewportInfo;
    glm::vec3           mModelRatation;
    glm::vec3           mLengthAll;
    glm::vec3           mScale;
    QQuickWindow        *m_window;
    FileExtension       mModelExtension;

    GLSpace::Line               mAxisLine[3];
    bool                        mIsShowOfAxisLine;
    map<string, vector<int>>    mMaterialMap;
    string                      mCurrMaterialName;
};

#endif // MODEL_GLRENDER_H
