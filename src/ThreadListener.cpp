#include "ThreadListener.h"

ThreadListener::ThreadListener(PcapHandler * PcapH_in)
{
    assert(PcapH_in!=NULL);
    PcapH = PcapH_in;
    setTerminationEnabled();
}

void ThreadListener::run()
{
    cout<<"Threaded Listener Started"<<endl;
    PcapH->StartListenOnDevice_countData();
    cout<<"Threaded Listener Stopped"<<endl;
}

void ThreadListener::instructStop()
{
    PcapH->instructStop();
}
