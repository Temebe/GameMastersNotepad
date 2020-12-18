import QtQuick 2.12
import QtQuick.Window 2.12
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.12
import org.gmn.viewcontroller 1.0

Rectangle {
    id: objectSelectionBar
    implicitHeight: objectsRowLayout.implicitHeight + 12
    color: "gray"
    
    property alias model: objectsComboBox.model
    property alias currentIndex: objectsComboBox.currentIndex
    property alias objectsCount: objectsComboBox.count
    property var properIndexSelected: objectSelectionBar.currentIndex >= 0 && objectSelectionBar.currentIndex < objectSelectionBar.objectsCount
    
    signal objectActivated(int index)
    signal addObjectClicked()
    signal removeObjectClicked()
    
    RowLayout {
        id: objectsRowLayout
        anchors.fill: parent
        
        ComboBox {
            id: objectsComboBox
            Layout.fillWidth: true
            onActivated: objectSelectionBar.objectActivated(index)
            textRole: "modelData"
        }
        
        ColumnLayout {
            property var buttonsColumnWidth: removeObjectButton.implicitWidth > addObjectButton.implicitWidth ? removeObjectButton.implicitWidth : addObjectButton.implicitWidth
            
            Button {
                id: addObjectButton
                text: "Add character"
                Layout.alignment: Qt.AlignHCenter | Qt.AlignVCenter
                Layout.preferredWidth: parent.buttonsColumnWidth
                onClicked: {
                    objectSelectionBar.addObjectClicked()
                    objectsComboBox.currentIndex = objectsComboBox.count - 1
                }
            }
            
            Button {
                id: removeObjectButton
                text: "Remove Character"
                Layout.alignment: Qt.AlignHCenter | Qt.AlignVCenter
                enabled: objectsComboBox.currentIndex != -1
                Layout.preferredWidth: parent.buttonsColumnWidth
                onClicked: {
                    objectSelectionBar.removeObjectClicked()
                }
            }
        }
    }
}
