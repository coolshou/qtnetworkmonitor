#ifndef MASTERWINDOW_H
#define MASTERWINDOW_H

#include <QMainWindow>
#include <QAction>
#include <QSystemTrayIcon>
#include <QCloseEvent>
#include <QMenu>
#include <QComboBox>
#include <QMenuBar>

#include "MainWindow.h"

class MasterWindow : public QMainWindow
{

    Q_OBJECT

    public:

        MasterWindow();

    private slots:

        //!What to do when the tray icon is clicked
        void iconActivated(QSystemTrayIcon::ActivationReason reason);

        //!What to do when the 'X' is clicked
        void closeEvent(QCloseEvent *event);

    private:

        void buildMenu();
        void buildTray();

        MainWindow * widgetMainWindow;

        //Menu
        QMenu       * showMenu;
        QMenu       * optionMenu;
        QMenu       * AboutMenu;
        QAction     * ShowHideConsoleAct;
        QAction     * ClearConsoleAct;
        QAction     * OptionAct;
        QAction     * AboutAct;
        //Tray
        QComboBox * iconComboBox;
        QSystemTrayIcon *trayIcon;
        QMenu *trayIconMenu;

        QAction *minimizeAction;
        QAction *maximizeAction;
        QAction *restoreAction;
        QAction *quitAction;

};
#endif

