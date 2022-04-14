#ifndef MODELLOADER_H
#define MODELLOADER_H

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <glm/glm.hpp>
#include <GLES2/gl2.h>
#include <string>
#include <vector>
#include <unordered_map>
#include <QStringList>

#define MODEL_LOADER_SUCCESS   (1)
#define MODEL_LOADER_FAIL      (0)

using namespace std;

typedef enum {
    TEXTURE_TYPE_NONE = 0,
    TEXTURE_TYPE_BASE_COLOR,
    TEXTURE_TYPE_NORMAL,
    TEXTURE_TYPE_METALLIC,
    TEXTURE_TYPE_EMISSIVE,
    TEXTURE_TYPE_HEIGHT,
    TEXTURE_TYPE_ROUGHNESS,
    NUM_TEXTURE_TYPE
} TEXTURE_TYPE;

typedef enum {
    VBO_ID_TYPE_VERTEX = 0,
    VBO_ID_TYPE_NORMAL,
    VBO_ID_TYPE_TEXCOORD,
    VBO_ID_TYPE_INDEX,
    NUM_VBO_ID_TYPE
} VBO_ID_TYPE;

typedef struct {
   GLuint id;
   TEXTURE_TYPE type;
} ModelTexture;

typedef struct MODEL_SIZE{
    glm::vec3 min;
    glm::vec3 max;
    glm::vec3 length;
    MODEL_SIZE() {
        min = glm::vec3(0.0, 0.0, 0.0);
        max = glm::vec3(0.0, 0.0, 0.0);
    }
} ModelSize;

typedef struct GL_TRIANGLE{
    uint32_t index[3];

    GL_TRIANGLE() {
        index[0] = 0U;
        index[1] = 0U;
        index[2] = 0U;
    }
} GLTriangle;

typedef struct ModelData {
    vector<glm::vec3> vertices;
    vector<glm::vec3> normals;
    vector<uint32_t> indices;
    vector<glm::vec2> texcoord;
    unordered_map<aiTextureType, vector<GLuint>>  textures;
    GLuint vboId[NUM_VBO_ID_TYPE];
    ModelSize size;
    ModelData() {
        vertices.clear();
        normals.clear();
        indices.clear();
        texcoord.clear();
        textures.clear();
        vector<glm::vec3>().swap(vertices);
        vector<glm::vec3>().swap(normals);
        vector<uint32_t>().swap(indices);
        vector<glm::vec2>().swap(texcoord);
        unordered_map<aiTextureType, vector<GLuint>>().swap(textures);
    }
} MODEL_DATA;

class ModelLoader
{
public:
    ModelLoader();

    /// Load graphic model data using assimp
    vector<ModelData>* loadModel(string path);

    void createVBO(int32_t index);
    GLuint getVboId(int32_t type, ModelData *modelData);

private :
    vector<ModelData> mModelList;
    string            mDirectoryPath;
    /// Intialize graphic model data
    void initModelData();

    /// Load Texture
    QStringList getTextureFilePath(string path);
    GLuint getTextureId(aiString path);
    void loadTexture(aiMaterial *material, vector<GLuint> *textureId, aiTextureType type);
    ModelData parseModel(const aiScene *scene, aiMesh* mesh);
    aiTextureType getTextureTypeUsingFileName(string name);
    string getBasePath(string fullPath);
};

#endif // MODELLOADER_H
