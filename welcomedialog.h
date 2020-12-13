#ifndef WELCOMEDIALOG_H
#define WELCOMEDIALOG_H

#include "gmnpaths.h"

#include <QDialog>
#include <QStandardItemModel>
#include <QStandardPaths>

namespace Ui {
class WelcomeDialog;
}

class WelcomeDialog : public QDialog
{
    Q_OBJECT

public:
    explicit WelcomeDialog(QWidget *parent = nullptr);
    ~WelcomeDialog();

signals:
    void newCampaignChosen(QString name);
    void loadCampaignChosen(QString name);

public slots:
    void closeWindow();
    void showCampaignLoadError(QString message);

private slots:
    void exitApplication();
    void createNewCampaign();
    void loadFromList();
    void loadCampaignFromIndex(const QModelIndex &index);

private:
    Ui::WelcomeDialog *ui;
    QStandardItemModel campaignsModel;

    void populateCampaigns();
    void configureTableView();
};

#endif // WELCOMEDIALOG_H
