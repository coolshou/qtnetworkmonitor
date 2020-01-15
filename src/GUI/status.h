#ifndef STATUS_H
#define STATUS_H

#include <QFrame>
#include <QString>

#include "chart.h"
#include <QtCharts/QChartView>
#include "Scope.h"

namespace Ui {
class Status;
}

class Status : public QFrame
{
    Q_OBJECT

public:
    explicit Status(const QString &title, QWidget *parent = nullptr);
    ~Status();
    void setTitle(QString title);
    void Set_Data(vector<float> data_in, int target);
    void addData(qreal x, qreal y);
    void setTotal(QString total);
    void setCurrent(QString current);

private:
    Ui::Status *ui;
    Scope      *dataScope;
    Chart *chart;
    QChartView *chartView;
};

#endif // STATUS_H
