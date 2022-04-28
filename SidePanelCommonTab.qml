import QtQuick 2.2
import QtQuick.Controls 1.4
import QtQuick.Dialogs 1.0
import Qt.labs.controls 1.0

Rectangle {
    id : commonTab
    color : "transparent";

    property int leftMargin : 25
    property int yMargin : 25
    property string pathStr : ""
    property string displayStr : ""

    function setLightPos(pos) {
        lightPosEditor.setValue(pos.x, pos.y, pos.z)
    }

    function setViewPos(pos) {
        viewPosEditor.setValue(pos.x, pos.y, pos.z)
    }

    function setModelRotation(pos) {
        modelRotationEditor.setValue(pos.x, pos.y, pos.z)
    }

    FileDialog {
        id: fileDialog
        title: "Please choose a file"
        folder: "file:///home/dmlee/3d_models"
        nameFilters: [ "3d Model files (*.obj *.fbx *.blend)", "All files (*)" ]

        onAccepted: {
            pathStr = fileUrl.toString().replace(/^(file:\/{2})/,"");
            if (pathStr.length > 45) {
                displayStr = pathStr.substring(0, 42) + "..."
            } else {
                displayStr = pathStr;
            }

            Qt.quit()
        }
        onRejected: {
            Qt.quit()
        }
    }

    Rectangle {
        x : leftMargin
        y : yMargin
        Column {
            anchors.fill: parent
            spacing: yMargin
            /// File Loader
            SidePanelCommonTab_FileLoader {
                id : fileLoader
            }

            /// Light  Pos
            Row {
                spacing : 15
                Controller3D {
                    id : lightPosEditor
                    title: "Light Position"
                }

                CustomButton {
                    id :lightPosButton

                    onClickedChanged: {
                        viewModel.setLightPos(lightPosEditor.getValue())
                    }
                }
            }

            /// View Pos
            Row {
                spacing : 15
                Controller3D {
                    id : viewPosEditor
                    title: "View Position"
                }

                CustomButton {
                    id :viewPosButton

                    onClickedChanged: {
                        viewModel.setViewPos(viewPosEditor.getValue())
                    }
                }
            }

            /// Model Rotation
            Row {
                spacing : 15
                Controller3D {
                    id : modelRotationEditor
                    title: "Model Rotation"
                }

                CustomButton {
                    id :modelRotationButton

                    onClickedChanged: {
                        loadingCircle.running = true;
                        viewModel.setModelRotation(modelRotationEditor.getValue())
                    }
                }
            }

            Column {
                spacing : 10
                Rectangle {
                    width : 20
                    height : 20
                    color: "transparent"
                    Text {
                        anchors.fill: parent
                        verticalAlignment: Text.AlignVCenter
                        text: "Mesh"
                        font.pixelSize: 16
                        color : gBlack1
                        font.family: sfPro.name
                        font.bold: true
                    }
                }

                SidePanelCommonTab_LogBox {
                    id : logBox
                    width : commonTab.width - (leftMargin * 2)
                    height : 234
                }
            }
        }
    }
}
