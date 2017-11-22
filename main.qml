import QtQuick 2.6
import QtQuick.Window 2.2
import QtQuick.Controls 2.2
import QtQuick.Controls.Styles 1.4
import uibackend 1.0

Window {
    visible: true
    width: 640
    height: 480
    title: qsTr("Hello World")

    ComboBox{
        id : ports_combobox

        model: ComboBoxModel


    }
}
