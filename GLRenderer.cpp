#include "GLRenderer.h"
#include "debugmacro.h"

GLRenderer::GLRenderer()
{
    initializeOpenGLFunctions();
    qDebug("[%s][%s:%d]", __FILE__, __func__, __LINE__);
    mLoadded = false;
    mType = SHADER_TYPE_DEFAULT;
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

        mSpaceInfo.lightSource = glm::vec3(GL_SPACE_DEFUALT_LIGHT_SOURCE_POS_X,
                                           GL_SPACE_DEFUALT_LIGHT_SOURCE_POS_Y,
                                           GL_SPACE_DEFUALT_LIGHT_SOURCE_POS_Z);


        mSpaceInfo.fov = 100.f;
    }

    mModelData = mModelLoader.loadModel("/home/dmlee/3d_models/planet/planet_2_obj_.obj");

    mModelData->vboId[VBO_ID_TYPE_VERTEX] =  mModelLoader.getVboId(VBO_ID_TYPE_VERTEX, mModelData, mContext.attribute[DEFAULT_SHADER_ATTR_POSTION]);
    mModelData->vboId[VBO_ID_TYPE_TEXCOORD] =  mModelLoader.getVboId(VBO_ID_TYPE_TEXCOORD, mModelData, mContext.attribute[DEFAULT_SHADER_ATTR_TEXCOORD]);
    mModelData->vboId[VBO_ID_TYPE_INDEX] =  mModelLoader.getVboId(VBO_ID_TYPE_INDEX, mModelData);


    mSpaceInfo.viewPoint.x = 0;
    mSpaceInfo.viewPoint.y = mModelData->size.max.y * 1.5;
    mSpaceInfo.viewPoint.z = mModelData->size.max.z * 1.1;
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

    mModelData = mModelLoader.loadModel(path);

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
    case SHADER_TYPE_DEFAULT :
        attributeId = glGetAttribLocation(mContext.program, "a_position");
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

    qDebug("[%s] : %d", __func__, ret);
    return ret;
}

int32_t GLRenderer::registerUniform(SHADER_TYPE type)
{
    int32_t ret = GL_RENDERER_FAIL;

    GLint uniformId = -1;

    switch (type) {
    case SHADER_TYPE_DEFAULT :
        uniformId = glGetUniformLocation(mContext.program, "u_mvp");
        if (uniformId >= 0) {
            mContext.uniform.push_back(uniformId);
        } else {
            break;
        }
#if 0
        uniformId = glGetUniformLocation(mContext.program, "u_normal");
        if (uniformId >= 0) {
            mContext.uniform.push_back(uniformId);
        } else {
            break;
        }
#endif
        ret = GL_RENDERER_SUCCESS;
        break;
    case SHADER_TYPE_BRDF :
        break;
    default:
        break;
    }
    qDebug("[%s] : %d", __func__, ret);
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
        qDebug("%s : %s", errMsg.data(), error);
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
    case SHADER_TYPE_DEFAULT :
        mContext.shader[GLES_SHADER_TYPE_VERTEX]    = registerShader(VERTEX_SHADER_DEFAULT_STR, GL_VERTEX_SHADER);
        mContext.shader[GLES_SHADER_TYPE_FRAGMENT]  = registerShader(FRAGMENT_SHADER_DEFAULT_STR, GL_FRAGMENT_SHADER);
        qDebug("Program(%d)", mContext.program);
        qDebug("Shader V(%d) F(%d)", mContext.shader[GLES_SHADER_TYPE_VERTEX], mContext.shader[GLES_SHADER_TYPE_FRAGMENT]);
        if (connectShader2Program() == GL_RENDERER_FAIL) {
            break;
        } else {}
        if (registerAttribute(SHADER_TYPE_DEFAULT) == GL_RENDERER_SUCCESS && registerUniform(SHADER_TYPE_DEFAULT) == GL_RENDERER_SUCCESS) {
            ret = GL_RENDERER_SUCCESS;
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

void GLRenderer::paint()
{

    glViewport(0, 0, mViewPort.width(), mViewPort.height());
    glm::mat4 mvpMatrix         = glm::mat4(1.0f);
    glm::mat4 modelMatrix       = glm::mat4(1.0f);
    glm::mat4 viewMatrix        = glm::mat4(1.0f);
    glm::mat4 projectionMatrix  = glm::mat4(1.0f);

    projectionMatrix = glm::perspective(glm::radians(mSpaceInfo.fov), (float)mViewPort.width() /  (float)mViewPort.height(), 0.1f, 500000.f);
    viewMatrix = glm::lookAt(mSpaceInfo.viewPoint, glm::vec3(0,
                                                             0,
                                                             0), GLSpace::getUpVector());
    //modelMatrix = glm::scale(modelMatrix, glm::vec3(50.0 / mModelData->size.length.x , 50.0 / mModelData->size.length.y, 50.0 / mModelData->size.length.z));
    modelMatrix = glm::scale(modelMatrix, glm::vec3(1.0, 1.0, 1.0));
    mvpMatrix = projectionMatrix * viewMatrix * modelMatrix;
    glClearColor(0.0, 0.f, 0.f, 0.f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Upate Uniform
    glUseProgram(mContext.program);
    glUniformMatrix4fv(mContext.uniform[DEFAULT_SHADER_UNIFORM_MVP], 1, GL_FALSE, (const float *)&mvpMatrix[0][0]);

    glBindBuffer(GL_ARRAY_BUFFER, mModelData->vboId[VBO_ID_TYPE_VERTEX]);
    glVertexAttribPointer(mContext.attribute[DEFAULT_SHADER_ATTR_POSTION], 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), (void*)0);
    glEnableVertexAttribArray(mContext.attribute[DEFAULT_SHADER_ATTR_POSTION]);

    glBindBuffer(GL_ARRAY_BUFFER, mModelData->vboId[VBO_ID_TYPE_TEXCOORD]);
    glVertexAttribPointer(mContext.attribute[DEFAULT_SHADER_ATTR_TEXCOORD], 2, GL_FLOAT, GL_FALSE, sizeof(glm::vec2), (void*)0);
    glEnableVertexAttribArray(mContext.attribute[DEFAULT_SHADER_ATTR_TEXCOORD]);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mModelData->vboId[VBO_ID_TYPE_INDEX]);

    auto baseColor = mModelData->textures.find(TEXTURE_TYPE_BASE_COLOR);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, baseColor->second);
    glDrawElements(GL_TRIANGLE_STRIP, mModelData->indices.size(), GL_UNSIGNED_INT, 0);

    glBindBuffer(GL_ARRAY_BUFFER,0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,0);
    glBindTexture(GL_TEXTURE_2D, 0);
    glUseProgram(0);
}
