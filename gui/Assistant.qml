import QtQuick 2.12
import QtQuick.Layouts 1.12
import QtQuick.Controls 2.12
import QtQuick.Controls.Material 2.12
import OscMackieControl 1.0

Popup {
    id: root

    modal: true
    Material.elevation: 20

    IconLabel {
        id: title
        padding: 10
        icon: '\uf059'
        text: qsTr("Configuration Assistant")
        anchors.top: parent.top
        anchors.topMargin: 0
        anchors.horizontalCenter: parent.horizontalCenter
    }

    ColumnLayout {
        anchors.fill: parent
        anchors.leftMargin: 64
        anchors.rightMargin: 64
        anchors.topMargin: title.implicitHeight

        Label {
            text: qsTr("Please click on each value and match the settings\nso your iPad screen looks like this one")
            topPadding: 50
            horizontalAlignment: Qt.AlignHCenter
            Layout.alignment: Qt.AlignCenter
        }

        TouchOSCAnimation {
            id: touchOsc
            Layout.alignment: Qt.AlignCenter
            onShouldUpdateAddr: root.apply_remote_only()
        }

        Button {
            text: qsTr("Apply")
            highlighted: true
            Layout.alignment: Qt.AlignHCenter
            onClicked: root.apply()
        }
    }

    onVisibleChanged: hydrate()

    function hydrate() {
        if(!root.visible)
            return

        let o = App.oscStatus

        touchOsc.remote_addr = App.osc.remoteAddr
        touchOsc.remote_port = App.osc.remotePort
        touchOsc.local_port  = App.osc.localPort
        touchOsc.local_addr  = App.osc.localAddr
        touchOsc.anim_start  = true
    }

    function apply() {
        App.osc.remoteAddr = touchOsc.remote_addr
        App.osc.remotePort = touchOsc.remote_port
        App.osc.localPort  = touchOsc.local_port

        touchOsc.anim_start = false
        close();
    }

    function apply_remote_only() {
        App.osc.remoteAddr = touchOsc.remote_addr
        hydrate();
    }
}

/*##^##
Designer {
    D{i:0;autoSize:true;height:480;width:640}
}
##^##*/
