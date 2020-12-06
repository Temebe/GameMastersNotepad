#include "mainwindow.h"
#include "welcomedialog.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    //MainWindow w;
    //w.show();
    WelcomeDialog welcomeDialog;
    welcomeDialog.show();
    return a.exec();
}
