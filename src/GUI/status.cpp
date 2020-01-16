#include "status.h"
#include "ui_status.h"
#include <QDateTime>
#include <QString>

#include <QDebug>


Status::Status(const QString &title, QWidget *parent) :
    QFrame(parent),
    ui(new Ui::Status)
{
    ui->setupUi(this);
    setTitle(title);
    setStartTime(QDateTime::currentSecsSinceEpoch());
    QColor color;
    if (title == "Received:"){
        color = Qt::red;
    } else{
        color = Qt::blue;
    }
    chart = new Chart(color);
    chart->legend()->hide();
    chart->setAnimationOptions(QChart::SeriesAnimations);

    chartView= new QChartView(chart);
    chartView->setRenderHint(QPainter::Antialiasing);
    chartView->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(chartView, SIGNAL(customContextMenuRequested(const QPoint &)),
            SLOT(onCustomContextMenuRequested(const QPoint &)));
    //ui->chartview->setChart(chart);

    ui->vlLayout->addWidget(chartView, Qt::AlignCenter);
    max = 0;
    unitIdx = 0;
    // menu
    context= new QMenu(this);
    actShowX = new QAction(this);
    actShowX->setText("Show X axis");
    connect(actShowX,SIGNAL(triggered(bool)),SLOT(onTriggered(bool)));
    context->addAction(actShowX);
    //context->addAction("Delete");

}

Status::~Status()
{
    delete ui;
}

void Status::onCustomContextMenuRequested(const QPoint &pos)
{
    context->exec(chartView->viewport()->mapToGlobal(pos));
}
void Status::onTriggered(bool checked)
{
    Q_UNUSED(checked);
    //show/hide X asix
    bool b = chart->getAxisXVisiable();
    chart->showAxisX(!b);
    if (b){
        actShowX->setText("Show X axis");
    }else{
        actShowX->setText("Hide X axis");
    }
}
void Status::setTitle(QString title)
{
    ui->groupBox->setTitle(title);
}

void Status::addData(qreal x, qreal y)
{
    if (y > max){
        max = y;
        ui->leMax->setText(QString::number(max) + " " + unit);
    }
    chart->addData(x - startTime, y);
}

void Status::setTotal(QString total)
{
    ui->leTotal->setText(total);
}

void Status::setTotal(QString total, QString sUnit)
{
     setTotal(total + " " + sUnit);
     setTotalUnit(sUnit);
}

void Status::setCurrent(QString current)
{
    ui->leCurrent->setText(current + " " + unit);
}

void Status::setCurrent(QString current, QString sUnit)
{
    setCurrent(current + " " + sUnit);
    setUnit(sUnit);
}

void Status::setStartTime(qint64 stime)
{
    startTime = stime;
}

void Status::setUnit(QString sUnit)
{
    unit = sUnit;
}

void Status::setTotalUnit(QString sUnit)
{
    totalunit = sUnit;
}

int Status::getUnitIdx()
{
    return unitIdx;
}

void Status::setUnitIdx(int idx)
{
    unitIdx = idx;
}

void Status::setAxisYLabelFormat(QString t)
{
    chart->setAxisYLabelFormat(t);
}

void Status::resetData()
{
    max = 0;
    chart->resetData();
    setStartTime(QDateTime::currentSecsSinceEpoch());
}
