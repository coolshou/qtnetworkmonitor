#include "MasterWindow.h"


MasterWindow::MasterWindow()
{
    setWindowTitle(tr("Qt Network Monitor v%1").arg(qApp->applicationVersion()));
    setWindowIcon(QIcon(":/GFX/systray/TrayIconNormal.png"));

    resize(520, 340);

    widgetMainWindow = new MainWindow();
    setCentralWidget(widgetMainWindow);

    buildMenu();
    buildTray();
}

void MasterWindow::buildTray()
{
    //Tray
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
    ShowHideConsoleAct = new QAction(tr("&Show/Hide"), this);
    ShowHideConsoleAct->setStatusTip(tr("Show/Hide Console"));
    connect(ShowHideConsoleAct, SIGNAL(triggered()), widgetMainWindow, SLOT(toggleConsoleView()));
    optionAct = new QAction(tr("&Config"), this);
    optionAct->setStatusTip(tr("Option config"));
    //TODO: optin act connect
    AboutAct = new QAction(tr("&About"), this);
    AboutAct->setStatusTip(tr("About"));
    connect(AboutAct, SIGNAL(triggered()), widgetMainWindow, SLOT(showAbout()));

    showMenu = menuBar()->addMenu(tr("&Console"));
    showMenu->addAction(ShowHideConsoleAct);
    optionMenu = menuBar()->addMenu(tr("&Option"));
    optionMenu->addAction(optionAct);
    AboutMenu = menuBar()->addMenu(tr("&Help"));
    AboutMenu->addAction(AboutAct);
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

