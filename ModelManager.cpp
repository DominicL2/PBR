#include "ModelManager.h"
#include <assimp/postprocess.h>
#include <QDir>
#include <string>
#include <iostream>
#include <QPixmap>
#include "debugmacro.h"

ModelManager::ModelManager()
{

}

ModelManager::~ModelManager()
{

}

QStringList ModelManager::getTextureFilePath(string path)
{    
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
            textureFilePath << fileList[i].absoluteFilePath();
        }
    }
    return textureFilePath;
}

aiTextureType ModelManager::getTextureTypeUsingFileName(string name)
{
    aiTextureType type = aiTextureType_NONE;

    for (int i = aiTextureType_DIFFUSE; i < aiTextureType_UNKNOWN; i++) {
        switch (i) {
        case aiTextureType_BASE_COLOR :
            if (strstr(name.c_str(), "BaseColor") != NULL) {
                type = aiTextureType_BASE_COLOR;
            } else {}
            break;
        case aiTextureType_NORMALS :
            if (strstr(name.c_str(), "Normal") != NULL) {
                type = aiTextureType_NORMALS;
            } else {}
            break;
        case aiTextureType_METALNESS :
            if (strstr(name.c_str(), "Metallic") != NULL) {
                type = aiTextureType_METALNESS;
            } else {}
            break;
        case aiTextureType_EMISSIVE :
            if (strstr(name.c_str(), "Emissive") != NULL) {
                type = aiTextureType_EMISSIVE;
            } else {}
            break;
        case aiTextureType_HEIGHT :
            if (strstr(name.c_str(), "Height") != NULL) {
                type = aiTextureType_HEIGHT;
            } else {}
            break;
        case aiTextureType_DIFFUSE_ROUGHNESS :
            if (strstr(name.c_str(), "Roughness") != NULL) {
                type = aiTextureType_DIFFUSE_ROUGHNESS;
            } else {}
            break;
        default :
            break;
        }

        if (type != TEXTURE_TYPE_NONE) {
            break;
        } else {}
    }

    return type;
}

string ModelManager::getBasePath(string fullPath)
{
    int32_t pos = fullPath.find_last_of("/") + 1;

    return fullPath.substr(0, pos);
}

GLuint ModelManager::getVboId(int32_t type, ModelData *modelData)
{
    GLuint vboId = -1;

    glGenBuffers(1, &vboId);

    switch (type) {
    case VBO_ID_TYPE_VERTEX :
        glBindBuffer(GL_ARRAY_BUFFER, vboId);
        glBufferData(GL_ARRAY_BUFFER, modelData->vertices.size() * sizeof(glm::vec3), &modelData->vertices[0], GL_STATIC_DRAW);
        glBindBuffer(GL_ARRAY_BUFFER,0);
        break;
    case VBO_ID_TYPE_NORMAL :
        glBindBuffer(GL_ARRAY_BUFFER, vboId);
        glBufferData(GL_ARRAY_BUFFER, modelData->normals.size() * sizeof(glm::vec3), &modelData->normals[0], GL_STATIC_DRAW);
        glBindBuffer(GL_ARRAY_BUFFER,0);
        break;
    case VBO_ID_TYPE_TANGENT :
        glBindBuffer(GL_ARRAY_BUFFER, vboId);
        glBufferData(GL_ARRAY_BUFFER, modelData->tangents.size() * sizeof(glm::vec3), &modelData->tangents[0], GL_STATIC_DRAW);
        glBindBuffer(GL_ARRAY_BUFFER,0);
        break;
    case VBO_ID_TYPE_BITANGENT :
        glBindBuffer(GL_ARRAY_BUFFER, vboId);
        glBufferData(GL_ARRAY_BUFFER, modelData->biTangents.size() * sizeof(glm::vec3), &modelData->biTangents[0], GL_STATIC_DRAW);
        glBindBuffer(GL_ARRAY_BUFFER,0);
        break;
    case VBO_ID_TYPE_TEXCOORD :
        glBindBuffer(GL_ARRAY_BUFFER, vboId);
        glBufferData(GL_ARRAY_BUFFER, modelData->texcoord.size() * sizeof(modelData->texcoord[0]), &modelData->texcoord[0], GL_STATIC_DRAW);
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

GLuint ModelManager::getTextureId(string path)
{
    GLuint id           = 0U;
    QImage *srcImg = new QImage();
    std::replace(path.begin(), path.end(), '\\', '/');
    path = path.substr((int)path.find("/", 0) + 1, path.length());

    srcImg->load((mDirectoryPath + path).c_str());
    *srcImg = srcImg->convertToFormat(QImage::Format_RGBA8888);
    qDebug("Texture : %s - %d %d %d", (mDirectoryPath + path).c_str(), srcImg->bitPlaneCount(), srcImg->depth(), srcImg->format());
    glGenTextures(1, &id);
    glBindTexture(GL_TEXTURE_2D, id);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, srcImg->width(), srcImg->height(), 0, GL_RGBA, GL_UNSIGNED_BYTE, srcImg->bits());
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glBindTexture(GL_TEXTURE_2D, 0);
    delete srcImg;

    mTextureMap[path.c_str()] = id;
    return id;
}

void ModelManager::loadTexture(vector<GLuint> *textureId, string path)
{
    GLuint texId = getTextureId(path);
    if (texId > 0) {
        textureId->push_back(texId);
    }
}

ModelData ModelManager::parseModel(const aiScene *scene, aiMesh* mesh, uint32_t meshIndex)
{
    ModelData modelData;
    modelData.objectName = mesh->mName.C_Str();

    for (uint32_t v = 0U; v < mesh->mNumVertices; v++) {
        modelData.size.max.x = (mesh->mVertices[v].x > modelData.size.max.x) ? mesh->mVertices[v].x : modelData.size.max.x;
        modelData.size.max.y = (mesh->mVertices[v].y > modelData.size.max.y) ? mesh->mVertices[v].y  : modelData.size.max.y;
        modelData.size.max.z = (mesh->mVertices[v].z > modelData.size.max.z) ? mesh->mVertices[v].z : modelData.size.max.z;
        modelData.size.min.x = (mesh->mVertices[v].x < modelData.size.min.x) ? mesh->mVertices[v].x : modelData.size.min.x;
        modelData.size.min.y = (mesh->mVertices[v].y < modelData.size.min.y) ? mesh->mVertices[v].y  : modelData.size.min.y;
        modelData.size.min.z = (mesh->mVertices[v].z < modelData.size.min.z) ? mesh->mVertices[v].z  : modelData.size.min.z;

        modelData.vertices.push_back(glm::vec3(     mesh->mVertices[v].x,
                                                    mesh->mVertices[v].y,
                                                    mesh->mVertices[v].z));
        modelData.normals.push_back(glm::vec3(      mesh->mNormals[v].x,
                                                    mesh->mNormals[v].y,
                                                    mesh->mNormals[v].z));

       modelData.tangents.push_back(glm::vec3(      mesh->mTangents[v].x,
                                                    mesh->mTangents[v].y,
                                                    mesh->mTangents[v].z));

       modelData.biTangents.push_back(glm::vec3(    mesh->mBitangents[v].x,
                                                    mesh->mBitangents[v].y,
                                                    mesh->mBitangents[v].z));

        if (mesh->HasTextureCoords(meshIndex)) {
            modelData.texcoord.push_back(glm::vec2(     mesh->mTextureCoords[0][v].x,
                                                        mesh->mTextureCoords[0][v].y));
        }
    }

    modelData.size.length.x = abs(modelData.size.max.x - modelData.size.min.x);
    modelData.size.length.y = abs(modelData.size.max.y - modelData.size.min.y);
    modelData.size.length.z = abs(modelData.size.max.z - modelData.size.min.z);

    for (uint32_t f = 0U; f < mesh->mNumFaces; f++) {
        aiFace face = mesh->mFaces[f];
        for (uint32_t i = 0; i < face.mNumIndices; i++) {
            modelData.indices.push_back(face.mIndices[i]);
        }
    }

    modelData.vboId[VBO_ID_TYPE_VERTEX]     =  getVboId(VBO_ID_TYPE_VERTEX,     &modelData);
    modelData.vboId[VBO_ID_TYPE_NORMAL]     =  getVboId(VBO_ID_TYPE_NORMAL,     &modelData);
    if (mesh->HasTextureCoords(meshIndex)) {
        modelData.vboId[VBO_ID_TYPE_TEXCOORD]   =  getVboId(VBO_ID_TYPE_TEXCOORD,   &modelData);
    } else {
        modelData.vboId[VBO_ID_TYPE_TEXCOORD] = 0;
    }
    modelData.vboId[VBO_ID_TYPE_INDEX]      =  getVboId(VBO_ID_TYPE_INDEX,      &modelData);
    modelData.vboId[VBO_ID_TYPE_TANGENT]    =  getVboId(VBO_ID_TYPE_TANGENT,    &modelData);
    modelData.vboId[VBO_ID_TYPE_BITANGENT]  =  getVboId(VBO_ID_TYPE_BITANGENT,  &modelData);

    if (mesh->mMaterialIndex >= 0U) {
        aiMaterial *material = scene->mMaterials[mesh->mMaterialIndex];
        modelData.materialName = material->GetName().C_Str();
        material->Get(AI_MATKEY_COLOR_AMBIENT, modelData.weight[LIGHT_WEIGHT_TYPE_AMBIENT]);
        material->Get(AI_MATKEY_COLOR_DIFFUSE, modelData.weight[LIGHT_WEIGHT_TYPE_DIFFUSE]);
        material->Get(AI_MATKEY_COLOR_SPECULAR, modelData.weight[LIGHT_WEIGHT_TYPE_SPECULAR]);

        float shiness = 0.0;
        if (material->Get(AI_MATKEY_SHININESS, shiness) == aiReturn_SUCCESS) {
            modelData.parameter.push_back(shiness);
            modelData.textures.clear();
        }

        for (int type = aiTextureType_DIFFUSE; type < aiTextureType_UNKNOWN; type++) {
            modelData.textures[(aiTextureType)type].clear();
            for (uint32_t i = 0; i < material->GetTextureCount((aiTextureType)type); i++) {
                aiString path;
                material->GetTexture((aiTextureType)type, i, &path);
                if (mTextureMap[path.C_Str()] != 0U) {
                    modelData.textures[(aiTextureType)type].push_back(mTextureMap[path.C_Str()]);
                } else {
                    modelData.textures[(aiTextureType)type].push_back(getTextureId(path.C_Str()));
                }
            }
        }
    }

    return modelData;
}

void ModelManager::init(ModelData* model) {
    model->vertices.clear();
    model->normals.clear();
    model->tangents.clear();
    model->biTangents.clear();
    model->texcoord.clear();
    model->indices.clear();
    model->parameter.clear();
    model->textures.clear();
    vector<glm::vec3>().swap(model->vertices);
    vector<glm::vec3>().swap(model->normals);
    vector<glm::vec3>().swap(model->tangents);
    vector<glm::vec3>().swap(model->biTangents);
    vector<glm::vec2>().swap(model->texcoord);
    vector<uint32_t>().swap(model->indices);
    vector<float>().swap(model->parameter);

    for (auto iter = model->textures.begin(); iter != model->textures.end(); iter++) {
        for (GLuint i = 0; i < iter->second.size(); i++) {
            glDeleteTextures(1, &iter->second[i]);
        }
        iter->second.clear();
        vector<GLuint>().swap(iter->second);
    }
    model->textures.clear();
    unordered_map<aiTextureType, vector<GLuint>>().swap(model->textures);

    glDeleteBuffers(NUM_VBO_ID_TYPE, model->vboId);
    for (int i = 0; i < NUM_VBO_ID_TYPE; i++) {
        model->vboId[i] = 0U;
    }
    model->size.length  = glm::vec3();
    model->size.min     = glm::vec3();
    model->size.max     = glm::vec3();

    for (int i = 0; i < NUM_LIGHT_WEIGHT_TYPE; i++) {
        model->weight[i] = aiColor3D(0, 0, 0);
    }

    model->objectName.clear();
    model->materialName.clear();
}

void ModelManager::loadModel(string path, vector<ModelData> *modelList)
{
    uint i = 0U;
    uint j = 0U;

    if (modelList->size() > 0) {
        modelList->clear();
        vector<ModelData>().swap(*modelList);
        mTextureMap.clear();
    }

    mDirectoryPath = getBasePath(path);

    Assimp::Importer importer;
    const aiScene* scene = importer.ReadFile(   path,
                                              aiProcess_Triangulate |
                                              aiProcess_CalcTangentSpace |
                                              aiProcess_FlipUVs);

    const aiNode *node = scene->mRootNode;
    aiMesh* mesh;
    for (i = 0U; node->mNumMeshes; i++) {
        mesh = scene->mMeshes[node->mMeshes[i]];
        modelList->push_back(parseModel(scene, mesh, node->mMeshes[i]));
    }

    for (i = 0U; i < node->mNumChildren; i++) {
        const aiNode *childrenNode = node->mChildren[i];
        for (j = 0U; j < childrenNode->mNumMeshes; j++) {
            mesh = scene->mMeshes[childrenNode->mMeshes[j]];
            modelList->push_back(parseModel(scene, mesh, childrenNode->mMeshes[j]));
        }
    }
}
