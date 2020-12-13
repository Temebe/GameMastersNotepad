#include "welcomedialog.h"
#include "ui_welcomedialog.h"
#include "campaigninfo.h"

#include <QDirIterator>
#include <QErrorMessage>
#include <QInputDialog>
#include <QStandardPaths>

WelcomeDialog::WelcomeDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::WelcomeDialog)
{
    ui->setupUi(this);
    populateCampaigns();
}

WelcomeDialog::~WelcomeDialog()
{
    delete ui;
}

void WelcomeDialog::closeWindow()
{
    close();
}

void WelcomeDialog::showCampaignLoadError(QString message)
{
    QErrorMessage().showMessage(message);
}

void WelcomeDialog::exitApplication()
{
    QApplication::exit();
}

void WelcomeDialog::createNewCampaign()
{
    bool ok;
    QString campaignName;
    auto getName = [this, &ok](const QString &message){
        return QInputDialog::getText(
                this,
                tr("Create new campaign"),
                message,
                QLineEdit::Normal,
                tr("Name"),
                &ok
        );
    };

    campaignName = getName(tr("Write down name for your new campaign:"));
    if (!ok) {
        return;
    }

    while(QFile::exists(GMN::campaignsFolder + "/" + campaignName) || campaignName.isEmpty()) {
        if (campaignName.isEmpty()) {
            campaignName = getName(tr("Empty name was given! Write down name for campaign:"));
        } else {
            campaignName = getName(tr("Name already taken! Write down name for new campaign:"));
        }

        if (!ok) {
            return;
        }
    }

    emit newCampaignChosen(campaignName);
}

void WelcomeDialog::loadFromList()
{
    auto currentIndex = ui->campaignsTableView->selectionModel()->currentIndex();
    if (!currentIndex.isValid()) {
        return;
    }

    auto nameIndex = currentIndex.siblingAtColumn(0);
    auto campaignName = campaignsModel.data(nameIndex).toString();
    emit loadCampaignChosen(campaignName);
}

void WelcomeDialog::loadCampaignFromIndex(const QModelIndex &index)
{
    auto nameIndex = index.siblingAtColumn(0);
    auto campaignName = campaignsModel.data(nameIndex).toString();
    emit loadCampaignChosen(campaignName);
}

void WelcomeDialog::populateCampaigns()
{
    configureTableView();
    QDirIterator it(GMN::campaignsFolder);
    int row = 0;

    while(it.hasNext()) {
        auto path = it.next();
        auto infoPath = path + "/" + GMN::infoFileName;
        if (!QFile::exists(infoPath)) {
            continue;
        }

        CampaignInfo campaignInfo;

        if (!campaignInfo.loadFromFile(infoPath)) {
            continue;
        }

        campaignsModel.setItem(row, 0, new QStandardItem(campaignInfo.getName()));
        campaignsModel.setItem(row, 1, new QStandardItem(campaignInfo.getCreationDate().toString()));
        campaignsModel.setItem(row, 2, new QStandardItem(campaignInfo.getLastOpenedDate().toString()));
        ++row;
    }

    ui->campaignsTableView->resizeColumnsToContents();
}

void WelcomeDialog::configureTableView()
{
    ui->campaignsTableView->setModel(&campaignsModel);
    auto header = ui->campaignsTableView->horizontalHeader();

    campaignsModel.setHorizontalHeaderItem(0, new QStandardItem(tr("Name")));
    header->setSectionResizeMode(0, QHeaderView::Stretch);
    campaignsModel.setHorizontalHeaderItem(1, new QStandardItem(tr("Created")));
    header->setSectionResizeMode(1, QHeaderView::ResizeToContents);
    campaignsModel.setHorizontalHeaderItem(2, new QStandardItem(tr("Last opened")));
    header->setSectionResizeMode(2, QHeaderView::ResizeToContents);
}
