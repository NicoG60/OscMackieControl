import QtQuick 2.12
import QtQuick.Controls 2.12

Button {
    id: root
    states: [
        State {
            name: "hover"
            when: root.hovered

            PropertyChanges {
                target: content
                opacity: 1
            }

            PropertyChanges {
                target: background
                opacity: 0.75
                border.width: 3
            }

            PropertyChanges {
                target: animBackground
                running: false
            }
        }
    ]

    transitions: Transition {
        to: "hover"
        reversible: true

        NumberAnimation {
            target: background
            properties: "opacity,border.width"
            duration: 200
            easing.type: Easing.InOutQuad
        }

        NumberAnimation {
            target: content
            property: "opacity"
            duration: 200
            easing.type: Easing.InOutQuad
        }
    }

    contentItem: Text {
        id: content
        color: "black"
        opacity: 0
        text: root.text
        font: root.font
        verticalAlignment: Text.AlignVCenter
        horizontalAlignment: Text.AlignHCenter
    }

    background: Rectangle {
        id: background

        color: "white"
        opacity: 0.5
        radius: 2
        border.width: 0
        border.color: "red"

        SequentialAnimation {
            id: animBackground

            loops: Animation.Infinite
            running: true

            NumberAnimation {
                target: background
                property: "opacity"
                from: 0.25
                to: 0.50
                duration: 1000
                easing.type: Easing.InOutSine
            }
            NumberAnimation {
                target: background
                property: "opacity"
                from: 0.50
                to: 0.25
                duration: 1000
                easing.type: Easing.InOutSine
            }
        }
    }
}

/*##^##
Designer {
    D{i:0;height:50;width:100}
}
##^##*/
