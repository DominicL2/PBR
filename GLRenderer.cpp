#include "GLRenderer.h"
#include "primitiveshader.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <QDir>

GLRenderer::GLRenderer()
{
    /// Intialize GL for Qt method
    initializeOpenGLFunctions();
    
    mContextCreated = false;
    mType = SHADER_TYPE_PHONG;
    if (createContext() == GL_RENDERER_FAIL) {
        qDebug("[%s|%d] : Fail to create GL context", __func__, __LINE__);
    } else {
        qDebug("[%s][%s:%d]", __FILE__, __func__, __LINE__);
        mSpaceInfo.fov = 100.f;
    }

    mIsContextSwitching = false;
    mModelLoadded       = false;
    mIsShowOfAxisLine   = true;

    mSpaceInfo.viewPoint    = glm::vec3(GL_SPACE_DEFUALT_VIEW_POINT_POS_X, GL_SPACE_DEFUALT_VIEW_POINT_POS_Y, GL_SPACE_DEFUALT_VIEW_POINT_POS_Z);
    mSpaceInfo.lightColor   = glm::vec3(1.0, 1.0, 1.0);
    mSpaceInfo.lightSource  = glm::vec3(GL_SPACE_DEFUALT_LIGHT_SOURCE_POS_X, GL_SPACE_DEFUALT_LIGHT_SOURCE_POS_Y, GL_SPACE_DEFUALT_LIGHT_SOURCE_POS_Z);
    mViewportInfo           = GLSpace::Rectangle(0, 0, 0, 0);

    mCurrMaterialName = "";

    mAxisLine[0].start.x = 0.f;
    mAxisLine[0].start.y = 0.f;
    mAxisLine[0].start.z = 0.f;

    mAxisLine[0].end.x = GL_SPACE_AXIS_LINE_LENGTH;
    mAxisLine[0].end.y = 0.f;
    mAxisLine[0].end.z = 0.f;

    mAxisLine[1].start.x = 0.f;
    mAxisLine[1].start.y = 0.f;
    mAxisLine[1].start.z = 0.f;

    mAxisLine[1].end.x = 0.f;
    mAxisLine[1].end.y = GL_SPACE_AXIS_LINE_LENGTH;
    mAxisLine[1].end.z = 0.f;

    mAxisLine[2].start.x = 0.f;
    mAxisLine[2].start.y = 0.f;
    mAxisLine[2].start.z = 0.f;

    mAxisLine[2].end.x = 0.f;
    mAxisLine[2].end.y = 0.f;
    mAxisLine[2].end.z = GL_SPACE_AXIS_LINE_LENGTH;

    createmPrimitiveContext();
    mModelManager = new ModelManager();
}

GLRenderer::~GLRenderer()
{
    init();

    for (ModelData& model : mModelList) {
        mModelManager->init(&model);
    }

    delete mModelManager;
}

void GLRenderer::setViewPortsize(GLSpace::Rectangle rect) {
        mViewportInfo = rect;
}

string GLRenderer::loadShaderFile(string path)
{
    ifstream file(path.c_str());
    stringstream shaderSource;

    if (file.is_open()) {
        shaderSource << file.rdbuf();
        file.close();
    }

    return shaderSource.str();
}

int32_t GLRenderer::init()
{
    int32_t ret = GL_RENDERER_FAIL;

    int32_t i = 0;

    if (mContextCreated) {
        for (i = 0; i < (int32_t)mContext.attribute.size(); i++) {
            glDisableVertexAttribArray(mContext.attribute[i]);
        }
        mContext.attribute.clear();
        vector<GLuint>().swap(mContext.attribute);

        for (i = 0; i < (int32_t)mContext.uniform.size(); i++)  {
            glDisableVertexAttribArray(mContext.uniform[i]);
        }
        mContext.uniform.clear();
        vector<GLuint>().swap(mContext.uniform);

        for (i = 0; i < (int32_t)mContext.texId.size(); i++) {
            glDeleteTextures(1, &mContext.texId[i]);
        }
        mContext.texId.clear();
        vector<GLuint>().swap(mContext.texId);

        for (i = 0; i < NUM_GLES_SHADER_TYPE; i++) {
            glDetachShader(mContext.program, mContext.shader[i]);
            glDeleteShader(mContext.shader[i]);
            mContext.shader[i] = 0U;
        }


        glDeleteProgram(mContext.program);

        mContext.program = 0U;
        mContextCreated = false;
        ret = GL_RENDERER_SUCCESS;
    } else {}

    return ret;
}
FileExtension GLRenderer::getFileExtension(string path)
{
    FileExtension ret = NUM_FILE_EXTENSION;
    for (int i = 0; i < NUM_FILE_EXTENSION; i++) {
        string targetExtension = "";
        switch (i) {
        case FILE_EXTENSION_OBJ :
            targetExtension = ".obj";
            break;
        case FILE_EXTENSION_FBX :
            targetExtension = ".fbx";
            break;
        case FILE_EXTENSION_BLEND :
            targetExtension = ".blend";
            break;
        default :
            break;
        }

        int pos = path.find(targetExtension);
        if (pos > 0) {
            ret = (FileExtension)i;
            break;
        }
    }
    return ret;
}

int32_t GLRenderer::load(string path)
{
    int32_t ret = GL_RENDERER_FAIL;
    mModelExtension = getFileExtension(path);
    if (mModelExtension != FILE_EXTENSION_OBJ
            && mModelExtension != FILE_EXTENSION_FBX) {
        return ret;
    }

    for (int i = 0; i < mModelList.size(); i ++) {
        mModelManager->init(&mModelList[i]);
    }

    mModelManager->loadModel(path, &mModelList);
    if (mModelList.size() > 0) {
        mMaterialMap.clear();
        mCurrMaterialName = "";
        mModelLoadded = true;
        mModelRatation = glm::vec3(0.f, 0.f, 0.f);
        mLengthAll = glm::vec3(0.f, 0.f, 0.f);
        ret = GL_RENDERER_SUCCESS;        
    } else {}

    stringstream log;
    log << "[Info] ToTal Mesh : " << mModelList.size() << endl;

    for (size_t i = 0; i < mModelList.size(); i++) {
        qDebug("[%s] Min : %f %f %f", mModelList[i].materialName.c_str(), mModelList[i].size.min.x, mModelList[i].size.min.y, mModelList[i].size.min.z);
        qDebug("[%s] Max : %f %f %f", mModelList[i].materialName.c_str(), mModelList[i].size.max.x, mModelList[i].size.max.y, mModelList[i].size.max.z);
        mMaterialMap[mModelList[i].materialName].push_back(i);
        mLengthAll.x = mModelList[i].size.length.x > mLengthAll.x ? mModelList[i].size.length.x :  mLengthAll.x;
        mLengthAll.y = mModelList[i].size.length.y > mLengthAll.y ? mModelList[i].size.length.y :  mLengthAll.y;
        mLengthAll.z = mModelList[i].size.length.z > mLengthAll.z ? mModelList[i].size.length.z :  mLengthAll.z;
        log << "[" << i << "] "<< "Vetex : " << mModelList.at(i).vertices.size() << endl;
        log << "[" << i << "] "<< "Normal : " << mModelList.at(i).normals.size() << endl;
        log << "[" << i << "] "<< "Index : " << mModelList.at(i).indices.size() << endl;
        log << "[" << i << "] "<< "Texcoord : " << mModelList.at(i).texcoord.size() << endl;
        log << "[" << i << "] "<< "Size : " <<
               "x(" << mModelList.at(i).size.length.x << ") " <<
               "y(" << mModelList.at(i).size.length.y << ") " <<
               "z(" << mModelList.at(i).size.length.z << ") " << endl;
    }

    mCurrMaterialName = mModelList[0].materialName;
    float minLength = mLengthAll.x;
    minLength = mLengthAll.y < minLength ?  mLengthAll.y : minLength;
    minLength = mLengthAll.z < minLength ?  mLengthAll.z : minLength;

    mScale.x = mLengthAll.x / minLength;
    mScale.y = mLengthAll.y / minLength;
    mScale.z = mLengthAll.z / minLength;

    emit sigMeshInfo(log.str());
    return ret;
}

GLuint GLRenderer::registerShader(const string text, uint32_t type)
{
    GLuint shader = glCreateShader(type);
    const GLchar* shaderStr[1];
    shaderStr[0] = text.c_str();
    GLint length[1] = {(GLint)text.length()};
    glShaderSource(shader, 1, shaderStr, length);
    glCompileShader(shader);

    return shader;
}

int32_t GLRenderer::registerAttribute(SHADER_TYPE type)
{
    int32_t ret = GL_RENDERER_FAIL;
    GLint attributeId = -1;

    switch (type) {
    case SHADER_TYPE_PHONG :
    case SHADER_TYPE_BLINN_PHONG :
    case SHADER_TYPE_COOK_TORRNACE :
        attributeId = glGetAttribLocation(mContext.program, "a_position");
        if (attributeId >= 0) {
            mContext.attribute.push_back(attributeId);
        } else {
            break;
        }

        attributeId = glGetAttribLocation(mContext.program, "a_normal");
        if (attributeId >= 0) {
            mContext.attribute.push_back(attributeId);
        } else {
            break;
        }

        attributeId = glGetAttribLocation(mContext.program, "a_tangent");
        if (attributeId >= 0) {
            mContext.attribute.push_back(attributeId);
        } else {
            break;
        }

        attributeId = glGetAttribLocation(mContext.program, "a_biTangent");
        if (attributeId >= 0) {
            mContext.attribute.push_back(attributeId);
        } else {
            break;
        }

        attributeId = glGetAttribLocation(mContext.program, "a_texcoord");
        if (attributeId >= 0) {
            mContext.attribute.push_back(attributeId);
        } else {
            break;
        }

        ret = GL_RENDERER_SUCCESS;
        break;
    default:
        break;
    }

    qDebug("[%s] : %s", __func__, ret == GL_RENDERER_SUCCESS ? "true" : "false");
    return ret;
}

void GLRenderer::rotateLightSourceThread(bool *isRunning, glm::vec3  *inputPos, float stepAngle)
{
    glm::vec3 rotatePos = *inputPos;
    float radius = sqrt(abs(rotatePos.x) + abs(rotatePos.z));
    float currAngle = atan2(rotatePos.z, rotatePos.x) * 180.f / GL_PHY;
    while (*isRunning) {
        currAngle = currAngle + stepAngle;
        if (currAngle > 360) {
            currAngle = 0;
        }
        *inputPos = glm::vec3(cos(currAngle * (GL_PHY / 180.f)) * radius, rotatePos.y, sin(currAngle* (GL_PHY / 180.f)) * radius);
        QThread::msleep(100);
    }
}

int32_t GLRenderer::registerUniformForPhong()
{
    int32_t ret = GL_RENDERER_FAIL;

    GLint uniformId = glGetUniformLocation(mContext.program, "u_mvp");
    if (uniformId >= 0) {
        mContext.uniform.push_back(uniformId);
    } else {
        return ret;
    }

    uniformId = glGetUniformLocation(mContext.program, "u_mv");
    if (uniformId >= 0) {
        mContext.uniform.push_back(uniformId);
    } else {
        return ret;
    }

    uniformId = glGetUniformLocation(mContext.program, "u_lightPos");
    if (uniformId >= 0) {
        mContext.uniform.push_back(uniformId);
    } else {
        return ret;
    }

    uniformId = glGetUniformLocation(mContext.program, "u_viewPos");
    if (uniformId >= 0) {
        mContext.uniform.push_back(uniformId);
    } else {
        return ret;
    }

    uniformId = glGetUniformLocation(mContext.program, "u_lightColor");
    if (uniformId >= 0) {
        mContext.uniform.push_back(uniformId);
    } else {
        return ret;
    }

    uniformId = glGetUniformLocation(mContext.program, "ambientW");
    if (uniformId >= 0) {
        mContext.uniform.push_back(uniformId);
    } else {
        return ret;
    }

    uniformId = glGetUniformLocation(mContext.program, "diffuseW");
    if (uniformId >= 0) {
        mContext.uniform.push_back(uniformId);
    } else {
        return ret;
    }

    uniformId = glGetUniformLocation(mContext.program, "specularW");
    if (uniformId >= 0) {
        mContext.uniform.push_back(uniformId);
    } else {
        return ret;
    }

    uniformId = glGetUniformLocation(mContext.program, "shiness");
    if (uniformId >= 0) {
        mContext.uniform.push_back(uniformId);
    } else {
        return ret;
    }

    uniformId = glGetUniformLocation(mContext.program, "defaultColor");
    if (uniformId >= 0) {
        mContext.uniform.push_back(uniformId);
    } else {
        return ret;
    }

    uniformId = glGetUniformLocation(mContext.program, "texAlbedo");
    if (uniformId >= 0) {
        mContext.uniform.push_back(uniformId);
    } else {
        return ret;
    }

    uniformId = glGetUniformLocation(mContext.program, "texNormalMap");
    if (uniformId >= 0) {
        mContext.uniform.push_back(uniformId);
    } else {
        return ret;
    }

    ret = GL_RENDERER_SUCCESS;
    return ret;
}

int32_t GLRenderer::registerUniformForCookTorrance()
{
    int32_t ret = GL_RENDERER_FAIL;

    GLint uniformId = glGetUniformLocation(mContext.program, "u_mvp");
    if (uniformId >= 0) {
        mContext.uniform.push_back(uniformId);
    } else {
        return ret;
    }

    uniformId = glGetUniformLocation(mContext.program, "u_mv");
    if (uniformId >= 0) {
        mContext.uniform.push_back(uniformId);
    } else {
        return ret;
    }

    uniformId = glGetUniformLocation(mContext.program, "u_lightPos");
    if (uniformId >= 0) {
        mContext.uniform.push_back(uniformId);
    } else {
        return ret;
    }

    uniformId = glGetUniformLocation(mContext.program, "u_viewPos");
    if (uniformId >= 0) {
        mContext.uniform.push_back(uniformId);
    } else {
        return ret;
    }
    uniformId = glGetUniformLocation(mContext.program, "u_lightColor");
    if (uniformId >= 0) {
        mContext.uniform.push_back(uniformId);
    } else {
        return ret;
    }

    uniformId = glGetUniformLocation(mContext.program, "u_ambientW");
    if (uniformId >= 0) {
        mContext.uniform.push_back(uniformId);
    } else {
        return ret;
    }

    uniformId = glGetUniformLocation(mContext.program, "texAlbedo");
    if (uniformId >= 0) {
        mContext.uniform.push_back(uniformId);
    } else {
        return ret;
    }

    uniformId = glGetUniformLocation(mContext.program, "texNormalMap");
    if (uniformId >= 0) {
        mContext.uniform.push_back(uniformId);
    } else {
        return ret;
    }

    uniformId = glGetUniformLocation(mContext.program, "texRoughnessMap");
    if (uniformId >= 0) {
        mContext.uniform.push_back(uniformId);
    } else {
        return ret;
    }

    uniformId = glGetUniformLocation(mContext.program, "texMetallicMap");
    if (uniformId >= 0) {
        mContext.uniform.push_back(uniformId);
    } else {
        return ret;
    }
    ret = GL_RENDERER_SUCCESS;
    return ret;
}

int32_t GLRenderer::registerUniform(SHADER_TYPE type)
{
    int32_t ret = GL_RENDERER_FAIL;

    switch (type) {
    case SHADER_TYPE_PHONG :
    case SHADER_TYPE_BLINN_PHONG :
        ret = registerUniformForPhong();
        break;
    case SHADER_TYPE_COOK_TORRNACE :
        ret = registerUniformForCookTorrance();
        break;
    default:
        break;
    }
    qDebug("[%s] : %s", __func__, ret == GL_RENDERER_SUCCESS ? "true" : "false");
    return ret;
}

void GLRenderer::checkShaderError(GLuint shader, GLuint flag, bool isProgram, const string &errMsg) {
    GLint success = 0;
    GLchar error[1024] = { 0 };

    if (isProgram) {
        glGetProgramiv(shader, flag, &success);
    } else {
        glGetShaderiv(shader, flag, &success);
    }

    if (success == GL_FALSE) {
        if (isProgram) {
            glGetProgramInfoLog(shader, sizeof(error), NULL, error);
        } else {
            glGetShaderInfoLog(shader, sizeof(error), NULL, error);
        }
        qDebug("[%s] : %s", errMsg.data(), error);
        glDeleteShader(shader);
    }
}

int32_t GLRenderer::connectShader2Program(GLRendererContext *context)
{
    int32_t ret = GL_RENDERER_SUCCESS;

    if (context->shader[GLES_SHADER_TYPE_VERTEX] <= 0 || context->shader[GLES_SHADER_TYPE_FRAGMENT] <= 0) {
        ret = GL_RENDERER_FAIL;
        return ret;
    } else {}    

    glAttachShader(context->program, context->shader[GLES_SHADER_TYPE_VERTEX]);
    glAttachShader(context->program, context->shader[GLES_SHADER_TYPE_FRAGMENT]);

    glLinkProgram(context->program);
    checkShaderError(context->program, GL_LINK_STATUS, true, "Invalid Program");
    checkShaderError(context->program, GL_LINK_STATUS, false, "Invalid Shader");
    glValidateProgram(context->program);
    return ret;
}

int32_t GLRenderer::createmPrimitiveContext()
{
    int32_t ret = GL_RENDERER_FAIL;
    mPrimitiveContext.program = glCreateProgram();
    mPrimitiveContext.shader[GLES_SHADER_TYPE_VERTEX]   = registerShader(VERTEX_SHADER_PRIMITIVE_STR, GL_VERTEX_SHADER);
    mPrimitiveContext.shader[GLES_SHADER_TYPE_FRAGMENT] = registerShader(FRAGMENT_SHADER_PRIMITIVE_STR, GL_FRAGMENT_SHADER);

    if (connectShader2Program(&mPrimitiveContext) == GL_RENDERER_SUCCESS) {
        GLuint id = glGetAttribLocation(mPrimitiveContext.program, "a_position");
        if (id >= 0) {
            mPrimitiveContext.attribute.push_back(id);
            id = glGetUniformLocation(mPrimitiveContext.program, "u_mvp");
            mPrimitiveContext.uniform.push_back(id);
            id = glGetUniformLocation(mPrimitiveContext.program, "u_color");
            mPrimitiveContext.uniform.push_back(id);
        }

    } else {}

    return ret;
}

int32_t GLRenderer::createContext()
{
    int32_t ret = GL_RENDERER_FAIL;

    if (mContextCreated) {
        init();
    } else {}
    mContext.program = glCreateProgram();
    if (mContext.program == 0) {
        return ret;
    } else {}
    switch (mType) {
    case SHADER_TYPE_PHONG :
    case SHADER_TYPE_BLINN_PHONG :
    {
        string vertexShaderPath = mType == SHADER_TYPE_PHONG? (QDir::currentPath().toStdString() + "/" + PBR_SHADER_PATH_PHONG + ".vert").c_str() : (QDir::currentPath().toStdString() + "/" + PBR_SHADER_PATH_BLINN_PHONG + ".vert").c_str();
        string fragmentShaderPath = mType == SHADER_TYPE_PHONG? (QDir::currentPath().toStdString() + "/" + PBR_SHADER_PATH_PHONG + ".frag").c_str() : (QDir::currentPath().toStdString() + "/" + PBR_SHADER_PATH_BLINN_PHONG + ".frag").c_str();

        mContext.shader[GLES_SHADER_TYPE_VERTEX]    = registerShader(loadShaderFile(vertexShaderPath), GL_VERTEX_SHADER);
        mContext.shader[GLES_SHADER_TYPE_FRAGMENT]  = registerShader(loadShaderFile(fragmentShaderPath), GL_FRAGMENT_SHADER);

        qDebug("Program(%d)", mContext.program);
        qDebug("Shader V(%d) F(%d)", mContext.shader[GLES_SHADER_TYPE_VERTEX], mContext.shader[GLES_SHADER_TYPE_FRAGMENT]);
        if (connectShader2Program(&mContext) == GL_RENDERER_FAIL) {
            break;
        } else {}
        if (registerAttribute(SHADER_TYPE_PHONG) == GL_RENDERER_SUCCESS) {
            if (registerUniform(SHADER_TYPE_PHONG) == GL_RENDERER_SUCCESS) {
                ret = GL_RENDERER_SUCCESS;
            } else {}
        } else {
            init();
        }
        break;
    }
    case SHADER_TYPE_COOK_TORRNACE :
        mContext.shader[GLES_SHADER_TYPE_VERTEX]    = registerShader(loadShaderFile((QDir::currentPath().toStdString() + "/" + PBR_SHADER_PATH_COOK_TORRANCE + ".vert").c_str()), GL_VERTEX_SHADER);
        mContext.shader[GLES_SHADER_TYPE_FRAGMENT]  = registerShader(loadShaderFile((QDir::currentPath().toStdString() + "/" + PBR_SHADER_PATH_COOK_TORRANCE + ".frag").c_str()), GL_FRAGMENT_SHADER);

        qDebug("Program(%d)", mContext.program);
        qDebug("Shader V(%d) F(%d)", mContext.shader[GLES_SHADER_TYPE_VERTEX], mContext.shader[GLES_SHADER_TYPE_FRAGMENT]);
        if (connectShader2Program(&mContext) == GL_RENDERER_FAIL) {
            break;
        } else {}
        if (registerAttribute(SHADER_TYPE_COOK_TORRNACE) == GL_RENDERER_SUCCESS) {
            if (registerUniform(SHADER_TYPE_COOK_TORRNACE) == GL_RENDERER_SUCCESS) {
                ret = GL_RENDERER_SUCCESS;
            } else {}
        } else {
            init();
        }
        break;
        break;
    default:
        break;
    }

    if (ret == GL_RENDERER_SUCCESS) {
        mContextCreated = true;
    }
    return ret;
}
void GLRenderer::drawAxis()
{
    glUseProgram(mPrimitiveContext.program);

    if ((mViewportInfo.x == 0) &&  (mViewportInfo.width == 0) &&
        (mViewportInfo.y == 0) &&  (mViewportInfo.height == 0)) {
        return;
    }
    glm::vec3 color[3];
    color[0] = glm::vec3(1.0, 0.231, 0.188);
    color[1] = glm::vec3(0.203, 0.78, 0.349);
    color[2] = glm::vec3(0.039, 0.517, 1.0);

    glViewport(mViewportInfo.x, mViewportInfo.y , mViewportInfo.width, mViewportInfo.height);

    glm::mat4 mvpMatrix         = glm::mat4(1.0f);
    glm::mat4 viewMatrix        = glm::mat4(1.0f);
    glm::mat4 projectionMatrix  = glm::mat4(1.0f);
    glm::mat4 modelMatrix       = glm::mat4(1.0f);

    float ratio = (float)mViewportInfo.width /  (float)mViewportInfo.height;
    projectionMatrix = glm::perspective(glm::radians(mSpaceInfo.fov), ratio, 0.1f, GL_SPACE_DEFUALT_MAX_DISTANCE);
    viewMatrix = glm::lookAt(mSpaceInfo.viewPoint, glm::vec3(0,
                                                             0,
                                                             0), GLSpace::getUpVector());

    mvpMatrix = projectionMatrix * viewMatrix * modelMatrix;
    for (int i = 0; i < 3; i++) {
        glVertexAttribPointer(mPrimitiveContext.attribute[0], 3, GL_FLOAT, GL_FALSE, 0, (const float *)&mAxisLine[i]);
        glEnableVertexAttribArray(mPrimitiveContext.attribute[0]);
        glUniformMatrix4fv(mPrimitiveContext.uniform[0], 1, GL_FALSE, (const float *)&mvpMatrix[0][0]);

        glUniform3f(mPrimitiveContext.uniform[1],
                     color[i].x,
                     color[i].y,
                     color[i].z );

        glDrawArrays(GL_LINES, 0, 2);

        glDisableVertexAttribArray(mPrimitiveContext.attribute[0]);
    }

    glUseProgram(9);
}

void GLRenderer::drawUsingPhong(const ModelData *modelData)
{
    glUseProgram(mContext.program);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);

    glViewport(mViewportInfo.x, mViewportInfo.y , mViewportInfo.width, mViewportInfo.height);
    glm::mat4 mvpMatrix         = glm::mat4(1.0f);
    glm::mat4 mvMatrix          = glm::mat4(1.0f);
    glm::mat4 modelMatrix       = glm::mat4(1.0f);
    glm::mat4 viewMatrix        = glm::mat4(1.0f);
    glm::mat4 projectionMatrix  = glm::mat4(1.0f);

    float ratio = (float)mViewportInfo.width /  (float)mViewportInfo.height;
    projectionMatrix = glm::perspective(glm::radians(mSpaceInfo.fov), ratio, 0.1f, GL_SPACE_DEFUALT_MAX_DISTANCE);
    viewMatrix = glm::lookAt(mSpaceInfo.viewPoint, glm::vec3(0,
                                                             0,
                                                             0), GLSpace::getUpVector());

    modelMatrix = glm::scale(modelMatrix, glm::vec3(mScale.x / mLengthAll.x , mScale.y / mLengthAll.y, mScale.z / mLengthAll.z));
    modelMatrix = glm::rotate(modelMatrix, glm::radians(mModelRatation.z), glm::vec3(0.f, 0.f, 1.f));
    modelMatrix = glm::rotate(modelMatrix, glm::radians(mModelRatation.y), glm::vec3(0.f, 1.f, 0.f));
    modelMatrix = glm::rotate(modelMatrix, glm::radians(mModelRatation.x), glm::vec3(1.f, 0.f, 0.f));

    mvpMatrix = projectionMatrix * viewMatrix * modelMatrix;
    mvMatrix = viewMatrix * modelMatrix;

    // Upate Uniform
    glUniform1f(mContext.uniform[PHONG_SHADER_UNIFORM_SHINESS], modelData->parameter[PHONG_PARAMETER_TYPE_SHINESS]);

    glUniformMatrix4fv(mContext.uniform[PHONG_SHADER_UNIFORM_MVP], 1, GL_FALSE, (const float *)&mvpMatrix[0][0]);
    glUniformMatrix4fv(mContext.uniform[PHONG_SHADER_UNIFORM_MV], 1, GL_FALSE, (const float *)&mvMatrix[0][0]);

    glUniform3f(mContext.uniform[PHONG_SHADER_UNIFORM_LIGHT_POS], mSpaceInfo.lightSource.x, mSpaceInfo.lightSource.y, mSpaceInfo.lightSource.z);
    glUniform3f(mContext.uniform[PHONG_SHADER_UNIFORM_VIEW_POS], mSpaceInfo.viewPoint.x, mSpaceInfo.viewPoint.y, mSpaceInfo.viewPoint.z);
    glUniform3f(mContext.uniform[PHONG_SHADER_UNIFORM_LIGHT_COLOR], mSpaceInfo.lightColor.x * 1.0,
                                                                            mSpaceInfo.lightColor.y * 1.0,
                                                                            mSpaceInfo.lightColor.z * 1.0);

    glUniform3f(mContext.uniform[PHONG_SHADER_UNIFORM_AMBIENT],
                modelData->weight[LIGHT_WEIGHT_TYPE_AMBIENT].r,
                modelData->weight[LIGHT_WEIGHT_TYPE_AMBIENT].g,
                modelData->weight[LIGHT_WEIGHT_TYPE_AMBIENT].b );

    glUniform3f(mContext.uniform[PHONG_SHADER_UNIFORM_DIFFUSE],
                modelData->weight[LIGHT_WEIGHT_TYPE_DIFFUSE].r,
                modelData->weight[LIGHT_WEIGHT_TYPE_DIFFUSE].g,
                modelData->weight[LIGHT_WEIGHT_TYPE_DIFFUSE].b );

    glUniform3f(mContext.uniform[PHONG_SHADER_UNIFORM_SPECULAR],
                modelData->weight[LIGHT_WEIGHT_TYPE_SPECULAR].r,
                modelData->weight[LIGHT_WEIGHT_TYPE_SPECULAR].g,
                modelData->weight[LIGHT_WEIGHT_TYPE_SPECULAR].b );

    glBindBuffer(GL_ARRAY_BUFFER, modelData->vboId[VBO_ID_TYPE_VERTEX]);
    glVertexAttribPointer(mContext.attribute[PHONG_SHADER_ATTR_VERTEX], 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), (void*)0);
    glEnableVertexAttribArray(mContext.attribute[PHONG_SHADER_ATTR_VERTEX]);

    glBindBuffer(GL_ARRAY_BUFFER, modelData->vboId[VBO_ID_TYPE_NORMAL]);
    glVertexAttribPointer(mContext.attribute[PHONG_SHADER_ATTR_NORMAL], 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), (void*)0);
    glEnableVertexAttribArray(mContext.attribute[PHONG_SHADER_ATTR_NORMAL]);

    glBindBuffer(GL_ARRAY_BUFFER, modelData->vboId[VBO_ID_TYPE_TANGENT]);
    glVertexAttribPointer(mContext.attribute[PHONG_SHADER_ATTR_TANGENT], 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), (void*)0);
    glEnableVertexAttribArray(mContext.attribute[PHONG_SHADER_ATTR_TANGENT]);

    glBindBuffer(GL_ARRAY_BUFFER, modelData->vboId[VBO_ID_TYPE_BITANGENT]);
    glVertexAttribPointer(mContext.attribute[PHONG_SHADER_ATTR_BITANGENT], 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), (void*)0);
    glEnableVertexAttribArray(mContext.attribute[PHONG_SHADER_ATTR_BITANGENT]);

    if (modelData->vboId[VBO_ID_TYPE_TEXCOORD] > 0) {
        glBindBuffer(GL_ARRAY_BUFFER, modelData->vboId[VBO_ID_TYPE_TEXCOORD]);
        glVertexAttribPointer(mContext.attribute[PHONG_SHADER_ATTR_TEXCOORD], 2, GL_FLOAT, GL_FALSE, sizeof(glm::vec2), (void*)0);
        glEnableVertexAttribArray(mContext.attribute[PHONG_SHADER_ATTR_TEXCOORD]);
    } else {}
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, modelData->vboId[VBO_ID_TYPE_INDEX]);

    int textureIndex = 0;
    auto baseColor = modelData->textures.find(aiTextureType_DIFFUSE);
    glm::vec3 defaultColor = glm::vec3(0.0, 0.0, 0.0);

    if (baseColor->second.size() > 0U) {
        glUniform1i(mContext.uniform[PHONG_SHADER_UNIFORM_TEXTURE_ALBEDO], 0);
        for (size_t i = 0; i < baseColor->second.size(); i++) {
            glActiveTexture(GL_TEXTURE0 + textureIndex);
            glBindTexture(GL_TEXTURE_2D, baseColor->second[i]);
            textureIndex++;
        }
    } else {
        defaultColor = glm::vec3(0.55, 0.55, 0.55);
    }

    auto normalMap = modelData->textures.find(aiTextureType_NORMALS);
    if (normalMap->second.size() > 0U) {
        glUniform1i(mContext.uniform[PHONG_SHADER_UNIFORM_TEXTURE_NORMAL_MAP], 1);
        for (size_t i = 0; i < normalMap->second.size(); i++) {
            glActiveTexture(GL_TEXTURE0 + textureIndex);
            glBindTexture(GL_TEXTURE_2D, normalMap->second[i]);
            textureIndex++;
        }
    }

    glUniform3f(mContext.uniform[PHONG_SHADER_UNIFORM_DEFAULT_COLOR],
                 defaultColor.x,
                 defaultColor.y,
                 defaultColor.z );

    glDrawElements(GL_TRIANGLES, modelData->indices.size(), GL_UNSIGNED_INT, 0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    glDisableVertexAttribArray(mContext.attribute[PHONG_SHADER_ATTR_VERTEX]);
    glDisableVertexAttribArray(mContext.attribute[PHONG_SHADER_ATTR_NORMAL]);
    glDisableVertexAttribArray(mContext.attribute[PHONG_SHADER_ATTR_TANGENT]);
    glDisableVertexAttribArray(mContext.attribute[PHONG_SHADER_ATTR_BITANGENT]);
    glDisableVertexAttribArray(mContext.attribute[PHONG_SHADER_ATTR_TEXCOORD]);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, 0);

    glDisable(GL_DEPTH_TEST);
    glDisable(GL_CULL_FACE);
    glDisable(GL_BLEND);
    glBindBuffer(GL_ARRAY_BUFFER,0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,0);
    glUseProgram(0);
}

void GLRenderer::drawUsingCookTorrance(const ModelData *modelData)
{
    glUseProgram(mContext.program);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);

    glViewport(mViewportInfo.x, mViewportInfo.y , mViewportInfo.width, mViewportInfo.height);
    glm::mat4 mvpMatrix         = glm::mat4(1.0f);
    glm::mat4 mvMatrix          = glm::mat4(1.0f);
    glm::mat4 modelMatrix       = glm::mat4(1.0f);
    glm::mat4 viewMatrix        = glm::mat4(1.0f);
    glm::mat4 projectionMatrix  = glm::mat4(1.0f);

    float ratio = (float)mViewportInfo.width /  (float)mViewportInfo.height;
    projectionMatrix = glm::perspective(glm::radians(mSpaceInfo.fov), ratio, 0.1f, GL_SPACE_DEFUALT_MAX_DISTANCE);
    viewMatrix = glm::lookAt(mSpaceInfo.viewPoint, glm::vec3(0,
                                                             0,
                                                             0), GLSpace::getUpVector());

    modelMatrix = glm::scale(modelMatrix, glm::vec3(mScale.x / mLengthAll.x , mScale.y / mLengthAll.y, mScale.z / mLengthAll.z));
    modelMatrix = glm::rotate(modelMatrix, glm::radians(mModelRatation.z), glm::vec3(0.f, 0.f, 1.f));
    modelMatrix = glm::rotate(modelMatrix, glm::radians(mModelRatation.y), glm::vec3(0.f, 1.f, 0.f));
    modelMatrix = glm::rotate(modelMatrix, glm::radians(mModelRatation.x), glm::vec3(1.f, 0.f, 0.f));

    mvpMatrix = projectionMatrix * viewMatrix * modelMatrix;
    mvMatrix = viewMatrix * modelMatrix;

    glUniform3f(mContext.uniform[COOK_TORRANCE_SHADER_UNIFORM_AMBIENT],
                modelData->weight[LIGHT_WEIGHT_TYPE_AMBIENT].r,
                modelData->weight[LIGHT_WEIGHT_TYPE_AMBIENT].g,
                modelData->weight[LIGHT_WEIGHT_TYPE_AMBIENT].b );

    glUniformMatrix4fv(mContext.uniform[COOK_TORRANCE_SHADER_UNIFORM_MVP], 1, GL_FALSE, (const float *)&mvpMatrix[0][0]);
    glUniformMatrix4fv(mContext.uniform[COOK_TORRANCE_SHADER_UNIFORM_MV], 1, GL_FALSE, (const float *)&mvMatrix[0][0]);

    glUniform3f(mContext.uniform[COOK_TORRANCE_SHADER_UNIFORM_LIGHT_POS], mSpaceInfo.lightSource.x, mSpaceInfo.lightSource.y, mSpaceInfo.lightSource.z);
    glUniform3f(mContext.uniform[COOK_TORRANCE_SHADER_UNIFORM_VIEW_POS], mSpaceInfo.viewPoint.x, mSpaceInfo.viewPoint.y, mSpaceInfo.viewPoint.z);
    glUniform3f(mContext.uniform[COOK_TORRANCE_SHADER_UNIFORM_LIGHT_COLOR], mSpaceInfo.lightColor.x * PBR_LIGHT_COLOR_WEIGHT,
                                                                            mSpaceInfo.lightColor.y * PBR_LIGHT_COLOR_WEIGHT,
                                                                            mSpaceInfo.lightColor.z * PBR_LIGHT_COLOR_WEIGHT);

    glBindBuffer(GL_ARRAY_BUFFER, modelData->vboId[VBO_ID_TYPE_VERTEX]);
    glVertexAttribPointer(mContext.attribute[PHONG_SHADER_ATTR_VERTEX], 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), (void*)0);
    glEnableVertexAttribArray(mContext.attribute[PHONG_SHADER_ATTR_VERTEX]);

    glBindBuffer(GL_ARRAY_BUFFER, modelData->vboId[VBO_ID_TYPE_NORMAL]);
    glVertexAttribPointer(mContext.attribute[PHONG_SHADER_ATTR_NORMAL], 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), (void*)0);
    glEnableVertexAttribArray(mContext.attribute[PHONG_SHADER_ATTR_NORMAL]);

    glBindBuffer(GL_ARRAY_BUFFER, modelData->vboId[VBO_ID_TYPE_TANGENT]);
    glVertexAttribPointer(mContext.attribute[PHONG_SHADER_ATTR_TANGENT], 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), (void*)0);
    glEnableVertexAttribArray(mContext.attribute[PHONG_SHADER_ATTR_TANGENT]);

    glBindBuffer(GL_ARRAY_BUFFER, modelData->vboId[VBO_ID_TYPE_BITANGENT]);
    glVertexAttribPointer(mContext.attribute[PHONG_SHADER_ATTR_BITANGENT], 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), (void*)0);
    glEnableVertexAttribArray(mContext.attribute[PHONG_SHADER_ATTR_BITANGENT]);

    if (modelData->vboId[VBO_ID_TYPE_TEXCOORD] > 0) {
        glBindBuffer(GL_ARRAY_BUFFER, modelData->vboId[VBO_ID_TYPE_TEXCOORD]);
        glVertexAttribPointer(mContext.attribute[PHONG_SHADER_ATTR_TEXCOORD], 2, GL_FLOAT, GL_FALSE, sizeof(glm::vec2), (void*)0);
        glEnableVertexAttribArray(mContext.attribute[PHONG_SHADER_ATTR_TEXCOORD]);
    } else {}
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, modelData->vboId[VBO_ID_TYPE_INDEX]);

    int textureIndex = 0;
    auto baseColor = modelData->textures.find(aiTextureType_DIFFUSE);
    if (baseColor->second.size() > 0U) {
        glUniform1i(mContext.uniform[COOK_TORRANCE_SHADER_UNIFORM_TEXTURE_ALBEDO], 0);
        for (size_t i = 0; i < baseColor->second.size(); i++) {
            glActiveTexture(GL_TEXTURE0 + textureIndex);
            glBindTexture(GL_TEXTURE_2D, baseColor->second[i]);
            textureIndex++;
        }
    }

    auto normalMap = modelData->textures.find(aiTextureType_NORMALS);
    if (normalMap->second.size() > 0U) {
        glUniform1i(mContext.uniform[COOK_TORRANCE_SHADER_UNIFORM_TEXTURE_NORMAL_MAP], 1);
        for (size_t i = 0; i < normalMap->second.size(); i++) {
            glActiveTexture(GL_TEXTURE0 + textureIndex);
            glBindTexture(GL_TEXTURE_2D, normalMap->second[i]);
            textureIndex++;
        }
    }

    auto roughnessMap = modelData->textures.find(mModelExtension == FILE_EXTENSION_OBJ ? aiTextureType_SHININESS : aiTextureType_DIFFUSE_ROUGHNESS);
    if (roughnessMap->second.size() > 0U) {
        glUniform1i(mContext.uniform[COOK_TORRANCE_SHADER_UNIFORM_TEXTURE_ROUGHNESS_MAP], 2);
        for (size_t i = 0; i < roughnessMap->second.size(); i++) {
            glActiveTexture(GL_TEXTURE0 + textureIndex);
            glBindTexture(GL_TEXTURE_2D, roughnessMap->second[i]);
            textureIndex++;
        }
    }

    auto metallicMap = modelData->textures.find(mModelExtension == FILE_EXTENSION_OBJ ? aiTextureType_SHININESS : aiTextureType_METALNESS);
    if (metallicMap->second.size() > 0U) {
        glUniform1i(mContext.uniform[COOK_TORRANCE_SHADER_UNIFORM_TEXTURE_METALLIC_MAP], 3);
        for (size_t i = 0; i < metallicMap->second.size(); i++) {
            glActiveTexture(GL_TEXTURE0 + textureIndex);
            glBindTexture(GL_TEXTURE_2D, metallicMap->second[i]);
            textureIndex++;
        }
    }

    glDrawElements(GL_TRIANGLES, modelData->indices.size(), GL_UNSIGNED_INT, 0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    glDisableVertexAttribArray(mContext.attribute[PHONG_SHADER_ATTR_VERTEX]);
    glDisableVertexAttribArray(mContext.attribute[PHONG_SHADER_ATTR_NORMAL]);
    glDisableVertexAttribArray(mContext.attribute[PHONG_SHADER_ATTR_TANGENT]);
    glDisableVertexAttribArray(mContext.attribute[PHONG_SHADER_ATTR_BITANGENT]);
    glDisableVertexAttribArray(mContext.attribute[PHONG_SHADER_ATTR_TEXCOORD]);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, 0);

    glDisable(GL_DEPTH_TEST);
    glDisable(GL_CULL_FACE);
    glDisable(GL_BLEND);
    glBindBuffer(GL_ARRAY_BUFFER,0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,0);
    glUseProgram(0);
}

void GLRenderer::draw(const ModelData *modelData)
{
    if ((mViewportInfo.x == 0) &&  (mViewportInfo.width == 0) &&
        (mViewportInfo.y == 0) &&  (mViewportInfo.height == 0)) {
        return;
    }

    switch (mType) {
    case SHADER_TYPE_PHONG :
    case SHADER_TYPE_BLINN_PHONG :
        drawUsingPhong(modelData);
        break;
    case SHADER_TYPE_COOK_TORRNACE :
        drawUsingCookTorrance(modelData);
        break;
    }
}

bool GLRenderer::loadded() const
{
    return mContextCreated;
}

void GLRenderer::setLoadded(bool loadded)
{
    mContextCreated = loadded;
}

void GLRenderer::paint()
{
    if (mModelLoadded  && isLoadded() && !mIsContextSwitching) {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glClearColor(0.0823f, 0.09f, 0.129f, 0.f);

        if (mIsShowOfAxisLine) {
            drawAxis();
        } else {}

        for (const auto& modelData : mModelList) {
            draw(&modelData);
        }
    } else {}
}

void GLRenderer::setAmbient(glm::vec3 pos)
{
    for (size_t i = 0; i < mMaterialMap[mCurrMaterialName].size(); i++) {
        mModelList[mMaterialMap[mCurrMaterialName][i]].weight[LIGHT_WEIGHT_TYPE_AMBIENT].r = pos.x;
        mModelList[mMaterialMap[mCurrMaterialName][i]].weight[LIGHT_WEIGHT_TYPE_AMBIENT].g = pos.y;
        mModelList[mMaterialMap[mCurrMaterialName][i]].weight[LIGHT_WEIGHT_TYPE_AMBIENT].b = pos.z;
    }
}

void GLRenderer::setDiffuse(glm::vec3 pos)
{
    for (size_t i = 0; i < mMaterialMap[mCurrMaterialName].size(); i++) {
        mModelList[mMaterialMap[mCurrMaterialName][i]].weight[LIGHT_WEIGHT_TYPE_DIFFUSE].r = pos.x;
        mModelList[mMaterialMap[mCurrMaterialName][i]].weight[LIGHT_WEIGHT_TYPE_DIFFUSE].g = pos.y;
        mModelList[mMaterialMap[mCurrMaterialName][i]].weight[LIGHT_WEIGHT_TYPE_DIFFUSE].b = pos.z;
    }
}

void GLRenderer::setSpecular(glm::vec3 pos)
{    
    for (size_t i = 0; i < mMaterialMap[mCurrMaterialName].size(); i++) {
        mModelList[mMaterialMap[mCurrMaterialName][i]].weight[LIGHT_WEIGHT_TYPE_SPECULAR].r = pos.x;
        mModelList[mMaterialMap[mCurrMaterialName][i]].weight[LIGHT_WEIGHT_TYPE_SPECULAR].g = pos.y;
        mModelList[mMaterialMap[mCurrMaterialName][i]].weight[LIGHT_WEIGHT_TYPE_SPECULAR].b = pos.z;
    }
}

glm::vec3 GLRenderer::getAmbient()
{
    return glm::vec3(mModelList[mMaterialMap[mCurrMaterialName][0]].weight[LIGHT_WEIGHT_TYPE_AMBIENT].r, mModelList[mMaterialMap[mCurrMaterialName][0]].weight[LIGHT_WEIGHT_TYPE_AMBIENT].g, mModelList[mMaterialMap[mCurrMaterialName][0]].weight[LIGHT_WEIGHT_TYPE_AMBIENT].b);
}

glm::vec3 GLRenderer::getDiffuse()
{
    return glm::vec3(mModelList[mMaterialMap[mCurrMaterialName][0]].weight[LIGHT_WEIGHT_TYPE_DIFFUSE].r, mModelList[mMaterialMap[mCurrMaterialName][0]].weight[LIGHT_WEIGHT_TYPE_DIFFUSE].g, mModelList[mMaterialMap[mCurrMaterialName][0]].weight[LIGHT_WEIGHT_TYPE_DIFFUSE].b);
}

glm::vec3 GLRenderer::getSpecular()
{
    return glm::vec3(mModelList[mMaterialMap[mCurrMaterialName][0]].weight[LIGHT_WEIGHT_TYPE_SPECULAR].r, mModelList[mMaterialMap[mCurrMaterialName][0]].weight[LIGHT_WEIGHT_TYPE_SPECULAR].g, mModelList[mMaterialMap[mCurrMaterialName][0]].weight[LIGHT_WEIGHT_TYPE_SPECULAR].b);
}

void GLRenderer::setShiness(float val)
{
    for (size_t i = 0; i < mMaterialMap[mCurrMaterialName].size(); i++) {
        mModelList[mMaterialMap[mCurrMaterialName][i]].parameter[PHONG_PARAMETER_TYPE_SHINESS] = val;
    }
}

float GLRenderer::getShiness()
{
    return mModelList[mMaterialMap[mCurrMaterialName][0]].parameter[PHONG_PARAMETER_TYPE_SHINESS];
}

void GLRenderer::setMaterial(string name)
{
    mCurrMaterialName = name;
}

void GLRenderer::getMaterialList(vector<string> *materialList)
{   
    for (auto iter = mMaterialMap.begin(); iter != mMaterialMap.end(); iter++) {
        materialList->push_back(iter->first);
    }
}

void GLRenderer::setShdaerType(SHADER_TYPE type)
{
    if ((SHADER_TYPE_PHONG <= type && type < NUM_SHADER_TYPE) && type != mType) {
        mIsContextSwitching = true;
        SHADER_TYPE prevType = mType;
        mType = type;
        if (init() == GL_RENDERER_SUCCESS) {
            if (createContext() == GL_RENDERER_SUCCESS) {
                qDebug("Change shader from %d to %d", (int)prevType, (int)mType);
            } else {
                qDebug("[%d][%s] Fail to context creation", __LINE__, __func__);
            }
        } else {
            qDebug("[%d][%s] Fail to context intialization", __LINE__, __func__);
        }
        mIsContextSwitching = false;
    }
}

void GLRenderer::loadTexture(string path) {
    for (size_t i = 0; i < mMaterialMap[mCurrMaterialName].size(); i++) {
        mModelManager->loadTexture(&mModelList[mMaterialMap[mCurrMaterialName][i]], path);
    }
}

void GLRenderer::rotateLightSource(bool isRun)
{
    mIsRotationRunning = isRun;
    if (mIsRotationRunning) {
        mRotatationThr = std::thread(rotateLightSourceThread, &mIsRotationRunning, &mSpaceInfo.lightSource, 2.f);
        mRotatationThr.detach();
    }
}
