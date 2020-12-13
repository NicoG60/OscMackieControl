import QtQuick 2.0
import QtQuick.Controls 2.15

Item {
    id: root

    height: 120
    width: 200

    MouseArea {
        id: mouse
        anchors.fill: parent
        hoverEnabled: true
    }

    Rectangle {
        id: btn
        x: 0
        y: 0
        width: 70
        height: 50
        color: "#645f5f"
        radius: 5
        border.width: 0
        transform: rot
    }

    Rectangle {
        id: led
        x: 25
        y: 30
        width: 20
        height: 20
        color: "#660000"
        radius: 10
        border.width: 0
        transform: rot
    }

    Text {
        id: lb1
        x: 0
        y: 0
        width: 70
        height: 24
        color: "#ffffff"
        text: qsTr("Label 1")
        font.pixelSize: 14
        horizontalAlignment: Text.AlignHCenter
        verticalAlignment: Text.AlignVCenter
        transform: rot
    }

    Text {
        id: lb2
        x: 0
        y: 26
        width: 70
        height: 24
        color: "#ffffff"
        text: qsTr("Label 2")
        font.pixelSize: 14
        horizontalAlignment: Text.AlignHCenter
        verticalAlignment: Text.AlignVCenter
        transform: rot
    }

    Rotation {
        id: rot
        axis { x: 0; y: 1; z: 0 }
        angle: 0
    }

    Label {
        id: label
        x: 5
        y: 99
        text: qsTr("Button")
        opacity: 0
    }

    Label {
        id: label1
        x: 50
        y: 83
        text: qsTr("LED")
        opacity: 0
    }

    states: [
        State {
            name: "breakdown"
            when: mouse.containsMouse

            PropertyChanges {
                target: rot
                angle: 40
            }

            PropertyChanges {
                target: btn
                x: 0
                y: 30
            }

            PropertyChanges {
                target: led
                x: 50
                y: 48
            }

            PropertyChanges {
                target: lb1
                x: 100
                y: 0
            }

            PropertyChanges {
                target: lb2
                x: 70
                y: 35
            }

            PropertyChanges {
                target: label
                opacity: 1
            }

            PropertyChanges {
                target: label1
                opacity: 1
            }
        }
    ]

    transitions: Transition {
        from: "*"
        to: "*"

        NumberAnimation {
            targets: [btn, led, lb1, lb2, label, label1, rot]
            properties: "x,y,opacity,angle"
            duration: 1000
        }
    }
}

/*##^##
Designer {
    D{i:0;formeditorZoom:0.8999999761581421}D{i:4}
}
##^##*/
