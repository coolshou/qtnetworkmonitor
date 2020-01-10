#include <iostream>

#include <QApplication>
#include <QPushButton>

#include "WinPcapFns.h"
#include "GUI/MainWindow.h"
#include "GUI/MasterWindow.h"
#include "version.h"

using namespace std;

int main(int argc, char *argv[])
{

    cout<<"App. Begin"<<endl;

    QApplication app(argc, argv);
    QApplication::setApplicationName(APP_PRODUCTNAME);
    QApplication::setOrganizationName(VER_COMPANYNAME_STR);
    QApplication::setApplicationVersion(APP_VERSION);
#ifdef WIN32
    #ifndef USE_WINPCAP
    // use npcap first
    QApplication::addLibraryPath("C:/Windows/System32/Npcap/");
    #endif
#endif
    MasterWindow MasterWin;
    MasterWin.show();

    //MainWindow MainWin;
    //MainWin.show();

    int return_v = app.exec();

    cout<<"App. End"<<endl;

    return return_v;

}
