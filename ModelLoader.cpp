#include "ModelLoader.h"
#include <assimp/postprocess.h>
#include <QDir>
#include <string>
#include <QPixmap>
#include "debugmacro.h"

ModelLoader::ModelLoader()
{

}

void ModelLoader::initModelData()
{
    mModelData.vertices.clear();
    mModelData.normals.clear();
    mModelData.indices.clear();
    mModelData.texcoord.clear();
    vector<glm::vec3>().swap(mModelData.vertices);
    vector<glm::vec3>().swap(mModelData.normals);
    //vector<GLTriangle>().swap(mModelData.indices);
    vector<uint32_t>().swap(mModelData.indices);
    vector<glm::vec2>().swap(mModelData.texcoord);
    for (int i = 0; i < NUM_VBO_ID_TYPE; i++) {
        glDeleteBuffers(i, &mModelData.vboId[i]);
        mModelData.vboId[i] = 0;
    }

}
QStringList ModelLoader::getTextureFilePath(string path)
{
    //string basePath = getBasePath(path);
    qDebug("[%s|%d] Texture Base Path : %s", __func__, __LINE__, path.c_str());
    QStringList textureFilePath;
    QDir directoryPath(path.c_str());
    if (directoryPath.exists()) {
        QStringList filter;
        filter << QLatin1String("*.png");
        filter << QLatin1String("*.jpg");
        filter << QLatin1String("*.jpeg");
        directoryPath.setNameFilters(filter);
        QFileInfoList fileList = directoryPath.entryInfoList();
        for (int i = 0; i < fileList.size(); i++) {
            qDebug("[%s|%d] %d. Texture Path : %s", __func__, __LINE__, i, fileList[i].absoluteFilePath().toStdString().c_str());
            textureFilePath << fileList[i].absoluteFilePath();
        }
    }
    return textureFilePath;
}

TEXTURE_TYPE ModelLoader::getTextureTypeUsingFileName(string name)
{
    TEXTURE_TYPE type = TEXTURE_TYPE_NONE;

    for (int i = TEXTURE_TYPE_BASE_COLOR; i < NUM_TEXTURE_TYPE; i++) {
        switch (i) {
        case TEXTURE_TYPE_BASE_COLOR :
            if (strstr(name.c_str(), "BaseColor") != NULL) {
                type = TEXTURE_TYPE_BASE_COLOR;
            } else {}
            break;
        case TEXTURE_TYPE_NORMAL :
            if (strstr(name.c_str(), "Normal") != NULL) {
                type = TEXTURE_TYPE_NORMAL;
            } else {}
            break;
        case TEXTURE_TYPE_METALLIC :
            if (strstr(name.c_str(), "Metallic") != NULL) {
                type = TEXTURE_TYPE_METALLIC;
            } else {}
            break;
        case TEXTURE_TYPE_EMISSIVE :
            if (strstr(name.c_str(), "Emissive") != NULL) {
                type = TEXTURE_TYPE_EMISSIVE;
            } else {}
            break;
        case TEXTURE_TYPE_HEIGHT :
            if (strstr(name.c_str(), "Height") != NULL) {
                type = TEXTURE_TYPE_HEIGHT;
            } else {}
            break;
        case TEXTURE_TYPE_ROUGHNESS :
            if (strstr(name.c_str(), "Roughness") != NULL) {
                type = TEXTURE_TYPE_ROUGHNESS;
            } else {}
            break;
        }

        if (type != TEXTURE_TYPE_NONE) {
            break;
        } else {}
    }

    return type;
}

void ModelLoader::loadMaterialTexture(QStringList paths)
{
    GLuint id           = 0U;
    TEXTURE_TYPE type   = TEXTURE_TYPE_NONE;

    for (int32_t i = 0; i < (int32_t)paths.size(); i++) {
        qDebug("[%d] Texture Path : %s", i, paths[i].toStdString().c_str());
        QImage *srcImg = new QImage();
        srcImg->load(paths[i]);
        qDebug("Resolution : %dx%d - %d", srcImg->width(), srcImg->height(), srcImg->depth() ? 1 : 0);
        glGenTextures(1, &id);
        glBindTexture(GL_TEXTURE_2D, id);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, srcImg->width(), srcImg->height(), 0, GL_RGBA, GL_UNSIGNED_BYTE, srcImg->bits());
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glBindTexture(GL_TEXTURE_2D, 0);
        type = getTextureTypeUsingFileName(paths[i].toStdString());
        qDebug("Type:%d | Id:%d", type, id);
        mModelData.textures[type] = id;
        delete srcImg;
    }
}

string ModelLoader::getBasePath(string fullPath)
{
    int32_t pos = fullPath.find_last_of("/") + 1;

    return fullPath.substr(0, pos);
}

GLuint ModelLoader::getVboId(int32_t type, ModelData *modelData, GLuint attribute)
{
    GLuint vboId = -1;

    glGenBuffers(1, &vboId);

    switch (type) {
    case VBO_ID_TYPE_VERTEX :
        glBindBuffer(GL_ARRAY_BUFFER, vboId);
        glBufferData(GL_ARRAY_BUFFER, modelData->vertices.size() * sizeof(glm::vec3), &modelData->vertices[0], GL_STATIC_DRAW);
        glVertexAttribPointer(attribute, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), (void*)0);
        glEnableVertexAttribArray(attribute);
        break;
    case VBO_ID_TYPE_NORMAL :
        glBindBuffer(GL_ARRAY_BUFFER, vboId);
        glBufferData(GL_ARRAY_BUFFER, modelData->normals.size() * sizeof(modelData->normals[0]), &modelData->normals[0], GL_STATIC_DRAW);
        glVertexAttribPointer(attribute, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), (void*)0);
        glEnableVertexAttribArray(attribute);
        glBindBuffer(GL_ARRAY_BUFFER,0);
        break;
    case VBO_ID_TYPE_TEXCOORD :
        glBindBuffer(GL_ARRAY_BUFFER, vboId);
        glBufferData(GL_ARRAY_BUFFER, modelData->texcoord.size() * sizeof(modelData->texcoord[0]), &modelData->texcoord[0], GL_STATIC_DRAW);
        glVertexAttribPointer(attribute, 2, GL_FLOAT, GL_FALSE, sizeof(glm::vec2), (void*)0);
        glEnableVertexAttribArray(attribute);
        glBindBuffer(GL_ARRAY_BUFFER,0);
        break;
    case VBO_ID_TYPE_INDEX :
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vboId);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, modelData->indices.size() * sizeof(uint32_t), &modelData->indices[0], GL_STATIC_DRAW);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,0);
        break;
    default :
        break;
    }


    return vboId;
}

ModelData* ModelLoader::loadModel(string path)
{
    initModelData();
    Assimp::Importer importer;
    const aiScene* mScene = importer.ReadFile(path,
                                              aiProcess_Triangulate |
                                             aiProcess_SortByPType |
                                              aiProcess_FlipUVs);
    const aiMesh* mMesh = mScene->mMeshes[0];

    qDebug("Mesh Size   : %d", mScene->mNumMeshes);
    qDebug("Mesh Vertex : %d", mMesh->mNumVertices);
    qDebug("Mesh Face   : %d", mMesh->mNumFaces);

    for (uint32_t v = 0; v < mMesh->mNumVertices; v++) {
        mModelData.size.max.x = (mMesh->mVertices[v].x > mModelData.size.max.x) ? mMesh->mVertices[v].x : mModelData.size.max.x;
        mModelData.size.max.y = (mMesh->mVertices[v].y > mModelData.size.max.y) ? mMesh->mVertices[v].y  : mModelData.size.max.y;
        mModelData.size.max.z = (mMesh->mVertices[v].z > mModelData.size.max.z) ? mMesh->mVertices[v].z : mModelData.size.max.z;
        mModelData.size.min.x = (mMesh->mVertices[v].x  < mModelData.size.min.x) ? mMesh->mVertices[v].x : mModelData.size.min.x;
        mModelData.size.min.y = (mMesh->mVertices[v].y  < mModelData.size.min.y) ? mMesh->mVertices[v].y  : mModelData.size.min.y;
        mModelData.size.min.z = (mMesh->mVertices[v].z  < mModelData.size.min.z) ? mMesh->mVertices[v].z  : mModelData.size.min.z;

        mModelData.vertices.push_back(glm::vec3(    mMesh->mVertices[v].x,
                                                    mMesh->mVertices[v].y,
                                                    mMesh->mVertices[v].z));
        mModelData.normals.push_back(glm::vec3(     mMesh->mNormals[v].x,
                                                    mMesh->mNormals[v].y,
                                                    mMesh->mNormals[v].z));
        mModelData.texcoord.push_back(glm::vec2(mMesh->mTextureCoords[0][v].x, mMesh->mTextureCoords[0][v].y));
    }

    qDebug("Min : %f %f %f", mModelData.size.min.x, mModelData.size.min.y, mModelData.size.min.z);
    qDebug("Max : %f %f %f", mModelData.size.max.x, mModelData.size.max.y, mModelData.size.max.z);
    mModelData.size.length.x = abs(mModelData.size.max.x - mModelData.size.min.x);
    mModelData.size.length.y = abs(mModelData.size.max.y - mModelData.size.min.y);
    mModelData.size.length.z = abs(mModelData.size.max.z - mModelData.size.min.z);
    qDebug("Length : %f %f %f", mModelData.size.length.x, mModelData.size.length.y, mModelData.size.length.z);

    for (uint32_t f = 0U; f < mMesh->mNumFaces; f++) {
        aiFace face = mMesh->mFaces[f];
        for (int i = 0; i < face.mNumIndices; i++) {
            mModelData.indices.push_back(face.mIndices[i]);
        }
    }

    if (mMesh->mMaterialIndex >= 0) {
        //aiMaterial *material = mScene->mMaterials[mMesh->mMaterialIndex];
        string basePath = getBasePath(path);
        QStringList textureFileList = getTextureFilePath((basePath + "/texture").c_str());
        loadMaterialTexture(textureFileList);
        qDebug("[%s|%d] : texture size = %d", __func__, __LINE__, mModelData.textures.size());
    }
    return &mModelData;
}
