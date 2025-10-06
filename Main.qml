import QtQuick
import QtQuick.Controls
import ElevatorSim 1.0

Window {
    width: 960; height: 600; visible: true
    title: "Elevator Simulator (Qt)"

    Building { id: building }

    Column {
      anchors.fill: parent
      anchors.margins: 16
      spacing: 12

      // ① 타이틀 (맨 위)
      Text {
        id: title
        text: "Elevator Simulator"
        font.pixelSize: 28; font.bold: true
        color: "#1d2b4a"
        horizontalAlignment: Text.AlignHCenter
        anchors.horizontalCenter: parent.horizontalCenter
      }

      // ② 설정바 (타이틀 바로 아래)
      Row {
        id: toolbar
        spacing: 8
        Text { text: "엘리베이터"; color: "#333" }
        SpinBox { id: carCnt; from:1; to:8; value: 3; editable: true }
        Text { text: "건물"; color: "#333" }
        SpinBox { id: flr; from:2; to:40; value: 10; editable: true }
        Button { text: "적용"; onClicked: building.configure(flr.value, carCnt.value) }
        Button {
          text: building.running ? "정지" : "시작"
          onClicked: building.running ? building.stop() : building.start()
        }
        Text { text: "Tick(ms)"; color:"#333"; anchors.verticalCenter: parent.verticalCenter }
        Slider {
          id: spd; from: 60; to: 300; width: 180
          value: building.tickMs
          onValueChanged: building.tickMs = Math.round(value)
        }
        Text { text: `${Math.round(spd.value)}` }

        CheckBox {
          id: spawnToggle
          text: "자동 승객"
          checked: true
          onToggled: building.autoSpawn = checked
        }
      }

      // ③ 표(스크롤 가능)
      ScrollView {
        id: scroller
        clip: true
        anchors.left: parent.left; anchors.right: parent.right
        // 남은 공간 전부 차지
        height: parent.height - (title.height + toolbar.height + 48)

        BuildingView {
          id: view
          width: parent.width
          // 내용 높이를 컴포넌트가 계산해 주도록 (아래 2)참고)
          implicitHeight: contentHeight
          floors: building.floors
          cars: building.cars
          property var waitUp: building.waitUp
          property var waitDown: building.waitDown
          property var waitDestLabel: building.waitDestLabel  // ④에서 추가할 라벨
        }
      }
    }
}
