import QtQuick 2.0
import Qt.labs.controls 1.0

TabBar {
    id : tabBar
    x : 0
    y : 75
    width : 350
    height : 30
    function getTabButtonHeight() {
        return commonButton.height
    }

    background: Rectangle {
        color : "transparent"
    }

    TabButton {
        id : commonButton
        contentItem: Text {
            text: "Common"
            font.family: sfPro.name
            font.bold: true
            font.pixelSize: 13
            horizontalAlignment: Text.AlignHCenter
            verticalAlignment: Text.AlignVCenter
            color : gBlue0
        }

        background : Rectangle {
            anchors.fill: parent
            color : "transparent"
            Rectangle {
                x : parent.x
                y : (parent.y + parent.height) - 2
                width : parent.width
                height : 2
                color : tabBar.currentIndex == 0 ? gBlue0 : "transparent"
            }
        }
    }
    TabButton {
        id: parameterButton
        contentItem: Text {
            text: "Material"
            font.family: sfPro.name
            font.bold: true
            font.pixelSize: 13
            horizontalAlignment: Text.AlignHCenter
            verticalAlignment: Text.AlignVCenter
            color : gBlue0
        }
        background : Rectangle {
            anchors.fill: parent
            color : "transparent"
            Rectangle {
                x : parent.x
                y : (parent.y + parent.height) - 2
                width : parent.width
                height : 2
                color : tabBar.currentIndex == 1 ? gBlue0 : "transparent"
            }
        }
    }

}
