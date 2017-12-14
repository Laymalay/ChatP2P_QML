import QtQuick 2.6
import QtQuick.Window 2.2
import QtQuick.Controls 1.4
import QtQuick.Controls 2.2
import QtQuick.Controls.Styles 1.4
import QtQuick.Layouts 1.3
import QtGraphicalEffects 1.0
import QtQuick.Controls.Styles 1.4



Window {
    id: window
    objectName: "window"
    visible: true
    width: 700
    height: 600
    color: '#e2e8e4'
    title: qsTr("TCPCONNECTION")
    flags : Qt.WindowStaysOnTopHint;


    SplitView {
        id: splitView
        anchors.fill: parent
        orientation: Qt.Horizontal

        Rectangle{
            id: rectangle
            color:'#6eb5c0'
            width: 250
            Button {
                id: fileButton
                background: Rectangle {radius: 10}
                text: "Choose port"
                spacing: 0
                anchors.left: parent.left
                anchors.leftMargin: 10
                anchors.top: parent.top
                anchors.topMargin: 10
                onClicked: menu.open()
                Menu {
                    id: menu
                    y: fileButton.height
                    Repeater {
                              model: master.comboList
                              MenuItem {
                                  id: menuItem
                                    text: modelData
                                    onClicked: {
                                        master.slotStartServer(this.text)
                                        fileButton.enabled = false
                                        logoutbtn.visible = true}
                              }
                    }
                }
            }
            Button {
                id:logoutbtn
                background: Rectangle {radius: 10}
                text: "logout"
                anchors.left: fileButton.right
                anchors.right: portsOnline.right
                anchors.leftMargin: 34
                anchors.bottom: portsOnline.top
                anchors.bottomMargin: 10
                anchors.top: parent.top
                anchors.topMargin: 10
                visible: false
                onClicked: {
                    master.slotLogout()
                    fileButton.enabled = true
                    this.visible = false}

            }


            Component {
                  id: userDelegate
                  Item {
                      id: wrapper
                      width: portsOnline.width; height: 60
                      Rectangle{
                          id:rect
                          width: 50
                          height: 50
                          radius: 50
                          border{color: modelData.isOnline?"#006c84":"#ffccbb";width:5}
                          Image {
                              id: img
                              width:40
                              height: 40
                              source: "qrc:/userpic2.png"
                              anchors.centerIn: rect}
                          }
                      Text{
                          anchors.centerIn: parent
                          text: modelData.address}
                      states: State {
                          name: "Current"
                          when: wrapper.ListView.isCurrentItem
                          PropertyChanges { target: wrapper; x: 20 }
                      }
                      transitions: Transition {
                          NumberAnimation { properties: "x"; duration: 200 }
                      }
                      MouseArea {
                          anchors.fill: parent
                          onClicked: {
                              wrapper.ListView.view.currentIndex = index
                          }
                      }
                  }
              }

            Component {
                   id: highlightBar
                   Rectangle {
                       width: portsOnline.width; height: 60
                       color: "#e2e8e4"
                       radius: 10
                       y: portsOnline.currentItem.y - 5;
                       Behavior on y { SpringAnimation { spring: 0.5; damping: 0.1 } }
                   }
               }

            ListView{
                id: portsOnline
                width: 220
                anchors.top: fileButton.bottom
                anchors.topMargin: 10
                anchors.bottom: parent.bottom
                anchors.bottomMargin: 10
                anchors.left: parent.left
                anchors.leftMargin: 10
                spacing: 20
                model:master.users


                delegate: userDelegate
                focus: true
                highlight: highlightBar
                highlightFollowsCurrentItem: false
            }
        }


        ListView {
            id: msgView
            anchors.left: rectangle.right
            anchors.right: parent.right
            anchors.bottom: parent.bottom
            anchors.top: parent.top
            anchors.rightMargin: 10
            anchors.leftMargin: 10
            anchors.bottomMargin: 10
            anchors.topMargin: 10
            layoutDirection: Qt.LeftToRight
            boundsBehavior: Flickable.DragOverBounds
            model:master.messages
            spacing: 20
            delegate: Rectangle {
                   width: parent.width;
                   height: 30
                   border{color:"#006c84"}
                   color: "white"
                   radius: 10
                   Text{
                       anchors.centerIn: parent
                       text: modelData}
               }

        }

    }
}
