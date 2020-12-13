#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "gmnpaths.h"

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
    configureCharactersListView();
    if (!charactersModel->isEmpty()) {
        enableCharacterView();
    }
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
    auto characterOpt = charactersModel->getCharacter(mappedIndex);

    if (!characterOpt) {
        clearCharacterView();
        disableCharacterView();
        ui->buttonRemove->setDisabled(true);
        return;
    }

    ui->buttonRemove->setDisabled(false);
    showCharacter(characterOpt.value());
}

void MainWindow::onCharactersSearchTextChanged(const QString& text)
{
    auto selectionModel = ui->charactersListView->selectionModel();
    auto currentIndex = charactersFilterModel.mapToSource(selectionModel->currentIndex());
    ui->charactersListView->selectionModel()->clearCurrentIndex();
    charactersFilterModel.setFilterRegExp(QRegExp(text, Qt::CaseSensitive, QRegExp::FixedString));
    selectionModel->setCurrentIndex(charactersFilterModel.mapFromSource(currentIndex), QItemSelectionModel::ClearAndSelect);
}

void MainWindow::removeCharacter()
{
    auto currentIndex = charactersFilterModel.mapToSource(ui->charactersListView->selectionModel()->currentIndex());
    auto characterName = charactersModel->getCharacter(currentIndex).value_or(Character()).getName();

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

void MainWindow::addCharacter()
{
    QString name = QInputDialog::getText(this, tr("Add new character"), tr("Name for new character:"));
    if (name.isEmpty()) {
        return;
    }
    auto index = charactersModel->addCharacter(name);
    auto mappedIndex = charactersFilterModel.mapFromSource(index);
    ui->charactersListView->selectionModel()->setCurrentIndex(mappedIndex, QItemSelectionModel::ClearAndSelect);
}

void MainWindow::saveCampaign()
{
    auto currentIndex = ui->charactersListView->selectionModel()->currentIndex();
    auto mappedIndex = charactersFilterModel.mapToSource(currentIndex);
    saveCurrentViewToCharacter(mappedIndex);
    campaign.saveToFile(charactersModel);
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
    charactersModel->setData(mappedIndex.row(), CharacterElement::IMAGEPATH, relativeImagePath);
}

void MainWindow::setCharacterChanged()
{
    characterChanged = true;
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

void MainWindow::disableCharacterView()
{
    setDisabledCharacterView(true);
}

void MainWindow::enableCharacterView()
{
    setDisabledCharacterView(false);
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

void MainWindow::saveCurrentViewToCharacter(const QModelIndex &index)
{
    if (!index.isValid() || !characterChanged || savingView) {
        return;
    }
    savingView = true;

    auto currentRow = index.row();
    charactersModel->setData(currentRow, CharacterElement::AGE, ui->lineEditAge->text());
    charactersModel->setData(currentRow, CharacterElement::RACE, ui->lineEditRace->text());
    charactersModel->setData(currentRow, CharacterElement::PROFESSION, ui->lineEditProfession->text());
    charactersModel->setData(currentRow, CharacterElement::DESCRIPTION, ui->textEditDescription->toPlainText());
    charactersModel->setData(currentRow, CharacterElement::NOTES, ui->textEditNotes->toPlainText());
    charactersModel->setData(currentRow, CharacterElement::BACKSTORY, ui->textEditBackstory->toPlainText());
    charactersModel->setData(currentRow, CharacterElement::NAME, ui->lineEditName->text());
    characterChanged = false;

    savingView = false;
}
