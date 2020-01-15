#include "status.h"
#include "ui_status.h"

Status::Status(const QString &title, QWidget *parent) :
    QFrame(parent),
    ui(new Ui::Status)
{
    ui->setupUi(this);
    setTitle(title);
    //Speed Scope
    //dataScope = new Scope(this);
    //ui->hlayoutScope->addWidget(dataScope, Qt::AlignCenter);

    chart = new Chart;
    //chart->setTitle(title);

    chart->legend()->hide();
    chart->setAnimationOptions(QChart::AllAnimations);
    chartView= new QChartView(chart);
    chartView->setRenderHint(QPainter::Antialiasing);

    ui->vlLayout->addWidget(chartView, Qt::AlignCenter);
    //chartView.show();
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
    dataScope->Set_Data(data_in, target);
}

void Status::addData(qreal x, qreal y)
{
    chart->addData(x, y);
}

void Status::setTotal(QString total)
{
    ui->leTotal->setText(total);
}

void Status::setCurrent(QString current)
{
    ui->leCurrent->setText(current);
}
