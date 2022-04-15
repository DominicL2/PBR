import QtQuick 2.0

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

    SidePanelTitle {
        id : title
    }

    SidePanelTabbar {
        id : taBbar
    }
}
