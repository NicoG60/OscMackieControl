import QtQuick 2.12
import QtQuick.Layouts 1.11
import OscMackieControl 1.0

Item {
    RowLayout {
        id: column
        anchors.centerIn: parent

        spacing: 10

        OscStatus {
            id: oscStatus

            Layout.minimumWidth: 400
            Layout.minimumHeight: 400

            is_listening: App.osc.isListening
            remote_addr:  App.osc.remoteAddr
            local_addr:   App.osc.localAddr
            remote_port:  App.osc.remotePort
            local_port:   App.osc.localPort
            avg_in:       App.oscMonitor.averageIn
            avg_out:      App.oscMonitor.averageOut
        }

        MidiStatus {
            id: midiStatus

            Layout.minimumWidth: 400
            Layout.minimumHeight: 400

            is_open:      App.midi.isOpen
            input_iface:  App.midi.inputInterface.name
            output_iface: App.midi.outputInterface.name
            avg_in:       App.midiMonitor.averageIn
            avg_out:      App.midiMonitor.averageOut
        }
    }

    Connections {
        target: App.oscMonitor
        function onUpdated() {
            oscStatus.push(App.oscMonitor.lastCountIn, App.oscMonitor.lastCountOut)
        }
    }

    Connections {
        target: App.midiMonitor
        function onUpdated() {
            midiStatus.push(App.midiMonitor.lastCountIn, App.midiMonitor.lastCountOut)
        }
    }
}

/*##^##
Designer {
    D{i:0;autoSize:true;height:480;width:640}
}
##^##*/
