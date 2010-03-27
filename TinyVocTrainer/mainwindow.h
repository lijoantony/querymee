#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

class QAction;
class TinyVocTrainer;
class TinyVocTrainerSettings;

class MainWindow : public QMainWindow
{
public:
    MainWindow();
private:
    QAction *settingAction;
    QMenu *mainMenu;
    void createActions();
    void createMenus();

    TinyVocTrainer *trainer;
};

#endif // MAINWINDOW_H
