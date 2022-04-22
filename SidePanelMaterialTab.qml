import QtQuick 2.0

Rectangle {
    id : materialTab
    color : "transparent"

    property int leftMargin : 25
    property int yMargin : 30

    function setComboBox(list) {
        for (var i = 0; i < list.length; i++) {
            comboBoxList.append({text:list[i]})
        }
    }

    function setAmbient(pos) {
        ambientEditor.setValue(pos.x, pos.y, pos.z)
    }

    function setDiffuse(pos) {
        difuseEditor.setValue(pos.x, pos.y, pos.z)
    }

    function setSpecular(pos) {
        specularEditor.setValue(pos.x, pos.y, pos.z)
    }

    function setShiness(weight) {
        shinessEditor.setValue(weight)
    }

    ListModel {
        id : comboBoxList

    }

    Rectangle {
        x : leftMargin
        y : yMargin
        Column {
            anchors.fill: parent
            spacing: yMargin

            Row {
                spacing : 10
                Rectangle {
                    width : 40
                    height : 20
                    color: "transparent"
                    Text {
                        anchors.fill: parent
                        verticalAlignment: Text.AlignVCenter
                        text: "Name"
                        font.pixelSize: 15
                        color : gBlack2
                        font.family: sfPro.name
                        font.bold: true
                    }
                }
                SidePanelMaterialTab_ComboBox {
                    id : comboBox
                    model : comboBoxList
                }
            }

            /// Ambient
            Row {
                spacing : 15
                Controller3D {
                    id : ambientEditor
                    title: "Ambient"
                    type: "Color"
                }
                CustomButton {
                    id :ambientButton

                    onClickedChanged: {
                        viewModel.setAmbient(ambientEditor.getValue())
                    }
                }
            }

            /// Difuse
            Row {
                spacing : 15
                Controller3D {
                    id : difuseEditor
                    title: "Diffuse"
                    type: "Color"
                }
                CustomButton {
                    id :difuseButton

                    onClickedChanged: {
                        viewModel.setDiffuse(difuseEditor.getValue())
                    }
                }
            }

            /// Specular
            Row {
                spacing : 15
                Controller3D {
                    id : specularEditor
                    title: "Specular"
                    type: "Color"
                }
                CustomButton {
                    id :specularButton

                    onClickedChanged: {
                        viewModel.setSpecular(specularEditor.getValue())
                    }
                }
            }

            /// Shiness
            Row {
                spacing : 15
                Controller1D {
                    id : shinessEditor
                    title : "Shiness"
                    boxName : "w"
                }

                CustomButton {
                    id :shinessButton

                    onClickedChanged: {
                        viewModel.setShiness(shinessEditor.getValue())
                    }
                }
            }
        }
    }
}
