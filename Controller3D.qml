import QtQuick 2.0

Column {
    id : controller3D
    spacing : 10
    property string title: ""
    property string type: "vector"
    function getValue() {
        return Qt.vector3d(parseFloat(axisX.getText()), parseFloat(axisY.getText()), parseFloat(axisZ.getText()));
    }

    function setValue(_x, _y, _z) {
        axisX.setText(_x)
        axisY.setText(_y)
        axisZ.setText(_z)
    }

    Rectangle {
        width : 150
        height : 20
        color: "transparent"
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
    Row {
        spacing : 20
        Editor3D {
            id : axisX
            name : controller3D.type == "vector" ? "x" : "r"
        }
        Editor3D {
            id : axisY
            name : controller3D.type == "vector" ? "y" : "g"
        }
        Editor3D {
            id : axisZ
            name : controller3D.type == "vector" ? "z" : "b"
        }
    }
}
