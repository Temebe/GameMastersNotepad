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

void WelcomeDialog::onSuccesfullCampaignLoad()
{
    close();
}

void WelcomeDialog::onCampaignLoadError(QString message)
{
    QErrorMessage().showMessage(message);
}

void WelcomeDialog::onCancelButtonClicked()
{
    QApplication::exit();
}

void WelcomeDialog::onCreateNewButtonClicked()
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

void WelcomeDialog::onLoadFromListButtonClicked()
{
    auto selectedRows = ui->campaignsTableView->selectionModel()->selectedRows();
    if (selectedRows.isEmpty()) {
        return;
    }

    auto selectedRow = selectedRows.first().row();
    auto selectedName = campaignsModel.index(selectedRow, 0);
    auto campaignName = campaignsModel.data(selectedName).toString();
    emit loadCampaignChosen(campaignName);
}

void WelcomeDialog::populateCampaigns()
{
    configureTableView();
    QDirIterator it(GMN::campaignsFolder);
    int row = 0;

    while(it.hasNext()) {
        auto path = it.next();
        auto infoPath = path + "/" + QString("info.json");
        if (!QFile::exists(infoPath)) {
            continue;
        }

        //FIXME Can I somehow use try catch and then go with rest of code outside of try scope
        // And without std::optional and pointers
        QStandardItem *name {nullptr};
        QStandardItem *creationDate {nullptr};
        QStandardItem *lastOpenedDate {nullptr};
        try {
            CampaignInfo campaignInfo(infoPath);
            name = new QStandardItem(campaignInfo.getName());
            creationDate = new QStandardItem(campaignInfo.getCreationDate().toString());
            lastOpenedDate = new QStandardItem(campaignInfo.getLastOpenedDate().toString());

        }  catch (std::runtime_error& e) {
            continue;
        }

        campaignsModel.setItem(row, 0, name);
        campaignsModel.setItem(row, 1, creationDate);
        campaignsModel.setItem(row, 2, lastOpenedDate);
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

    //header->setStretchLastSection(true);
}
