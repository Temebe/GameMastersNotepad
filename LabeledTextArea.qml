import QtQuick 2.12
import QtQuick.Window 2.12
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.12
import org.gmn.viewcontroller 1.0

Column {
    property alias labelText: label.text
    property alias text: textArea.text
    property alias viewWidth: scrollView.width

    Label {
        id: label
        font.pointSize: 16
        color: "#2e2e2e"
    }
    
    ScrollView {
        id: scrollView
        height: 300
        contentWidth: -1
        contentHeight: -1

        TextArea {
            id: textArea
            anchors.fill: parent
            wrapMode: Text.WordWrap

            background: Rectangle {
                color: textArea.enabled ? "white" : "#a0a0a0"
                implicitHeight: 40
                implicitWidth: 200
                border.width: 2
                border.color: textArea.activeFocus ? "#4b83f8" : "transparent"

                
            }
        }
    }
}

/*##^##
Designer {
    D{i:0;autoSize:true;height:480;width:640}
}
##^##*/
