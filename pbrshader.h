#ifndef PBRSHADER_H
#define PBRSHADER_H

#include <string.h>

using namespace std;

typedef enum {
    SHADER_TYPE_PHONG = 0,
    SHADER_TYPE_BRDF,
    NUM_SHADER_TYPE
} SHADER_TYPE;


typedef enum {
    PHONG_SHADER_ATTR_POSTION = 0,
    PHONG_SHADER_ATTR_NORMAL,
    PHONG_SHADER_ATTR_TEXCOORD,
    NUM_PHONG_SHADER_ATTR
} PHONG_SHADER_ATTR;

typedef enum {
    PHONG_SHADER_UNIFORM_MVP = 0,
    PHONG_SHADER_UNIFORM_MV,
    PHONG_SHADER_UNIFORM_LIGHT_POS,
    PHONG_SHADER_UNIFORM_VIEW_POS,
    PHONG_SHADER_UNIFORM_AMBIENT,
    PHONG_SHADER_UNIFORM_DIFFUSE,
    PHONG_SHADER_UNIFORM_SPECULAR,
    NUM_PHONG_SHADER_UNIFORM
} PHONG_SHADER_UNIFORM;

const string VERTEX_SHADER_PHONG_STR =
        "attribute vec4 a_position;\n"  \
        "attribute vec4 a_normal;\n"    \
        "attribute vec2 a_texcoord;\n"  \
        "uniform mat4 u_mvp;\n"         \
        "uniform mat4 u_mv;\n"          \
        "uniform vec3 u_lightPos;\n"    \
        "uniform vec3 u_viewPos;\n"     \
        "varying vec2 v_texcoord;\n"    \
        "varying vec3 v_normal;\n"      \
        "varying vec4 v_surface;\n"     \
        "varying vec3 v_viewPos;\n"     \
        "varying vec3 v_lightPos;\n"    \
        "void main()\n"                 \
        "{\n"                           \
        "   gl_Position =  u_mvp * a_position;\n"       \
        "   v_surface = u_mv * a_position;\n"           \
        "   v_texcoord = a_texcoord;\n"                 \
        "   v_normal = a_normal.xyz;\n"                 \
        "   v_viewPos = u_viewPos;\n"                   \
        "   v_lightPos = u_lightPos;\n"                 \
        "}\n";

const string FRAGMENT_SHADER_PHONG_STR =
    "varying vec2 v_texcoord;\n"    \
    "varying vec3 v_normal;\n"      \
    "varying vec4 v_surface;\n"     \
    "varying vec3 v_viewPos;\n"     \
    "varying vec3 v_lightPos;\n"    \
    "uniform float ambientW;\n"     \
    "uniform float diffuseW;\n"     \
    "uniform float specularW;\n"    \
    "uniform sampler2D texture;\n"  \
    "vec3 calcPhongModel(vec3 color)\n" \
    "{\n" \
    "  vec3 ambient = ambientW * color;\n"  \
    "  vec3 normal = normalize(v_normal);\n"                                            \
    "  vec3 light = normalize(v_lightPos - v_surface.xyz);\n"                           \
    "  vec3 view = normalize(v_viewPos - v_surface.xyz);\n"                             \
    "  vec3 diffuse = diffuseW * color * max(dot(normal, light), 0.0);\n"                      \
    "  float shiness = 2.0;\n"                                                          \
    "  vec3 reflection = reflect(-light, normal);\n"                                    \
    "  vec3 specular = vec3(specularW, specularW, specularW) * pow(max(dot(reflection, view), 0.0), shiness);\n"   \
    "  return (ambient + diffuse + specular);\n"           \
    "}\n" \
    "void main()\n" \
    "{\n" \
    "  vec4 baseColor = texture2D(texture, v_texcoord);\n "     \
    "  gl_FragColor = vec4(calcPhongModel(baseColor.xyz), 1.0);\n"  \
    "}";

const string VERTEX_SHADER_BLINN_PHONG_STR =
        "attribute vec4 a_position;\n"  \
        "attribute vec4 a_normal;\n"    \
        "attribute vec2 a_texcoord;\n"  \
        "uniform mat4 u_mvp;\n"         \
        "uniform mat4 u_mv;\n"          \
        "uniform vec3 u_lightPos;\n"    \
        "uniform vec3 u_viewPos;\n"     \
        "varying vec2 v_texcoord;\n"    \
        "varying vec3 v_normal;\n"      \
        "varying vec4 v_surface;\n"     \
        "varying vec3 v_viewPos;\n"     \
        "varying vec3 v_lightPos;\n"    \
        "void main()\n"                 \
        "{\n"                           \
        "   gl_Position =  u_mvp * a_position;\n"       \
        "   v_surface = u_mv * a_position;\n"           \
        "   v_texcoord = a_texcoord;\n"                 \
        "   v_normal = a_normal.xyz;\n"                 \
        "   v_viewPos = u_viewPos;\n"                   \
        "   v_lightPos = u_lightPos;\n"                 \
        "}\n";

const string FRAGMENT_SHADER_BLINN_PHONG_STR =
    "varying vec2 v_texcoord;\n"    \
    "varying vec3 v_normal;\n"      \
    "varying vec4 v_surface;\n"     \
    "varying vec3 v_viewPos;\n"     \
    "varying vec3 v_lightPos;\n"    \
    "uniform float ambientW;\n"     \
    "uniform float diffuseW;\n"     \
    "uniform float specularW;\n"    \
    "uniform sampler2D texture;\n"  \
    "vec3 calcPhongModel(vec3 color)\n" \
    "{\n" \
    "  vec3 ambient = ambientW * color;\n"  \
    "  vec3 normal = normalize(v_normal);\n"                                            \
    "  vec3 light = normalize(v_lightPos - v_surface.xyz);\n"                           \
    "  vec3 view = normalize(v_viewPos - v_surface.xyz);\n"                             \
    "  vec3 diffuse = diffuseW * color * max(dot(normal, light), 0.0);\n"                      \
    "  float shiness = 2.0;\n"                                                                        \
    "  vec3 halfVector = normalize(light + view);\n"   \
    "  vec3 specular = vec3(specularW, specularW, specularW) * pow(max(dot(normal, halfVector), 0.0), shiness);\n"   \
    "  return (ambient + diffuse + specular);\n"           \
    "}\n" \
    "void main()\n" \
    "{\n" \
    "  vec4 baseColor = texture2D(texture, v_texcoord);\n "     \
    "  gl_FragColor = vec4(calcPhongModel(baseColor.xyz), 1.0);\n"  \
    "}";
#endif // PBRSHADER_H
