import QtQuick 2.12
import QtQuick.Controls 2.12

Row {
    id: root
    spacing: 10
    leftPadding: 5
    rightPadding: 5

    property alias iconSize: icon.font.pointSize
    property alias iconColor: icon.color
    property alias icon: icon.text
    property alias text: text.text

    FontLoader {
        id: loader
        source: "qrc:/fonts/fa-solid-900.ttf"
    }

    Label {
        id: icon
        font.pointSize: 32
        font.family: loader.name
        anchors.verticalCenter: parent.verticalCenter
    }
    Label {
        id: text
        anchors.verticalCenter: parent.verticalCenter
    }
}
