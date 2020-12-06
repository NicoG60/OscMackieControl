import QtQuick 2.12
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.12
import QtQuick.Controls.Material 2.12

GenericStatus {
    id: root

    property alias  iface_in: midiIn.model
    property string midi_in: ""
    property alias  iface_out: midiOut.model
    property string midi_out: ""
    property bool   is_virtual: false

    title: qsTr("MIDI Settings")
    icon:  '\uf001'

    is_ok: true
    graph: false

    Label { text: qsTr("Midi Input") }

    ComboBox {
        id: midiIn
        Layout.fillWidth: true
        enabled: !is_virtual
    }

    Label { text: qsTr("Midi Output") }

    ComboBox {
        id: midiOut
        Layout.fillWidth: true
        enabled: !is_virtual
    }

    Frame {
        id: virtualFrame
        Material.elevation: 2
        Material.background: Material.color(Material.Green, Material.Shade200)

        Layout.alignment: Qt.AlignCenter
        Layout.fillWidth: true
        Layout.columnSpan: 2

        visible: is_virtual

        Label {
            anchors.fill: parent
            color: Material.color(Material.Green, Material.Shade900)
            text: qsTr("You're all set! Your system supports virtual ports")
        }
    }

    onMidi_inChanged: {
        let i = midiIn.indexOfValue(root.midi_in)
        midiIn.currentIndex = i
    }

    onMidi_outChanged: {
        let o = midiOut.indexOfValue(root.midi_out)
        midiOut.currentIndex = o
    }
}
