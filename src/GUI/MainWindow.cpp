#include "MainWindow.h"
#if QT_VERSION >= 0x050000
#include <QStandardPaths>
#else
#include <QDesktopServices>
#endif
#include <QFile>
#include <QDir>
#include <QMessageBox>
#include <QDateTime>


MainWindow::MainWindow()
{
#if QT_VERSION >= 0x050000
    homePath = QStandardPaths::writableLocation(QStandardPaths::TempLocation);
#else
    homePath = QDesktopServices::storageLocation(QDesktopServices::TempLocation);
#endif
    QDir path(QDir::cleanPath(homePath + QDir::separator() +
                              get_username() + QDir::separator() +
                              qApp->applicationName()));
    StatsFile = path.filePath("Stats.dat");
    if (! QDir(path.absolutePath()).exists()){
        QDir().mkpath(path.absolutePath());
    }
    QFile file( StatsFile );
    if ( file.open(QIODevice::ReadWrite) )
    {
        QTextStream stream( &file );
        stream << endl;
        file.close();
    }
    if (! path.exists(StatsFile)) {
        qDebug() << "file not exist: " << StatsFile;
    }
    setting = new QSettings();
    LoadOptionsFromFile();

    //Members inits:
    LastAmountData_download = 0;
    LastAmountData_upload = 0;
    DataDownloadedSinceLastCall = 0;
    DataUploadedSinceLastCall = 0;
    Data_Timestamp = get_time();

    //GUI defaults TODO: DeviceNo come from setting
    if (deviceno < 1){
        Default_DeviceNo = 1;
    } else {
        Default_DeviceNo = deviceno;
    }

    //Load from file (erase the defaults if needed)
    LoadDataFromFile();

    setupGUI();

    //Start the device
    cout<<"Attempting to open device"<<endl;

    OpenDevice();

    cout<<"Attempting to start capture device"<<endl;

    StartCapture();
}

void  MainWindow::setupGUI()
{
    //-----------------Sets up the widgets---------------------------
    QString ufmt;
    if (unittype == UnitBYTES){
        ufmt = "%i MBps";
    }else{
        ufmt = "%i Mbps";
    }
    DownloadStatus = new Status("Received:", this);
    DownloadStatus->setAxisYLabelFormat(ufmt);
    UploadStatus = new Status("Sent:", this);
    UploadStatus->setAxisYLabelFormat(ufmt);

    {//Device choice menu
        DropListDeviceChoice = new QComboBox(this);
        DropListDeviceChoice->setMinimumSize(60,10);
        PushBDropListSetToCurrent = new QPushButton("Set",this);

        DropListDeviceGB = new QGroupBox(tr("Device to monitor:"));
        QHBoxLayout *layoutHDropListDevice = new QHBoxLayout;
        layoutHDropListDevice->addWidget(DropListDeviceChoice);
        layoutHDropListDevice->addWidget(PushBDropListSetToCurrent);
        layoutHDropListDevice->setStretch(0,1);
        DropListDeviceGB->setLayout(layoutHDropListDevice);
    }

    //Console
    Console = new QConsole(this);
    Console->hide();

    //Speed Scope
    dataScope = new Scope(this);

    {//Sets-up the layout
        //QVBoxLayout *mainLayout = new QVBoxLayout;
        QGridLayout *mainLayout = new QGridLayout;
        //mainLayout->addWidget(AboutWebsite, Qt::AlignBottom);

        mainLayout->addWidget(DropListDeviceGB);

        mainLayout->addWidget(DownloadStatus);
        mainLayout->addWidget(UploadStatus);

        //mainLayout->addWidget(DownloadUploadGB);
        mainLayout->addWidget(dataScope);

        mainLayout->addWidget(Console);

        setLayout(mainLayout);
    }
    //---------------------------------------------------------------

    //Set a clock to update the display
    QTimer *timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(update()));
    timer->start(250);

    //Keep track on the speed of the data received
    QTimer *timer_data = new QTimer(this);
    connect( timer_data, SIGNAL(timeout()), this, SLOT(updateKBPS()) );
    timer_data->start(TRACK_TIME);

    ThreadL = new ThreadListener(&PCHandler);

    SpeedHist_Download = vector<float>(256,0);
    SpeedHist_Upload = vector<float>(256,0);

    //Signals/Slots
    connect( PushBDropListSetToCurrent, SIGNAL( clicked() ), this, SLOT(ChangeDevice()) );
}

void MainWindow::About()
{
    cout<<"Opening a browser at the project's website."<<endl;

    QDesktopServices QDS;
    QUrl ProjectWebsite(WEBURL);
    QDS.openUrl( ProjectWebsite );
}

void MainWindow::toggleConsoleView()
{
    if ( Console->isVisible() )
        Console->hide();
    else
        Console->show();
}
void MainWindow::clearConsoleView()
{
    Console->clear();
}
void MainWindow::showAbout()
{
    QMessageBox msgBox(this);
    msgBox.setIcon(QMessageBox::Information);
    msgBox.setWindowTitle(tr("About"));
    msgBox.setTextFormat(Qt::RichText);   //this is what makes the links clickable
    msgBox.setText(QString("A Free, Open Source, Portable (Windows, Unix / Linux) Network Monitor. \n <a href=\"%1\">visit website</a>").arg(WEBURL));
    msgBox.setStandardButtons(QMessageBox::Ok);
    msgBox.exec();
}
int MainWindow::OpenDevice(int DeviceNO)
{
    vector<string> * Devices = new vector<string>;
    PCHandler.FindAvailDevices(Devices);

    qDebug() <<"Availables devices"<<endl;

    DropListDeviceChoice->clear();
    for(unsigned int x=0; x< Devices->size();x++)
    {
        QString temp( (*Devices)[x].c_str() );
        DropListDeviceChoice->addItem(temp);
    }

    int Device_to_Open;

    if (DeviceNO != -1)
        Device_to_Open = DeviceNO;
    else
        Device_to_Open = static_cast<int>(Default_DeviceNo);

    if ( Devices->size() < 1 )
    {
        qDebug()<<"No device available. (Linux-> Sudo?)"<<endl;
        return -1;
    }

    if ( (Device_to_Open < 1) || (Device_to_Open > static_cast<int>(Devices->size())) )
    {
        qDebug()<<"Invalid device number or default device, trying device #1."<<endl;
        Device_to_Open = 1;
    }

    PCHandler.openDevice(Device_to_Open);

    DropListDeviceChoice->setCurrentIndex(Device_to_Open-1);

    return 0;
}

int MainWindow::StartCapture()
{
    ThreadL->start();
    return 0;
}

void MainWindow::ChangeDevice()
{

    int idx = DropListDeviceChoice->currentIndex();
    int device_No = idx + 1;
    Default_DeviceNo = static_cast<unsigned int>(device_No);

    ThreadL->instructStop();
    if(!ThreadL->wait(2500))
    {
        Console->Display_Message("ERROR: Thread didn't stop properly, process had to be killed.");
        cout<<"Error stopping thread"<<endl;

        ThreadL->terminate();//Very dangerous
        ThreadL->wait(500);

        //Memory leak?
        PcapHandler freshPCH;
        PCHandler = freshPCH;
        delete ThreadL;
        ThreadL = new ThreadListener(&PCHandler);
    }
    DownloadStatus->resetData();
    UploadStatus->resetData();
    clearMemory();
    OpenDevice(device_No);
    StartCapture();
}

string MainWindow::getUnits(float nBits)
{
    if ( nBits < static_cast<float>(1000.0)){
        if (unittype == UnitBYTES){
            return "B";
        } else {
            return "b";
        }
    }
    if ( nBits < static_cast<float>(1000.0*1000.0)){
        if (unittype == UnitBYTES){
            return "KB";
        } else {
            return "Kb";
        }
    }
    if ( nBits < static_cast<float>(1000.0*1000.0*1000.0)){
        if (unittype == UnitBYTES){
            return "MB";
        } else {
            return "Mb";
        }
    }
    if ( nBits < static_cast<float>(1000.0*1000.0*1000.0*1000.0)){
        if (unittype == UnitBYTES){
            return "GB";
        } else {
            return "Gb";
        }
    } else {
        if (unittype == UnitBYTES){
            return "TB";
        } else {
            return "Tb";
        }
    }
    //PB, EB, ZB, YB
}

float MainWindow::getDivisor(float nBits)
{
    if ( nBits < static_cast<float>(1000.0))
        return 1.0f;
    if ( nBits < static_cast<float>(1000.0*1000.0))
        return 1000.0f;
    if ( nBits < static_cast<float>(1000.0*1000.0*1000.0))
        return 1000.0f*1000.0f;
    if ( nBits < static_cast<float>(1000.0*1000.0*1000.0*1000.0))
        return 1000.0f*1000.0f*1000.0f;
    else
        return 1000.0f*1000.0f*1000.0f*1000.0f;
}

void MainWindow::updateGUI()
{
    //Total data downloaded:
    QString DownloadKBTot_S;
    QString UploadKBTot_S;
    qint64 curTime = QDateTime::currentSecsSinceEpoch();

    float DataDownloadedSoFar = PCHandler.get_TotalDataDownloaded_bytes() + Download_offset;
    float DataUploadedSoFar = PCHandler.get_TotalDataUploaded_bytes() + Upload_offset;
    if (unittype == UnitBITS){
        DataDownloadedSoFar = DataDownloadedSoFar * 8;
        DataUploadedSoFar = DataUploadedSoFar * 8;
    }
    float divisor = getDivisor( DataDownloadedSoFar );
    QString DownloadKBTotal_S = DownloadKBTot_S.setNum( static_cast<int>((DataDownloadedSoFar / divisor) ));
    QString DownloadKBTotal_units_S = QString( getUnits(DataDownloadedSoFar).c_str());

    DownloadStatus->setTotal(DownloadKBTotal_S, DownloadKBTotal_units_S);

    divisor = getDivisor( DataUploadedSoFar );
    QString UploadKBTotal_S = UploadKBTot_S.setNum( static_cast<int>((DataUploadedSoFar / (divisor) ) ));
    QString UploadKBTotal_units_S = QString( getUnits( DataUploadedSoFar ).c_str());

    UploadStatus->setTotal(UploadKBTotal_S, UploadKBTotal_units_S);
    // #######################################################################
    //current speed kb/s:
    QString DownloadKBpS_S;
    QString UploadKBpS_S;
    float DataDownloadCurrent = DataDownloadedSinceLastCall;
    if (unittype == UnitBITS){
        DataDownloadCurrent = DataDownloadCurrent * 8;
    }
    divisor = getDivisor(DataDownloadCurrent);
    int DownloadKBp = static_cast<int>((DataDownloadCurrent/divisor));
    QString DownloadKBp_S = DownloadKBpS_S.setNum(DownloadKBp);
    QString DownloadKBpS_units_S = QString( getUnits(DataDownloadCurrent).c_str() + QString("ps"));
    int idx=0;
    if (unittype == UnitBITS){
        idx = get_BITS_IDX(DownloadKBpS_units_S);
    } else {
        idx = get_BYTES_IDX(DownloadKBpS_units_S);
    }
    if (idx > DownloadStatus->getUnitIdx()){
        DownloadStatus->setUnitIdx(idx);
        DownloadStatus->setCurrent(DownloadKBp_S, DownloadKBpS_units_S);
    }else{
        DownloadStatus->setCurrent(DownloadKBp_S);
    }


    float DataUploadCurrent = DataUploadedSinceLastCall;
    if (unittype == UnitBITS){
        DataUploadCurrent = DataUploadCurrent * 8;
    }
    divisor = getDivisor(DataUploadCurrent);
    int UploadKBp = static_cast<int>((DataUploadCurrent/divisor));
    QString UploadKBp_S = UploadKBpS_S.setNum(UploadKBp);
    QString UploadKBpS_units_S = QString( getUnits(DataUploadCurrent).c_str() + QString("ps"));
    if (unittype == UnitBITS){
        idx = get_BITS_IDX(UploadKBpS_units_S);
    } else {
        idx = get_BYTES_IDX(UploadKBpS_units_S);
    }
    if (idx > UploadStatus->getUnitIdx()){
        UploadStatus->setUnitIdx(idx);
        UploadStatus->setCurrent(UploadKBp_S, UploadKBpS_units_S);
    }else{
        UploadStatus->setCurrent(UploadKBp_S);
    }

    //update chart data (unit should be MB/Mb? for chart to show)
    //DownloadStatus->addData(static_cast<qreal>(curTime), DownloadKBp);
    //UploadStatus->addData(static_cast<qreal>(curTime), UploadKBp);
    DownloadStatus->addData(static_cast<qreal>(curTime), static_cast<int>(DataDownloadCurrent/(1000.0f*1000.0f)));
    UploadStatus->addData(static_cast<qreal>(curTime), static_cast<int>(DataUploadCurrent/(1000.0f*1000.0f)));
    //Update the messages
    Console->Display_Messages( PCHandler.get_messages() );
}

void MainWindow::updateKBPS()
{
    //Refrest timestamp, fush data if needed (new day)
    if (0){
        //cause not monitor on new day!!
    if ( !is_today(Data_Timestamp) )
        {//New day, flush data
            cout<<"A new day is coming, the birds are chirping."<<endl;
            clearMemory();
        }
    }
    Data_Timestamp = get_time();

    DataDownloadedSinceLastCall =
    (PCHandler.get_TotalDataDownloaded_bytes() + Download_offset)-LastAmountData_download;
    LastAmountData_download = (PCHandler.get_TotalDataDownloaded_bytes() + Download_offset);

    DataUploadedSinceLastCall =
    (PCHandler.get_TotalDataUploaded_bytes() + Upload_offset)-LastAmountData_upload;
    LastAmountData_upload = (PCHandler.get_TotalDataUploaded_bytes() + Upload_offset);

    if ( DataDownloadedSinceLastCall < static_cast<float>(0.0) )
        DataDownloadedSinceLastCall = 0.0;

    if ( DataUploadedSinceLastCall < static_cast<float>(0.0) )
        DataUploadedSinceLastCall = 0.0;

    //Push one in, remove one
    SpeedHist_Download.push_back( DataDownloadedSinceLastCall );
    SpeedHist_Download.erase( SpeedHist_Download.begin(), SpeedHist_Download.begin()+1);

    SpeedHist_Upload.push_back( DataUploadedSinceLastCall );
    SpeedHist_Upload.erase( SpeedHist_Upload.begin(), SpeedHist_Upload.begin()+1);

    dataScope->Set_Data(SpeedHist_Download, 0);
    dataScope->Set_Data(SpeedHist_Upload, 1);

    SaveDataToFile();
    updateGUI();
}

void MainWindow::clearMemory()
{
    for (unsigned int x=0; x< SpeedHist_Download.size(); x++)
    {
        SpeedHist_Download[x] = 0.0;
    }
    for (unsigned int x=0; x< SpeedHist_Upload.size(); x++)
    {
        SpeedHist_Upload[x] = 0.0;
    }

    LastAmountData_download=0.0;
    LastAmountData_upload=0.0;
    DataDownloadedSinceLastCall=0.0;
    DataUploadedSinceLastCall=0.0;
    Download_offset=0.0;
    Upload_offset=0.0;

    PCHandler.resetMemory();
}

void MainWindow::LoadDataFromFile()
{
    //Set the data offset
    InfoReadWrite Reader;

    Reader.Read(StatsFile.toUtf8().constData());

    if( Reader.getData().size() >= 2 )
        {

            Default_DeviceNo = static_cast<unsigned int>(Reader.getData()[0]);//1212 Haha, not right!

            //Checks the timestamp of the data
            if ( is_today( Reader.get_timestamp() ) )
            {
                Download_offset = Reader.getData()[1];
                Upload_offset   = Reader.getData()[2];
                LastAmountData_download = Download_offset;
                LastAmountData_upload = Upload_offset;
            }
            else
            {
                cout<<"Stats.dat's Timestamp is out of date, ignoring data."<<endl;
            }
        }
        else
        {
            Download_offset=0.0f;
            Upload_offset=0.0f;
            cout<<"There was a problem reading the stats file."<<endl;
        }
}

void MainWindow::SaveDataToFile()
{
    //Save the data to a file
    InfoReadWrite Writer;

    vector<float> data_out;
    data_out.push_back( static_cast<float>(Default_DeviceNo) );
    data_out.push_back( PCHandler.get_TotalDataDownloaded_bytes() + Download_offset);
    data_out.push_back( PCHandler.get_TotalDataUploaded_bytes() + Upload_offset);

    Writer.setData(data_out);
    Writer.Write(StatsFile.toUtf8().constData());
}

void MainWindow::LoadOptionsFromFile()
{
    //setting->beginGroup("MainWindow");
    //this->setGeometry(setting->value("geometry", QRect(0,0, 520, 500)).toRect());
    //setting->endGroup();
    setting->beginGroup("Main");
    setUnitType(setting->value("unittype", 0).toInt());
    setAutoStart(setting->value("autostart", false).toBool());
    setDeviceNo(setting->value("DeviceNo", 1).toUInt());
    setting->endGroup();
}

void MainWindow::SaveOptionsToFile()
{
    //setting->beginGroup("MainWindow");
    //setting->setValue("geometry", this->window()->geometry());
    //setting->endGroup();
    setting->beginGroup("Main");
    setting->setValue("unittype", getUnitType());
    setting->setValue("autostart", getAutoStart());
    setting->setValue("DeviceNo", getDeviceNo());
    setting->endGroup();
    setting->sync();

}

void MainWindow::setAutoStart(bool start)
{
    autostart = start;
}
//TODO: create/remove autostart to system

void MainWindow::setUnitType(int utype)
{
    unittype = utype;
}

void MainWindow::setDeviceNo(unsigned int devno)
{
    deviceno = devno;
}

bool MainWindow::getAutoStart()
{
    return autostart;
}

int MainWindow::getUnitType()
{
    return unittype;
}

unsigned int MainWindow::getDeviceNo()
{
    return deviceno;
}

