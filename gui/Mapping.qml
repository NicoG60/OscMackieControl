import QtQuick 2.12
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.12
import OscMackieControl 1.0

Item {
    id: root

    Mcu {
        id: mcu
        anchors.verticalCenter: parent.verticalCenter
        anchors.verticalCenterOffset: -100
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.horizontalCenterOffset: -0.5*drawer.width*drawer.position

        McuButton {
            id: lcd
            x: 5
            y: 10
            width: 250
            height: 40
            text: "LCD"

            onClicked: {
                drawer.open()
                listView.model.loadLCD()
            }
        }

        McuButton {
            id: displays
            x: 260
            y: 10
            width: 210
            height: 40
            text: "Displays"

            onClicked: {
                drawer.open()
                listView.model.loadDisplays()
            }
        }

        McuButton {
            id: vpots
            x: 5
            y: 82
            width: 245
            height: 40
            text: "Virtual Pots"

            onClicked: {
                drawer.open()
                listView.model.loadvPot()
            }
        }

        McuButton {
            id: buttons
            x: 5
            y: 125
            width: 245
            height: 140
            text: "Channel Buttons"

            onClicked: {
                drawer.open()
                listView.model.loadButtons()
            }
        }

        McuButton {
            id: faders
            x: 5
            y: 280
            width: 300
            height: 175
            text: "Faders"

            onClicked: {
                drawer.open()
                listView.model.loadFaders()
            }
        }

        McuButton {
            id: assignment
            x: 253
            y: 82
            width: 60
            height: 183
            text: "Assignments"

            onClicked: {
                drawer.open()
                listView.model.loadAssignment()
            }
        }

        McuButton {
            id: global
            x: 314
            y: 82
            width: 160
            height: 132
            text: "Global buttons"

            onClicked: {
                drawer.open()
                listView.model.loadGlobal()
            }
        }

        McuButton {
            id: automation
            x: 314
            y: 220
            width: 160
            height: 56
            text: "Automation"

            onClicked: {
                drawer.open()
                listView.model.loadAutomation()
            }
        }

        McuButton {
            id: transport
            x: 314
            y: 282
            width: 160
            height: 178
            text: "Transport"

            onClicked: {
                drawer.open()
                listView.model.loadTransport()
            }
        }
    }

    Label {
        width: mcu.width

        anchors.top: mcu.bottom
        anchors.topMargin: 50
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.horizontalCenterOffset: -0.5*drawer.width*drawer.position

        horizontalAlignment: Qt.AlignHCenter

        text: qsTr("Don't forget to update your controller mapping as well.\nYou can download the TouchOSC as usual, this software should be recognized in the layout sync panel.");
    }

    Drawer {
        id: drawer
        edge: Qt.RightEdge
        width: 0.33 * parent.width
        height: parent.height
        dragMargin: 20
        dim: false
        modal: false

        ListView {
            id: listView
            model: App.mapping.createModel()

            anchors.fill: parent

            header: Label {
                text: qsTr("Mapping Editor")
                height: 60
                width: parent.width
                font.pointSize: 18
                font.bold: true
                verticalAlignment: Qt.AlignVCenter
                horizontalAlignment: Qt.AlignHCenter
            }

            delegate: Frame {
                width: listView.width
                topInset: 5
                bottomInset: 5
                leftInset: 5
                rightInset: 5

                ColumnLayout {
                    id: col
                    anchors.fill: parent
                    spacing: 5

                    Label {
                        text: model.name
                        Layout.fillWidth: true
                        font.bold: true
                    }
                    TextField {
                        Layout.fillWidth: true
                        visible: type === "base"
                        text: model.base
                        onEditingFinished: model.base = text

                        font.pointSize: 14

                        ToolTip.visible: hovered
                        ToolTip.text: qsTr(`Base address for component ${model.name}.`)
                    }
                    RowLayout {
                        visible: type === "button"
                        Layout.fillWidth: true

                        spacing: 5

                        TextField {
                            text: model.button
                            onEditingFinished: model.button = text

                            font.pointSize: 14

                            ToolTip.visible: hovered
                            ToolTip.text: qsTr(`Address for button ${model.name}.`)
                        }
                        TextField {
                            visible: text.length > 0
                            text: model.led
                            onEditingFinished: model.led = text

                            font.pointSize: 14

                            ToolTip.visible: hovered
                            ToolTip.text: qsTr(`Address for led indicator ${model.name}.`)
                        }
                    }
                    RowLayout {
                        visible: type === "button" || type === "label"
                        Layout.fillWidth: true

                        spacing: 5

                        TextField {
                            text: model.label
                            visible: text.length > 0
                            onEditingFinished: model.label = text

                            font.pointSize: 14

                            ToolTip.visible: hovered
                            ToolTip.text: qsTr(`Address of label ${model.name}.`)
                        }
                        TextField {
                            text: model.default
                            visible: text.length > 0
                            onEditingFinished: model.default = text

                            font.pointSize: 14

                            ToolTip.visible: hovered
                            ToolTip.text: qsTr(`Default text label of ${model.name}.`)
                        }
                    }
                    RowLayout {
                        visible: type === "button"
                        Layout.fillWidth: true

                        spacing: 5

                        TextField {
                            text: model.label2
                            visible: text.length > 0
                            onEditingFinished: model.label2 = text

                            font.pointSize: 14

                            ToolTip.visible: hovered
                            ToolTip.text: qsTr(`Address of second line label ${model.name}.`)
                        }
                        TextField {
                            visible: text.length > 0
                            text: model.default2
                            onEditingFinished: model.default2 = text

                            font.pointSize: 14

                            ToolTip.visible: hovered
                            ToolTip.text: qsTr(`Default text of second label ${model.name}.`)
                        }
                    }
                }
            }
        }
    }
}

/*##^##
Designer {
    D{i:0;autoSize:true;height:720;width:640}
}
##^##*/
