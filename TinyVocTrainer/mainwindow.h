/***************************************************************************

    Copyright 2010 Reto Zingg <g.d0b3rm4n@gmail.com>

 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

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
