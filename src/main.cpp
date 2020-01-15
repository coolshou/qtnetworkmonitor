#include <iostream>

#include <QApplication>
#include <QPushButton>

#include "WinPcapFns.h"
#include "GUI/MainWindow.h"
#include "GUI/MasterWindow.h"
#include "version.h"
#include "Misc.h"

#include <QDebug>

using namespace std;

int main(int argc, char *argv[])
{
    int return_v;
    if (is_root()){
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

        return_v = app.exec();
    }else{
        qDebug() << "please run as root/administrator\n";
        return_v = -1;
    }
    return return_v;
}
