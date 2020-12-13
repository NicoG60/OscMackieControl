import QtQuick 2.12
import QtQuick.Layouts 1.12

Rectangle {
    id: root
    width: 480
    height: 480
    color: "#eae6e6"
    radius: 20
    border.color: "#000000"
    border.width: 2

    Rectangle {
        id: head
        height: 60
        color: "#000000"
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.top: parent.top
        anchors.rightMargin: 0
        anchors.leftMargin: 0
        anchors.topMargin: 0

        Rectangle {
            id: lcd
            x: 8
            y: 24
            width: 240
            height: 15
            color: "#006fd4"
            anchors.verticalCenter: parent.verticalCenter
            anchors.left: parent.left
            anchors.verticalCenterOffset: 1
            anchors.leftMargin: 10
        }

        Rectangle {
            id: assignment
            x: 267
            y: 31
            width: 38
            height: 15
            color: "#6a0000"
            anchors.verticalCenter: parent.verticalCenter

            Rectangle {
                id: lb_assignemnt
                height: 5
                color: "#ffffff"
                anchors.left: parent.left
                anchors.right: parent.right
                anchors.top: parent.bottom
                anchors.topMargin: 0
                anchors.rightMargin: 0
                anchors.leftMargin: 0
            }
        }

        Rectangle {
            id: timecode
            x: 346
            y: 135
            width: 116
            height: 15
            color: "#6a0000"
            anchors.verticalCenter: parent.verticalCenter
            anchors.right: parent.right
            anchors.rightMargin: 30

            Rectangle {
                id: lb_timecode
                height: 5
                color: "#ffffff"
                anchors.left: parent.left
                anchors.right: parent.right
                anchors.top: parent.bottom
                anchors.rightMargin: 0
                anchors.leftMargin: 0
                anchors.topMargin: 0
            }

            Rectangle {
                id: led1
                width: 5
                height: 5
                color: "#6a0000"
                radius: 2
                anchors.right: parent.left
                anchors.top: parent.top
                anchors.topMargin: 0
                anchors.rightMargin: 10
            }

            Rectangle {
                id: led2
                width: 5
                height: 5
                color: "#6a0000"
                radius: 2
                anchors.right: parent.left
                anchors.bottom: parent.bottom
                anchors.bottomMargin: 0
                anchors.rightMargin: 10
            }

            Rectangle {
                id: led3
                width: 5
                height: 5
                color: "#6a0000"
                radius: 2
                anchors.verticalCenter: parent.verticalCenter
                anchors.left: parent.right
                anchors.leftMargin: 10
            }
        }
    }

    Row {
        id: row
        width: 2.12
        height: 350
        anchors.left: parent.left
        anchors.top: parent.top
        anchors.topMargin: 70
        anchors.leftMargin: 10

        Repeater {
            model: 8

            Column {
                id: track
                width: 30
                anchors.top: parent.top
                anchors.bottom: parent.bottom
                spacing: 15
                anchors.bottomMargin: 0
                anchors.topMargin: 0

                Rectangle {
                    id: lb
                    width: 30
                    height: 5
                    color: "#999999"
                }

                Rectangle {
                    id: vpot
                    width: 25
                    height: 25
                    color: "#000000"
                    radius: 12
                    anchors.horizontalCenter: parent.horizontalCenter
                }

                Rectangle {
                    id: rec
                    width: 15
                    height: 15
                    color: "#80ff0000"
                    radius: 5
                    anchors.horizontalCenter: parent.horizontalCenter
                }

                Rectangle {
                    id: vu
                    width: 5
                    height: 5
                    color: "#00ff00"
                    radius: 2
                    anchors.horizontalCenter: parent.horizontalCenter
                }

                Rectangle {
                    id: solo
                    width: 20
                    height: 15
                    color: "#80ff0000"
                    radius: 5
                    anchors.horizontalCenter: parent.horizontalCenter
                }

                Rectangle {
                    id: mute
                    width: 20
                    height: 15
                    color: "#80ff9000"
                    radius: 5
                    anchors.horizontalCenter: parent.horizontalCenter
                }

                Rectangle {
                    id: sel
                    width: 20
                    height: 15
                    color: "#800090ff"
                    radius: 5
                    anchors.horizontalCenter: parent.horizontalCenter
                }

                Rectangle {
                    id: sp
                    width: 30
                    height: 5
                    color: "#999999"
                    anchors.horizontalCenter: parent.horizontalCenter
                }

                Rectangle {
                    id: fader
                    width: 1
                    height: 150
                    color: "#000000"
                    anchors.horizontalCenter: parent.horizontalCenter
                }
            }
        }
    }

    Rectangle {
        id: vpot_assign
        x: 256
        y: 70
        width: 55
        height: 100
        color: "#000000"

        Grid {
            id: grid
            anchors.fill: parent
            anchors.rightMargin: 5
            anchors.leftMargin: 5
            spacing: 5
            columns: 2
            anchors.topMargin: 20

            Repeater {
                model: 6

                Rectangle {
                    id: rectangle
                    width: 20
                    height: 15
                    color: "#808080"
                }
            }
        }
    }

    Grid {
        id: grid1
        x: 261
        y: 186
        Repeater {
            model: 6
            Rectangle {
                id: rectangle1
                width: 20
                height: 15
                color: "#808080"
            }
        }
        spacing: 5
        columns: 2
    }

    Rectangle {
        id: master
        x: 100
        y: 289
        width: 1
        height: 150
        color: "#000000"
        anchors.horizontalCenterOffset: 44
        anchors.horizontalCenter: parent.horizontalCenter
    }

    Row {
        id: row1
        x: 317
        y: 95
        width: 155
        height: 15
        spacing: 5

        Repeater {
            model: 2
            Rectangle {
                id: rectangle2
                width: 15
                height: 10
                color: "#808080"
            }
        }
    }

    Row {
        id: row2
        x: 317
        y: 141
        width: 155
        height: 15
        Repeater {
            model: 8
            Rectangle {
                id: rectangle3
                width: 15
                height: 10
                color: "#808080"
            }
        }
        spacing: 5
    }

    Row {
        id: row3
        x: 317
        y: 194
        width: 155
        height: 15
        Repeater {
            model: 8
            Rectangle {
                id: rectangle4
                width: 15
                height: 10
                color: "#808080"
            }
        }
        spacing: 5
    }

    Grid {
        id: grid2
        x: 317
        y: 232
        width: 155
        height: 55
        Repeater {
            model: 12
            Rectangle {
                id: rectangle5
                width: 20
                height: 15
                color: "#808080"
            }
        }
        spacing: 7
        columns: 6
    }

    Grid {
        id: grid3
        x: 317
        y: 289
        width: 155
        height: 20
        Repeater {
            model: 6
            Rectangle {
                id: rectangle6
                width: 20
                height: 15
                color: "#808080"
            }
        }
        spacing: 7
        columns: 6
    }

    Grid {
        id: grid4
        x: 316
        y: 317
        width: 163
        height: 20
        Repeater {
            model: 5
            Rectangle {
                id: rectangle7
                width: 30
                height: 20
                color: "#808080"
            }
        }
        spacing: 2
        columns: 6
    }

    Rectangle {
        id: rectangle8
        x: 338
        y: 357
        width: 15
        height: 15
        color: "#808080"
        radius: 7
    }

    Rectangle {
        id: rectangle9
        x: 316
        y: 377
        width: 15
        height: 15
        color: "#808080"
        radius: 7
    }

    Rectangle {
        id: rectangle10
        x: 361
        y: 376
        width: 15
        height: 15
        color: "#808080"
        radius: 7
    }

    Rectangle {
        id: rectangle11
        x: 338
        y: 395
        width: 15
        height: 15
        color: "#808080"
        radius: 7
    }

    Rectangle {
        id: rectangle12
        x: 338
        y: 377
        width: 15
        height: 15
        color: "#808080"
        radius: 7
    }

    Rectangle {
        id: rectangle13
        x: 457
        y: 357
        width: 15
        height: 15
        color: "#808080"
        radius: 7
    }

    Rectangle {
        id: rectangle14
        x: 390
        y: 376
        width: 80
        height: 80
        color: "#808080"
        radius: 40
    }
}


