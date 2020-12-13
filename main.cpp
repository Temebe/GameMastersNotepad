#include "mainwindow.h"
#include "welcomedialog.h"
#include "campaign.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    a.setApplicationName("Game Master's Notepad");
    a.setApplicationVersion("0.1.1");
    MainWindow w;
    Campaign campaign;
    WelcomeDialog welcomeDialog;
    welcomeDialog.show();

    w.init();
    QObject::connect(&w, &MainWindow::windowReady, &welcomeDialog, &WelcomeDialog::closeWindow);
    QObject::connect(&w, &MainWindow::loadingErrorOccured, &welcomeDialog, &WelcomeDialog::showCampaignLoadError);
    QObject::connect(&welcomeDialog, &WelcomeDialog::newCampaignChosen, &w, &MainWindow::createNewCampaign);
    QObject::connect(&welcomeDialog, &WelcomeDialog::loadCampaignChosen, &w, &MainWindow::loadCampaign);

    return a.exec();
}
