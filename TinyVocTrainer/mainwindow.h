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
    // bool loadFile(const QString &fileName);
    // void setCurrentFile(const QString &fileName);
    QString curFile;
    void open();
};

#endif // MAINWINDOW_H
