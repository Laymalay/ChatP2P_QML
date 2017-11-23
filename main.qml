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
    title: qsTr("Hello World")

    ComboBox{
        id : ports_combobox
        width: 131
        height: 49
        opacity: 1
        anchors.top: parent.top
        anchors.topMargin: 10
        anchors.left: parent.left
        anchors.leftMargin: 10
        model: master.comboList

    }
    Button{
        id: button
        x: 276
        width: 114
        height: 49
        text: "Start"
        font.family: "Tahoma"
        font.letterSpacing: 7
        font.wordSpacing: 0
        font.weight: Font.Light
        font.capitalization: Font.AllUppercase
        font.bold: true
        transformOrigin: Item.Center
        opacity: 1
        anchors.top: parent.top
        anchors.topMargin: 10
        anchors.right: parent.right
        anchors.rightMargin: 10
        onClicked: {
            master.slotStartServer(ports_combobox.currentText)
        }
    }

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

        Connections {
               target: master
               onMessagesChanged: console.log("Image failed to load:", messages)
           }
    }

}
