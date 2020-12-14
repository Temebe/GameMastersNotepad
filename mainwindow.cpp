#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "gmnpaths.h"
#include "gmnobjectmodel.tpp"

#include <QInputDialog>
#include <QMessageBox>
#include <QSortFilterProxyModel>
#include <QAction>
#include <QPixmap>
#include <QFileDialog>
#include <QCloseEvent>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent, Qt::Window)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::init()
{
    disableCharacterView();

    // CAMPAIGN CONNECTS
    connect(&campaign, &Campaign::succesfullyLoaded, this, &MainWindow::configureViews);
    connect(&campaign, &Campaign::loadingFailed, this, &MainWindow::loadingErrorOccured);
    connect(&campaign, &Campaign::savingFailed, this, &MainWindow::showSavingError);

    // ACTION CONNECTS
    connect(ui->actionSave, &QAction::triggered, this, &MainWindow::saveCampaign);
    connect(ui->actionAboutApp, &QAction::triggered, this, &MainWindow::showAboutApp);
    connect(ui->actionAboutQt, &QAction::triggered, this, &MainWindow::showAboutQt);

    connect(ui->lineEditAge, &QLineEdit::textEdited, this, &MainWindow::setCharacterChanged);
    connect(ui->lineEditName, &QLineEdit::textEdited, this, &MainWindow::setCharacterChanged);
    connect(ui->lineEditRace, &QLineEdit::textEdited, this, &MainWindow::setCharacterChanged);
    connect(ui->lineEditProfession, &QLineEdit::textEdited, this, &MainWindow::setCharacterChanged);
    connect(ui->textEditBackstory, &QTextEdit::textChanged, this, &MainWindow::setCharacterChanged);
    connect(ui->textEditDescription, &QTextEdit::textChanged, this, &MainWindow::setCharacterChanged);
    connect(ui->textEditNotes, &QTextEdit::textChanged, this, &MainWindow::setCharacterChanged);

    connect(ui->lineEditLocationName, &QLineEdit::textEdited, this, &MainWindow::setLocationChanged);
    connect(ui->textEditLocationNotes, &QTextEdit::textChanged, this, &MainWindow::setLocationChanged);

    connect(ui->labelImage, &ClickableImageLabel::clicked, this, &MainWindow::chooseCharacterImage);
}

void MainWindow::createNewCampaign(const QString& name)
{
    campaign.createNewCampaign(name);
}

void MainWindow::loadCampaign(const QString& name)
{
    campaign.loadFromName(name);
}

void MainWindow::configureViews()
{
    charactersModel = campaign.createCharactersModel();
    locationsModel = campaign.createLocationsModel();

    configureCharactersListView();
    configureLocationsListView();

    if (!charactersModel->isEmpty()) {
        enableCharacterView();
    }
    if (!locationsModel->isEmpty()) {
        enableLocationView();
    }

    locationChanged = false;
    characterChanged = false;
    campaignChanged = false;

    show();
    emit windowReady();
}

void MainWindow::showSavingError(const QString &reason, const QString& details)
{
    Q_UNUSED(details);
    QMessageBox::critical(this, tr("Saving error"), reason);
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    if (campaignChanged) {
        auto clickedButton = QMessageBox::question(
                    this,
                    tr("Unsaved changes"),
                    tr("You have unsaved changes, do you want to save them?"),
                    QMessageBox::Yes | QMessageBox::No | QMessageBox::Cancel
        );

        if (clickedButton == QMessageBox::Cancel) {
            event->ignore();
            return;

        } else if (clickedButton == QMessageBox::Yes) {
            saveCampaign();
        }
    }

    QMainWindow::closeEvent(event);
}

void MainWindow::onCharacterSelectionChanged(const QModelIndex &index, const QModelIndex &previousIndex)
{
    auto mappedIndex = charactersFilterModel.mapToSource(index);
    auto mappedPreviousIndex = charactersFilterModel.mapToSource(previousIndex);

    saveCurrentViewToCharacter(mappedPreviousIndex);
    auto characterOpt = charactersModel->getObject(mappedIndex);

    if (!characterOpt) {
        clearCharacterView();
        disableCharacterView();
        ui->buttonRemoveCharacter->setDisabled(true);
        return;
    }

    ui->buttonRemoveCharacter->setDisabled(false);
    showCharacter(characterOpt.value());
}

void MainWindow::onLocationSelectionChanged(const QModelIndex &index, const QModelIndex &previousIndex)
{
    auto mappedIndex = locationsFilterModel.mapToSource(index);
    auto mappedPreviousIndex = locationsFilterModel.mapToSource(previousIndex);

    saveCurrentViewToLocation(mappedPreviousIndex);
    auto locationOpt = locationsModel->getObject(mappedIndex);

    if (!locationOpt) {
        clearLocationView();
        disableLocationView();
        ui->buttonRemoveLocation->setDisabled(true);
        return;
    }

    ui->buttonRemoveLocation->setDisabled(false);
    showLocation(locationOpt.value());
}

void MainWindow::onCharactersSearchTextChanged(const QString& text)
{
    auto selectionModel = ui->charactersListView->selectionModel();
    auto currentIndex = charactersFilterModel.mapToSource(selectionModel->currentIndex());
    ui->charactersListView->selectionModel()->clearCurrentIndex();
    charactersFilterModel.setFilterRegExp(QRegExp(text, Qt::CaseSensitive, QRegExp::FixedString));
    selectionModel->setCurrentIndex(charactersFilterModel.mapFromSource(currentIndex), QItemSelectionModel::ClearAndSelect);
}

void MainWindow::onLocationsSearchTextChanged(const QString &text)
{
    auto selectionModel = ui->locationsListView->selectionModel();
    auto currentIndex = locationsFilterModel.mapToSource(selectionModel->currentIndex());
    ui->locationsListView->selectionModel()->clearCurrentIndex();
    locationsFilterModel.setFilterRegExp(QRegExp(text, Qt::CaseSensitive, QRegExp::FixedString));
    selectionModel->setCurrentIndex(locationsFilterModel.mapFromSource(currentIndex), QItemSelectionModel::ClearAndSelect);
}

void MainWindow::removeCharacter()
{
    auto currentIndex = charactersFilterModel.mapToSource(ui->charactersListView->selectionModel()->currentIndex());
    auto characterName = charactersModel->getObject(currentIndex).value_or(Character()).getName();

    auto clickedButton = QMessageBox::question(
                this,
                tr("Removing character"),
                tr("You are about to remove character %1, you want to proceed?").arg(characterName)
    );

    if (clickedButton == QMessageBox::No) {
        return;
    }

    charactersModel->removeRow(currentIndex.row());
    selectFirstCharacter();
}

void MainWindow::removeLocation()
{
    auto currentIndex = locationsFilterModel.mapToSource(ui->locationsListView->selectionModel()->currentIndex());
    auto locationName = locationsModel->getObject(currentIndex).value_or(Location()).getName();

    auto clickedButton = QMessageBox::question(
                this,
                tr("Removing location"),
                tr("You are about to remove location %1, you want to proceed?").arg(locationName)
    );

    if (clickedButton == QMessageBox::No) {
        return;
    }

    locationsModel->removeRow(currentIndex.row());
    selectFirstLocation();
}

void MainWindow::addCharacter()
{
    QString name = QInputDialog::getText(this, tr("Add new character"), tr("Name for new character:"));
    if (name.isEmpty()) {
        return;
    }
    auto index = charactersModel->addObject(name);
    auto mappedIndex = charactersFilterModel.mapFromSource(index);
    ui->charactersListView->selectionModel()->setCurrentIndex(mappedIndex, QItemSelectionModel::ClearAndSelect);
}

void MainWindow::addLocation()
{
    QString name = QInputDialog::getText(this, tr("Add new location"), tr("Name of new location:"));
    if (name.isEmpty()) {
        return;
    }
    auto index = locationsModel->addObject(name);
    auto mappedIndex = locationsFilterModel.mapFromSource(index);
    ui->locationsListView->selectionModel()->setCurrentIndex(mappedIndex, QItemSelectionModel::ClearAndSelect);

}

void MainWindow::saveCampaign()
{
    auto currentCharacterIndex = ui->charactersListView->selectionModel()->currentIndex();
    auto mappedCharacterIndex = charactersFilterModel.mapToSource(currentCharacterIndex);
    auto currentLocationIndex = ui->locationsListView->selectionModel()->currentIndex();
    auto mappedLocationIndex = locationsFilterModel.mapToSource(currentLocationIndex);
    saveCurrentViewToCharacter(mappedCharacterIndex);
    saveCurrentViewToLocation(mappedLocationIndex);
    campaign.saveToFile(charactersModel, locationsModel);
    campaignChanged = false;
}

void MainWindow::chooseCharacterImage()
{
    auto currentIndex = ui->charactersListView->selectionModel()->currentIndex();
    auto mappedIndex = charactersFilterModel.mapToSource(currentIndex);
    if (!mappedIndex.isValid()) {
        return;
    }

    QString imagePath = QFileDialog::getOpenFileName(
                this,
                tr("Select character image"),
                campaign.getPath(),
                tr("Images (*.png *.xpm *.jpg *.jpeg *.bmp *.gif *.pgb *.pgm *.ppm *.xbm *.xpm);;All (*)"),
                nullptr,
                QFileDialog::DontConfirmOverwrite
    );

    if(imagePath.isEmpty()) {
        return;
    }

    auto pixmap = QPixmap(imagePath);
    if (pixmap.isNull()) {
        QMessageBox::critical(this, tr("Image error"), tr("Unable to read the image!"));
        return;
    }

    QDir campaignDir(campaign.getPath());
    auto relativeImagePath = campaignDir.relativeFilePath(imagePath);
    ui->labelImage->setPixmap(pixmap);
    characterChanged = true;
    charactersModel->setData(
                mappedIndex.siblingAtColumn(static_cast<int>(CharacterProperty::IMAGEPATH)), relativeImagePath);
}

void MainWindow::setCharacterChanged()
{
    characterChanged = true;
    campaignChanged = true;
}

void MainWindow::setLocationChanged()
{
    locationChanged = true;
    campaignChanged = true;
}

void MainWindow::showAboutApp()
{
    QMessageBox::about(this, tr("About Game Master's Notepad"),
                       tr("Current version: %1").arg(QCoreApplication::applicationVersion()));
}

void MainWindow::showAboutQt()
{
    QMessageBox::aboutQt(this);
}

void MainWindow::configureCharactersListView()
{
    if (!charactersModel) {
        return;
    }
    charactersFilterModel.setSourceModel(charactersModel.get());
    charactersFilterModel.setFilterKeyColumn(0);
    ui->charactersListView->setModel(&charactersFilterModel);
    connect(ui->charactersListView->selectionModel(), &QItemSelectionModel::currentChanged, this, &MainWindow::onCharacterSelectionChanged);
    selectFirstCharacter();
}

void MainWindow::configureLocationsListView()
{
    if (!locationsModel) {
        return;
    }
    locationsFilterModel.setSourceModel(locationsModel.get());
    locationsFilterModel.setFilterKeyColumn(0);
    ui->locationsListView->setModel(&locationsFilterModel);
    connect(ui->locationsListView->selectionModel(), &QItemSelectionModel::currentChanged, this, &MainWindow::onLocationSelectionChanged);
    selectFirstLocation();
}

void MainWindow::selectFirstCharacter()
{
    if (charactersFilterModel.rowCount() > 0) {
        auto firstIndex = charactersFilterModel.index(0, 0);
        ui->charactersListView->selectionModel()->setCurrentIndex(firstIndex, QItemSelectionModel::ClearAndSelect);
    } else {
        clearCharacterView();
        disableCharacterView();
    }
}

void MainWindow::selectFirstLocation()
{
    if (locationsFilterModel.rowCount() > 0) {
        auto firstIndex = locationsFilterModel.index(0, 0);
        ui->locationsListView->selectionModel()->setCurrentIndex(firstIndex, QItemSelectionModel::ClearAndSelect);
    } else {
        clearLocationView();
        disableLocationView();
    }
}

void MainWindow::showCharacter(const Character &character)
{
    ui->textEditBackstory->blockSignals(true);
    ui->textEditDescription->blockSignals(true);
    ui->textEditNotes->blockSignals(true);

    enableCharacterView();
    ui->lineEditAge->setText(character.getAge());
    ui->lineEditName->setText(character.getName());
    ui->lineEditProfession->setText(character.getProfession());
    ui->lineEditRace->setText(character.getRace());
    ui->textEditBackstory->setText(character.getBackstory());
    ui->textEditDescription->setText(character.getDescription());
    ui->textEditNotes->setText(character.getNotes());

    ui->textEditBackstory->blockSignals(false);
    ui->textEditDescription->blockSignals(false);
    ui->textEditNotes->blockSignals(false);

    QFileInfo imageFileInfo(campaign.getDir(), character.getImagePath());
    auto pixmap = QPixmap(imageFileInfo.canonicalFilePath());
    if (pixmap.isNull()) {
        pixmap = QPixmap(GMN::defaultCharacterImagePath);
    }
    ui->labelImage->setPixmap(pixmap);
}

void MainWindow::showLocation(const Location &location)
{
    ui->textEditLocationNotes->blockSignals(true);

    enableLocationView();
    ui->lineEditLocationName->setText(location.getName());
    ui->textEditLocationNotes->setText(location.getNotes());

    ui->textEditLocationNotes->blockSignals(false);
}

void MainWindow::clearCharacterView()
{
    disableCharacterView();
    ui->lineEditAge->clear();
    ui->lineEditName->clear();
    ui->lineEditProfession->clear();
    ui->lineEditRace->clear();
    ui->textEditBackstory->clear();
    ui->textEditDescription->clear();
    ui->textEditNotes->clear();
    ui->labelImage->setPixmap(QPixmap(GMN::defaultCharacterImagePath));
}

void MainWindow::clearLocationView()
{
    disableLocationView();
    ui->lineEditLocationName->clear();
    ui->textEditLocationNotes->clear();
}

void MainWindow::disableCharacterView()
{
    setDisabledCharacterView(true);
}

void MainWindow::disableLocationView()
{
    setDisabledLocationView(true);
}

void MainWindow::enableCharacterView()
{
    setDisabledCharacterView(false);
}

void MainWindow::enableLocationView()
{
    setDisabledLocationView(false);
}

void MainWindow::setDisabledCharacterView(const bool disabled)
{
    ui->lineEditAge->setDisabled(disabled);
    ui->lineEditName->setDisabled(disabled);
    ui->lineEditProfession->setDisabled(disabled);
    ui->lineEditRace->setDisabled(disabled);
    ui->textEditBackstory->setDisabled(disabled);
    ui->textEditDescription->setDisabled(disabled);
    ui->textEditNotes->setDisabled(disabled);
}

void MainWindow::setDisabledLocationView(const bool disabled)
{
    ui->lineEditLocationName->setDisabled(disabled);
    ui->textEditLocationNotes->setDisabled(disabled);
}

void MainWindow::saveCurrentViewToCharacter(const QModelIndex &index)
{
    if (!index.isValid() || !characterChanged || savingView) {
        return;
    }
    savingView = true;

    auto setData = [this, &index](const CharacterProperty property, const QString& value){
        this->charactersModel->setData(index.siblingAtColumn(static_cast<int>(property)), value);
    };

    setData(CharacterProperty::AGE, ui->lineEditAge->text());
    setData(CharacterProperty::RACE, ui->lineEditRace->text());
    setData(CharacterProperty::PROFESSION, ui->lineEditProfession->text());
    setData(CharacterProperty::DESCRIPTION, ui->textEditDescription->toPlainText());
    setData(CharacterProperty::NOTES, ui->textEditNotes->toPlainText());
    setData(CharacterProperty::BACKSTORY, ui->textEditBackstory->toPlainText());
    setData(CharacterProperty::NAME, ui->lineEditName->text());

    characterChanged = false;
    savingView = false;
}

void MainWindow::saveCurrentViewToLocation(const QModelIndex &index)
{
    if (!index.isValid() || !locationChanged || savingView) {
        return;
    }
    savingView = true;

    auto setData = [this, &index](const LocationProperty property, const QString& value){
        this->locationsModel->setData(index.siblingAtColumn(static_cast<int>(property)), value);
    };

    setData(LocationProperty::NAME, ui->lineEditLocationName->text());
    setData(LocationProperty::NOTES, ui->textEditLocationNotes->toPlainText());

    locationChanged = false;
    savingView = false;
}
