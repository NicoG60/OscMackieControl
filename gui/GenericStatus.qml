import QtQuick 2.12
import QtQuick.Layouts 1.12
import QtQuick.Controls 2.12
import QtQuick.Controls.Material 2.12

GroupBox {
    id: control

    default property alias texts: grid.children
    property alias icon: label.icon
    property bool  is_ok: false
    property alias avg_in: chart.avg_in
    property alias avg_out: chart.avg_out
    property bool  graph: true

    Material.elevation: 2
    Material.background: Qt.lighter(parent.Material.background)

    label: IconLabel {
        id: label
        iconColor: Material.color(Material.Green)
        iconSize: 16
        text: control.title
    }

    states: State {
        name: "error"
        when: !control.is_ok
        PropertyChanges {
            target: label
            iconColor: Material.color(Material.Red)
            icon: '\uf00d'
        }
    }

    ColumnLayout {
        anchors.fill: parent

        GridLayout {
            id: grid

            columnSpacing: 5
            rowSpacing: 5

            columns: 2
        }

        IOChart {
            id: chart

            visible: control.graph

            Layout.alignment: Qt.AlignBottom
            Layout.preferredHeight: parent.width / 2
            Layout.fillWidth: true
        }
    }

    function push(last_in, last_out) {
        chart.push(last_in, last_out)
    }
}

