Qt Network Monitor : A Free, Open Source, Portable (Windows, Unix / Linux) Network Monitor.

Go to: http://reachme.web.googlepages.com/qtnetworkmonitor
for the latest version.

Win32 installation:

    1. Install WinPcap. (An installation file should be in the same directory as QtNetMon was installed, 'WinPcap_4_0.exe')
        If not, go to: http://www.winpcap.org/install/default.htm and download WinPcap 4.0.

    2. Double click on the binary file (QtNetworkMonitor.exe)

Linux installation:

    1. Insall libpcap (sudo apt-get install libpcap)

    2. Run the binary file (sudo ./QtNetworkMonitor) from the program's directory.
    It HAS to be run in super-user mode.

Version history:

    v 0.2: (14 May 2007)
        Now saves data in file. Can have statistics on data on a daily basic. Data is kept between
        application closes.

    v 0.1: (10 May 2007)
        Initial beta release. Displays how much bandwidth a given device uses.