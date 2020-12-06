import QtQuick 2.12
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.11
import OscMackieControl 1.0

Item {
    id: root

    signal applied()

    Connections{
        target: root
        function onVisibleChanged() { if(root.visible) root.hydrate()}
    }

    ColumnLayout {
        anchors.centerIn: parent

        RowLayout {
            id: flow

            spacing: 10

            OscSettings {
                id: oscSettings

                Layout.minimumWidth: 400
                Layout.minimumHeight: 400
            }

            MidiSettings {
                id: midiSettings

                Layout.minimumWidth: 400
                Layout.minimumHeight: 400
            }
        }

        RowLayout {

            Layout.fillWidth: true
            Layout.alignment: Qt.AlignHCenter

            spacing: 10

            Button {
                id: btnCancel
                Layout.alignment: Qt.AlignLeft

                text: qsTr("Cancel")
                onClicked: root.hydrate()
            }

            Item {
                Layout.fillWidth: true
            }

            Button {
                id: btnAssist
                Layout.alignment: Qt.AlignHCenter

                text: qsTr("TouchOSC Assitant")
                onClicked: assistant.open()
            }

            Item {
                Layout.fillWidth: true
            }

            Button {
                id: btnApply
                Layout.alignment: Qt.AlignRight

                text: qsTr("Apply")
                highlighted: true

                onClicked: root.update()
            }
        }
    }

    Assistant {
        id: assistant

        anchors.centerIn: parent

        width: 700
        height: 700

        onClosed: root.hydrate()
    }

    function hydrate() {
        let s = App.settings

        oscSettings.remote_addr = App.osc.remoteAddr
        oscSettings.remote_port = App.osc.remotePort
        oscSettings.local_port  = App.osc.localPort

        midiSettings.iface_in = App.midi.availableInputInterfaces.map(i => i.name)
        midiSettings.midi_in  = App.midi.inputInterface.name
        midiSettings.iface_out = App.midi.availableOutputInterfaces.map(i => i.name)
        midiSettings.midi_out  = App.midi.outputInterface.name
        midiSettings.is_virtual = (Qt.platform.os === "osx")
    }

    function update() {
        App.osc.remoteAddr = oscSettings.remote_addr
        App.osc.remotePort = oscSettings.remote_port
        App.osc.localPort =  oscSettings.local_port

        if(!midiSettings.is_virtual)
        {
            App.midi.inputInterface = App.midi.availableInputInterfaces.find(i => i.name === midiSettings.midi_in)
            App.midi.outputInterface = App.midi.availableOutputInterfaces.find(i => i.name === midiSettings.midi_out)
        }

        root.applied()
    }
}

/*##^##
Designer {
    D{i:0;autoSize:true;height:480;width:640}
}
##^##*/
