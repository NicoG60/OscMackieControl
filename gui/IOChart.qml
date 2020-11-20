import QtQuick 2.12
import QtCharts 2.12
import QtQuick.Controls.Material 2.12

Item {
    id: root

    property real avg_in: 0
    property real avg_out: 0

    ChartView {
        id: chartView
        anchors.fill: parent
        legend.alignment: Qt.AlignBottom
        animationDuration: 1500
        animationEasingCurve.type: Easing.Linear
        animationOptions: ChartView.SeriesAnimations
        antialiasing: true

        margins.left:   0
        margins.right:  0
        margins.top:    0
        margins.bottom: 0

        //theme: Material.theme == Material.Light ? ChartView.ChartThemeLight : ChartView.ChartThemeDark

        backgroundColor: "transparent"

        legend.labelColor: Material.theme == Material.Light ? "black" : "white"

        ValueAxis {
            id: axisY
            visible: false
            min: -150
            max: 150
            titleText: qsTr("packets")
        }

        ValueAxis {
            id: axisX
            visible: false
            min: 0
            max: 30
            titleText: qsTr("seconds")
        }

        LineSeries {
            id: inSeries
            name: qsTr("Input (%1 p/s)").arg(avg_in)
            axisX: axisX
            axisY: axisY
            color: Material.color(Material.Blue)
        }
        LineSeries {
            id: outSeries
            name: qsTr("Output (%1 p/s)").arg(avg_out)
            axisX: axisX
            axisY: axisY
            color: Material.color(Material.Red)
        }
    }

    Component.onCompleted: {
        for(let i = 0; i < axisX.max; i++) {
            inSeries.append(i, 0)
            outSeries.append(i, 0)
        }
    }

    function push(last_in, last_out) {
        shift(inSeries, last_in)
        shift(outSeries, -last_out)
    }

    function shift(serie, value) {
        let pt = { x: 0, y: value }
        let tmp = pt;
        for(let i = 0; i < serie.count; i++) {
            tmp = serie.at(i)
            serie.replace(tmp.x, tmp.y, pt.x, pt.y)
            pt = tmp
            pt.x++
        }
    }
}
