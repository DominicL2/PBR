import QtQuick 2.0

Column {
    spacing : 10

    function getValue() {
        return Qt.vector3d(parseFloat(lightAxis_x.getText()), parseFloat(lightAxis_y.getText()), parseFloat(lightAxis_z.getText()));
    }

    function setValue(_x, _y, _z) {
        lightAxis_x.setText(_x)
        lightAxis_y.setText(_y)
        lightAxis_z.setText(_z)
    }

    Rectangle {
        width : 150
        height : 20
        color: "transparent"
        Text {
            anchors.fill: parent
            verticalAlignment: Text.AlignVCenter
            text: "Light Position"
            font.pixelSize: 18
            color : gBlack1
            font.family: sfPro.name
            font.bold: true
        }
    }
    Row {
        spacing : 20
        VectorEditor {
            id : lightAxis_x
            name : "x"
        }
        VectorEditor {
            id : lightAxis_y
            name : "y"
        }
        VectorEditor {
            id : lightAxis_z
            name : "z"
        }
    }
}
