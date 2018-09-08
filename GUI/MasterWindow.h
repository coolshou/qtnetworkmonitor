#ifndef MASTERWINDOW_H
#define MASTERWINDOW_H

#include <QMainWindow>

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
        QMenu       *   showMenu;
        QAction     *   ShowHideConsoleAct;

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
