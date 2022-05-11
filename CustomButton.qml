import QtQuick 2.0

Rectangle {
    id : vectorButton
    y : 30
    width : 40
    height : 20
    color : gBlue0
    border.color: gBlue0
    border.width: 2
    radius : 4

    property bool clicked : false
    property bool doubleClicked : false
    property string buttonName : "Apply"

    function setText(text) {
        buttonName = text
    }

    Text {
        id : vectorButtonText
        anchors.fill: parent
        verticalAlignment: Text.AlignVCenter
        horizontalAlignment: Text.AlignHCenter
        text: buttonName
        font.pixelSize: 10
        color : gWhite0
        font.family: sfPro.name
        font.bold: true
    }
    MouseArea {
        id : mouseArea;
        anchors.fill: parent
        onPressed: {
            vectorButton.color = gWhite0
            vectorButtonText.color = gBlue0
        }

        onReleased: {
            vectorButton.color = gBlue0
            vectorButtonText.color = gWhite0
        }

        onClicked: {
            parent.clicked = !parent.clicked;
        }

        onDoubleClicked: {
            parent.doubleClicked = !parent.doubleClicked;
        }
    }
}
