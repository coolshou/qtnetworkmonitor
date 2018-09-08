#include "MasterWindow.h"


MasterWindow::MasterWindow()
{
    setWindowTitle(tr("Qt Network Monitor v%1").arg(qApp->applicationVersion()));
    resize(520, 340);

    widgetMainWindow = new MainWindow();
    setCentralWidget(widgetMainWindow);

    buildMenu();
    buildTray();
}

void MasterWindow::buildTray()
{
    //Tray
    setWindowIcon(QIcon(":/GFX/systray/trash.svg"));

    quitAction = new QAction(tr("&Quit"), this);
    connect(quitAction, SIGNAL(triggered()), qApp, SLOT(quit()));

    trayIcon = new QSystemTrayIcon(this);
    trayIcon->setIcon((QIcon(":/GFX/systray/TrayIconNormal.png")));

    trayIconMenu = new QMenu(this);
    trayIconMenu->setIcon((QIcon(":/GFX/systray/TrayIconNormal.png")));
    trayIconMenu->addAction(quitAction);

    trayIcon->setContextMenu(trayIconMenu);
    //trayIcon->setVisible(true);

    connect(trayIcon, SIGNAL(activated(QSystemTrayIcon::ActivationReason) ),
             this, SLOT(iconActivated(QSystemTrayIcon::ActivationReason)) );
}

void MasterWindow::buildMenu()
{
    //Menu
    ShowHideConsoleAct = new QAction(tr("&Console"), this);
    ShowHideConsoleAct->setStatusTip(tr("Show/Hide Console"));
    connect(ShowHideConsoleAct, SIGNAL(triggered()), widgetMainWindow, SLOT(toggleConsoleView()));
    AboutAct = new QAction(tr("&Console"), this);
    AboutAct->setStatusTip(tr("About"));
    connect(AboutAct, SIGNAL(triggered()), widgetMainWindow, SLOT(showAbout()));

    showMenu = menuBar()->addMenu(tr("&Show/Hide"));
    showMenu->addAction(ShowHideConsoleAct);
}


void MasterWindow::closeEvent(QCloseEvent *event)
{
    trayIcon->setVisible(true);

    if (trayIcon->isVisible())
    {
    hide();
    event->ignore();
    }
}

void MasterWindow::iconActivated(QSystemTrayIcon::ActivationReason reason)
{
    switch (reason)
    {
    case QSystemTrayIcon::Trigger:
    case QSystemTrayIcon::DoubleClick:
    //trayIcon->setVisible(false);
    show();

    default:;
    }
}

