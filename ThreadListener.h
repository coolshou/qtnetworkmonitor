#ifndef THREADLISTENER_H
#define THREADLISTENER_H

#include <iostream>
#include <assert.h>

#include <QThread>

#include "WinPcapFns.h"

using namespace std;

/*!
This class is used to encapsulate a PcapHandler in a thread.
*/
class ThreadListener : public QThread
{
    Q_OBJECT
    public:
        ThreadListener(PcapHandler * PcapH_in);
        void instructStop();

    protected:
        void run();

    private:
        PcapHandler * PcapH;
};



#endif
