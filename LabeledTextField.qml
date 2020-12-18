import QtQuick 2.12
import QtQuick.Window 2.12
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.12
import org.gmn.viewcontroller 1.0

Column {
    property alias labelText: label.text
    property alias text: textField.text
    
    Label {
        id: label
        font.pointSize: 16
    }
    
    TextField {
        id: textField
    }
}
