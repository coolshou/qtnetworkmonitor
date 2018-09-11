TEMPLATE = app
TARGET = QtNetworkMon

QT += gui widgets


VERSION = 0.4.0
DEFINES += APP_VERSION=\"\\\"$${VERSION}\\\"\" \


#INCLUDEPATH += .

INCLUDEPATH += ./uthash/include

CONFIG += app

RESOURCES = Ressources.qrc

win32{
    INCLUDEPATH += ./Wdp/include
    LIBS += -L ./Wdp/lib -lwpcap

    #For the win32 icon
    RC_FILE = setIcon.rc
}

unix{
    LIBS += -L ./usr/lib -lpcap
}

# Input
HEADERS += WinPcapFns.h \
           Misc.h \
           ThreadListener.h \
    GUI/Console.h \
    GUI/MainWindow.h \
    GUI/MasterWindow.h \
    GUI/Scope.h \
    IO/ReadWriteFile.h \
    const.h


SOURCES += main.cpp \
           ThreadListener.cpp \
           Misc.cpp \
           WinPcapFns.cpp \
    GUI/Console.cpp \
    GUI/MainWindow.cpp \
    GUI/MasterWindow.cpp \
    GUI/Scope.cpp \
    IO/ReadWriteFile.cpp

DISTFILES += \
    Readme.txt \
    GFX/icon/QtNetworkMon.png \
    QtNetworkMon.desktop

