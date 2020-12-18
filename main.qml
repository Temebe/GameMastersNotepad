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
                            onTextEdited: {
                                var index = controller.charactersModel.index(characterSelectionBar.currentIndex, 0)
                                controller.charactersModel.setData(index, text)
                            }
                        }

                        FramedImage {
                            id: characterImage
                            anchors.horizontalCenter: parent.horizontalCenter
                            maxSize: charactersView.width > charactersView.height ? charactersView.height * 0.5 : charactersView.width * 0.5
                            enabled: characterSelectionBar.properIndexSelected

                            onClicked: {
                                imageChooseDialog.open()
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
                            viewWidth: charactersView.width * 0.7
                            anchors.horizontalCenter: parent.horizontalCenter
                            enabled: characterSelectionBar.properIndexSelected
                        }

                        LabeledTextArea {
                            id: textAreaDescription
                            labelText: "Description"
                            viewWidth: charactersView.width * 0.7
                            anchors.horizontalCenter: parent.horizontalCenter
                            enabled: characterSelectionBar.properIndexSelected
                        }

                        LabeledTextArea {
                            id: textAreaNotes
                            labelText: "Notes"
                            viewWidth: charactersView.width * 0.7
                            anchors.horizontalCenter: parent.horizontalCenter
                            enabled: characterSelectionBar.properIndexSelected
                        }
                    }
                }
            }

            Rectangle {
                color: "#7b7373"
                id: locationTab

                ObjectSelectionBar {
                    id: locationsSelectionBar
                    anchors.left: parent.left
                    anchors.right: parent.right
                    anchors.top: parent.top
                    model: controller.locationsModel
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
                        controller.addLocation()
                        currentIndex = objectsCount - 1
                    }

                    onRemoveObjectClicked: {
                        var indexToRemove = currentIndex
                        currentIndex = -1
                        controller.removeLocation(indexToRemove)
                        currentIndex = objectsCount > 0 ? 0 : -1
                    }

                    function loadIndexToView(index) {
                        var getData = (propertyIndex) => {
                            return controller.locationsModel.data(controller.locationsModel.index(index, propertyIndex))
                        }

                        textFieldLocationName.text = getData(0)
                        textAreaLocationNotes.text = getData(1)
                    }

                    function clearView() {
                        textFieldLocationName.text = ""
                        textAreaLocationNotes.text = ""
                    }

                    function saveView(index) {
                        var setData = (propertyIndex, value) => {
                            return controller.locationsModel.setData(controller.locationsModel.index(index, propertyIndex), value)
                        }

                        setData(0, textFieldLocationName.text)
                        setData(1, textAreaLocationNotes.text)
                        controller.saveCampaign()
                    }

                    function refresh() {
                        var temp = currentIndex
                        currentIndex = -1
                        currentIndex = temp
                    }
                }

                ScrollView {
                    id: locationsView
                    anchors.left: parent.left
                    anchors.right: parent.right
                    anchors.top: locationsSelectionBar.bottom
                    anchors.bottom: parent.bottom
                    contentWidth: -1
                    anchors.topMargin: 0
                    clip: true

                    Column {
                        anchors.fill: parent
                        anchors.topMargin: 2
                        spacing: 10

                        TextField {
                            id: textFieldLocationName
                            anchors.horizontalCenter: parent.horizontalCenter
                            placeholderText: "Location name"
                            enabled: locationsSelectionBar.properIndexSelected
                            onTextEdited: {
                                var index = controller.locationsModel.index(locationsSelectionBar.currentIndex, 0)
                                controller.locationsModel.setData(index, text)
                            }
                        }

                        LabeledTextArea {
                            id: textAreaLocationNotes
                            labelText: "Notes"
                            viewWidth: charactersView.width * 0.7
                            anchors.horizontalCenter: parent.horizontalCenter
                            enabled: locationsSelectionBar.properIndexSelected
                        }
                    }
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
        contentChildren: TextField {
            id: campaignNameTextField
            placeholderText: "i.e. \"Lord of the Kings\""
        }


        onAccepted: {
            var campaignName = campaignNameTextField.text
            campaignNameTextField.text = ""
            controller.createCampaign(campaignName)
        }

        footer: DialogButtonBox {
            Button {
                id: okButton
                enabled: campaignNameTextField.text != ""
                text: qsTr("Ok")
                DialogButtonBox.buttonRole: DialogButtonBox.AcceptRole
            }
            Button {
                text: qsTr("Cancel")
                DialogButtonBox.buttonRole: DialogButtonBox.DestructiveRole
            }
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

    Shortcut {
        sequence: StandardKey.Save
        onActivated: save()
        context: Qt.ApplicationShortcut
    }

    // Autosave
    Timer {
        interval: 10000
        repeat: true
        running: controller.campaignLoaded
        onTriggered: save()
    }

    function showError(title, message) {
        errorDialog.title = title
        errorDialog.errorText = message
        errorDialog.open()
    }

    function save() {
        characterSelectionBar.saveView(characterSelectionBar.currentIndex)
        locationsSelectionBar.saveView(locationsSelectionBar.currentIndex)
        controller.saveCampaign()
    }

    Component.onCompleted: {
        controller.errorOccured.connect(showError)
    }
}
