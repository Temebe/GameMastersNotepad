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
    void onCreateNewCampaignChosen(const QString& name);
    void onLoadCampaignChosen(const QString& name);
    void onCampaignReady();
    void onSavingError(const QString& reason, const QString& details);

private slots:
    void onCharacterSelectionChanged(const QModelIndex& index, const QModelIndex &previousIndex = QModelIndex());
    void onCharactersSearchTextChanged(const QString& text);
    void onRemoveCharacterClicked();
    void onAddCharacterClicked();
    void onSaveTriggered();
    void onCharacterImageClicked();
    void onCharacterChanged();


private:
    Ui::MainWindow *ui;
    Campaign campaign;
    std::unique_ptr<CharactersModel> charactersModel;
    QSortFilterProxyModel charactersFilterModel;
    bool characterChanged = false;

    void configureCharactersListView();
    void selectFirstCharacter();
    void showCharacter(const Character& character);
    void clearCharacterView();
    void disableCharacterView();
    void enableCharacterView();
    void saveCurrentViewToCharacter(const QModelIndex& index);
};
#endif // MAINWINDOW_H
