#ifndef STATUS_H
#define STATUS_H

#include <QFrame>
#include <QString>

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

private:
    Ui::Status *ui;
};

#endif // STATUS_H
