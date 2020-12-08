#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QSortFilterProxyModel>

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
    connect(&campaign, &Campaign::succesfullyLoaded, this, &MainWindow::onCampaignReady);
    connect(&campaign, &Campaign::loadingFailed, this, &MainWindow::loadingErrorOccured);
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
    configureCharactersListView();
    show();
    emit windowReady();
}

void MainWindow::configureCharactersListView()
{
    campaign.passCharactersModel(ui->charactersListView);
    campaign.passCharactersModel(ui->tableView);
}
