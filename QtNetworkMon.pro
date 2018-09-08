######################################################################
# Automatically generated by qmake (2.01a) jeu. 3. mai 23:27:47 2007
######################################################################

TEMPLATE = app
TARGET = QtNetworkMon
QT += gui widgets
INCLUDEPATH += .

CONFIG += console

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
            GUI\MainWindow.h \
            GUI\MasterWindow.h \
            GUI\Scope.h \
            GUI\Console.h \
            IO\ReadWriteFile.h \

SOURCES += main.cpp \
            ThreadListener.cpp \
            Misc.cpp \
            WinPcapFns.cpp \
            GUI\MainWindow.cpp \
            GUI\MasterWindow.cpp \
            GUI\Scope.cpp \
            GUI\Console.cpp \
            IO\ReadWriteFile.cpp \
