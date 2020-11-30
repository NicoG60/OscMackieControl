import QtQuick 2.12
import QtQuick.Layouts 1.11
import OscMackieControl.app 1.0

Item {
    RowLayout {
        id: column
        anchors.centerIn: parent

        spacing: 10

        OscStatus {
            id: oscStatus

            Layout.minimumWidth: 400
            Layout.minimumHeight: 400

            is_listening: App.oscStatus.is_listening
            remote_addr:  App.oscStatus.remote_addr
            local_addr:   App.oscStatus.local_addr
            remote_port:  App.oscStatus.remote_port
            local_port:   App.oscStatus.local_port
            avg_in:       App.oscStatus.avg_in
            avg_out:      App.oscStatus.avg_out
        }

        MidiStatus {
            id: midiStatus

            Layout.minimumWidth: 400
            Layout.minimumHeight: 400

            is_open:      App.midiStatus.is_open
            input_iface:  App.midiStatus.input_iface
            output_iface: App.midiStatus.output_iface
            avg_in:       App.midiStatus.avg_in
            avg_out:      App.midiStatus.avg_out
        }
    }

    Connections {
        target: App
        function onOscStatusChanged() {
            oscStatus.push(App.oscStatus.last_in, App.oscStatus.last_out)
            midiStatus.push(App.midiStatus.last_in, App.midiStatus.last_out)
        }
    }
}

/*##^##
Designer {
    D{i:0;autoSize:true;height:480;width:640}
}
##^##*/
