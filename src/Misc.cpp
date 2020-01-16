#include "Misc.h"
#include <QtGlobal>
#include <QtDebug>

#ifdef Q_OS_LINUX
#include <unistd.h>
#include <sys/types.h>
#include <pwd.h>
#endif

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

QString get_username()
{
#ifdef Q_OS_WIN
    char acUserName[MAX_USERNAME];
    DWORD nUserName = sizeof(acUserName);
    if (GetUserName(acUserName, &nUserName))
        //qDebug() << acUserName;
        return QString::fromLocal8Bit(acUserName);
    return "";
#endif
#ifdef Q_OS_LINUX
    uid_t uid = geteuid();
    struct passwd *pw = getpwuid(uid);
    if (pw)
    {
        //qDebug() << "user:" << QString::fromLocal8Bit(pw->pw_name);
        return QString::fromLocal8Bit(pw->pw_name);
    }
    return "";
#endif
}

bool is_root()
{
#ifdef Q_OS_WIN
    //TODO: windows administrator
    return true
#endif
#ifdef Q_OS_LINUX
    uid_t uid = geteuid();
    if (uid) {
        qDebug() << "You are not root!" << "(" << uid << ")";
        return false;
    } else {
        return true;
    }
#endif
}

int get_BYTES_IDX(QString s)
{
    return slBYTES.indexOf(s);
}
int get_BITS_IDX(QString s)
{
    return slBITS.indexOf(s);
}
