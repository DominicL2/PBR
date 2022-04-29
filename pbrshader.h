#ifndef PBRSHADER_H
#define PBRSHADER_H

#include <string.h>

using namespace std;

typedef enum {
    SHADER_TYPE_PHONG = 0,
    SHADER_TYPE_BLINN_PHONG,
    SHADER_TYPE_COOK_TORRNACE,
    NUM_SHADER_TYPE
} SHADER_TYPE;


typedef enum {
    PHONG_SHADER_ATTR_VERTEX = 0,
    PHONG_SHADER_ATTR_NORMAL,
    PHONG_SHADER_ATTR_TANGENT,
    PHONG_SHADER_ATTR_BITANGENT,
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
    PHONG_SHADER_UNIFORM_SHINESS,
    PHONG_SHADER_UNIFORM_DEFAULT_COLOR,
    PHONG_SHADER_UNIFORM_TEXTURE_ALBEDO,
    PHONG_SHADER_UNIFORM_TEXTURE_NORMAL_MAP,
    NUM_PHONG_SHADER_UNIFORM
} PHONG_SHADER_UNIFORM;

typedef enum {
  PHONG_PARAMETER_TYPE_SHINESS = 0,
  NUM_PHONG_PARAMETER_TYPE
} PHONG_PARAMETER_TYPE;

const string VERTEX_SHADER_PHONG_STR =
        "attribute vec4 a_position;\n"                                          \
        "attribute vec4 a_normal;\n"                                            \
        "attribute vec4 a_tangent;\n"                                           \
        "attribute vec4 a_biTangent;\n"                                         \
        "attribute vec2 a_texcoord;\n"                                          \
        "uniform mat4 u_mvp;\n"                                                 \
        "uniform mat4 u_mv;\n"                                                  \
        "uniform vec3 u_lightPos;\n"                                            \
        "uniform vec3 u_viewPos;\n"                                             \
        "varying vec2 v_texcoord;\n"                                            \
        "varying vec3 v_normal;\n"                                              \
        "varying vec3 v_viewPos;\n"                                             \
        "varying vec3 v_lightPos;\n"                                            \
        "varying mat3 v_TBN;\n"                                                 \
        "varying vec3 v_surface;\n"                                             \
        "void main()\n"                                                         \
        "{\n"                                                                   \
        "   gl_Position =  u_mvp * a_position;\n"                               \
        "   v_texcoord = a_texcoord;\n"                                         \
        "   v_normal = a_normal.xyz;\n"                                         \
        "   vec3 T = normalize(vec3(u_mv * vec4(a_tangent.xyz, 0.0)));\n"       \
        "   vec3 B = normalize(vec3(u_mv * vec4(a_biTangent.xyz, 0.0)));\n"     \
        "   vec3 N = normalize(vec3(u_mv * vec4(a_normal.xyz, 0.0)));\n"        \
        "   v_TBN = mat3(T, B, N);\n"                                           \
        "   v_surface = vec3(u_mv * a_position);\n"                             \
        "   v_lightPos =  u_lightPos;\n"                                        \
        "   v_viewPos = u_viewPos;\n"                                           \
        "}\n";

const string FRAGMENT_SHADER_PHONG_STR =
    "varying vec2 v_texcoord;\n"                        \
    "varying vec3 v_normal;\n"                          \
    "varying vec3 v_viewPos;\n"                         \
    "varying vec3 v_lightPos;\n"                        \
    "varying mat3 v_TBN;\n"                             \
    "varying vec3 v_surface;\n"                         \
    "uniform vec3 ambientW;\n"                          \
    "uniform vec3 diffuseW;\n"                          \
    "uniform vec3 specularW;\n"                         \
    "uniform vec3 defaultColor;\n"                      \
    "uniform float shiness;\n"                          \
    "uniform sampler2D texAlbedo;\n"                    \
    "uniform sampler2D texNormalMap;\n"                 \
    "vec3 calcPhongModel(vec3 color, vec3 normal)\n"    \
    "{\n"                                                                               \
    "  vec3 ambient = ambientW * color;\n"                                              \
    "  vec3 light = normalize(v_lightPos - v_surface);\n"                               \
    "  vec3 view = normalize(v_viewPos - v_surface);\n"                                 \
    "  vec3 diffuse = diffuseW * color * max(dot(normal, light), 0.0);\n"               \
    "  vec3 reflection = reflect(-light, normal);\n"                                    \
    "  vec3 specular = specularW * pow(max(dot(reflection, view), 0.0), shiness);\n"    \
    "  return (ambient + diffuse + specular);\n"                                        \
    "}\n"                                                                               \
    "void main()\n"                                                                     \
    "{\n"                                                                               \
    "  vec4 baseColor = texture2D(texAlbedo, v_texcoord);\n "                           \
    "  if (defaultColor.x > 0.0 || defaultColor.y > 0.0 || defaultColor.z > 0.0) {\n"   \
    "   baseColor = vec4(defaultColor, 1.0);\n"                                         \
    "  }\n"                                                                             \
    "  vec3 normal = texture2D(texNormalMap, v_texcoord).rgb;\n"                        \
    "  if (normal.r > 0.0 || normal.g > 0.0 || normal.b > 0.0) {\n"                     \
    "       normal = normal * 2.0 - 1.0;\n "                                            \
    "       normal = normalize(v_TBN * normal);\n"                                      \
    "   } else {\n"                                                                     \
    "       normal = normalize(v_normal);\n"                                            \
    "   }\n"                                                                            \
    "  gl_FragColor = vec4(calcPhongModel(baseColor.xyz, normal), 1.0);\n"              \
    "}";

const string VERTEX_SHADER_BLINN_PHONG_STR =
        "attribute vec4 a_position;\n"                                          \
        "attribute vec4 a_normal;\n"                                            \
        "attribute vec4 a_tangent;\n"                                           \
        "attribute vec4 a_biTangent;\n"                                         \
        "attribute vec2 a_texcoord;\n"                                          \
        "uniform mat4 u_mvp;\n"                                                 \
        "uniform mat4 u_mv;\n"                                                  \
        "uniform vec3 u_lightPos;\n"                                            \
        "uniform vec3 u_viewPos;\n"                                             \
        "varying vec2 v_texcoord;\n"                                            \
        "varying vec3 v_normal;\n"                                              \
        "varying vec3 v_viewPos;\n"                                             \
        "varying vec3 v_lightPos;\n"                                            \
        "varying mat3 v_TBN;\n"                                                 \
        "varying vec3 v_surface;\n"                                             \
        "void main()\n"                                                         \
        "{\n"                                                                   \
        "   gl_Position =  u_mvp * a_position;\n"                               \
        "   v_texcoord = a_texcoord;\n"                                         \
        "   v_normal = a_normal.xyz;\n"                                         \
        "   vec3 T = normalize(vec3(u_mv * vec4(a_tangent.xyz, 0.0)));\n"       \
        "   vec3 B = normalize(vec3(u_mv * vec4(a_biTangent.xyz, 0.0)));\n"     \
        "   vec3 N = normalize(vec3(u_mv * vec4(a_normal.xyz, 0.0)));\n"        \
        "   v_TBN = mat3(T, B, N);\n"                                           \
        "   v_surface = vec3(u_mv * a_position);\n"                             \
        "   v_lightPos =  u_lightPos;\n"                                        \
        "   v_viewPos = u_viewPos;\n"                                           \
        "}\n";
const string FRAGMENT_SHADER_BLINN_PHONG_STR =
    "varying vec2 v_texcoord;\n"        \
    "varying vec3 v_normal;\n"          \
    "varying vec3 v_viewPos;\n"         \
    "varying vec3 v_lightPos;\n"        \
    "varying mat3 v_TBN;\n"             \
    "varying vec3 v_surface;\n"         \
    "uniform vec3 ambientW;\n"          \
    "uniform vec3 diffuseW;\n"          \
    "uniform vec3 specularW;\n"         \
    "uniform vec3 defaultColor;\n"      \
    "uniform float shiness;\n"          \
    "uniform sampler2D texAlbedo;\n"    \
    "uniform sampler2D texNormalMap;\n" \
    "vec3 calcBlinnPhongModel(vec3 color, vec3 normal)\n"                               \
    "{\n"                                                                               \
    "  vec3 ambient = ambientW * color;\n"                                              \
    "  vec3 light = normalize(v_lightPos - v_surface);\n"                               \
    "  vec3 view = normalize(v_viewPos - v_surface);\n"                                 \
    "  vec3 diffuse = diffuseW * color * max(dot(normal, light), 0.0);\n"               \
    "  vec3 halfVector = normalize(light + view);\n"                                    \
    "  vec3 specular = specularW * pow(max(dot(normal, halfVector), 0.0), shiness);\n"  \
    "  return (ambient + diffuse + specular);\n"                                        \
    "}\n"                                                                               \
    "void main()\n"                                                                     \
    "{\n"                                                                               \
    "  vec4 baseColor = texture2D(texAlbedo, v_texcoord);\n "                           \
    "  if (defaultColor.x > 0.0 || defaultColor.y > 0.0 || defaultColor.z > 0.0) {\n"   \
    "   baseColor = vec4(defaultColor, 1.0);\n"                                         \
    "  }\n"                                                                             \
    "  vec3 normal = texture2D(texNormalMap, v_texcoord).rgb;\n "                       \
    "  if (normal.r > 0.0 || normal.g > 0.0 || normal.b > 0.0) {\n"                     \
    "       normal = normal * 2.0 - 1.0;\n "                                            \
    "       normal = normalize(v_TBN * normal);\n "                                     \
    "   } else {\n"                                                                     \
    "       normal = normalize(v_normal);\n "                                           \
    "   }\n"                                                                            \
    "  gl_FragColor = vec4(calcBlinnPhongModel(baseColor.xyz, normal), 1.0);\n"         \
    "}";

#endif // PBRSHADER_H
