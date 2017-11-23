import QtQuick 2.6
import QtQuick.Window 2.2
import QtQuick.Controls 2.2
import QtQuick.Controls.Styles 1.4


Window {
    id: window
    objectName: "window"
    visible: true
    width: 300
    height: 400
    title: qsTr("Hello World")

    ComboBox{
        id : ports_combobox
        anchors.top: parent.top
        anchors.topMargin: 10
        anchors.left: parent.left
        anchors.leftMargin: 10
        model: master.comboList

    }
    Button{
        x: 400
        text: "Start"
        anchors.top: parent.top
        anchors.topMargin: 10
        anchors.right: parent.right
        anchors.rightMargin: 10
        onClicked: {
            master.slotStartServer(ports_combobox.currentText)
        }
    }

    ListView {
        id: view
        y: 74
        height: 316
        anchors.bottom: parent.bottom
        anchors.bottomMargin: 10
        anchors.left: parent.left
        anchors.leftMargin: 10
        anchors.right: parent.right
        anchors.rightMargin: 10
        model:master.comboList
        delegate: Text {
                text:modelData
            }
    }

}
