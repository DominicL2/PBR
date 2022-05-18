import QtQuick 2.0

Rectangle {
    id : controller3D
    x : 0
    y : 0
    width: 196
    height : 60
    color : "transparent"
    property string title: ""
    property string type: "vector"
    property bool clickText : false
    function getValue() {
        return Qt.vector3d(parseFloat(axisX.getText()), parseFloat(axisY.getText()), parseFloat(axisZ.getText()));
    }

    function setValue(_x, _y, _z) {
        axisX.setText(_x)
        axisY.setText(_y)
        axisZ.setText(_z)
    }

    Column {
    spacing : 10
        Rectangle {
            x : 0
            y : 0
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

            MouseArea {
                anchors.fill: parent
                onClicked: {
                    clickText = !clickText
                }
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
}
