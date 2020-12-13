import QtQuick 2.12
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.11
import QtQuick.Controls.Material 2.12

Rectangle {
    id: root

    property alias local_addr: local_addr.text
    property alias remote_addr: remote_addr.text
    property alias local_port: local_port.text
    property alias remote_port: remote_port.text
    property bool anim_start: false

    signal shouldUpdateAddr()

    width: 320
    height: 320
    color: "#efeff4"
    clip: true

    Material.elevation: 6

    Rectangle {
        id: topBar
        height: 48
        color: "black"
        border.width: 0
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.top: parent.top
        anchors.topMargin: 0
        anchors.rightMargin: 0
        anchors.leftMargin: 0


        // ============ NOTIF BAR
        Text {
            text: "iPad"
            anchors.left: parent.left
            anchors.top: parent.top
            font.pointSize: 9
            font.bold: true
            font.family: "Arial"
            anchors.leftMargin: 6
            anchors.topMargin: 6
            color: "white"
        }

        Text {
            color: "#ffffff"
            text: "00:00"
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.top: parent.top
            font.pointSize: 9
            font.bold: true
            anchors.topMargin: 6
            font.family: "Arial"
        }

        Text {
            text: "100%"
            anchors.right: parent.right
            anchors.top: parent.top
            font.pointSize: 9
            font.bold: true
            font.family: "Arial"
            anchors.rightMargin: 6
            anchors.topMargin: 6
            color: "white"
        }

        // ========== TOP BAR
        Text {
            id: leftTopControl1
            text: "Help"
            anchors.left: parent.left
            anchors.bottom: parent.bottom
            font.pointSize: 12
            font.bold: true
            font.family: "Arial"
            anchors.leftMargin: 10
            anchors.bottomMargin: 10
            color: "#2a6fe6"
        }

        Text {
            id: centerTopControl1
            color: "#ffffff"
            text: "TouchOSC"
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.bottom: parent.bottom
            font.pointSize: 12
            font.bold: true
            anchors.bottomMargin: 10
            font.family: "Arial"
        }

        Text {
            id: leftTopControl2
            text: "< TouchOSC"
            anchors.left: parent.left
            anchors.bottom: parent.bottom
            font.pointSize: 12
            font.bold: true
            font.family: "Arial"
            anchors.leftMargin: 10
            anchors.bottomMargin: 10
            color: "#2a6fe6"
            opacity: 0
        }

        Text {
            id: centerTopControl2
            color: "#ffffff"
            text: "OSC"
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.horizontalCenterOffset: parent.width
            anchors.bottom: parent.bottom
            font.pointSize: 12
            font.bold: true
            anchors.bottomMargin: 10
            font.family: "Arial"
            opacity: 0
        }

        Text {
            id: rightTopControl
            text: "Done"
            anchors.right: parent.right
            anchors.bottom: parent.bottom
            font.pointSize: 12
            font.bold: true
            font.family: "Arial"
            anchors.rightMargin: 10
            anchors.bottomMargin: 10
            color: "#2a6fe6"
        }
    }
    Item {
        id: item2
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.top: parent.top
        anchors.topMargin: 75

        Text {
            id: connections
            color: "#b0b0b5"
            text: qsTr("CONNECTIONS")
            anchors.left: parent.left
            anchors.top: parent.top
            anchors.leftMargin: 32
            anchors.topMargin: 0

            font.pointSize: 9
        }

        Frame {
            id: connectionsMenu
            anchors.left: parent.left
            anchors.right: parent.right
            anchors.top: parent.top
            anchors.topMargin: connections.height+5
            bottomPadding: 10
            topPadding: 10
            rightPadding: 32
            leftPadding: 32
            anchors.rightMargin: 0
            anchors.leftMargin: 0
            background: Rectangle {
                color: "white"
                border.width: 1
                border.color: "#e0dfe3"
            }

            ColumnLayout {
                anchors.fill: parent
                RowLayout {
                    Layout.fillWidth: true
                    Text {
                        color: "#7283a6"
                        text: qsTr("OSC: %1").arg(root.local_addr)
                        font.pointSize: 12
                    }
                    Item {
                        Layout.fillWidth: true
                    }
                    Text {
                        color: "#aeaeae"
                        text: qsTr(">")
                        font.pointSize: 12
                        Layout.alignment: Qt.AlignRight | Qt.AlignVCenter
                    }
                }

                ToolSeparator {
                    Layout.fillWidth: true
                    leftPadding: 0
                    rightPadding: 0
                    bottomPadding: 5
                    topPadding: 5
                    orientation: Qt.Horizontal
                }

                RowLayout {
                    Layout.fillWidth: true
                    Text {
                        color: "#aeaeae"
                        text: qsTr("TouchOSC Bridge: Disabled")
                        font.pointSize: 12
                    }
                    Item {
                        Layout.fillWidth: true
                    }
                    Text {
                        color: "#aeaeae"
                        text: qsTr(">")
                        font.pointSize: 12
                    }
                }

                ToolSeparator {
                    Layout.fillWidth: true
                    leftPadding: 0
                    rightPadding: 0
                    bottomPadding: 5
                    topPadding: 5
                    orientation: Qt.Horizontal
                }

                RowLayout {
                    Layout.fillWidth: true
                    Text {
                        color: "#aeaeae"
                        text: qsTr("CoreMIDI: Disabled")
                        font.pointSize: 12
                    }
                    Item {
                        Layout.fillWidth: true
                    }
                    Text {
                        color: "#aeaeae"
                        text: qsTr(">")
                        font.pointSize: 12
                    }
                }
            }
        }

        Frame {
            id: oscMenu
            anchors.left: parent.left
            width: parent.width
            anchors.top: parent.top
            leftPadding: 32
            anchors.topMargin: 0
            anchors.leftMargin: parent.width
            topPadding: 5
            ColumnLayout {
                anchors.fill: parent
                RowLayout {
                    Layout.fillWidth: true
                    Text {
                        color: "#5a5a5a"
                        text: qsTr("Enabled")
                        font.pointSize: 12
                    }

                    Item {
                        Layout.fillWidth: true
                    }

                    Rectangle {
                        id: rectangle
                        width: 48
                        height: 24
                        radius: 13
                        color: "#76d472"
                        border.width: 0

                        Rectangle {
                            width: 20
                            height: 20
                            radius: 10
                            color: "#ffffff"
                            border.width: 0
                            anchors.verticalCenter: parent.verticalCenter
                            anchors.right: parent.right
                            anchors.rightMargin: 2
                        }
                    }

                }

                ToolSeparator {
                    leftPadding: 0
                    Layout.fillWidth: true
                    topPadding: 0
                    orientation: Qt.Horizontal
                    rightPadding: 0
                    bottomPadding: 5
                }

                RowLayout {
                    Layout.fillWidth: true
                    Text {
                        color: "#5a5a5a"
                        text: qsTr("Host")
                        font.pointSize: 12
                    }

                    Item {
                        Layout.fillWidth: true
                    }

                    TextInput {
                        id: local_addr
                        color: "#7283a6"
                        font.pointSize: 12
                        readOnly: true
                        selectByMouse: true
                        ToolTip.visible: activeFocus
                        ToolTip.text: qsTr("Read only: Write the same value to TouchOSC.\nClick on Apply once you're happy with the settings")
                        onAccepted: {
                            remote_addr.forceActiveFocus()
                            remote_addr.selectAll()
                        }

                        KeyNavigation.tab: remote_addr
                    }
                }

                ToolSeparator {
                    leftPadding: 0
                    Layout.fillWidth: true
                    topPadding: 5
                    orientation: Qt.Horizontal
                    rightPadding: 0
                    bottomPadding: 5
                }

                RowLayout {
                    Layout.fillWidth: true
                    Text {
                        color: "#5a5a5a"
                        text: qsTr("Port (outgoing)")
                        font.pointSize: 12
                    }

                    Item {
                        Layout.fillWidth: true
                    }

                    TextInput {
                        id: local_port
                        color: "#7283a6"
                        font.pointSize: 12
                        selectByMouse: true
                        ToolTip.visible: activeFocus
                        ToolTip.text: qsTr("Write the port number of your choice both here and in TouchOSC.\nHit Return when you're done.")
                        validator: IntValidator {
                            bottom: 0
                            top: 65535
                        }
                        onAccepted: {
                            local_addr.forceActiveFocus()
                            local_addr.selectAll()
                        }

                        KeyNavigation.tab: local_addr
                    }
                }

                ToolSeparator {
                    leftPadding: 0
                    Layout.fillWidth: true
                    topPadding: 5
                    orientation: Qt.Horizontal
                    rightPadding: 0
                    bottomPadding: 5
                }

                RowLayout {
                    Layout.fillWidth: true
                    Text {
                        color: "#5a5a5a"
                        text: qsTr("Port (incoming)")
                        font.pointSize: 12
                    }

                    Item {
                        Layout.fillWidth: true
                    }

                    TextInput {
                        id: remote_port
                        color: "#7283a6"
                        font.pointSize: 12
                        selectByMouse: true
                        ToolTip.visible: activeFocus
                        ToolTip.text: qsTr("Write the port number of your choice both here and in TouchOSC.\nHit Return when you're done.")
                        validator: IntValidator {
                            bottom: 0
                            top: 65535
                        }
                        onAccepted: {
                            local_port.forceActiveFocus()
                            local_port.selectAll()
                        }

                        KeyNavigation.tab: local_port
                    }
                }

                ToolSeparator {
                    leftPadding: 0
                    Layout.fillWidth: true
                    topPadding: 5
                    orientation: Qt.Horizontal
                    rightPadding: 0
                    bottomPadding: 5
                }

                RowLayout {
                    Layout.fillWidth: true
                    Text {
                        color: "#5a5a5a"
                        text: qsTr("Local IP address")
                        font.pointSize: 12
                    }

                    Item {
                        Layout.fillWidth: true
                    }

                    TextInput {
                        id: remote_addr
                        color: "#aeaeae"
                        font.pointSize: 12
                        selectByMouse: true
                        ToolTip.visible: activeFocus
                        ToolTip.text: qsTr("Get the value from TouchOSC and write it here.\nHit Return when you're done.")
                        validator: RegExpValidator {
                            regExp: /(\d{1,3})(\.\d{1,3}){3}/
                        }
                        onAccepted: {
                            if(remote_addr.acceptableInput)
                                shouldUpdateAddr()

                            remote_port.forceActiveFocus()
                            remote_port.selectAll()
                        }

                        KeyNavigation.tab: remote_port
                    }
                }
            }
            rightPadding: 32
            background: Rectangle {
                color: "#ffffff"
                border.color: "#e0dfe3"
                border.width: 1
            }
            bottomPadding: 10
            anchors.rightMargin: 0
        }
    }

    Rectangle {
        id: finger

        width: 32
        height: 32

        color: "#808080"
        border.color: "#dddddd"
        border.width: 4

        radius: 24
        opacity: 0
    }

    states: [
        State {
            name: "osc"
            when: root.anim_start

            PropertyChanges {
                target: finger
                x: 48
                y: 95
            }

            PropertyChanges {
                target: leftTopControl1
                opacity: 0
            }

            PropertyChanges {
                target: leftTopControl2
                opacity: 1
            }

            PropertyChanges {
                target: centerTopControl1
                opacity: 0
            }

            PropertyChanges {
                target: centerTopControl2
                opacity: 1
                anchors.horizontalCenterOffset: 0
            }

            PropertyChanges {
                target: oscMenu
                anchors.leftMargin: 0
            }
        }
    ]

    transitions: [
        Transition {
            to: "osc"

            SequentialAnimation {
                PauseAnimation {
                    duration: 1000
                }

                NumberAnimation {
                    target: finger
                    properties: "opacity"
                    to: 0.75
                    duration: 500
                }

                ParallelAnimation {
                    NumberAnimation {
                        target: finger
                        properties: "x"
                        duration: 500
                    }

                    NumberAnimation {
                        target: finger
                        properties: "y"
                        duration: 500
                    }
                }

                PauseAnimation {
                    duration: 200
                }

                ParallelAnimation {
                    NumberAnimation {
                        target: finger
                        properties: "width"
                        to: 24
                        duration: 100
                    }

                    NumberAnimation {
                        target: finger
                        properties: "height"
                        to: 24
                        duration: 100
                    }
                }

                ParallelAnimation {
                    NumberAnimation {
                        target: finger
                        properties: "width"
                        to: 32
                        duration: 100
                    }

                    NumberAnimation {
                        target: finger
                        properties: "height"
                        to: 32
                        duration: 100
                    }
                }

                ParallelAnimation {
                    NumberAnimation {
                        target: finger
                        properties: "opacity"
                        to: 0
                        duration: 500
                    }

                    NumberAnimation {
                        target: leftTopControl1
                        properties: "opacity"
                        duration: 200
                        easing.type: Easing.OutCubic
                    }

                    NumberAnimation {
                        target: centerTopControl1
                        properties: "opacity"
                        duration: 200
                        easing.type: Easing.OutCubic
                    }

                    NumberAnimation {
                        target: rightTopControl
                        properties: "opacity"
                        duration: 200
                        easing.type: Easing.OutCubic
                    }
                }

                ParallelAnimation {
                    NumberAnimation {
                        target: centerTopControl2
                        properties: "anchors.horizontalCenterOffset"
                        duration: 200
                        easing.type: Easing.OutCubic
                    }

                    NumberAnimation {
                        target: centerTopControl2
                        properties: "opacity"
                        duration: 200
                        easing.type: Easing.OutCubic
                    }

                    NumberAnimation {
                        target: oscMenu;
                        properties: "anchors.leftMargin";
                        duration: 200
                        easing.type: Easing.OutCubic
                    }
                }

                NumberAnimation {
                    target: leftTopControl2
                    properties: "opacity"
                    duration: 200
                    easing.type: Easing.OutCubic
                }

                ScriptAction {
                    script: {
                        remote_addr.forceActiveFocus()
                        remote_addr.selectAll()
                    }
                }
            }
        }
    ]
}


