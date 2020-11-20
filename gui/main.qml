import QtQuick 2.12
import QtQuick.Controls 2.12
import QtQuick.Controls.Material 2.12

ApplicationWindow {
    width: 1024
    height: 768

    minimumWidth: width
    minimumHeight: height

    maximumWidth: width
    maximumHeight: height

    Material.theme: Material.System
    Material.primary: Material.Blue
    Material.accent: Material.BlueGrey

    header: TopBar {}

    visible: true
    title: qsTr("Mackie Control to OSC Bridge")
}
