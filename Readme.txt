Qt Network Monitor : A Free, Open Source, Portable (Windows, Unix / Linux) Network Monitor.

Go to: https://github.com/coolshou/qtnetworkmonitor.git
for the latest version.

#Build from source:
> git clone https://github.com/coolshou/qtnetworkmonitor.git
> cd qtnetworkmonitor
> git submodule init
> git submodule update
> qmake
> make


#Win32 installation:

    1. Install WinPcap. (An installation file should be in the same directory as QtNetMon was installed, 'WinPcap_4_0.exe')
        If not, go to: http://www.winpcap.org/install/default.htm and download WinPcap 4.0.

    2. Double click on the binary file (QtNetworkMon.exe)

#Linux installation:

    1. Insall libpcap (sudo apt-get install libpcap)

    2. Run the binary file (sudo ./QtNetworkMon) from the program's directory.
    It HAS to be run in super-user mode.

#Version history:
    v 0.4: 2018
        qt5 build

    v 0.2: (14 May 2007)
        Now saves data in file. Can have statistics on data on a daily basic. Data is kept between
        application closes.

    v 0.1: (10 May 2007)
        Initial beta release. Displays how much bandwidth a given device uses.
