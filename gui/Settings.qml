import QtQuick 2.12
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.11
import OscMackieControl.app 1.0

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

        oscSettings.remote_addr = s.remote_addr
        oscSettings.remote_port = s.remote_port
        oscSettings.local_port  = s.local_port

        midiSettings.iface_in = s.iface_in
        midiSettings.midi_in  = s.midi_in
        midiSettings.iface_out = s.iface_out
        midiSettings.midi_out  = s.midi_out
        midiSettings.is_virtual = s.is_virtual
    }

    function update() {
        let s = {
            remote_addr: oscSettings.remote_addr,
            remote_port: oscSettings.remote_port,
            local_port:  oscSettings.local_port
        }

        if(!midiSettings.is_virtual)
        {
            s["midi_in"]  = midiSettings.midi_in
            s["midi_out"] = midiSettings.midi_out
        }

        App.settings = s;
        root.applied()
    }
}

/*##^##
Designer {
    D{i:0;autoSize:true;height:480;width:640}
}
##^##*/
