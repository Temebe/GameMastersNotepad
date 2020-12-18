import QtQuick 2.12
import QtQuick.Window 2.12
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.12
import org.gmn.viewcontroller 1.0

Rectangle {
    id: welcomeView
    property alias campaignsListModel: campaignsListView.model

    signal campaignChosen(string name)
    signal createCampaignChosen()

    ColumnLayout {
        anchors.fill: parent
        spacing: 0
        
        Text {
            text: "Hello Game Master! Please select campaign from list below or create a new one."
            horizontalAlignment: Text.AlignHCenter
            wrapMode: Text.WordWrap
            Layout.fillWidth: true
            fontSizeMode: Text.Fit
            minimumPointSize: 10
            font.pointSize: 32
            Layout.alignment: Qt.AlignHCenter | Qt.AlignVCenter
            Layout.preferredHeight: parent.height * 0.1
            Layout.preferredWidth: parent.width
        }
        
        Rectangle {
            id: campaignsList
            border.width: 3
            Layout.fillWidth: true
            Layout.fillHeight: true
            color: "#a52b2b"
            
            ListView {
                id: campaignsListView
                anchors.fill: parent
                anchors.margins: parent.border.width
                clip: true
                
                delegate: ItemDelegate {
                    width: parent.width

                    onClicked: {
                        welcomeView.campaignChosen(model.display)
                    }

                    contentItem: Text {
                        text: model.display
                        fontSizeMode: Text.Fit
                        font.pointSize: 32
                        minimumPointSize: 10
                    }

                    background: Rectangle {
                        implicitHeight: 40
                        implicitWidth: 100
                        color: index % 2 == 0 ? "#a6905a" : "#a39676"
                    }

                }
            }
        }

        Button {
            id: createCampaignButton
            Layout.alignment: Qt.AlignHCenter | Qt.AlignVCenter
            Layout.preferredHeight: parent.height * 0.1
            Layout.preferredWidth: parent.width
            text: "Create new campaign"

            onClicked: {
                welcomeView.createCampaignChosen()
            }

            contentItem: Text {
                text: createCampaignButton.text
                horizontalAlignment: Text.AlignHCenter
                verticalAlignment: Text.AlignVCenter
                fontSizeMode: Text.Fit
                font.pointSize: 32
                minimumPointSize: 10
            }
        }
    }
    
    
}

/*##^##
Designer {
    D{i:0;autoSize:true;height:480;width:640}
}
##^##*/
