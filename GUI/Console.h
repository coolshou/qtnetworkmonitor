#ifndef CONSOLE_H
#define CONSOLE_H

#include <vector>

#include <QTextEdit>
#include <QString>

using namespace std;

class QConsole : public QTextEdit
{
Q_OBJECT

    public:

        QConsole(QWidget *parent = 0);

        void Display_Messages(vector<string>);
        void Display_Message(string);


};
#endif
