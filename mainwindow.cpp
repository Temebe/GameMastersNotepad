#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QInputDialog>
#include <QMessageBox>
#include <QSortFilterProxyModel>
#include <QAction>

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
    // CAMPAIGN CONNECTS
    connect(&campaign, &Campaign::succesfullyLoaded, this, &MainWindow::onCampaignReady);
    connect(&campaign, &Campaign::loadingFailed, this, &MainWindow::loadingErrorOccured);
    connect(&campaign, &Campaign::savingFailed, this, &MainWindow::onSavingError);

    // ACTION CONNECTS
    connect(ui->saveAction, &QAction::triggered, this, &MainWindow::onSaveTriggered);

    // CHARACTER TEXT CONNECTS
    // TODO This is incredibely inefficient, change this
    connect(ui->lineEditAge, &QLineEdit::textEdited, this, &MainWindow::onCharacterChanged);
    connect(ui->lineEditName, &QLineEdit::textEdited, this, &MainWindow::onCharacterChanged);
    connect(ui->lineEditRace, &QLineEdit::textEdited, this, &MainWindow::onCharacterChanged);
    connect(ui->lineEditProfession, &QLineEdit::textEdited, this, &MainWindow::onCharacterChanged);
    connect(ui->textEditBackstory, &QTextEdit::textChanged, this, &MainWindow::onCharacterChanged);
    connect(ui->textEditDescription, &QTextEdit::textChanged, this, &MainWindow::onCharacterChanged);
    connect(ui->textEditNotes, &QTextEdit::textChanged, this, &MainWindow::onCharacterChanged);
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
    show();
    emit windowReady();
}

void MainWindow::onSavingError(const QString &reason)
{
    QMessageBox::critical(this, tr("Saving error"), reason);
}

void MainWindow::onCharacterNameClicked(const QModelIndex &index)
{
    auto characterOpt = charactersModel->getCharacter(index);
    if (!characterOpt) {
        return;
    }

    showCharacter(characterOpt.value());
}

void MainWindow::onCharactersSearchTextChanged(const QString& text)
{
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
    onCharacterNameClicked(index);
}

void MainWindow::onSaveTriggered()
{
    campaign.saveToFile(charactersModel);
}

// TODO If name is changed it should be reflected in charactersListView
void MainWindow::onCharacterChanged()
{
    if (!charactersModel) {
        return;
    }

    auto currentIndex = ui->charactersListView->selectionModel()->currentIndex();
    if (!currentIndex.isValid()) {
        return;
    }

    auto currentRow = currentIndex.row();
    charactersModel->setData(currentRow, CharacterElement::AGE, ui->lineEditAge->text());
    charactersModel->setData(currentRow, CharacterElement::RACE, ui->lineEditRace->text());
    charactersModel->setData(currentRow, CharacterElement::NAME, ui->lineEditName->text());
    charactersModel->setData(currentRow, CharacterElement::PROFESSION, ui->lineEditProfession->text());
    charactersModel->setData(currentRow, CharacterElement::DESCRIPTION, ui->textEditDescription->toPlainText());
    charactersModel->setData(currentRow, CharacterElement::NOTES, ui->textEditNotes->toPlainText());
    charactersModel->setData(currentRow, CharacterElement::BACKSTORY, ui->textEditBackstory->toPlainText());
}

void MainWindow::configureCharactersListView()
{
    if (!charactersModel) {
        return;
    }
    charactersFilterModel.setSourceModel(charactersModel.get());
    ui->charactersListView->setModel(&charactersFilterModel);
    charactersFilterModel.setFilterKeyColumn(0);
    connect(ui->charactersListView->selectionModel(), &QItemSelectionModel::currentChanged, this, &MainWindow::onCharacterNameClicked);

    selectFirstCharacter();
}

void MainWindow::selectFirstCharacter()
{
    if (!charactersModel->isEmpty()) {
        auto firstIndex = charactersModel->index(0, 0);
        ui->charactersListView->selectionModel()->setCurrentIndex(firstIndex, QItemSelectionModel::ClearAndSelect);
        onCharacterNameClicked(firstIndex);
    } else {
        clearCharacterView();
    }
}

void MainWindow::showCharacter(const Character &character)
{
    ui->lineEditAge->setText(character.getAge());
    ui->lineEditName->setText(character.getName());
    ui->lineEditProfession->setText(character.getProfession());
    ui->lineEditRace->setText(character.getRace());
    ui->textEditBackstory->setText(character.getBackstory());
    ui->textEditDescription->setText(character.getDescription());
    ui->textEditNotes->setText(character.getNotes());
}

void MainWindow::clearCharacterView()
{
    ui->lineEditAge->clear();
    ui->lineEditName->clear();
    ui->lineEditProfession->clear();
    ui->lineEditRace->clear();
    ui->textEditBackstory->clear();
    ui->textEditDescription->clear();
    ui->textEditNotes->clear();
}
