#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSortFilterProxyModel>

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
    void loadingErrorOccured(const QString &reason);

public slots:
    void createNewCampaign(const QString& name);
    void loadCampaign(const QString& name);
    void configureViews();
    void showSavingError(const QString& reason, const QString& details);

protected slots:
    void closeEvent(QCloseEvent *event) override;

private slots:
    void onCharacterSelectionChanged(const QModelIndex& index, const QModelIndex &previousIndex = QModelIndex());
    void onCharactersSearchTextChanged(const QString& text);
    void removeCharacter();
    void addCharacter();
    void saveCampaign();
    void chooseCharacterImage();
    void setCharacterChanged();
    void showAboutApp();
    void showAboutQt();


private:
    Ui::MainWindow *ui;
    Campaign campaign;
    std::unique_ptr<CharactersModel> charactersModel;
    QSortFilterProxyModel charactersFilterModel;
    bool characterChanged = false;
    bool campaignChanged = false;
    bool savingView = false;

    void configureCharactersListView();
    void selectFirstCharacter();
    void showCharacter(const Character& character);
    void clearCharacterView();
    void disableCharacterView();
    void enableCharacterView();
    void setDisabledCharacterView(const bool disabled);
    void saveCurrentViewToCharacter(const QModelIndex& index);
};
#endif // MAINWINDOW_H
