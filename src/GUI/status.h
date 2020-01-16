#ifndef STATUS_H
#define STATUS_H

#include <QFrame>
#include <QString>
#include <QMenu>

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
    void addData(qreal x, qreal y);
    void setTotal(QString total);
    void setTotal(QString total, QString sUnit);
    void setCurrent(QString current);
    void setCurrent(QString current, QString sUnit);
    void setStartTime(qint64 stime);
    void setUnit(QString sUnit);
    void setTotalUnit(QString sUnit);
    int getUnitIdx();
    void setUnitIdx(int idx);
    void setAxisYLabelFormat(QString t="%i");
    void resetData();

private slots:
    void onCustomContextMenuRequested(const QPoint &pos);
    void onTriggered(bool checked);

private:
    Ui::Status *ui;
    Chart *chart;
    QChartView *chartView;
    qint64 startTime;
    QMenu *context;
    QAction *actShowX;
    //
    qreal max;
    QString unit;
    int unitIdx;
    QString totalunit;
};

#endif // STATUS_H
