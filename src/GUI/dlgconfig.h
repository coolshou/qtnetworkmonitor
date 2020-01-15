#ifndef DLGCONFIG_H
#define DLGCONFIG_H

#include <QDialog>
#include "../const.h"

namespace Ui {
class DlgConfig;
}

class DlgConfig : public QDialog
{
    Q_OBJECT

public:
    explicit DlgConfig(QWidget *parent = nullptr);
    ~DlgConfig();
    int get_UnitType();
    bool get_AutoStart();
    void set_autostart(bool start);
    void set_unittype(int unittype);

private:
    Ui::DlgConfig *ui;
};

#endif // DLGCONFIG_H
