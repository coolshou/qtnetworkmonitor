TEMPLATE = app
TARGET = QtNetworkMon

QT += gui widgets charts

MAJOR = 0
MINOR = 5
#year
BUILD = 2020
#month+day
PATCH = 0116
COMPANYNAME = "coolshou.idv.tw"
PRODUCTNAME = "qtnetworkmonitor"
PRODUCTDESCRIPTION = "Qt Network Monitor is a KISS network monitor that monitors how much bandwidth is being used on a daily basis"

VERSION = $${MAJOR}.$${MINOR}.$${BUILD}.$${PATCH}
VERSION_FILE = "$${MAJOR},$${MINOR},$${BUILD},$${PATCH}"
USE_WINPCAP = 1

DEFINES += APP_VERSION=\"\\\"$${VERSION}\\\"\"
DEFINES += APP_PRODUCTNAME=\"\\\"$${PRODUCTNAME}\\\"\"
DEFINES += USE_WINPCAP=\"\\\"$${USE_WINPCAP}\\\"\"

ParentDirectory = $$PWD/bin
contains(QT_ARCH, i386) {
    ARCH=x86
} else {
    ARCH=x64
}
CONFIG(debug, debug|release) {
    BUILD=debug
} else {
    BUILD=release
}

RCC_DIR = "$$ParentDirectory/Build/$$BUILD/$$ARCH/RCCFiles"
#UI_DIR = "$$ParentDirectory/Build/$$BUILD/$$ARCH/UICFiles"
MOC_DIR = "$$ParentDirectory/Build/$$BUILD/$$ARCH/MOCFiles"
OBJECTS_DIR = "$$ParentDirectory/Build/$$BUILD/$$ARCH/ObjFiles"

DESTDIR = "$$ParentDirectory/$$BUILD/$$ARCH"

INCLUDEPATH += .
INCLUDEPATH += ./include

CONFIG += app

RESOURCES = src/Ressources.qrc

win32{
    QMAKE_SUBSTITUTES += ver_conf
    ver_conf.input = $$PWD/src/version.h.in
    ver_conf.output  = $$PWD/src/version.h

    contains(USE_WINPCAP, 1) {
        # Winpcap
        INCLUDEPATH += "$$PWD/Wdp/include"
        contains(QT_ARCH, i386) {
            LIBS += -L "$$PWD/Wdp/lib" -lwpcap
        } else {
            LIBS += -L "$$PWD/Wdp/lib/x64" -lwpcap
        }
    } else {
        # npcap for Win10
        INCLUDEPATH += "$$PWD/npcap/Include"
        contains(QT_ARCH, i386) {
            LIBS += -L "$$PWD/npcap/Lib" -lwpcap
        } else {
            LIBS += -L "$$PWD/npcap/Lib/x64" -lwpcap
        }
    }
    CONFIG -= embed_manifest_exe
    #For the win32 icon/version
    RC_FILE = src/setIcon.rc
    QMAKE_MANIFEST = "$$PWD/QtNetworkMon.manifest"
    #QMAKE_LFLAGS += /MANIFESTFILE:"$$PWD/QtNetworkMon.manifest"
    #/MANIFEST
    # add require qt dll and files
    QMAKE_POST_LINK += "windeployqt.exe $$DESTDIR"
    #
}

unix{
    QMAKE_SUBSTITUTES += $$PWD/src/version.h.in
    QMAKE_SUBSTITUTES += $$PWD/debian/changelog.in
    LIBS += -L ./usr/lib -lpcap
}

# Input
HEADERS += \
    src/WinPcapFns.h \
    src/Misc.h \
    src/ThreadListener.h \
    src/const.h \
    src/GUI/Console.h \
    src/GUI/MainWindow.h \
    src/GUI/MasterWindow.h \
    src/GUI/Scope.h \
    src/IO/ReadWriteFile.h \
    src/version.h.in \
    src/GUI/status.h \
    src/const.h \
    src/GUI/dlgconfig.h \
    src/GUI/chart.h


SOURCES += \
    src/main.cpp \
    src/ThreadListener.cpp \
    src/Misc.cpp \
    src/WinPcapFns.cpp \
    src/GUI/Console.cpp \
    src/GUI/MainWindow.cpp \
    src/GUI/MasterWindow.cpp \
    src/GUI/Scope.cpp \
    src/IO/ReadWriteFile.cpp \
    src/GUI/status.cpp \
    src/GUI/dlgconfig.cpp \
    src/GUI/chart.cpp

DISTFILES += \
    QtNetworkMon.manifest \
    Readme.txt \
    installer/config/config.xml.in \
    installer/packages/coolshou.idv/meta/package.xml.in \
    installer/packages/npcap/meta/package.xml.in \
    src/setIcon.rc \
    debian/control \
    debian/rules \
    QtNetworkMon.desktop.in \
    debian/qtnetworkmonitor.install \
    debian/changelog.in \
    debian/qtnetworkmonitor.postinst \
    debian/qtnetworkmonitor.postrm

unix{
    DISTFILES += \
        src/GFX/icon/QtNetworkMon.png \
}

FORMS += \
    src/GUI/status.ui \
    src/GUI/dlgconfig.ui

