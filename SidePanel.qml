import QtQuick 2.0
import Qt.labs.controls 1.0
import QtQuick.Layouts 1.3

Rectangle {
    id : sidePanel
    property int gSidePannelX : 730
    property int gSidePannelY : 0
    property int gSidePannelWidth : 350
    property int gSidePannelHeight : 720
    property string dirPath : ""


    x : gSidePannelX
    y : gSidePannelY
    width : gSidePannelWidth
    height : gSidePannelHeight
    color : gWhite0
    radius: 5
    border.color: gWhite1
    border.width: 2

    function setDefaultSpaceVal(lightPos, viewPos, modelRotation) {
        commonTab.setLightPos(lightPos)
        commonTab.setViewPos(viewPos)
        commonTab.setModelRotation(modelRotation)
    }

    function setMaterialParamForPhong(ambient, diffuse, specular, shiness) {
        materialTab.setMaterialParamForPhong(ambient, diffuse, specular, shiness)
    }

    function setMaterialParamForBlinnPhong(ambient, diffuse, specular, shiness) {
        materialTab.setMaterialParamForBlinnPhong(ambient, diffuse, specular, shiness)
    }

    function setMaterialList(list) {
        materialTab.setComboBox(list)
    }

    function getShaderType() {
        return title.shaderIndex
    }

    function showAxisCheckBox() {
        commonTab.showAixsCheckBox()
    }

    SidePanelTitle {
        id : title
        onShaderIndexChanged: {
            viewModel.setShaderType(shaderIndex)
        }
    }

    SidePanelTabbar {
        id : tabbar
    }    

    StackLayout {
        id : tabLayout
        x : 0
        y : 115
        width : parent.width
        height : 605
        currentIndex: tabbar.currentIndex
        SidePanelCommonTab {
            id : commonTab

            onPathStrChanged: {
                viewModel.loadGrpahicsModel(pathStr)
                dirPath = pathStr.substring(0, pathStr.lastIndexOf('/'))
            }

        }

        SidePanelMaterialTab {
            id : materialTab
            shaderPageIndex : title.shaderIndex
            dirPath : sidePanel.dirPath
        }
    }
}
