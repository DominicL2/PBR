import QtQuick 2.0

Row {
    spacing : 10
    Rectangle {
        width : 20
        height : 20
        color: "transparent"
        Text {
            anchors.fill: parent
            verticalAlignment: Text.AlignVCenter
            text: "Path"
            font.pixelSize: 13
            color : gBlack1
            font.family: sfPro.name
            font.bold: true
        }
    }

    Rectangle {
        width : 230
        height : 20
        color: Qt.rgba(0.988, 0.988, 0.988)
        radius : 7
        TextEdit {
            anchors.fill: parent
            anchors.leftMargin: 5
            verticalAlignment: Text.AlignVCenter
            text: displayStr
            font.pixelSize: 10
            readOnly: true
            color : gBlack1
            font.overline: false
            font.family: sfPro.name
            textFormat: Text.RichText
        }
    }

    MouseArea {
        width : 20
        height : 20
        Rectangle {
            id : fileDialogButtonRect
            anchors.fill: parent
            radius : 4
            color : gWhite1
            Image {
                anchors.centerIn: parent
                source : "resource/image/forlder_icon.png"
                sourceSize.width: parent.width - 5
                sourceSize.height: parent.height - 5
            }
        }
        onClicked: {
            fileDialog.open()
        }

        onPressed: {
            fileDialogButtonRect.color = gWhite2
        }

        onReleased: {
            fileDialogButtonRect.color = gWhite1
        }
    }
}
