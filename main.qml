import QtQuick 2.6
import QtQuick.Controls 1.4
import PbrRender 0.5

Item {
    /* Item size */
    property int gWindowWidth : 1080
    property int gWindowHeight : 720
    property int gGLSurfaceWidth : 720
    property int gGLSurfaceHeight : 720
    property int gDefaultMargine : 10

    property color gWhite0  : Qt.rgba(0.949, 0.949, 0.968)
    property color gWhite1  : Qt.rgba(0.898, 0.898, 0.917)
    property color gWhite2  : Qt.rgba(0.819, 0.819, 0.839)
    property color gBlue0   : Qt.rgba(0.039, 0.517, 1.0)
    property color gBlue1   : Qt.rgba(0.0, 0.478, 1.0)
    property color gGreen0  : Qt.rgba(0.203, 0.78, 0.349)
    property color gBlack0  : Qt.rgba(0.109, 0.109, 0.117)
    property color gBlack1  : Qt.rgba(0.172, 0.172, 0.180)
    property color gBlack2  : Qt.rgba(0.386, 0.386, 0.398)
    property color gRed0    : Qt.rgba(1.0, 0.270, 0.227)
    property color gRed1    : Qt.rgba(1.0, 0.231, 0.188)
    property color gOrange0 : Qt.rgba(1.0, 0.584, 0.0)

    /* Shader Type */
    property int gShader_Phong          : 0
    property int gShader_Blinn_Phong    : 1
    property int gShader_Cook_Torrance  : 2

    width: gWindowWidth
    height: gWindowHeight

    FontLoader {
        id : sfPro
        source: "resource/font/SFPro.ttf"
    }
    /* Viewmodel is GL Surface and C++ connector*/
    ViewModel {
        id : viewModel
        x : 0
        y : 0
        width : gGLSurfaceWidth
        height : gGLSurfaceHeight

        SequentialAnimation on swap {
            NumberAnimation { to: 1; duration: 2500; easing.type: Easing.InQuad }
            NumberAnimation { to: 0; duration: 2500; easing.type: Easing.OutQuad }
            loops: Animation.Infinite
            running: true
        }

        Component.onCompleted: {
            setViewport(x, y, width, height)
        }
        BusyIndicator {
            id : loadingCircle
            anchors.centerIn: parent
            visible : true
            running: false
        }
    }

    Connections {
        id : viewModelConnector
        target :  viewModel
        onSigModelLoadded : {
            loadingCircle.running = false
            sidePannel.setDefaultSpaceVal(viewModel.getLightPos(), viewModel.getViewPos(), viewModel.getModelRotationPos())
            sidePannel.setMaterialList(viewModel.getMaterialList())
            sidePannel.showAxisCheckBox()
            switch (sidePannel.getShaderType()) {
            case gShader_Phong :
                sidePannel.setMaterialParamForPhong(viewModel.getAmbient(), viewModel.getDiffuse(), viewModel.getSpecular(), viewModel.getShiness())
                break;
            case gShader_Blinn_Phong :
                sidePannel.setMaterialParamForBlinnPhong(viewModel.getAmbient(), viewModel.getDiffuse(), viewModel.getSpecular(), viewModel.getShiness())
                break;
            case gShader_Cook_Torrance :
                break;
            }

        }
        onSigMaterialChanged : {
            if (viewModel.loadded) {
                switch (sidePannel.getShaderType()) {
                case gShader_Phong :
                    sidePannel.setMaterialParamForPhong(viewModel.getAmbient(), viewModel.getDiffuse(), viewModel.getSpecular(), viewModel.getShiness())
                    break;
                case gShader_Blinn_Phong :
                    sidePannel.setMaterialParamForBlinnPhong(viewModel.getAmbient(), viewModel.getDiffuse(), viewModel.getSpecular(), viewModel.getShiness())
                    break;
                case gShader_Cook_Torrance :
                    break;
                }
            }
        }
    }

    SidePanel {
        id : sidePannel
        visible: true
    }
}
