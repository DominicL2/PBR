import QtQuick 2.0

Column {
    id : controller1D
    spacing : 10
    property string title: ""
    property string boxName : ""

    function getValue() {
        return parseFloat(shinessEditor.getText())
    }

    function setValue(_value) {
        shinessEditor.setText(_value)
    }

    Rectangle {
        width : 40
        height : 20
        color : "transparent"
        Text {
            anchors.fill: parent
            verticalAlignment: Text.AlignVCenter
            text: title
            font.pixelSize: 16
            color : gBlack1
            font.family: sfPro.name
            font.bold: true
        }
    }

    Editor3D {
        id : shinessEditor
        name : boxName
        rowSpace : 5
    }
}
