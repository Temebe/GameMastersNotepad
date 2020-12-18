import QtQuick 2.12
import QtQuick.Window 2.12
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.12
import org.gmn.viewcontroller 1.0

Window {
    id: mainWindow
    width: 480
    height: 854
    visible: true
    title: qsTr("Hello World")

    ViewController {
        id: controller
    }

//    Rectangle {
//        color: "green"
//        anchors.fill: parent
//    }

    Page {
        anchors.fill: parent

        header: TabBar {
            id: bar
            width: parent.width

            TabButton {
                text: qsTr("Characters")
            }

            TabButton {
                text: qsTr("ABC")
            }
        }

        StackLayout {
            anchors.fill: parent
            currentIndex: bar.currentIndex

            Rectangle {
                color: "#de8b8b"
                id: characterTab

                ObjectSelectionBar {
                    id: objectSelectionBox
                    anchors.left: parent.left
                    anchors.right: parent.right
                    anchors.top: parent.top
                }
            }

            Rectangle {
                color: "#a5a5de"
                id: locationTab
            }

        }
    }
    WelcomeView {
        id: welcomeView
        anchors.fill: parent
        color: "#a52b2b"
        campaignsListModel: controller.getCampaignsList()
        //visible: !controller.campaignLoaded
        visible: false

        onCampaignChosen: {
            controller.loadCampaign(name)
        }

        onCreateCampaignChosen: {
            createCampaignDialog.open()

        }
    }




    Dialog {
        id: createCampaignDialog
        anchors.centerIn: parent
        title: "Input name of campaign"
        standardButtons: Dialog.Ok | Dialog.Cancel
        contentChildren: TextField {
            id: campaignNameTextField
            placeholderText: "i.e. \"Lord of the Kings\""
        }

        onAccepted: {
            var campaignName = campaignNameTextField.text
            campaignNameTextField.text = ""
            controller.loadCampaign(campaignName)
        }
    }

    Dialog {
        id: errorDialog
        anchors.centerIn: parent
        title: "Error"
        property var errorText: ""
        standardButtons: Dialog.Ok
        modal: true

        contentChildren: Text {
            text: errorDialog.errorText
        }
    }

    function showError(title, message) {
        errorDialog.title = title
        errorDialog.errorText = message
        errorDialog.open()
    }

    Component.onCompleted: {
        controller.errorOccured.connect(showError)
    }
}
