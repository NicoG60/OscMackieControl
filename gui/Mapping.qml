import QtQuick 2.0
import QtQuick.Controls 2.15

Item {
    id: root

    Mcu {
        id: mCU
        anchors.verticalCenter: parent.verticalCenter
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.horizontalCenterOffset: -0.5*drawer.width*drawer.position

        McuClickableArea {
            width: 100
            height: 50
            text: "TEST"
        }
    }

    Button {
        id: btn
        x: 0
        y: 0
        text: "open"
        onClicked: drawer.open()
    }

    Drawer {
        id: drawer
        edge: Qt.RightEdge
        width: 0.33 * parent.width
        height: parent.height
        dragMargin: 20
        dim: false
        modal: false
    }
}

/*##^##
Designer {
    D{i:0;autoSize:true;height:640;width:640}
}
##^##*/
