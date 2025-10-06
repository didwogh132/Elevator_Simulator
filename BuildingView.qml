import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

Item {
  id: root
  property int floors: 10
  property var cars: []
  property var waitUp: []
  property var waitDown: []
  property var waitDestLabel: []   // "↗ 15층" 같은 한 줄 라벨 (4)에서 공급

  // 행/헤더 높이 (디자인 한 번에 통일)
  property int rowH: 44
  property int headerH: 40
  // 스크롤뷰가 사용할 내용 높이
  property int contentHeight: headerH + floors * (rowH + grid.rowSpacing) + 16

  Rectangle { anchors.fill: parent; color: "#f4f7fb"; radius: 8 }

  GridLayout {
    id: grid
    anchors.fill: parent; anchors.margins: 8
    columns: (cars ? cars.length : 0) + 2   // [층수] + [카들] + [대기/목표]
    rowSpacing: 6; columnSpacing: 6

    // ─ 헤더 행 (중앙정렬, 동일 높이)
    Rectangle { Layout.row: 0; Layout.column: 0; height: headerH; color: "#e9eff7"; radius: 6
      border.color: "#d0d9e6"
      Text { anchors.centerIn: parent; text: "층수"; color:"#1d2b4a"; font.bold: true } }

    Repeater {
      model: cars ? cars.length : 0
      Rectangle {
        Layout.row: 0; Layout.column: index+1; height: headerH; color: "#e9eff7"; radius: 6
        border.color: "#d0d9e6"
        Text { anchors.centerIn: parent; text: (index+1)+"호 E/V"; color:"#1d2b4a"; font.bold: true }
      }
    }

    Rectangle {
      Layout.row: 0; Layout.column: (cars?cars.length:0)+1; height: headerH; color: "#e9eff7"; radius: 6
      border.color: "#d0d9e6"
      Text { anchors.centerIn: parent; text: "대기자 / 목표층"; color:"#1d2b4a"; font.bold: true }
    }

    // ─ 각 층
    Repeater {
      model: floors
      RowLayout {
        readonly property int rowFloor: floors - index   // 위가 높은 층
        Layout.row: index + 1
        Layout.columnSpan: grid.columns
        Layout.fillWidth: true; spacing: 6; height: rowH

        // 층수 셀
        Rectangle {
          Layout.preferredWidth: 84; Layout.fillHeight: true
          color: "#ffffff"; radius: 6; border.color: "#dfe6f0"
          Text { anchors.centerIn: parent; text: rowFloor + "층"; color:"#2c5bd9"; font.bold: true }
        }

        // 각 카 셀
        Repeater {
          model: cars ? cars.length : 0
          Rectangle {
            Layout.fillWidth: true; Layout.fillHeight: true
            color: "#ffffff"; radius: 10; border.color: "#dfe6f0"
            property var car: cars[index]
            Rectangle {
              anchors.fill: parent; radius: 10
              visible: car && car.floor === parent.parent.rowFloor
              color: car.full ? "#e74c3c" : (car.doorOpen ? "#2ecc71" : "#4aa3ff")
              Text { anchors.centerIn: parent; color: "white"; font.bold: true
                     text: (car ? (car.load + " / " + car.capacity) : "") }
            }
          }
        }

        // 대기자/목표층 셀 (오른쪽)
        Rectangle {
          Layout.preferredWidth: 220; Layout.fillHeight: true
          color: "#ffffff"; radius: 10; border.color: "#dfe6f0"
          Row { anchors.centerIn: parent; spacing: 8
            // 대기 숫자
            Text {
              color: "#6b7a90"
              text: `${(root.waitUp[rowFloor-1]||0)} ↑ / ${(root.waitDown[rowFloor-1]||0)} ↓`
            }
            // 목표층 라벨 (예: ↗ 15층 / ↘ 3층)
            Text {
              color: "#1d2b4a"; font.bold: true
              text: root.waitDestLabel[rowFloor-1] || ""
            }
          }
        }
      }
    }
  }
}
