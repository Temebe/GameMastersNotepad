#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "gmnpaths.h"

#include <QInputDialog>
#include <QMessageBox>
#include <QSortFilterProxyModel>
#include <QAction>
#include <QPixmap>
#include <QFileDialog>

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
    connect(&campaign, &Campaign::succesfullyLoaded, this, &MainWindow::onCampaignReady);
    connect(&campaign, &Campaign::loadingFailed, this, &MainWindow::loadingErrorOccured);
    connect(&campaign, &Campaign::savingFailed, this, &MainWindow::onSavingError);

    // ACTION CONNECTS
    connect(ui->saveAction, &QAction::triggered, this, &MainWindow::onSaveTriggered);

    connect(ui->lineEditAge, &QLineEdit::textEdited, this, &MainWindow::onCharacterChanged);
    connect(ui->lineEditName, &QLineEdit::textEdited, this, &MainWindow::onCharacterChanged);
    connect(ui->lineEditRace, &QLineEdit::textEdited, this, &MainWindow::onCharacterChanged);
    connect(ui->lineEditProfession, &QLineEdit::textEdited, this, &MainWindow::onCharacterChanged);
    connect(ui->textEditBackstory, &QTextEdit::textChanged, this, &MainWindow::onCharacterChanged);
    connect(ui->textEditDescription, &QTextEdit::textChanged, this, &MainWindow::onCharacterChanged);
    connect(ui->textEditNotes, &QTextEdit::textChanged, this, &MainWindow::onCharacterChanged);

    connect(ui->labelImage, &ClickableImageLabel::clicked, this, &MainWindow::onCharacterImageClicked);
}

void MainWindow::onCreateNewCampaignChosen(const QString& name)
{
    campaign.createNewCampaign(name);
}

void MainWindow::onLoadCampaignChosen(const QString& name)
{
    campaign.loadFromName(name);
}

void MainWindow::onCampaignReady()
{
    charactersModel = campaign.createCharactersModel();
    configureCharactersListView();
    if (!charactersModel->isEmpty()) {
        enableCharacterView();
    }
    show();
    emit windowReady();
}

void MainWindow::onSavingError(const QString &reason, const QString& details)
{
    Q_UNUSED(details);
    QMessageBox::critical(this, tr("Saving error"), reason);
}

void MainWindow::onCharacterSelectionChanged(const QModelIndex &index, const QModelIndex &previousIndex)
{
    saveCurrentViewToCharacter(previousIndex);
    auto characterOpt = charactersModel->getCharacter(index);

    if (!characterOpt) {
        clearCharacterView();
        disableCharacterView();
        return;
    }

    showCharacter(characterOpt.value());
}

void MainWindow::onCharactersSearchTextChanged(const QString& text)
{

        auto currentIndex = ui->charactersListView->selectionModel()->currentIndex();
        saveCurrentViewToCharacter(currentIndex);

    charactersFilterModel.setFilterRegExp(QRegExp(text, Qt::CaseSensitive, QRegExp::FixedString));
}

void MainWindow::onRemoveCharacterClicked()
{
    auto currentIndex = ui->charactersListView->selectionModel()->currentIndex();
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

void MainWindow::onAddCharacterClicked()
{
    QString name = QInputDialog::getText(this, tr("Add new character"), tr("Name for new character:"));
    auto index = charactersModel->addCharacter(name);
    ui->charactersListView->selectionModel()->select(index, QItemSelectionModel::ClearAndSelect);
    enableCharacterView();
    onCharacterSelectionChanged(index);
}

void MainWindow::onSaveTriggered()
{
    auto currentIndex = ui->charactersListView->selectionModel()->currentIndex();
    saveCurrentViewToCharacter(currentIndex);
    campaign.saveToFile(charactersModel);
}

void MainWindow::onCharacterImageClicked()
{
    auto currentIndex = ui->charactersListView->selectionModel()->currentIndex();
    if (!currentIndex.isValid()) {
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
    charactersModel->setData(currentIndex.row(), CharacterElement::IMAGEPATH, relativeImagePath);
}

void MainWindow::onCharacterChanged()
{
    characterChanged = true;
}

void MainWindow::configureCharactersListView()
{
    if (!charactersModel) {
        return;
    }
    charactersFilterModel.setSourceModel(charactersModel.get());
    ui->charactersListView->setModel(&charactersFilterModel);
    charactersFilterModel.setFilterKeyColumn(0);
    connect(ui->charactersListView->selectionModel(), &QItemSelectionModel::currentChanged, this, &MainWindow::onCharacterSelectionChanged);

    selectFirstCharacter();
}

void MainWindow::selectFirstCharacter()
{
    if (!charactersModel->isEmpty()) {
        auto firstIndex = charactersModel->index(0, 0);
        ui->charactersListView->selectionModel()->setCurrentIndex(firstIndex, QItemSelectionModel::ClearAndSelect);
        onCharacterSelectionChanged(firstIndex);
    } else {
        clearCharacterView();
        disableCharacterView();
    }
}

void MainWindow::showCharacter(const Character &character)
{
    enableCharacterView();
    ui->lineEditAge->setText(character.getAge());
    ui->lineEditName->setText(character.getName());
    ui->lineEditProfession->setText(character.getProfession());
    ui->lineEditRace->setText(character.getRace());
    ui->textEditBackstory->setText(character.getBackstory());
    ui->textEditDescription->setText(character.getDescription());
    ui->textEditNotes->setText(character.getNotes());

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
    ui->lineEditAge->setDisabled(true);
    ui->lineEditName->setDisabled(true);
    ui->lineEditProfession->setDisabled(true);
    ui->lineEditRace->setDisabled(true);
    ui->textEditBackstory->setDisabled(true);
    ui->textEditDescription->setDisabled(true);
    ui->textEditNotes->setDisabled(true);
}

void MainWindow::enableCharacterView()
{
    ui->lineEditAge->setDisabled(false);
    ui->lineEditName->setDisabled(false);
    ui->lineEditProfession->setDisabled(false);
    ui->lineEditRace->setDisabled(false);
    ui->textEditBackstory->setDisabled(false);
    ui->textEditDescription->setDisabled(false);
    ui->textEditNotes->setDisabled(false);
}

void MainWindow::saveCurrentViewToCharacter(const QModelIndex &index)
{
    if (!index.isValid()) {
        return;
    }
    if (!characterChanged) {
        return;
    }

    auto currentRow = index.row();
    charactersModel->setData(currentRow, CharacterElement::AGE, ui->lineEditAge->text());
    charactersModel->setData(currentRow, CharacterElement::RACE, ui->lineEditRace->text());
    charactersModel->setData(currentRow, CharacterElement::NAME, ui->lineEditName->text());
    charactersModel->setData(currentRow, CharacterElement::PROFESSION, ui->lineEditProfession->text());
    charactersModel->setData(currentRow, CharacterElement::DESCRIPTION, ui->textEditDescription->toPlainText());
    charactersModel->setData(currentRow, CharacterElement::NOTES, ui->textEditNotes->toPlainText());
    charactersModel->setData(currentRow, CharacterElement::BACKSTORY, ui->textEditBackstory->toPlainText());
    characterChanged = false;
}
