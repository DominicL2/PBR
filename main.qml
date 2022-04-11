import QtQuick 2.6
import PbrRender 1.0

Item {
    visible: true
    width: 1280
    height: 720

    ViewModel {
        x:0
        y:0
        width:1280
        height:720
        id : viewModel
        SequentialAnimation on swap {
            NumberAnimation { to: 1; duration: 2500; easing.type: Easing.InQuad }
            NumberAnimation { to: 0; duration: 2500; easing.type: Easing.OutQuad }
            loops: Animation.Infinite
            running: true
        }
    }
}
