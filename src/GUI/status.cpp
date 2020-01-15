#include "status.h"
#include "ui_status.h"
#include <QDateTime>

Status::Status(const QString &title, QWidget *parent) :
    QFrame(parent),
    ui(new Ui::Status)
{
    ui->setupUi(this);
    setTitle(title);
    //Speed Scope
    //dataScope = new Scope(this);
    //ui->hlayoutScope->addWidget(dataScope, Qt::AlignCenter);
    setStartTime(QDateTime::currentSecsSinceEpoch());

    chart = new Chart;
    //chart->setTitle(title);

    chart->legend()->hide();
    chart->setAnimationOptions(QChart::SeriesAnimations);

    chartView= new QChartView(chart);
    chartView->setRenderHint(QPainter::Antialiasing);

    ui->vlLayout->addWidget(chartView, Qt::AlignCenter);
    //chartView.show();
    max = 0;
}

Status::~Status()
{
    delete ui;
}

void Status::setTitle(QString title)
{
    ui->groupBox->setTitle(title);
}

void Status::Set_Data(vector<float> data_in, int target)
{
    //dataScope->Set_Data(data_in, target);
}

void Status::addData(qreal x, qreal y)
{
    if (y > max){
        max = y;
        ui->leMax->setText(QString(max));
    }
    chart->addData(x - startTime, y);
}

void Status::setTotal(QString total)
{
    ui->leTotal->setText(total);
}

void Status::setCurrent(QString current)
{
    ui->leCurrent->setText(current);
}

void Status::setStartTime(qint64 stime)
{
    startTime = stime;
}
