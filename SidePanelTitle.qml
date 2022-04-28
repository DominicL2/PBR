import QtQuick 2.0

Rectangle {
    property int gSidePannelTitleX : 20
    property int gSidePannelTitleY : 20
    property int gSidePannelTitleWidth : 330
    property int gSidePannelTitleHeight : 35
    property int shaderIndex : 0

    x : gSidePannelTitleX
    y : gSidePannelTitleY
    width : gSidePannelTitleWidth
    height : gSidePannelTitleHeight
    color : gWhite0

    function setTileName(name) {
        titleText.text = name
    }

    Image {
        id : lightImg
        x : 0
        y : 0
        width : 35
        height : 35
        source : "resource/image/light_icon.png"
    }

    SidePanelTitle_ComboBox {
        id : titleComboBox
        currentIndex: shaderIndex
        onCurrentIndexChanged: {
           shaderIndex = currentIndex
        }
    }
}
