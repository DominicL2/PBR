import QtQuick 2.0
import Qt.labs.controls 1.0

TabBar {
    id : tabBar
    x : 0
    y : 75
    width : 350
    height : 200
    background: Rectangle {
        color : "transparent"
    }

    TabButton {
        contentItem: Text {
            text: "Common"
            font.family: sfPro.name
            font.bold: true
            font.pixelSize: 13
            horizontalAlignment: Text.AlignHCenter
            verticalAlignment: Text.AlignVCenter
            color : tabBar.currentIndex == 0 ? gWhite0 : gBlue0
        }

        background : Rectangle {
            color : tabBar.currentIndex == 0 ? gBlue0 : "transparent"
        }
    }
    TabButton {
        contentItem: Text {
            text: "Parameter"
            font.family: sfPro.name
            font.bold: true
            font.pixelSize: 13
            horizontalAlignment: Text.AlignHCenter
            verticalAlignment: Text.AlignVCenter
            color : tabBar.currentIndex == 1 ? gWhite0 : gBlue0
        }
        background : Rectangle {
            color : tabBar.currentIndex == 1 ? gBlue0 : "transparent"
        }
    }

}
