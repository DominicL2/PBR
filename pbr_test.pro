TEMPLATE = app

QT += qml quick
CONFIG += c++11

SOURCES += main.cpp \
    viewmodel.cpp \
    GLRenderer.cpp \
    ModelManager.cpp

RESOURCES += qml.qrc

# Additional import path used to resolve QML modules in Qt Creator's code model
QML_IMPORT_PATH =

# default rules for deployment.
include(deployment.pri)

HEADERS += \
    viewmodel.h \
    GLRenderer.h \
    gldefine.h \
    pbrshader.h \
    spacedata.h \
    debugmacro.h \
    ModelManager.h \
    primitiveshader.h

unix:!macx: LIBS += -L/opt/Qt5.6.3/5.6.3/gcc_64
unix:!macx: LIBS += -L/opt/Qt5.6.3/5.6.3/gcc_64/lib
unix:!macx: LIBS += -L$$PWD/../../../../../lib/x86_64-linux-gnu/ -lGLESv2 -lassimp


INCLUDEPATH += $$PWD/../../../../../lib/x86_64-linux-gnu
DEPENDPATH += $$PWD/../../../../../lib/x86_64-linux-gnu
