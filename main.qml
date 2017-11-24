import QtQuick 2.6
import QtQuick.Window 2.2
import QtQuick.Controls 2.2
import QtQuick.Controls.Styles 1.4


Window {
    id: window
    objectName: "window"
    visible: true
    width: 400
    height: 500
    color: "#efe2ef"
    title: qsTr("TCPCONNECTION")


    ListView {
        id: msgView
        y: 92
        height: 398
        anchors.bottom: parent.bottom
        anchors.bottomMargin: 10
        anchors.left: parent.left
        anchors.leftMargin: 10
        anchors.right: parent.right
        anchors.rightMargin: 10
        model:master.messages
        spacing: 20

        delegate: Rectangle {
               width: parent.width; height: 30
               color: "#d4b9d4"
               Text{
                   anchors.centerIn: parent
                   text: modelData
               }
           }

    }


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
                            text: modelData
                            onClicked: {
                                master.slotStartServer(this.text)
                                fileButton.visible = false
                            }
                      }
            }
        }
    }

    RoundButton {
        id: roundButton
        x: 350
        y: 10
        text: "x"
        onClicked: master.destructor()
    }

}
