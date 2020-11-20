import QtQuick 2.12
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.11
import QtQuick.Controls.Material 2.12

Item {
    Flow {
        id: column
        anchors.centerIn: parent

        spacing: 10
        padding: 10

        Transition {
            id: anim
            NumberAnimation { properties: "x,y"; duration: 400; easing.type: Easing.InOutCubic }
        }

        move: anim
        populate: anim

        GenericStatus {
            id: oscSettings

            width: 400
            height: 400

            title: qsTr("OSC Settings")
            icon:  '\uf1eb'

            is_ok: true
            graph: false

            Label { text: qsTr("Controller IP:") }
            TextField {
                id: remoteAddr
                Layout.alignment: Qt.AlignHCenter
                placeholderText: qsTr("Text Field")
            }

            Label { text: qsTr("Outgoing Port:") }

            SpinBox {
                id: remotePort
                Layout.alignment: Qt.AlignHCenter
                to: 65535
                editable: true
            }

            Label { text: qsTr("Incoming Port:") }

            SpinBox {
                id: localPort
                Layout.alignment: Qt.AlignHCenter
                to: 65535
                editable: true
            }
        }

        GenericStatus {
            id: midiSettings

            width: 400
            height: 400

            title: qsTr("MIDI Settings")
            icon:  '\uf001'

            is_ok: true
            graph: false

            Label { text: qsTr("Midi Input") }

            ComboBox {
                id: midiIn
                Layout.alignment: Qt.AlignHCenter
            }

            Label { text: qsTr("Midi Output") }

            ComboBox {
                id: midiOut
                Layout.alignment: Qt.AlignHCenter
            }

            Frame {
                Material.elevation: 2
                Material.background: Material.color(Material.Green, Material.Shade200)

                Layout.alignment: Qt.AlignCenter
                Layout.columnSpan: 2

                Label {
                    anchors.fill: parent
                    color: Material.color(Material.Green, Material.Shade900)
                    text: qsTr("You're all set! Your system supports virtual ports")
                }
            }
        }
    }
}

/*##^##
Designer {
    D{i:0;autoSize:true;height:480;width:640}
}
##^##*/
