#include "mainwindow.h"
#include "welcomedialog.h"
#include "campaign.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    Campaign campaign;
    WelcomeDialog welcomeDialog;
    welcomeDialog.show();

    w.init();
    QObject::connect(&w, &MainWindow::windowReady, &welcomeDialog, &WelcomeDialog::onSuccesfullCampaignLoad);
    QObject::connect(&w, &MainWindow::loadingErrorOccured, &welcomeDialog, &WelcomeDialog::onCampaignLoadError);
    QObject::connect(&welcomeDialog, &WelcomeDialog::newCampaignChosen, &w, &MainWindow::onCreateNewCampaignChosen);
    QObject::connect(&welcomeDialog, &WelcomeDialog::loadCampaignChosen, &w, &MainWindow::onLoadCampaignChosen);

    return a.exec();
}
