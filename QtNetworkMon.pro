TEMPLATE = app
TARGET = QtNetworkMon

QT += gui widgets

MAJOR = 0
MINOR = 4
BUILD = 0
PATCH = 0
COMPANYNAME = "coolshou.idv"
PRODUCTDESCRIPTION = "Qt Network Monitor is a KISS network monitor that monitors how much bandwidth is being used on a daily basis"
VERSION = $${MAJOR}.$${MINOR}.$${BUILD}.$${PATCH}
VERSION_FILE = "$${MAJOR},$${MINOR},$${BUILD},$${PATCH}"

DEFINES += APP_VERSION=\"\\\"$${VERSION}\\\"\" \

INCLUDEPATH += .
INCLUDEPATH += ./src
# INCLUDEPATH += ./uthash/include

CONFIG += app

RESOURCES = src/Ressources.qrc

win32{
    QMAKE_SUBSTITUTES += ver_conf
    ver_conf.input = $$PWD/src/version.h.in
    ver_conf.output  = $$PWD/src/version.h
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
    QMAKE_SUBSTITUTES += $$PWD/src/version.h.in
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
    src/IO/ReadWriteFile.h \
    src/version.h.in


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
    Readme.txt \
    src/setIcon.rc

unix{
    DISTFILES += \
        src/GFX/icon/QtNetworkMon.png \
        QtNetworkMon.desktop \
}

