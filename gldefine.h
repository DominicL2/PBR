#ifndef GLDEFINE_H
#define GLDEFINE_H

#include <GLES2/gl2.h>
#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <vector>


typedef enum {
    GLES_SHADER_TYPE_VERTEX = 0,
    GLES_SHADER_TYPE_FRAGMENT,
    NUM_GLES_SHADER_TYPE
} GLES_SHADER_TYPE;

typedef struct GLRendererContext {
    GLuint program;
    GLuint shader[NUM_GLES_SHADER_TYPE];
    std::vector<GLuint> attribute;
    std::vector<GLuint> uniform;
    std::vector<GLuint> texId;

    GLRendererContext() {
        program = 0;
        shader[GLES_SHADER_TYPE_VERTEX] = 0;
        shader[GLES_SHADER_TYPE_FRAGMENT] = 0;
    }
}GL_RENDERER_CONTEXT;


#endif // GLDEFINE_H
