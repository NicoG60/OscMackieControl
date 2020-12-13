import QtQuick 2.12
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.12
import OscMackieControl 1.0

Item {
    id: root

    Mcu {
        id: mcu
        anchors.verticalCenter: parent.verticalCenter
        anchors.verticalCenterOffset: -100
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.horizontalCenterOffset: -0.5*drawer.width*drawer.position

        McuButton {
            id: lcd
            x: 5
            y: 10
            width: 250
            height: 40
            text: "LCD"

            onClicked: {
                drawer.open()
                listView.model.loadLCD()
            }
        }

        McuButton {
            id: displays
            x: 260
            y: 10
            width: 210
            height: 40
            text: "Displays"
        }

        McuButton {
            id: vpots
            x: 5
            y: 82
            width: 245
            height: 40
            text: "Virtual Pots"
        }

        McuButton {
            id: buttons
            x: 5
            y: 125
            width: 245
            height: 140
            text: "Channel Buttons"
        }

        McuButton {
            id: faders
            x: 5
            y: 280
            width: 300
            height: 175
            text: "Faders"

            onClicked: {
                drawer.open()
                listView.model.loadFaders()
            }
        }

        McuButton {
            id: assignment
            x: 253
            y: 82
            width: 60
            height: 183
            text: "Assignments"
        }

        McuButton {
            id: global
            x: 314
            y: 82
            width: 160
            height: 132
            text: "Global buttons"
        }

        McuButton {
            id: automation
            x: 314
            y: 220
            width: 160
            height: 56
            text: "Automation"
        }

        McuButton {
            id: transport
            x: 314
            y: 282
            width: 160
            height: 178
            text: "Transport"
        }
    }

    Drawer {
        id: drawer
        edge: Qt.RightEdge
        width: 0.33 * parent.width
        height: parent.height
        dragMargin: 20
        dim: false
        modal: false

        ListView {
            id: listView
            anchors.fill: parent
            model: App.mapping.createModel()

            header: Label {
                width: listView.width
                padding: 20
                text: qsTr("Mapping Editor")
                verticalAlignment: Qt.AlignVCenter
                horizontalAlignment: Qt.AlignHCenter
            }

            delegate: Frame {
                width: listView.width
                topInset: 5
                bottomInset: 5
                leftInset: 5
                rightInset: 5

                ColumnLayout {
                    id: col
                    anchors.fill: parent
                    spacing: 5

                    Label {
                        text: model.name
                        Layout.fillWidth: true
                    }
                    TextField {
                        Layout.fillWidth: true
                        visible: type === "base"
                        placeholderText: model.hint
                        text: model.base
                    }
                }
            }
        }
    }
}

/*##^##
Designer {
    D{i:0;autoSize:true;height:720;width:640}
}
##^##*/
