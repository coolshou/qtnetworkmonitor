#include "MasterWindow.h"


MasterWindow::MasterWindow()
{
    setWindowTitle(tr("Qt Network Monitor v%1").arg(qApp->applicationVersion()));
    setWindowIcon(QIcon(":/GFX/systray/TrayIconNormal.png"));
    setting= new QSettings();
    dlgconfig = new DlgConfig();
    //resize(520, 340);

    widgetMainWindow = new MainWindow();
    setCentralWidget(widgetMainWindow);

    buildMenu();
    buildTray();

    loadSetting();
}

void MasterWindow::buildTray()
{
    //Tray
    quitAction = new QAction(tr("&Quit"), this);
    connect(quitAction, SIGNAL(triggered()), this, SLOT(appClose()));

    trayIcon = new QSystemTrayIcon(this);
    trayIcon->setIcon((QIcon(":/GFX/systray/TrayIconNormal.png")));

    trayIconMenu = new QMenu(this);
    trayIconMenu->setIcon((QIcon(":/GFX/systray/TrayIconNormal.png")));
    trayIconMenu->addAction(quitAction);

    trayIcon->setContextMenu(trayIconMenu);
    trayIcon->setVisible(true);

    connect(trayIcon, SIGNAL(activated(QSystemTrayIcon::ActivationReason) ),
             this, SLOT(iconActivated(QSystemTrayIcon::ActivationReason)) );
}

void MasterWindow::buildMenu()
{
    //Action
    ShowHideConsoleAct = new QAction(tr("&Show/Hide"), this);
    ShowHideConsoleAct->setStatusTip(tr("Show/Hide Console"));
    connect(ShowHideConsoleAct, SIGNAL(triggered()), widgetMainWindow, SLOT(toggleConsoleView()));
    ClearConsoleAct = new QAction(tr("&Clear"), this);
    ClearConsoleAct->setStatusTip(tr("Clear Console"));
    connect(ClearConsoleAct, SIGNAL(triggered()), widgetMainWindow, SLOT(clearConsoleView()));

    OptionAct = new QAction(tr("&Config"), this);
    OptionAct->setStatusTip(tr("Option config"));
    connect(OptionAct, SIGNAL(triggered()), this, SLOT(showConfig()));

    AboutAct = new QAction(tr("&About"), this);
    AboutAct->setStatusTip(tr("About"));
    connect(AboutAct, SIGNAL(triggered()), widgetMainWindow, SLOT(showAbout()));

    //menu
    showMenu = menuBar()->addMenu(tr("&Console"));
    showMenu->addAction(ShowHideConsoleAct);
    showMenu->addAction(ClearConsoleAct);

    optionMenu = menuBar()->addMenu(tr("&Option"));
    optionMenu->addAction(OptionAct);

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
    default:
        ;
    }
}
void MasterWindow::appClose()
{
    this->saveSetting();
    trayIcon->setVisible(false);
    qApp->exit(0);
}

void MasterWindow::showConfig()
{
    int rc;
    //TODO: show config dialog
    qDebug() << "TODO: show config dialog";
    dlgconfig->set_autostart(widgetMainWindow->getAutoStart());
    dlgconfig->set_unittype(widgetMainWindow->getUnitType());
    rc = dlgconfig->exec();
    if (rc == QDialog::Accepted) {
        //TODO: auto start
        // unit
        qDebug() << "Unit type:" << dlgconfig->get_UnitType();
        widgetMainWindow->setUnitType(dlgconfig->get_UnitType());
        widgetMainWindow->setAutoStart(dlgconfig->get_AutoStart());
    }
}

void  MasterWindow::loadSetting()
{
    //loadSetting
    widgetMainWindow->LoadOptionsFromFile();
    setting->beginGroup("MainWindow");
    this->setGeometry(setting->value("geometry", QRect(0,0, 520, 500)).toRect());
    setting->endGroup();
    /*
    setting->beginGroup("Main");
    widgetMainWindow->setUnitType(setting->value("unittype", 0).toInt());
    widgetMainWindow->setAutoStart(setting->value("autostart", false).toBool());
    widgetMainWindow->setDeviceNo(setting->value("DeviceNo", 0).toInt());
    setting->endGroup();
    */
}
void  MasterWindow::saveSetting()
{
    // saveSetting
    widgetMainWindow->SaveOptionsToFile();
    setting->beginGroup("MainWindow");
    setting->setValue("geometry", this->window()->geometry());
    setting->endGroup();
    /*
    setting->beginGroup("Main");
    setting->setValue("unittype", widgetMainWindow->getUnitType());
    setting->setValue("autostart", widgetMainWindow->getAutoStart());
    setting->setValue("DeviceNo", widgetMainWindow->getDeviceNo());
    setting->endGroup();
    */
    setting->sync();
}
