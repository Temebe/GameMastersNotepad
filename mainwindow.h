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
    void onLocationSelectionChanged(const QModelIndex& index, const QModelIndex &previousIndex = QModelIndex());
    void onCharactersSearchTextChanged(const QString& text);
    void onLocationsSearchTextChanged(const QString& text);
    void removeCharacter();
    void removeLocation();
    void addCharacter();
    void addLocation();
    void saveCampaign();
    void chooseCharacterImage();
    void setCharacterChanged();
    void setLocationChanged();
    void showAboutApp();
    void showAboutQt();


private:
    Ui::MainWindow *ui;
    Campaign campaign;
    std::unique_ptr<GMNObjectModel<Character>> charactersModel;
    std::unique_ptr<GMNObjectModel<Location>> locationsModel;
    QSortFilterProxyModel charactersFilterModel;
    QSortFilterProxyModel locationsFilterModel;
    bool characterChanged = false;
    bool locationChanged = false;
    bool campaignChanged = false;
    bool savingView = false;

    void configureCharactersListView();
    void configureLocationsListView();
    void selectFirstCharacter();
    void selectFirstLocation();
    void showCharacter(const Character& character);
    void showLocation(const Location& location);
    void clearCharacterView();
    void clearLocationView();
    void disableCharacterView();
    void disableLocationView();
    void enableCharacterView();
    void enableLocationView();
    void setDisabledCharacterView(const bool disabled);
    void setDisabledLocationView(const bool disabled);
    void saveCurrentViewToCharacter(const QModelIndex& index);
    void saveCurrentViewToLocation(const QModelIndex& index);
};
#endif // MAINWINDOW_H
