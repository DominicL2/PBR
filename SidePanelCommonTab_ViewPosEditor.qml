import QtQuick 2.0

Column {
    spacing : 10

    function getValue() {
        return Qt.vector3d(parseFloat(viewAxis_x.getText()), parseFloat(viewAxis_y.getText()), parseFloat(viewAxis_z.getText()));
    }


    function setValue(_x, _y, _z) {
        viewAxis_x.setText(_x)
        viewAxis_y.setText(_y)
        viewAxis_z.setText(_z)
    }

    Rectangle {
        width : 150
        height : 20
        color: "transparent"
        Text {
            anchors.fill: parent
            verticalAlignment: Text.AlignVCenter
            text: "View Position"
            font.pixelSize: 16
            color : gBlack1
            font.family: sfPro.name
            font.bold: true
        }
    }
    Row {
        spacing : 20
        VectorEditor {
            id : viewAxis_x
            name : "x"
        }
        VectorEditor {
            id : viewAxis_y
            name : "y"
        }
        VectorEditor {
            id : viewAxis_z
            name : "z"
        }
    }
}
