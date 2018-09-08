#include "WinPcapFns.h"

PcapHandler::PcapHandler()
{
    TotalDataDownloaded_bytes = 0.0;
    TotalDataUploaded_bytes = 0.0;
    Ndevices = 0;
    DeviceReady = 0;
    stopInstructed = false;
}

void PcapHandler::Print_messages()
{
    for (unsigned int x=0; x<messages.size(); x++)
    {
        cout<<messages[x]<<endl;
    }
}

vector<string> PcapHandler::get_messages()
{
    vector<string> out_messages = messages;
    messages.clear();
    return out_messages;
}

int PcapHandler::FindAvailDevices(vector<string> * outDevices)
{
    if (outDevices!= NULL)
        outDevices->clear();

	pcap_if_t *d;// Iterator
	char errbuf[PCAP_ERRBUF_SIZE];

	/* Retrieve the device list */
	if(pcap_findalldevs(&alldevs, errbuf) == -1)
	{
	    //Has to be changed to handle the error message in errbuf
		messages.push_back("Error: (in pcap_findalldevs...)");
		pcap_freealldevs(alldevs);
		return(-1);
	}

	/* Print the list */
	int i=0;
	for(d=alldevs; d; d=d->next)
	{
	    if (i==0)
            messages.push_back("Found available devices:");

        string Dname(d->name);
        string Ddesc;

		if (d->description)
		{
            Ddesc = d->description;
		}
		else
		{
            Ddesc = "No description available";
		}

		i++;
		Ndevices = i;

        outDevices->push_back(int_to_string(i) + ": " + Dname + " : " + Ddesc);
		messages.push_back(int_to_string(i) + ": " + Dname + " : " + Ddesc);
	}

	if(i==0)
	{
        messages.push_back("Error: no interfaces found! Make sure WinPcap(Windows) or pcap(Unix) is installed.");
        messages.push_back("If the application is running under Linux, make sure to run it as root. (sudo appname).");
        pcap_freealldevs(alldevs);
		return(-1);
	}

	return 0;
}

int PcapHandler::openDevice(int inum)
{
    pcap_if_t *d; //Iterator
    char errbuf[PCAP_ERRBUF_SIZE];

    if(inum < 1 || inum > Ndevices)
    {
        messages.push_back("Interface number out of range.");

        /* Free the device list */
        DeviceReady = -1;
        pcap_freealldevs(alldevs);
        return -1;
    }

    getDeviceIP(inum);

    /* Jump to the selected adapter */
    int i=0;
    for(d=alldevs, i=0; i< inum-1 ;d=d->next, i++);

    /* Open the device */
    /* Open the adapter */
    if ((adhandle= pcap_open_live(d->name,	// name of the device
                             65536,			// portion of the packet to capture.
                                            // 65536 grants that the whole packet will be captured on all the MACs.
                             1,				// promiscuous mode (nonzero means promiscuous)
                             1000,			// read timeout
                             errbuf			// error buffer
                             )) == NULL)
    {
        string device_name(d->name);
        messages.push_back("Unable to open the adapter, device '" + device_name + "' is not supported by WinPcap");

        /* Free the device list */
        pcap_freealldevs(alldevs);
        return -1;
    }

    /* At this point, we don't need any more the device list. Free it */
    pcap_freealldevs(alldevs);

    DeviceReady = 1;
    return 0;
}

int PcapHandler::CheckIfCurrDeviceReady()
{
    if (DeviceReady==1)
        return 1;
    else
    {
        messages.push_back("Device is not ready to use.");
        return 0;
    }
}

/* From tcptraceroute, convert a numeric IP address to a string */
#define IPTOSBUFFERS	12
char *iptos(u_long in)
{
	static char output[IPTOSBUFFERS][3*4+3+1];
	static short which;
	u_char *p;

	p = (u_char *)&in;
	which = (which + 1 == IPTOSBUFFERS ? 0 : which + 1);
	sprintf(output[which], "%d.%d.%d.%d", p[0], p[1], p[2], p[3]);
	return output[which];
}

string PcapHandler::getDeviceIP(int DeviceNo)
{
    pcap_if_t *Adev;
    int it=0;
    assert(DeviceNo>0);

    for(Adev=alldevs;Adev;Adev=Adev->next)
    {
        if (it==DeviceNo-1)
            break;

        it++;
    }

    pcap_addr_t *a;

    string IPa ="";
    /* IP addresses */
    for(a=Adev->addresses;a;a=a->next) {
        switch(a->addr->sa_family)
        {
          case AF_INET:
            if (a->addr)
            {
              IPa = iptos(((struct sockaddr_in *)a->addr)->sin_addr.s_addr);
              messages.push_back("Device's IP address successfully grabbed, IP = "+IPa);
            break;
            }

          case AF_INET6:
            //messages.push_back("Warning: IPV6 detected, this program doesn't support this mode yet.");
            //IPa = "ERROR (IPV6)";
            break;

          default:
            //messages.push_back("ERROR: Can't get the device's IP");
            //IPa = "ERROR (UNKNOWN)";
            break;
        }
    }

    if (IPa=="")
        IPa = "ERROR";

    IPadd = IPa;
    return IPa;
}

int PcapHandler::StartListenOnDevice_countData()
{
    if (!CheckIfCurrDeviceReady())
        return CheckIfCurrDeviceReady();

    int res=0;
    struct pcap_pkthdr *header;
    const u_char *pkt_data;

    //To retreive the source and dest. ips
    ip_header *ih;

    while(((res = pcap_next_ex( adhandle, &header, &pkt_data)) >= 0) && !stopInstructed ){

        if(res == 0)
            /* Timeout elapsed */
            continue;

        /* retireve the position of the ip header */
        ih = (ip_header *) (pkt_data + 14);

        /* print ip addresses and udp ports */

        string source_IP = ip_to_String (   ih->saddr.byte1,
                                            ih->saddr.byte2,
                                            ih->saddr.byte3,
                                            ih->saddr.byte4
                                        );
        string dest_IP = ip_to_String   (   ih->daddr.byte1,
                                            ih->daddr.byte2,
                                            ih->daddr.byte3,
                                            ih->daddr.byte4
                                        );

        if (dest_IP == IPadd)
            set_TotalDataDownloaded_bytes (get_TotalDataDownloaded_bytes() + header->len);

        if (source_IP == IPadd)
            set_TotalDataUploaded_bytes (get_TotalDataUploaded_bytes() + header->len);
    }

    stopInstructed = false;

    if(res == -1)
    {
        string error_rep(pcap_geterr(adhandle));
        messages.push_back("Error reading the packets:" + error_rep);
        return -1;
    }

    return 0;
}

float PcapHandler::get_TotalDataDownloaded_bytes()
{
    return TotalDataDownloaded_bytes;
}

float PcapHandler::get_TotalDataUploaded_bytes()
{
    return TotalDataUploaded_bytes;
}

void PcapHandler::set_TotalDataDownloaded_bytes(float TDTb_in)
{
    TotalDataDownloaded_bytes = TDTb_in;
}

void PcapHandler::set_TotalDataUploaded_bytes(float TDTb_in)
{
    TotalDataUploaded_bytes = TDTb_in;
}

void PcapHandler::instructStop()
{
    messages.push_back("Asking the device thread to stop");
    stopInstructed = true;
}

void PcapHandler::resetMemory()
{
    TotalDataDownloaded_bytes = 0.0f;
    TotalDataUploaded_bytes = 0.0f;
}
