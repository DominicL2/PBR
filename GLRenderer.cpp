#include "GLRenderer.h"
#include "debugmacro.h"

GLRenderer::GLRenderer()
{
    initializeOpenGLFunctions();
    qDebug("[%s][%s:%d]", __FILE__, __func__, __LINE__);
    mLoadded = false;
    mType = SHADER_TYPE_PHONG;
    if (createContext() == GL_RENDERER_FAIL) {
        qDebug("[%s|%d] : Fail to create GL context", __func__, __LINE__);
    } else {
        qDebug("[%s][%s:%d]", __FILE__, __func__, __LINE__);
        mSpaceInfo.near.leftTop     = glm::vec3(-GL_SPACE_DEFUALT_DISTANCE_1KM, GL_SPACE_DEFUALT_DISTANCE_1KM, -GL_SPACE_DEFUALT_DISTANCE_1KM);
        mSpaceInfo.near.rightTop    = glm::vec3(GL_SPACE_DEFUALT_DISTANCE_1KM, GL_SPACE_DEFUALT_DISTANCE_1KM, -GL_SPACE_DEFUALT_DISTANCE_1KM);
        mSpaceInfo.near.leftBottom  = glm::vec3(-GL_SPACE_DEFUALT_DISTANCE_1KM, -GL_SPACE_DEFUALT_DISTANCE_1KM, -GL_SPACE_DEFUALT_DISTANCE_1KM);
        mSpaceInfo.near.rightBottom = glm::vec3(GL_SPACE_DEFUALT_DISTANCE_1KM, -GL_SPACE_DEFUALT_DISTANCE_1KM, -GL_SPACE_DEFUALT_DISTANCE_1KM);

        mSpaceInfo.far.leftTop      = glm::vec3(-GL_SPACE_DEFUALT_DISTANCE_1KM, GL_SPACE_DEFUALT_DISTANCE_1KM, GL_SPACE_DEFUALT_DISTANCE_1KM);
        mSpaceInfo.far.rightTop     = glm::vec3(GL_SPACE_DEFUALT_DISTANCE_1KM, GL_SPACE_DEFUALT_DISTANCE_1KM, GL_SPACE_DEFUALT_DISTANCE_1KM);
        mSpaceInfo.far.leftBottom   = glm::vec3(-GL_SPACE_DEFUALT_DISTANCE_1KM, -GL_SPACE_DEFUALT_DISTANCE_1KM, GL_SPACE_DEFUALT_DISTANCE_1KM);
        mSpaceInfo.far.rightBottom  = glm::vec3(GL_SPACE_DEFUALT_DISTANCE_1KM, -GL_SPACE_DEFUALT_DISTANCE_1KM, GL_SPACE_DEFUALT_DISTANCE_1KM);

        mSpaceInfo.fov = 100.f;
    }

    mModelList = mModelLoader.loadModel("/home/dmlee/3d_models/planet/planet_2_obj_.obj");

    mSpaceInfo.viewPoint.x = 0;
    mSpaceInfo.viewPoint.y = 27 * 1.5;
    mSpaceInfo.viewPoint.z = 27 * 1.1;
    mSpaceInfo.lightSource = glm::vec3(mSpaceInfo.viewPoint.x - 1000, mSpaceInfo.viewPoint.y + 600, mSpaceInfo.viewPoint.z + 400);

    //glm::vec3 normalSurface = glm::normalize(mModelData->size.max);
    glm::vec3 normalLight = glm::normalize(mSpaceInfo.lightSource - glm::vec3(0.0, 0.0, 0.0));
    glm::vec3 normalView = glm::normalize(mSpaceInfo.viewPoint - glm::vec3(0.0, 0.0, 0.0));


    qDebug("Light Pos : %f %f %f", mSpaceInfo.lightSource.x, mSpaceInfo.lightSource.y, mSpaceInfo.lightSource.z);
    qDebug("View Pos : %f %f %f", mSpaceInfo.viewPoint.x, mSpaceInfo.viewPoint.y, mSpaceInfo.viewPoint.z);
    //qDebug("Surface : %f %f %f", mModelData->size.max.x, mModelData->size.max.y, mModelData->size.max.z);

    //qDebug("Normalize Surface : %f %f %f", glm::normalize(mModelData->size.max).x, glm::normalize(mModelData->size.max).y, glm::normalize(mModelData->size.max).z);
    qDebug("Normalize Light : %f %f %f", normalLight.x, normalLight.y, normalLight.z);
    qDebug("Normalize View : %f %f %f", normalView.x, normalView.y, normalView.z);


}

int32_t GLRenderer::init()
{
    int32_t ret = GL_RENDERER_FAIL;

    int32_t i = 0;

    if (mLoadded) {
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
        mLoadded = false;
    } else {}

    return ret;
}

int32_t GLRenderer::load(string path)
{
    int32_t ret = GL_RENDERER_FAIL;

    mModelList = mModelLoader.loadModel(path);

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
    if (mLoadded) {
        init();
    } else {}
    mContext.program = glCreateProgram();
    if (mContext.program == 0) {
        return ret;
    } else {}
    switch (mType) {
    case SHADER_TYPE_PHONG :
        mContext.shader[GLES_SHADER_TYPE_VERTEX]    = registerShader(VERTEX_SHADER_PHONG_STR, GL_VERTEX_SHADER);

        mContext.shader[GLES_SHADER_TYPE_FRAGMENT]  = registerShader(FRAGMENT_SHADER_PHONG_STR, GL_FRAGMENT_SHADER);
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
    return ret;
}

void GLRenderer::draw(const ModelData *modelData)
{
    glViewport(0, 0, mViewPort.width(), mViewPort.height());
    glm::mat4 mvpMatrix         = glm::mat4(1.0f);
    glm::mat4 mvMatrix          = glm::mat4(1.0f);
    glm::mat4 modelMatrix       = glm::mat4(1.0f);
    glm::mat4 viewMatrix        = glm::mat4(1.0f);
    glm::mat4 projectionMatrix  = glm::mat4(1.0f);

    projectionMatrix = glm::perspective(glm::radians(mSpaceInfo.fov), (float)mViewPort.width() /  (float)mViewPort.height(), 0.1f, 500000.f);
    viewMatrix = glm::lookAt(mSpaceInfo.viewPoint, glm::vec3(0,
                                                             0,
                                                             0), GLSpace::getUpVector());
    //modelMatrix = glm::scale(modelMatrix, glm::vec3(50.0 / mModelData->size.length.x , 50.0 / mModelData->size.length.y, 50.0 / mModelData->size.length.z));
    modelMatrix = glm::scale(modelMatrix, glm::vec3(1.0, 1.0, 1.0));
    glClearColor(0.0, 0.f, 0.f, 0.f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    mvpMatrix = projectionMatrix * viewMatrix * modelMatrix;
    mvMatrix = viewMatrix * modelMatrix;

    // Upate Uniform
    glUseProgram(mContext.program);
    glUniformMatrix4fv(mContext.uniform[PHONG_SHADER_UNIFORM_MVP], 1, GL_FALSE, (const float *)&mvpMatrix[0][0]);
    glUniformMatrix4fv(mContext.uniform[PHONG_SHADER_UNIFORM_MV], 1, GL_FALSE, (const float *)&mvMatrix[0][0]);

    glUniform3f(mContext.uniform[PHONG_SHADER_UNIFORM_LIGHT_POS], mSpaceInfo.lightSource.x, mSpaceInfo.lightSource.y, mSpaceInfo.lightSource.z);
    glUniform3f(mContext.uniform[PHONG_SHADER_UNIFORM_VIEW_POS], mSpaceInfo.viewPoint.x, mSpaceInfo.viewPoint.y, mSpaceInfo.viewPoint.z);

    glUniform1f(mContext.uniform[PHONG_SHADER_UNIFORM_AMBIENT], 0.38);
    glUniform1f(mContext.uniform[PHONG_SHADER_UNIFORM_DIFFUSE], 0.54);
    glUniform1f(mContext.uniform[PHONG_SHADER_UNIFORM_SPECULAR], 0.6);

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
    for (int i = 0; i < baseColor->second.size(); i++) {
        glActiveTexture(GL_TEXTURE0 + i);
        glBindTexture(GL_TEXTURE_2D, baseColor->second[i]);
    }

    glDrawElements(GL_TRIANGLE_STRIP, modelData->indices.size(), GL_UNSIGNED_INT, 0);

    glBindBuffer(GL_ARRAY_BUFFER,0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,0);
    glBindTexture(GL_TEXTURE_2D, 0);
    glUseProgram(0);
}

void GLRenderer::paint()
{
    for (const auto& modelData : *mModelList) {
        draw(&modelData);
    }
}
