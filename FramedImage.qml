import QtQuick 2.12
import QtQuick.Window 2.12
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.12
import org.gmn.viewcontroller 1.0

Rectangle {
    id: frame
    radius: 10
    color: "#6c5454"
    width: image.width * 1.1
    height: image.height * 1.1
    property var maxSize: 0
    property alias source: image.source
    signal clicked()
    
    Image {
        id: image
        source: "qrc:/images/nopic.png"
        anchors.centerIn: parent
        width: maxSize
        height: maxSize
        fillMode: Image.PreserveAspectFit       
    }
    
    MouseArea {
        anchors.fill: parent
        enabled: frame.enabled
        onClicked: frame.clicked()
    }
    
    Rectangle {
        anchors.fill: image
        opacity: 0.9
        color: "black"
        visible: !frame.enabled
    }
}
