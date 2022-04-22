import QtQuick 2.0
import QtQuick.Controls 1.4
import QtQuick.Controls.Styles 1.4

ComboBox {
    id: box
    currentIndex: 0
    width: 200
    height : 20
    activeFocusOnPress: true
    style: ComboBoxStyle {
        id: comboBox
        background: Rectangle {
            id: rectCategory
            radius: 5
            border.width: 2
            border.color: Qt.rgba(0.988, 0.988, 0.988)
            color:  Qt.rgba(0.988, 0.988, 0.988)
            Image {
                visible: count > 0
                x : 200 -20
                y : 5
                width : 12
                height : 10
                smooth: true
                source: "resource/image/down_arrow.png"
            }
        }
        label: Text {
            verticalAlignment: Text.AlignVCenter
            font.pointSize: 10
            font.family: sfPro.name
            color: gBlack1
            text: control.currentText
        }

        // drop-down customization here
        property Component __dropDownStyle: MenuStyle {
            __maxPopupHeight: 600
            __menuItemType: "comboboxitem"

            frame: Rectangle {
                color: Qt.rgba(0.97, 0.97, 0.97)
                border.width: 2
                border.color: gWhite0
                radius: 5
            }

            itemDelegate.label:
                                            Text {
                verticalAlignment: Text.AlignVCenter
                horizontalAlignment: Text.AlignHCenter
                font.pointSize: 10
                font.family: sfPro.name
                font.capitalization: Font.SmallCaps
                color: gBlack1
                text: styleData.text
            }

            itemDelegate.background: Rectangle {
                radius: 2
                color: styleData.selected ? "darkGray" : "transparent"
            }

            __scrollerStyle: ScrollViewStyle { }
        }
    }
}
