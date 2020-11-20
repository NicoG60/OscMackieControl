import QtQuick 2.12
import QtQuick.Controls 2.12
import QtQuick.Controls.Material 2.12

TabBar {
    id: root
    width: parent.width

    Material.elevation: 6

    TabButton {
         text: qsTr("Home")
         icon.source: "qrc:/icons/eye.svg"
     }
     TabButton {
         text: qsTr("Settings")
         icon.source: "qrc:/icons/cog.svg"
     }
     TabButton {
         text: qsTr("Mapping")
         icon.source: "qrc:/icons/map-signs.svg"
     }
}
