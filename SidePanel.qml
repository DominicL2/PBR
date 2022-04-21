import QtQuick 2.0
import Qt.labs.controls 1.0
import QtQuick.Layouts 1.3

Rectangle {
    property int gSidePannelX : 730
    property int gSidePannelY : 0
    property int gSidePannelWidth : 350
    property int gSidePannelHeight : 720

    x : gSidePannelX
    y : gSidePannelY
    width : gSidePannelWidth
    height : gSidePannelHeight
    color : gWhite0
    radius: 5
    border.color: gWhite1
    border.width: 2

    function changeLayout(pageIdx) {
        switch (pageIdx) {
        case 0 :
            break;
        case 1 :
            break;
        }
    }

    function setDefaultSpaceVal(lightPos, viewPos, modelRotation) {
        commonTab.setLightPos(lightPos)
        commonTab.setViewPos(viewPos)
        commonTab.setModelRotation(modelRotation)
    }

    function setMaterialParam(ambient, diffuse, specular, shiness) {
        //materialTab.setComboBox(viewModel.getMaterialList())
        materialTab.setAmbient(ambient)
        materialTab.setDiffuse(diffuse)
        materialTab.setSpecular(specular)
        materialTab.setShiness(shiness)
    }

    SidePanelTitle {
        id : title
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
            }

        }
        SidePanelMaterialTab {
            id : materialTab
        }
    }
}