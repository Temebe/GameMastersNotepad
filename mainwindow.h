#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include "campaign.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    void init();

signals:
    void windowReady();
    void loadingErrorOccured(QString reason);

public slots:
    void onCreateNewCampaignChosen(const QString& name);
    void onLoadCampaignChosen(const QString& name);
    void onCampaignReady();


private:
    Ui::MainWindow *ui;
    Campaign campaign;

    void configureCharactersListView();
};
#endif // MAINWINDOW_H
