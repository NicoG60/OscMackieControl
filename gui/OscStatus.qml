import QtQuick 2.12
import QtQuick.Controls 2.12

GenericStatus {
    id: root

    property alias  is_listening: root.is_ok
    property string remote_addr: "127.0.0.1"
    property string local_addr: "127.0.0.1"
    property int    remote_port: 0
    property int    local_port: 0

    title: qsTr("OSC Status")
    icon:  '\uf1eb'

    Label { text: qsTr("Your IP: ") }
    Label { text: local_addr }

    Label { text: qsTr("Controller IP:") }
    Label { text: remote_addr }

    ToolSeparator {
        orientation: Qt.Horizontal
    }
    Label { text: " " }

    Label { text: qsTr("Input Port: ") }
    Label { text: local_port }

    Label { text: qsTr("Output Port:") }
    Label { text: remote_port }
}
