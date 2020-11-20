import QtQuick 2.12
import QtQuick.Controls 2.12
import OscMackieControl.ima 1.0

TabBar {
    width: parent.width
    TabButton {
         text: qsTr("Home")
         icon: IconManager.icon("home")
     }
     TabButton {
         text: qsTr("Discover")
     }
     TabButton {
         text: qsTr("Activity")
     }
}
