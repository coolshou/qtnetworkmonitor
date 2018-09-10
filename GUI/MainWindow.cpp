#include "MainWindow.h"
#if QT_VERSION >= 0x050000
#include <QStandardPaths>
#else
#include <QDesktopServices>
#endif
#include <QFile>
#include <QDir>
#include <QMessageBox>


MainWindow::MainWindow()
{
#if QT_VERSION >= 0x050000
    homePath = QStandardPaths::writableLocation(QStandardPaths::TempLocation);
#else
    homePath = QDesktopServices::storageLocation(QDesktopServices::TempLocation);
#endif
    QDir path(QDir::cleanPath(homePath + QDir::separator() + qApp->applicationName()));
    StatsFile = path.filePath("Stats.dat");
    if (! QDir(path.absolutePath()).exists()){
        QDir().mkdir(path.absolutePath());
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
    //Members inits:
    LastAmountData_download = 0;
    LastAmountData_upload = 0;
    DataDownloadedSinceLastCall = 0;
    DataUploadedSinceLastCall = 0;
    Data_Timestamp = get_time();

    //GUI defaults
    Default_DeviceNo = 1;

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
    {//Up/Down Stats
        DownloadKBpS = new QLabel("0",this);
        UploadKBpS = new QLabel("0",this);
        DownloadKBTotal = new QLabel("0",this);
        UploadKBTotal = new QLabel("0",this);

        DownloadKBpS_label = new QLabel("Down.:",this);
        UploadKBpS_label = new QLabel("Up.:",this);
        DownloadKBTotal_label = new QLabel("Down. total:",this);
        UploadKBTotal_label = new QLabel("Up. total:",this);

        DownloadKBpS_units = new QLabel("B/s",this);
        UploadKBpS_units = new QLabel("B/s",this);
        DownloadKBTotal_units = new QLabel("B/s",this);
        UploadKBTotal_units = new QLabel("B/s",this);

        DownloadUploadGB = new QGroupBox(tr("Download/Upload Stats"));
        QHBoxLayout *layoutH1 = new QHBoxLayout;

        layoutH1->addWidget(DownloadKBpS_label);
        layoutH1->addWidget(DownloadKBpS);
        layoutH1->addWidget(DownloadKBpS_units,Qt::AlignLeft);
        layoutH1->addWidget(UploadKBpS_label);
        layoutH1->addWidget(UploadKBpS);
        layoutH1->addWidget(UploadKBpS_units,Qt::AlignLeft);
        layoutH1->addWidget(DownloadKBTotal_label);
        layoutH1->addWidget(DownloadKBTotal);
        layoutH1->addWidget(DownloadKBTotal_units,Qt::AlignLeft);
        layoutH1->addWidget(UploadKBTotal_label);
        layoutH1->addWidget(UploadKBTotal);
        layoutH1->addWidget(UploadKBTotal_units,Qt::AlignLeft);

        DownloadUploadGB->setLayout(layoutH1);
    }

    {//Device choice menu
        DropListDeviceChoice = new QComboBox(this);
        DropListDeviceChoice->setMinimumSize(60,10);
        PushBDropListSetToCurrent = new QPushButton("Set",this);

        DropListDeviceGB = new QGroupBox(tr("Device to monitor:"));
        QHBoxLayout *layoutHDropListDevice = new QHBoxLayout;
        layoutHDropListDevice->addWidget(DropListDeviceChoice);
        layoutHDropListDevice->addWidget(PushBDropListSetToCurrent);
        DropListDeviceGB->setLayout(layoutHDropListDevice);
    }

    //Console
    Console = new QConsole(this);

    //Speed Scope
    dataScope = new Scope(this);

    //About
    /*
    AboutWebsite = new QPushButton("Click here to visit the project's website");
    AboutWebsite->setMaximumSize(QSize(600, 18));
    AboutWebsite->setFlat(true);

        //Color Palette
        QPalette QBluePalette;
        QBrush BlueBrush(QColor(0, 59, 255, 255));
        BlueBrush.setStyle(Qt::SolidPattern);
        QBluePalette.setBrush(QPalette::Active, QPalette::ButtonText, BlueBrush);

    AboutWebsite->setPalette(QBluePalette);

    connect( AboutWebsite, SIGNAL( clicked() ), this, SLOT(About()) );
    */
    {//Sets-up the layout
        QVBoxLayout *mainLayout = new QVBoxLayout;

        //mainLayout->addWidget(AboutWebsite, Qt::AlignBottom);

        mainLayout->addWidget(DropListDeviceGB);
        mainLayout->addWidget(DownloadUploadGB);

        mainLayout->addWidget(dataScope, Qt::AlignCenter);

        mainLayout->addWidget(Console, Qt::AlignBottom);

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
    timer_data->start(1000);

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
    //QUrl ProjectWebsite(QString("https://github.com/coolshou/qtnetworkmonitor.git"));
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
    QMessageBox msgBox;
    msgBox.setWindowTitle(tr("About"));
    msgBox.setText(QString("A Free, Open Source, Portable (Windows, Unix / Linux) Network Monitor. \n<a href='%1'>visit website</a>").arg(WEBURL));
    msgBox.setStandardButtons(QMessageBox::Ok);
    msgBox.exec();
}
int MainWindow::OpenDevice(int DeviceNO)
{
    vector<string> * Devices = new vector<string>;
    PCHandler.FindAvailDevices(Devices);

    cout<<"Availables devices"<<endl;

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
        Device_to_Open = Default_DeviceNo;

    if ( Devices->size() < 1 )
    {
        cout<<"No device available. (Linux-> Sudo?)"<<endl;
        return -1;
    }

    if ( (Device_to_Open < 1) || (Device_to_Open > (int)Devices->size()) )
    {
        cout<<"Invalid device number or default device, trying device #1."<<endl;
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

    int device_No = DropListDeviceChoice->currentIndex();
    Default_DeviceNo = device_No+1;

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

    clearMemory();
    OpenDevice(device_No+1);
    StartCapture();
}

string MainWindow::getUnits(float nBits)
{
    if ( nBits < 1000.0 )
        return "B";
    if ( nBits < 1000.0*1000.0 )
        return "kB";
    if ( nBits < 1000.0*1000.0*1000.0 )
        return "Mb";
    else
        return "Gb";
}

float MainWindow::getDivisor(float nBits)
{
    if ( nBits < 1000.0 )
        return 1.0f;
    if ( nBits < 1000.0*1000.0 )
        return 1000.0f;
    if ( nBits < 1000.0*1000.0*1000.0 )
        return 1000.0f*1000.0f;
    else
        return 1000.0f*1000.0f*1000.0f;
}

void MainWindow::updateGUI()
{
    //Total data downloaded:
    QString DownloadKBTot_S;
    QString UploadKBTot_S;

    float DataDownloadedSoFar = PCHandler.get_TotalDataDownloaded_bytes() + Download_offset;
    float DataUploadedSoFar = PCHandler.get_TotalDataUploaded_bytes() + Upload_offset;

    float divisor = getDivisor( DataDownloadedSoFar );
    DownloadKBTotal->setText( DownloadKBTot_S.setNum( (int) (DataDownloadedSoFar / divisor) ) );
    QString DownloadKBTotal_units_S = QString( getUnits(DataDownloadedSoFar).c_str());
    DownloadKBTotal_units->setText(DownloadKBTotal_units_S);

    divisor = getDivisor( DataUploadedSoFar );
    UploadKBTotal->setText( UploadKBTot_S.setNum( (int) (DataUploadedSoFar / (divisor) ) ) );
    QString UploadKBTotal_units_S = QString( getUnits( DataUploadedSoFar ).c_str());
    UploadKBTotal_units->setText(UploadKBTotal_units_S);

    //kb/s:
    QString DownloadKBpS_S;
    QString UploadKBpS_S;

    divisor = getDivisor( DataDownloadedSinceLastCall );
    DownloadKBpS->setText( DownloadKBpS_S.setNum( (int) (DataDownloadedSinceLastCall/divisor) ));
    QString DownloadKBpS_units_S = QString( getUnits(DataDownloadedSinceLastCall).c_str() + QString("/s")  );
    DownloadKBpS_units->setText(DownloadKBpS_units_S);

    divisor = getDivisor( DataUploadedSinceLastCall );
    UploadKBpS->setText( UploadKBpS_S.setNum((int) (DataUploadedSinceLastCall/divisor) ));
    QString UploadKBpS_units_S = QString( getUnits(DataUploadedSinceLastCall).c_str() + QString("/s") );
    UploadKBpS_units->setText(UploadKBpS_units_S);

    //Update the messages
    Console->Display_Messages( PCHandler.get_messages() );
}

void MainWindow::updateKBPS()
{
    //Refrest timestamp, fush data if needed (new day)
    if ( !is_today(Data_Timestamp) )
        {//New day, flush data
            cout<<"A new day is coming, the birds are chirping."<<endl;
            clearMemory();
        }

    Data_Timestamp = get_time();

    DataDownloadedSinceLastCall =
    (PCHandler.get_TotalDataDownloaded_bytes() + Download_offset)-LastAmountData_download;
    LastAmountData_download = (PCHandler.get_TotalDataDownloaded_bytes() + Download_offset);

    DataUploadedSinceLastCall =
    (PCHandler.get_TotalDataUploaded_bytes() + Upload_offset)-LastAmountData_upload;
    LastAmountData_upload = (PCHandler.get_TotalDataUploaded_bytes() + Upload_offset);

    if ( DataDownloadedSinceLastCall < 0.0 )
        DataDownloadedSinceLastCall = 0.0;

    if ( DataUploadedSinceLastCall < 0.0 )
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

            Default_DeviceNo = (unsigned int)Reader.getData()[0];//1212 Haha, not right!

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
    data_out.push_back( (float) Default_DeviceNo );
    data_out.push_back( PCHandler.get_TotalDataDownloaded_bytes() + Download_offset);
    data_out.push_back( PCHandler.get_TotalDataUploaded_bytes() + Upload_offset);

    Writer.setData(data_out);
    Writer.Write(StatsFile.toUtf8().constData());
}

void LoadOptionsFromFile(){cout<<"Not implemented yet."<<endl;}

void SaveOptionsToFile(){cout<<"Not implemented yet."<<endl;}

