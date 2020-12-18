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
    title: qsTr("Game Master's Notepad")

    ViewController {
        id: controller
    }

//    Rectangle {
//        color: "green"
//        anchors.fill: parent
//    }

    Page {
        id: viewsPage
        anchors.fill: parent

        header: TabBar {
            id: bar
            width: parent.width

            TabButton {
                text: qsTr("Characters")
            }

            TabButton {
                text: qsTr("Locations")
            }
        }

        StackLayout {
            anchors.fill: parent
            width: parent.width

            currentIndex: bar.currentIndex

            Rectangle {
                color: "#7b7373"
                id: characterTab

                ObjectSelectionBar {
                    id: characterSelectionBar
                    anchors.left: parent.left
                    anchors.right: parent.right
                    anchors.top: parent.top
                    model: controller.charactersModel
                    property var lastShownIndex: -1

                    onCurrentIndexChanged: {
                        saveView(lastShownIndex)
                        if (currentIndex == -1) {
                            clearView()
                        } else {
                            loadIndexToView(currentIndex)
                        }
                        lastShownIndex = currentIndex
                    }

                    onAddObjectClicked: {
                        controller.addCharacter()
                        currentIndex = objectsCount - 1
                    }

                    onRemoveObjectClicked: {
                        var indexToRemove = currentIndex
                        currentIndex = -1
                        controller.removeCharacter(indexToRemove)
                        currentIndex = objectsCount > 0 ? 0 : -1
                    }

                    function loadIndexToView(index) {
                        var getData = (propertyIndex) => {
                            return controller.charactersModel.data(controller.charactersModel.index(index, propertyIndex))
                        }

                        textFieldCharacterName.text = getData(0)
                        textFieldAge.text = getData(1)
                        textFieldRace.text = getData(2)
                        textFieldProfession.text = getData(3)
                        textAreaBackstory.text = getData(4)
                        textAreaNotes.text = getData(5)
                        textAreaDescription.text = getData(6)
                        var imagePath = controller.createImagePath(getData(7))
                        characterImage.source = imagePath
                    }

                    function clearView() {
                        textFieldCharacterName.text = ""
                        textFieldAge.text = ""
                        textFieldRace.text = ""
                        textFieldProfession.text = ""
                        textAreaBackstory.text = ""
                        textAreaNotes.text = ""
                        textAreaDescription.text = ""
                        characterImage.source = controller.createImagePath("")
                    }

                    function saveView(index) {
                        var setData = (propertyIndex, value) => {
                            return controller.charactersModel.setData(controller.charactersModel.index(index, propertyIndex), value)
                        }

                        setData(0, textFieldCharacterName.text)
                        setData(1, textFieldAge.text)
                        setData(2, textFieldRace.text)
                        setData(3, textFieldProfession.text)
                        setData(4, textAreaBackstory.text)
                        setData(5, textAreaNotes.text)
                        setData(6, textAreaDescription.text)
                        controller.saveCampaign()
                    }

                    function refresh() {
                        var temp = currentIndex
                        currentIndex = -1
                        currentIndex = temp
                    }
                }

                ScrollView {
                    id: charactersView
                    anchors.left: parent.left
                    anchors.right: parent.right
                    anchors.top: characterSelectionBar.bottom
                    anchors.bottom: parent.bottom
                    contentWidth: -1
                    anchors.topMargin: 0
                    clip: true

                    Column {
                        id: column
                        anchors.fill: parent
                        anchors.topMargin: 2
                        spacing: 10

                        TextField {
                            id: textFieldCharacterName
                            anchors.horizontalCenter: parent.horizontalCenter
                            placeholderText: "Character name"
                            enabled: characterSelectionBar.properIndexSelected
                        }

                        Rectangle {
                            width: characterImage.width * 1.1
                            height: characterImage.height * 1.10
                            radius: 10
                            anchors.horizontalCenter: parent.horizontalCenter
                            color: "#6c5454"

                            Image {
                                property var maxSize: charactersView.width > charactersView.height ? charactersView.height * 0.5 : charactersView.width * 0.5

                                id: characterImage
                                source: "qrc:/images/nopic.png"
                                anchors.centerIn: parent
                                width: maxSize
                                height: maxSize
                                fillMode: Image.PreserveAspectFit


                            }

                            MouseArea {
                                anchors.fill: parent
                                enabled: characterSelectionBar.properIndexSelected
                                onClicked: {
                                    imageChooseDialog.open()
                                }
                            }

                            Rectangle {
                                anchors.fill: characterImage
                                opacity: 0.9
                                color: "black"
                                visible: !characterSelectionBar.properIndexSelected
                            }
                        }

                        LabeledTextField {
                            id: textFieldAge
                            labelText: "Age"
                            anchors.horizontalCenter: parent.horizontalCenter
                            enabled: characterSelectionBar.properIndexSelected
                        }

                        LabeledTextField {
                            id: textFieldRace
                            labelText: "Race"
                            anchors.horizontalCenter: parent.horizontalCenter
                            enabled: characterSelectionBar.properIndexSelected
                        }

                        LabeledTextField {
                            id: textFieldProfession
                            labelText: "Profession"
                            anchors.horizontalCenter: parent.horizontalCenter
                            enabled: characterSelectionBar.properIndexSelected
                        }


                        LabeledTextArea {
                            id: textAreaBackstory
                            labelText: "Backstory"
                            anchors.horizontalCenter: parent.horizontalCenter
                            enabled: characterSelectionBar.properIndexSelected
                        }

                        LabeledTextArea {
                            id: textAreaDescription
                            labelText: "Description"
                            anchors.horizontalCenter: parent.horizontalCenter
                            enabled: characterSelectionBar.properIndexSelected
                        }

                        LabeledTextArea {
                            id: textAreaNotes
                            labelText: "Notes"
                            anchors.horizontalCenter: parent.horizontalCenter
                            enabled: characterSelectionBar.properIndexSelected
                        }
                    }

                        // TODO Why it does not work? Is this a good solution?
//                    Rectangle {
//                        anchors.fill: parent
//                        opacity: 0.9
//                        color: "black"
//                        visible: !characterSelectionBar.properIndexSelected

//                        //Just area for blocking input
//                        MouseArea {
//                            visible: parent.visible
//                            anchors.fill: parent
//                        }

//                        Text {
//                            anchors.fill: parent
//                            horizontalAlignment: Text.AlignHCenter
//                            verticalAlignment: Text.AlignVCenter
//                            text: "Select character from list at the top or create a new one."
//                            color: "white"
//                            wrapMode: Text.WordWrap
//                            fontSizeMode: Text.Fit
//                            minimumPointSize: 10
//                            font.pointSize: 50
//                        }
//                    }
                }
            }

            Rectangle {
                color: "#a5a5de"
                id: locationTab

                ObjectSelectionBar {
                    id: locationSelectionBar
                    anchors.left: parent.left
                    anchors.right: parent.right
                    anchors.top: parent.top
                    model: controller.locationsModel

                    onObjectActivated: {}
                }
            }

        }
    }
    WelcomeView {
        id: welcomeView
        anchors.fill: parent
        color: "#a52b2b"
        campaignsListModel: controller.getCampaignsList()
        visible: !controller.campaignLoaded

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

    ImageFileDialog {
        id: imageChooseDialog
        folder: "file:" + controller.imagesPath

        onAccepted: {
            controller.changeCharacterImage(characterSelectionBar.currentIndex, fileUrl)
            characterSelectionBar.refresh()
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
