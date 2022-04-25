import QtQuick 2.0
import QtQuick.Controls 1.4
import Qt.labs.controls 1.0

Rectangle {
    color: gBlack0
    border.color: gBlack2
    border.width: 1
    radius : 4
    ScrollView {
        anchors.fill: parent
        TextArea {
            anchors.leftMargin: 10
            anchors.rightMargin: 10
            anchors.topMargin: 10
            anchors.bottomMargin: 10
            text: viewModel.logMsg
            font.pixelSize: 12
            color : gWhite0
            font.family: sfPro.name
            font.bold: false
            readOnly: true
            wrapMode: Text.Wrap
            renderType: Text.NativeRendering
        }
    }
}
