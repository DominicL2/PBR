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

    function setLightPos(pos) {
        lightPosEditor.setValue(pos.x, pos.y, pos.z)
    }

    function setViewPos(pos) {
        viewPosEditor.setValue(pos.x, pos.y, pos.z)
    }

    function setModelRotation(pos) {
        modelRotationEditor.setValue(pos.x, pos.y, pos.z)
    }

    function showAixsCheckBox() {
        axisCheckBox.visible = true
    }



    Rectangle {
        x : leftMargin
        y : yMargin
        Column {
            anchors.fill: parent
            spacing: yMargin
            /// File Loader
            FileLoader {
                id : fileLoader
                name : "Path"
                title : "Please choose a file"
                folder: "file:///home/dmlee/3d_models"
                nameFilters: "3d Model files (*.obj *.fbx *.blend)"
                maxStrSize : 45
                nameWidth : 20
                nameHeight : 20
                dispWidth: 230
                dispHeight: 20
                onPathStrChanged: {
                    commonTab.pathStr = pathStr
                }
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
                    height : 244
                }
            }
        }

        CheckBox {
            id: axisCheckBox
            x : 266
            y : 540
            checked: true
            visible: false
            indicator: Rectangle {
                implicitWidth: 26
                implicitHeight: 26
                x: axisCheckBox.leftPadding
                y: parent.height / 2 - height / 2
                radius: 3
                color : "transparent"
                border.color: gBlack2

                Image {
                    id : axisImage
                    visible: axisCheckBox.checked ? true : false
                    x : 0
                    y : 0
                    width : 26
                    height : 26
                    source : "resource/image/axis_icon.png"
                }
            }
            onCheckedChanged: {
                viewModel.showAxisLine(checked)
            }
        }
    }
}
