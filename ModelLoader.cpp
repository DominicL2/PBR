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
#if 0
    mModelData.vertices.clear();
    mModelData.normals.clear();
    mModelData.indices.clear();
    mModelData.texcoord.clear();
    mModelData.textures.clear();
    vector<glm::vec3>().swap(mModelData.vertices);
    vector<glm::vec3>().swap(mModelData.normals);
    vector<uint32_t>().swap(mModelData.indices);
    vector<glm::vec2>().swap(mModelData.texcoord);
    map<TEXTURE_TYPE, GLuint>().swap(mModelData.textures);
    for (int i = 0; i < NUM_VBO_ID_TYPE; i++) {
        glDeleteBuffers(i, &mModelData.vboId[i]);
        mModelData.vboId[i] = 0;
    }
#endif
}
QStringList ModelLoader::getTextureFilePath(string path)
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

aiTextureType ModelLoader::getTextureTypeUsingFileName(string name)
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

string ModelLoader::getBasePath(string fullPath)
{
    int32_t pos = fullPath.find_last_of("/") + 1;

    return fullPath.substr(0, pos);
}

GLuint ModelLoader::getVboId(int32_t type, ModelData *modelData)
{
    GLuint vboId = -1;

    glGenBuffers(1, &vboId);

    switch (type) {
    case VBO_ID_TYPE_VERTEX :
        glBindBuffer(GL_ARRAY_BUFFER, vboId);
        glBufferData(GL_ARRAY_BUFFER, modelData->vertices.size() * sizeof(glm::vec3), &modelData->vertices[0], GL_STATIC_DRAW);
        //glVertexAttribPointer(attribute, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), (void*)0);
        //glEnableVertexAttribArray(attribute);
        glBindBuffer(GL_ARRAY_BUFFER,0);
        break;
    case VBO_ID_TYPE_NORMAL :
        glBindBuffer(GL_ARRAY_BUFFER, vboId);
        glBufferData(GL_ARRAY_BUFFER, modelData->normals.size() * sizeof(glm::vec3), &modelData->normals[0], GL_STATIC_DRAW);
        //glVertexAttribPointer(attribute, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), (void*)0);
        //glEnableVertexAttribArray(attribute);
        glBindBuffer(GL_ARRAY_BUFFER,0);
        break;
    case VBO_ID_TYPE_TEXCOORD :
        glBindBuffer(GL_ARRAY_BUFFER, vboId);
        glBufferData(GL_ARRAY_BUFFER, modelData->texcoord.size() * sizeof(modelData->texcoord[0]), &modelData->texcoord[0], GL_STATIC_DRAW);
        //glVertexAttribPointer(attribute, 2, GL_FLOAT, GL_FALSE, sizeof(glm::vec2), (void*)0);
        //glEnableVertexAttribArray(attribute);
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

GLuint ModelLoader::getTextureId(aiString path)
{
    GLuint id           = 0U;
    QImage *srcImg = new QImage();    
    srcImg->load((mDirectoryPath + "texture/" + path.C_Str()).c_str());
    qDebug("Resolution : %dx%d - %d", srcImg->width(), srcImg->height(), srcImg->depth() ? 1 : 0);
    glGenTextures(1, &id);
    glBindTexture(GL_TEXTURE_2D, id);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, srcImg->width(), srcImg->height(), 0, GL_RGBA, GL_UNSIGNED_BYTE, srcImg->bits());
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glBindTexture(GL_TEXTURE_2D, 0);
    delete srcImg;

    return id;
}

void ModelLoader::loadTexture(aiMaterial *material, vector<GLuint> *textureId, aiTextureType type)
{
    qDebug("Tpye : %d",  material->GetTextureCount(type));
    for (uint32_t i = 0; i < material->GetTextureCount(type); i++) {
        aiString path;
        material->GetTexture(type, i, &path);
        textureId->push_back(getTextureId(path));
    }
}

ModelData ModelLoader::parseModel(const aiScene *scene, aiMesh* mesh)
{
    ModelData modelData;
    qDebug("Mesh Vertex : %d", mesh->mNumVertices);
    qDebug("Mesh Face   : %d", mesh->mNumFaces);

    for (uint32_t v = 0U; v < mesh->mNumVertices; v++) {
        modelData.size.max.x = (mesh->mVertices[v].x > modelData.size.max.x) ? mesh->mVertices[v].x : modelData.size.max.x;
        modelData.size.max.y = (mesh->mVertices[v].y > modelData.size.max.y) ? mesh->mVertices[v].y  : modelData.size.max.y;
        modelData.size.max.z = (mesh->mVertices[v].z > modelData.size.max.z) ? mesh->mVertices[v].z : modelData.size.max.z;
        modelData.size.min.x = (mesh->mVertices[v].x  < modelData.size.min.x) ? mesh->mVertices[v].x : modelData.size.min.x;
        modelData.size.min.y = (mesh->mVertices[v].y  < modelData.size.min.y) ? mesh->mVertices[v].y  : modelData.size.min.y;
        modelData.size.min.z = (mesh->mVertices[v].z  < modelData.size.min.z) ? mesh->mVertices[v].z  : modelData.size.min.z;

        modelData.vertices.push_back(glm::vec3(    mesh->mVertices[v].x,
                                                    mesh->mVertices[v].y,
                                                    mesh->mVertices[v].z));
        modelData.normals.push_back(glm::vec3(     mesh->mNormals[v].x,
                                                    mesh->mNormals[v].y,
                                                    mesh->mNormals[v].z));
        modelData.texcoord.push_back(glm::vec2(    mesh->mTextureCoords[0][v].x,
                                                    mesh->mTextureCoords[0][v].y));
    }


    modelData.size.length.x = abs(modelData.size.max.x - modelData.size.min.x);
    modelData.size.length.y = abs(modelData.size.max.y - modelData.size.min.y);
    modelData.size.length.z = abs(modelData.size.max.z - modelData.size.min.z);

    qDebug("Min : %f %f %f"     , modelData.size.min.x, modelData.size.min.y, modelData.size.min.z);
    qDebug("Max : %f %f %f"     , modelData.size.max.x, modelData.size.max.y, modelData.size.max.z);
    qDebug("Length : %f %f %f"  , modelData.size.length.x, modelData.size.length.y, modelData.size.length.z);


    for (uint32_t f = 0U; f < mesh->mNumFaces; f++) {
        aiFace face = mesh->mFaces[f];
        for (int i = 0; i < face.mNumIndices; i++) {
            modelData.indices.push_back(face.mIndices[i]);
        }
    }

    modelData.vboId[VBO_ID_TYPE_VERTEX] =  getVboId(VBO_ID_TYPE_VERTEX, &modelData);
    modelData.vboId[VBO_ID_TYPE_NORMAL] =  getVboId(VBO_ID_TYPE_NORMAL, &modelData);
    modelData.vboId[VBO_ID_TYPE_TEXCOORD] = getVboId(VBO_ID_TYPE_TEXCOORD, &modelData);
    modelData.vboId[VBO_ID_TYPE_INDEX] =  getVboId(VBO_ID_TYPE_INDEX, &modelData);

    if (mesh->mMaterialIndex >= 0) {        
        aiMaterial *material = scene->mMaterials[mesh->mMaterialIndex];
        modelData.textures.clear();
        loadTexture(material, &modelData.textures[aiTextureType_DIFFUSE], aiTextureType_DIFFUSE);
    }

    return modelData;
}

vector<ModelData>* ModelLoader::loadModel(string path)
{
    int i = 0;
    int j = 0;

    mModelList.clear();
    vector<ModelData>().swap(mModelList);

    mDirectoryPath = getBasePath(path);

    Assimp::Importer importer;
    const aiScene* scene = importer.ReadFile(path,
                                              aiProcess_Triangulate |
                                             aiProcess_SortByPType |
                                              aiProcess_FlipUVs);

    const aiNode *node = scene->mRootNode;
    aiMesh* mesh;
    for (i = 0; node->mNumMeshes; i++) {
        mesh = scene->mMeshes[node->mMeshes[i]];
        mModelList.push_back(parseModel(scene, mesh));
    }

    for (i = 0; i < node->mNumChildren; i++) {
        const aiNode *childrenNode = node->mChildren[i];
        for (j = 0; j < childrenNode->mNumMeshes; j++) {
            mesh = scene->mMeshes[childrenNode->mMeshes[j]];
            mModelList.push_back(parseModel(scene, mesh));
        }
    }

    return &mModelList;
}
