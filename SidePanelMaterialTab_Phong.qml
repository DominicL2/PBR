import QtQuick 2.0

Rectangle {
    id : materialTab
    color : "transparent"

    property int yMargin : 40

    function setComboBox(list) {
        for (var i = 0; i < list.length; i++) {
            comboBoxList.append({text:list[i]})
        }
    }

    function clearComboBox() {
        comboBoxList.clear()
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

    Rectangle {
        x : 0
        y : yMargin
        Column {
            anchors.fill: parent
            spacing: yMargin

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
