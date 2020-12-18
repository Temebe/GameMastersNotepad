import QtQuick 2.12
import QtQuick.Window 2.12
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.12
import org.gmn.viewcontroller 1.0

Rectangle {
    id: objectSelectionBox
    implicitHeight: objectsRowLayout.implicitHeight + 12
    color: "gray"
    
    property var model: ["A", "B", "C", "D"]
    
    signal objectActivated(int index)
    signal addObjectClicked()
    signal removeObjectClicked()
    
    RowLayout {
        id: objectsRowLayout
        anchors.fill: parent
        
        ComboBox {
            id: objectsComboBox
            Layout.fillWidth: true
            model: characterTab.model
            onActivated: characterTab.objectActivated(index)
        }
        
        ColumnLayout {
            property var buttonsColumnWidth: removeObjectButton.implicitWidth > addObjectButton.implicitWidth ? removeObjectButton.implicitWidth : addObjectButton.implicitWidth
            
            Button {
                id: addObjectButton
                text: "Add character"
                Layout.alignment: Qt.AlignHCenter | Qt.AlignVCenter
                Layout.preferredWidth: parent.buttonsColumnWidth
                onClicked: characterTab.addObjectClicked()
            }
            
            Button {
                id: removeObjectButton
                text: "Remove Character"
                Layout.alignment: Qt.AlignHCenter | Qt.AlignVCenter
                enabled: objectsComboBox.count > 0
                Layout.preferredWidth: parent.buttonsColumnWidth
                onClicked: characterTab.removeObjectClicked()
            }
        }
    }
}
