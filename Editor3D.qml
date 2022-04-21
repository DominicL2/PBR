import QtQuick 2.0

Row {
    id : vectEditor
    property string name: ""
    property int rowSpace : 2
    function getText() {
        return parseFloat(vectorText.text).toFixed(3)
    }

    function setText(input) {
        vectorText.text = input.toFixed(3)
    }

    spacing : rowSpace
    Rectangle {
        width : 10
        height : 20
        color: "transparent"
        Text {
            anchors.fill: parent
            verticalAlignment: Text.AlignVCenter
            text: vectEditor.name
            font.pixelSize: 15
            color : gBlack2
            font.family: sfPro.name
            font.bold: true
        }
    }

    Rectangle {
        width : 40
        height : 20
        color: Qt.rgba(0.988, 0.988, 0.988)
        radius : 7
        TextEdit {
            id : vectorText
            anchors.fill: parent
            anchors.leftMargin: 5
            verticalAlignment: Text.AlignVCenter
            font.pixelSize: 10
            color : gBlack1
            font.family: sfPro.name
        }
    }
}
