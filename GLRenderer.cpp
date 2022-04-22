#include "GLRenderer.h"
#include "debugmacro.h"
#include <iostream>
#include <sstream>

GLRenderer::GLRenderer()
{
    initializeOpenGLFunctions();
    mContextCreated = false;
    mType = SHADER_TYPE_PHONG;
    if (createContext() == GL_RENDERER_FAIL) {
        qDebug("[%s|%d] : Fail to create GL context", __func__, __LINE__);
    } else {
        qDebug("[%s][%s:%d]", __FILE__, __func__, __LINE__);
        mSpaceInfo.fov = 100.f;
    }

    mModelLoadded = false;

    mSpaceInfo.viewPoint    = glm::vec3(GL_SPACE_DEFUALT_VIEW_POINT_POS_X, GL_SPACE_DEFUALT_VIEW_POINT_POS_Y, GL_SPACE_DEFUALT_VIEW_POINT_POS_Z);
    mSpaceInfo.lightSource  = glm::vec3(GL_SPACE_DEFUALT_LIGHT_SOURCE_POS_X, GL_SPACE_DEFUALT_LIGHT_SOURCE_POS_Y, GL_SPACE_DEFUALT_LIGHT_SOURCE_POS_Z);
    mViewportInfo           = GLSpace::Rectangle(0, 0, 0, 0);

    mCurrMaterialIndex = 0U;
}

GLRenderer::~GLRenderer()
{
    init();
}

void GLRenderer::setViewPortsize(GLSpace::Rectangle rect) {
        mViewportInfo = rect;
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
    } else {}

    return ret;
}

int32_t GLRenderer::load(string path)
{
    int32_t ret = GL_RENDERER_FAIL;

    mModelLoader.loadModel(path, &mModelList);
    if (mModelList.size() > 0) {
        mMaterialList.clear();
        vector<string>().swap(mMaterialList);
        mMaterialMap.clear();
        mCurrMaterialIndex = 0;
        mModelLoadded = true;
        mModelRatation = glm::vec3(0.f, 0.f, 0.f);
        mLengthAll = glm::vec3(0.f, 0.f, 0.f);
        ret = GL_RENDERER_SUCCESS;        
    } else {}

    stringstream log;
    log << "[Info] ToTal Mesh : " << mModelList.size() << endl;

    for (size_t i = 0; i < mModelList.size(); i++) {
        mModelList[i].parameter.resize(NUM_PHONG_PARAMETER_TYPE);
        mModelList[i].parameter[PHONG_PARAMETER_TYPE_SHINESS] = 1.0;
        mMaterialMap[mModelList[i].materialName] = i;
        mMaterialList.push_back(mModelList[i].materialName);
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

        attributeId = glGetAttribLocation(mContext.program, "a_texcoord");
        if (attributeId >= 0) {
            mContext.attribute.push_back(attributeId);
        } else {
            break;
        }

        ret = GL_RENDERER_SUCCESS;
        break;
    case SHADER_TYPE_BRDF :
        break;
    default:
        break;
    }

    qDebug("[%s] : %s", __func__, ret == GL_RENDERER_SUCCESS ? "true" : "false");
    return ret;
}

int32_t GLRenderer::registerUniform(SHADER_TYPE type)
{
    int32_t ret = GL_RENDERER_FAIL;

    GLint uniformId = -1;

    switch (type) {
    case SHADER_TYPE_PHONG :
        uniformId = glGetUniformLocation(mContext.program, "u_mvp");
        if (uniformId >= 0) {
            mContext.uniform.push_back(uniformId);
        } else {
            break;
        }

        uniformId = glGetUniformLocation(mContext.program, "u_mv");
        if (uniformId >= 0) {
            mContext.uniform.push_back(uniformId);
        } else {
            break;
        }

        uniformId = glGetUniformLocation(mContext.program, "u_lightPos");
        if (uniformId >= 0) {
            mContext.uniform.push_back(uniformId);
        } else {
            break;
        }

        uniformId = glGetUniformLocation(mContext.program, "u_viewPos");
        if (uniformId >= 0) {
            mContext.uniform.push_back(uniformId);
        } else {
            break;
        }

        uniformId = glGetUniformLocation(mContext.program, "ambientW");
        if (uniformId >= 0) {
            mContext.uniform.push_back(uniformId);
        } else {
            break;
        }

        uniformId = glGetUniformLocation(mContext.program, "diffuseW");
        if (uniformId >= 0) {
            mContext.uniform.push_back(uniformId);
        } else {
            break;
        }

        uniformId = glGetUniformLocation(mContext.program, "specularW");
        if (uniformId >= 0) {
            mContext.uniform.push_back(uniformId);
        } else {
            break;
        }

        uniformId = glGetUniformLocation(mContext.program, "shiness");
        if (uniformId >= 0) {
            mContext.uniform.push_back(uniformId);
        } else {
            break;
        }

        uniformId = glGetUniformLocation(mContext.program, "defaultColor");
        if (uniformId >= 0) {
            mContext.uniform.push_back(uniformId);
        } else {
            break;
        }
        ret = GL_RENDERER_SUCCESS;
        break;
    case SHADER_TYPE_BRDF :
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

int32_t GLRenderer::connectShader2Program()
{
    int32_t ret = GL_RENDERER_SUCCESS;

    if (mContext.shader[GLES_SHADER_TYPE_VERTEX] <= 0 || mContext.shader[GLES_SHADER_TYPE_FRAGMENT] <= 0) {
        ret = GL_RENDERER_FAIL;
        return ret;
    } else {}    

    glAttachShader(mContext.program, mContext.shader[GLES_SHADER_TYPE_VERTEX]);
    glAttachShader(mContext.program, mContext.shader[GLES_SHADER_TYPE_FRAGMENT]);

    glLinkProgram(mContext.program);
    checkShaderError(mContext.program, GL_LINK_STATUS, true, "Invalid Program");
    checkShaderError(mContext.program, GL_LINK_STATUS, false, "Invalid Shader");
    glValidateProgram(mContext.program);
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
        mContext.shader[GLES_SHADER_TYPE_VERTEX]    = registerShader(VERTEX_SHADER_BLINN_PHONG_STR, GL_VERTEX_SHADER);
        mContext.shader[GLES_SHADER_TYPE_FRAGMENT]  = registerShader(FRAGMENT_SHADER_BLINN_PHONG_STR, GL_FRAGMENT_SHADER);

        qDebug("Program(%d)", mContext.program);
        qDebug("Shader V(%d) F(%d)", mContext.shader[GLES_SHADER_TYPE_VERTEX], mContext.shader[GLES_SHADER_TYPE_FRAGMENT]);
        if (connectShader2Program() == GL_RENDERER_FAIL) {
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
    case SHADER_TYPE_BRDF :
        break;
    default:
        break;
    }

    if (ret == GL_RENDERER_SUCCESS) {
        mContextCreated = true;
    }
    return ret;
}

void GLRenderer::draw(const ModelData *modelData)
{
    if ((mViewportInfo.x == 0) &&  (mViewportInfo.width == 0) &&
        (mViewportInfo.y == 0) &&  (mViewportInfo.height == 0)) {
        return;
    }

    glViewport(mViewportInfo.x, mViewportInfo.y , mViewportInfo.width, mViewportInfo.height);
    glm::mat4 mvpMatrix         = glm::mat4(1.0f);
    glm::mat4 mvMatrix          = glm::mat4(1.0f);
    glm::mat4 modelMatrix       = glm::mat4(1.0f);
    glm::mat4 viewMatrix        = glm::mat4(1.0f);
    glm::mat4 projectionMatrix  = glm::mat4(1.0f);

    float ratio = (float)mViewportInfo.width /  (float)mViewportInfo.height;   projectionMatrix = glm::perspective(glm::radians(mSpaceInfo.fov), ratio, 0.1f, GL_SPACE_DEFUALT_MAX_DISTANCE);
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
    glVertexAttribPointer(mContext.attribute[PHONG_SHADER_ATTR_POSTION], 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), (void*)0);
    glEnableVertexAttribArray(mContext.attribute[PHONG_SHADER_ATTR_POSTION]);

    glBindBuffer(GL_ARRAY_BUFFER, modelData->vboId[VBO_ID_TYPE_NORMAL]);
    glVertexAttribPointer(mContext.attribute[PHONG_SHADER_ATTR_NORMAL], 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), (void*)0);
    glEnableVertexAttribArray(mContext.attribute[PHONG_SHADER_ATTR_NORMAL]);

    glBindBuffer(GL_ARRAY_BUFFER, modelData->vboId[VBO_ID_TYPE_TEXCOORD]);
    glVertexAttribPointer(mContext.attribute[PHONG_SHADER_ATTR_TEXCOORD], 2, GL_FLOAT, GL_FALSE, sizeof(glm::vec2), (void*)0);
    glEnableVertexAttribArray(mContext.attribute[PHONG_SHADER_ATTR_TEXCOORD]);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, modelData->vboId[VBO_ID_TYPE_INDEX]);

    auto baseColor = modelData->textures.find(aiTextureType_DIFFUSE);
    glm::vec3 defaultColor = glm::vec3(0.0, 0.0, 0.0);
    if (baseColor->second.size() > 0U) {
        for (size_t i = 0; i < baseColor->second.size(); i++) {
            glActiveTexture(GL_TEXTURE0 + i);
            glBindTexture(GL_TEXTURE_2D, baseColor->second[i]);
        }
    } else {
        defaultColor = glm::vec3(0.55, 0.55, 0.55);
    }

    glUniform3f(mContext.uniform[PHONG_SHADER_UNIFORM_DEFAULT_COLOR],
                 defaultColor.x,
                 defaultColor.y,
                 defaultColor.z );

    glDrawElements(GL_TRIANGLES, modelData->indices.size(), GL_UNSIGNED_INT, 0);

    glDisableVertexAttribArray(mContext.attribute[PHONG_SHADER_ATTR_POSTION]);
    glDisableVertexAttribArray(mContext.attribute[PHONG_SHADER_ATTR_NORMAL]);
    glDisableVertexAttribArray(mContext.attribute[VBO_ID_TYPE_TEXCOORD]);
    glBindTexture(GL_TEXTURE_2D, 0);
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
    if (mModelLoadded  && isLoadded()) {
        glUseProgram(mContext.program);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glClearColor(0.085f, 0.095f, 0.085f, 0.f);

        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        glEnable(GL_DEPTH_TEST);
        glEnable(GL_CULL_FACE);

        for (const auto& modelData : mModelList) {
            draw(&modelData);
        }

        glDisable(GL_DEPTH_TEST);
        glDisable(GL_CULL_FACE);
        glDisable(GL_BLEND);
        glBindBuffer(GL_ARRAY_BUFFER,0);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,0);
        glUseProgram(0);
    } else {}
}

void GLRenderer::setAmbient(glm::vec3 pos)
{
    mModelList[mCurrMaterialIndex].weight[LIGHT_WEIGHT_TYPE_AMBIENT].r = pos.x;
    mModelList[mCurrMaterialIndex].weight[LIGHT_WEIGHT_TYPE_AMBIENT].g = pos.y;
    mModelList[mCurrMaterialIndex].weight[LIGHT_WEIGHT_TYPE_AMBIENT].b = pos.z;
}

void GLRenderer::setDiffuse(glm::vec3 pos)
{
    mModelList[mCurrMaterialIndex].weight[LIGHT_WEIGHT_TYPE_DIFFUSE].r = pos.x;
    mModelList[mCurrMaterialIndex].weight[LIGHT_WEIGHT_TYPE_DIFFUSE].g = pos.y;
    mModelList[mCurrMaterialIndex].weight[LIGHT_WEIGHT_TYPE_DIFFUSE].b = pos.z;
}

void GLRenderer::setSpecular(glm::vec3 pos)
{    
    mModelList[mCurrMaterialIndex].weight[LIGHT_WEIGHT_TYPE_SPECULAR].r = pos.x;
    mModelList[mCurrMaterialIndex].weight[LIGHT_WEIGHT_TYPE_SPECULAR].g = pos.y;
    mModelList[mCurrMaterialIndex].weight[LIGHT_WEIGHT_TYPE_SPECULAR].b = pos.z;
}

glm::vec3 GLRenderer::getAmbient()
{
    return glm::vec3(mModelList[mCurrMaterialIndex].weight[LIGHT_WEIGHT_TYPE_AMBIENT].r, mModelList[mCurrMaterialIndex].weight[LIGHT_WEIGHT_TYPE_AMBIENT].g, mModelList[mCurrMaterialIndex].weight[LIGHT_WEIGHT_TYPE_AMBIENT].b);
}

glm::vec3 GLRenderer::getDiffuse()
{
    return glm::vec3(mModelList[mCurrMaterialIndex].weight[LIGHT_WEIGHT_TYPE_DIFFUSE].r, mModelList[mCurrMaterialIndex].weight[LIGHT_WEIGHT_TYPE_DIFFUSE].g, mModelList[mCurrMaterialIndex].weight[LIGHT_WEIGHT_TYPE_DIFFUSE].b);
}

glm::vec3 GLRenderer::getSpecular()
{
    return glm::vec3(mModelList[mCurrMaterialIndex].weight[LIGHT_WEIGHT_TYPE_SPECULAR].r, mModelList[mCurrMaterialIndex].weight[LIGHT_WEIGHT_TYPE_SPECULAR].g, mModelList[mCurrMaterialIndex].weight[LIGHT_WEIGHT_TYPE_SPECULAR].b);
}

void GLRenderer::setShiness(float val)
{
    mModelList[mCurrMaterialIndex].parameter[PHONG_PARAMETER_TYPE_SHINESS] = val;
}

float GLRenderer::getShiness()
{
    return mModelList[mCurrMaterialIndex].parameter[PHONG_PARAMETER_TYPE_SHINESS];
}

void GLRenderer::setMaterial(string materialName)
{
    mCurrMaterialIndex = mMaterialMap[materialName];
}

void GLRenderer::getMaterialList(vector<string> *materialList)
{
   materialList->resize(mMaterialList.size());
   std::copy(mMaterialList.begin(), mMaterialList.end(), materialList->begin());
}

