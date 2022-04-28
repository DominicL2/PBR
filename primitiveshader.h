#ifndef PRIMITIVESHADER_H
#define PRIMITIVESHADER_H

const string VERTEX_SHADER_PRIMITIVE_STR =
    "attribute vec4 a_position;\n" \
    "uniform mat4 u_mvp;\n"         \
    "void main()\n"
    "{\n"
    "   gl_Position = u_mvp * a_position;\n"
    "}\n";

const string FRAGMENT_SHADER_PRIMITIVE_STR =
    "uniform vec3 u_color;\n" \
    "void main()\n"
    "{\n"
    "  gl_FragColor = vec4(u_color, 0.5);\n"
    "}\n";

#endif // PRIMITIVESHADER_H
