#include "status.h"
#include "ui_status.h"

Status::Status(const QString &title, QWidget *parent) :
    QFrame(parent),
    ui(new Ui::Status)
{
    ui->setupUi(this);
    setTitle(title);
}

Status::~Status()
{
    delete ui;
}

void Status::setTitle(QString title)
{
    ui->groupBox->setTitle(title);
}
