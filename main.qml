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
                text: "Choose port"


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
                                        fileButton.visible = false
                                    }
                              }
                    }
                }
            }


                ListView{
                    id: portsOnline
                    anchors.top: fileButton.bottom
                    anchors.topMargin: 10
                    anchors.right: parent.right
                    anchors.rightMargin: 10
                    anchors.bottom: parent.bottom
                    anchors.bottomMargin: 10
                    anchors.left: parent.left
                    anchors.leftMargin: 10
                    spacing: 20
                    model:master.users
                    boundsBehavior: Flickable.DragOverBounds
                    delegate: Item {
                        width: parent.width; height: 60
                        Rectangle{
                            id:rect
                            width: 50
                            height: 50
                            Image {
                                    id: img
                                    width:40
                                    height: 40
                                    source: "qrc:/userpic2.png"
                                    anchors.centerIn: rect
                               }
                            border {color: modelData.isOnline?"#006c84":"#ffccbb"; width: 5}
                            radius: 50
                        }
                       Text{
                           anchors.centerIn: parent
                           text: modelData.address
                       }
                    }

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
                       text: modelData
                   }
               }

        }
    }
}
