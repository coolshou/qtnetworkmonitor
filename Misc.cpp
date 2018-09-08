#include "Misc.h"

string int_to_string(int Iin)
{
    std::stringstream ss;
    std::string str;
    ss << Iin;
    ss >> str;

    return str;
}

string ip_to_String(int a,int b, int c, int d)
{
    string ip;
    ip.append(int_to_string(a));
    ip.append(".");
    ip.append(int_to_string(b));
    ip.append(".");
    ip.append(int_to_string(c));
    ip.append(".");
    ip.append(int_to_string(d));

    return ip;
}

string float_to_string(float Fin)
{
    stringstream buff;
    string out;

    buff << Fin;
    buff >> out;

    return out;
}

float string_to_float(string Sin)
{
    stringstream buff;
    float out;

    buff << Sin;
    buff >> out;

    return out;
}

string get_time()
{
    time_t rawtime;
    struct tm * timeinfo;

    time ( &rawtime );
    timeinfo = localtime ( &rawtime );

    string time_now( asctime (timeinfo) );
    return time_now;
}

bool is_today(string S_in)
{

    unsigned int size_chk = 10;

    if ( S_in.size() < size_chk )
    {
        cout<<"Invalid timestamp provided, returning 'not today'"<<endl;
        return 0;
    }

    bool same = true;

    string now = get_time();

    assert(S_in.size() >= size_chk);
    assert(now.size() >= size_chk);

    for (unsigned int x=0; x < size_chk; x++)
    {
        if (now[x] != S_in[x])
        {
            same = false;
        }
    }

    return same;
}
