import QtQuick 2.12
import QtQuick.Controls 2.12

GenericStatus {
    id: root

    property alias  is_open: root.is_ok
    property string input_iface: ""
    property string output_iface: ""

    title: qsTr("MIDI Status")
    icon:  '\uf001'

    Label { text: qsTr("Input: ") }
    Label { text: input_iface }

    Label { text: qsTr("Output:") }
    Label { text: output_iface }
}
