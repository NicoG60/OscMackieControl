import QtQuick 2.0

Item {
    id: root

    property alias text: label.text

    signal clicked

    MouseArea {
        id: mouse
        anchors.fill: parent
        hoverEnabled: true
        onClicked: root.clicked()
    }

    Rectangle {
        anchors.fill: parent
        visible: mouse.containsMouse
        color: "#80ffffff"
        border.width: 2
        border.color: "red"

        Text {
            id: label
            anchors.fill: parent
            horizontalAlignment: Text.AlignHCenter
            verticalAlignment: Text.AlignVCenter
        }
    }
}
