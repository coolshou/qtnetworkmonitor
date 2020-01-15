#include "dlgconfig.h"
#include "ui_dlgconfig.h"

DlgConfig::DlgConfig(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DlgConfig)
{
    ui->setupUi(this);
}

DlgConfig::~DlgConfig()
{
    delete ui;
}

int DlgConfig::get_UnitType()
{
    if (ui->rbByte->isChecked())
        return UnitBYTES;
    else if(ui->rbBits->isChecked())
        return UnitBITS;
    else
        return UnitBITS;
}

bool DlgConfig::get_AutoStart()
{
    return ui->cbAutoStart->isChecked();
}

void DlgConfig::set_autostart(bool start)
{
    ui->cbAutoStart->setChecked(start);
}

void DlgConfig::set_unittype(int unittype)
{
    if (unittype == UnitBYTES)
        ui->rbByte->setChecked(true);
    if (unittype == UnitBITS)
        ui->rbBits->setChecked(true);
}
