import QtQuick 2.0
import QtQuick.Dialogs 1.0
Row {
    id : fileLoader

    property string title : ""
    property string folder : ""
    property string nameFilters : ""
    property string pathStr : ""
    property string displayStr : ""
    property int maxStrSize : 0
    property int dispWidth : 0
    property int dispHeight : 0
    property int nameWidth : 0
    property int nameHeight : 0
    property string name : ""
    property bool showNameOnly : false

    spacing : 10
    Rectangle {
        width : nameWidth
        height : nameHeight
        color: "transparent"
        Text {
            anchors.fill: parent
            verticalAlignment: Text.AlignVCenter
            text: name
            font.pixelSize: 13
            color : gBlack1
            font.family: sfPro.name
            font.bold: true
        }
    }

    Rectangle {
        width : dispWidth
        height : dispHeight
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

        FileDialog {
            id: fileDialog
            title: fileLoader.title
            folder: fileLoader.folder
            nameFilters: fileLoader.nameFilters

            onAccepted: {
                pathStr = fileUrl.toString().replace(/^(file:\/{2})/,"")
                displayStr = pathStr
                if (showNameOnly) {
                    displayStr = displayStr.substring(displayStr.lastIndexOf('/') + 1, displayStr.length)
                }

                if (displayStr.length > maxStrSize) {
                    displayStr = displayStr.substring(0, maxStrSize - 3) + "..."
                }
                Qt.quit()
            }
            onRejected: {
                Qt.quit()
            }
        }

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
