import QtQuick
import QtQuick.Controls

Item {
    id: root
    property bool running: false
    property int tickMs: 120

    signal toggleRun()
    signal tickChanged(int ms)
    signal reset()
    signal hallUp(int floor)
    signal hallDown(int floor)

    Column {
        spacing: 10

        Text { text: running ? "Running" : "Stopped"; font.pixelSize: 18 }

        Row {
            spacing: 6
            Text { text: "엘리베이터 수" }
            SpinBox { id: carCnt; from: 1; to: 8; value: 3; editable: true }
            Text { text: "최대 층" }
            SpinBox { id: flr; from: 2; to: 40; value: 10; editable: true }
            Button { text: "적용"; onClicked: building.configure(flr.value, carCnt.value) }
        }

        Row {
            spacing: 6
            Button { text: running ? "Stop" : "Start"; onClicked: root.toggleRun() }
            Button { text: "Reset(10층/3대)"; onClicked: root.reset() }
        }

        Row {
            spacing: 6
            Text { text: "Tick(ms)" }
            Slider {
                id: spd; from: 60; to: 300; width: 180
                value: tickMs
                onValueChanged: root.tickChanged(Math.round(value))
            }
            Text { text: `${Math.round(spd.value)}` }
        }

        GroupBox {
            title: "Hall Calls"
            Column {
                spacing: 6; padding: 8
                Row { spacing: 6
                    TextField { id: f1; width: 60; placeholderText: "층" }
                    Button { text: "Up";   onClicked: root.hallUp(parseInt(f1.text)) }
                    Button { text: "Down"; onClicked: root.hallDown(parseInt(f1.text)) }
                }
            }
        }
    }
}
