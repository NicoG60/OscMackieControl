import QtQuick 2.12
import QtQuick.Controls 2.12

RoundButton {
    FontLoader {
        id: loader
        source: "qrc:/fonts/fa-solid-900.ttf"
    }

    font: loader.name
}
