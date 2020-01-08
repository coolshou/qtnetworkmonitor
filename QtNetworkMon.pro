TEMPLATE = app
TARGET = QtNetworkMon

QT += gui widgets


VERSION = 0.4.0
DEFINES += APP_VERSION=\"\\\"$${VERSION}\\\"\" \


INCLUDEPATH += .
INCLUDEPATH += ./src
INCLUDEPATH += ./uthash/include

CONFIG += app

RESOURCES = src/Ressources.qrc

win32{
    # Winpcap
    #INCLUDEPATH += ./Wdp/include
    #LIBS += -L ./Wdp/lib -lwpcap
    # npcap for Win10
    INCLUDEPATH += "$$PWD/npcap/Include"
    contains(QT_ARCH, i386) {
        LIBS += -L "$$PWD/npcap/Lib" -lwpcap
    } else {
        LIBS += -L "$$PWD/npcap/Lib/x64" -lwpcap
    }

    #For the win32 icon
    RC_FILE = src/setIcon.rc
}

unix{
    LIBS += -L ./usr/lib -lpcap
}

# Input
HEADERS += src/WinPcapFns.h \
           src/Misc.h \
           src/ThreadListener.h \
    src/const.h \
    src/GUI/Console.h \
    src/GUI/MainWindow.h \
    src/GUI/MasterWindow.h \
    src/GUI/Scope.h \
    src/IO/ReadWriteFile.h


SOURCES += src/main.cpp \
           src/ThreadListener.cpp \
           src/Misc.cpp \
           src/WinPcapFns.cpp \
    src/GUI/Console.cpp \
    src/GUI/MainWindow.cpp \
    src/GUI/MasterWindow.cpp \
    src/GUI/Scope.cpp \
    src/IO/ReadWriteFile.cpp

DISTFILES += \
    Readme.txt

unix{
    DISTFILES += \
        src/GFX/icon/QtNetworkMon.png \
        QtNetworkMon.desktop \
}

