import QtQuick 2.0
import QtQuick.Layouts 1.3

Rectangle {
    id : materialTab_top
    color : "transparent"

    property int leftMargin : 25
    property int yMargin : 20
    property int shaderPageIndex : 0

    function setComboBox(list) {
        for (var i = 0; i < list.length; i++) {
            comboBoxList.append({text:list[i]})
        }
    }

    function clearComboBox() {
        comboBoxList.clear()
    }

    function setMaterialParamForPhong(ambient, diffuse, specular, shiness) {
        materialTab_phong.setAmbient(ambient)
        materialTab_phong.setDiffuse(diffuse)
        materialTab_phong.setSpecular(specular)
        materialTab_phong.setShiness(shiness)
    }

    function setMaterialParamForBlinnPhong(ambient, diffuse, specular, shiness) {
        materialTab_blinnPhong.setAmbient(ambient)
        materialTab_blinnPhong.setDiffuse(diffuse)
        materialTab_blinnPhong.setSpecular(specular)
        materialTab_blinnPhong.setShiness(shiness)
    }
    ListModel {
        id : comboBoxList
    }

    Rectangle {
        x : leftMargin
        y : yMargin
        Column {
            anchors.fill: parent
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
                        color : gBlack1
                        font.family: sfPro.name
                        font.bold: true
                    }
                }
                SidePanelMaterialTab_ComboBox {
                    id : comboBox
                    model : comboBoxList
                    onCurrentTextChanged: {
                        viewModel.setCurrentMaterial(currentText)
                    }
                }
            }

            StackLayout {
                id : shaderLayout
                anchors.fill: parent
                currentIndex : shaderPageIndex
                SidePanelMaterialTab_Phong {
                    id : materialTab_phong
                }
                SidePanelMaterialTab_Phong {
                    id : materialTab_blinnPhong
                }
                SidePanelMaterialTab_CookTorrance {
                    id : materialTab_cookTorrance
                }
            }
        }
    }
}
