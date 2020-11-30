import QtQuick 2.0
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.11

GenericStatus {
    id: root

    property alias remote_addr: remoteAddr.text
    property alias remote_port: remotePort.value
    property alias local_port:  localPort.value

    title: qsTr("OSC Settings")
    icon:  '\uf1eb'

    is_ok: true
    graph: false

    Label { text: qsTr("Controller IP: ") }
    TextField {
        id: remoteAddr
        horizontalAlignment: TextInput.AlignHCenter
        Layout.fillWidth: true
        placeholderText: qsTr("Remote Address")
    }

    Label { text: qsTr("Outgoing Port: ") }

    SpinBox {
        id: remotePort
        Layout.fillWidth: true
        to: 65535
        editable: true
    }

    Label { text: qsTr("Incoming Port: ") }

    SpinBox {
        id: localPort
        Layout.fillWidth: true
        to: 65535
        editable: true
    }
}
