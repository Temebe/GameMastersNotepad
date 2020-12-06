#ifndef WELCOMEDIALOG_H
#define WELCOMEDIALOG_H

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

private slots:
    void onCancelButtonClicked();
    void onCreateNewButtonClicked();
    void onLoadFromListButtonClicked();

private:
    Ui::WelcomeDialog *ui;
    QStandardItemModel campaignsModel;
    const QString defaultCampaignsFolder = QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation) + "/" + "GMNCampaigns";

    void populateCampaigns();
    void configureTableView();
};

#endif // WELCOMEDIALOG_H
