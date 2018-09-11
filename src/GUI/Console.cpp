#include "Console.h"

QConsole::QConsole(QWidget *parent)
: QTextEdit(parent)
{
     setReadOnly(true);
}

void QConsole::Display_Messages(vector<string> VM_in)
{
    for (unsigned int x=0; x<VM_in.size(); x++)
    {
        Display_Message(VM_in[x]);
    }
}

void QConsole::Display_Message(string Sin)
{
    append( QString( Sin.c_str() ) );
}
