#ifndef PBRSHADER_H
#define PBRSHADER_H

#include <string.h>

using namespace std;

typedef enum {
    SHADER_TYPE_DEFAULT = 0,
    SHADER_TYPE_BRDF,
    NUM_SHADER_TYPE
} SHADER_TYPE;


typedef enum {
    DEFAULT_SHADER_ATTR_POSTION = 0,
    DEFAULT_SHADER_ATTR_TEXCOORD,
    NUM_DEFAULT_SHADER_ATTR
} DEFAULT_SHADER_ATTR;

typedef enum {
    DEFAULT_SHADER_UNIFORM_MVP = 0,
    //DEFAULT_SHADER_UNIFORM_NORMAL,
    NUM_DEFAULT_SHADER_UNIFORM
} DEFAULT_SHADER_UNIFORM;

const string VERTEX_SHADER_DEFAULT_STR =
        "attribute vec4 a_position;\n" \
        "attribute vec2 a_texcoord;\n" \
        "uniform mat4 u_mvp;\n" \
        "varying vec2 v_texcoord;\n" \
        "void main()\n" \
        "{\n" \
        "   gl_Position = u_mvp * a_position;\n" \
        "   v_texcoord = a_texcoord;\n" \
        "}\n";

const string FRAGMENT_SHADER_DEFAULT_STR =
    "varying vec2 v_texcoord;\n" \
    "uniform sampler2D texture;\n" \
    "void main()\n" \
    "{\n" \
    "  gl_FragColor = texture2D(texture, v_texcoord);\n" \
    "}";

#endif // PBRSHADER_H
