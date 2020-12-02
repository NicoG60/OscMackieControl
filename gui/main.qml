import QtQuick 2.12
import QtQuick.Controls 2.12
import QtQuick.Controls.Material 2.12
import QtQuick.Layouts 1.12

ApplicationWindow {

    minimumWidth: 1024
    minimumHeight: bar.height + 768

    maximumWidth: 1024
    maximumHeight: bar.height + 768

    visible: true
    title: qsTr("Mackie Control to OSC Bridge")

    Material.theme: Material.System
    Material.primary: Material.BlueGrey
    Material.accent: Material.Blue

    header: TopBar {
        id: bar
    }

    StackLayout {
         anchors.fill: parent
         currentIndex: bar.currentIndex

         Status {
             id: statusTab
         }
         Settings {
             id: settingsTab
         }
         Mapping {
             id: activityTab
         }
     }
}
