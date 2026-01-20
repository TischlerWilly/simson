#include "prgpfade.h"

prgpfade::prgpfade()
{
    Inifile             = "einstellungen.ini";
    Wkz                 = "wkz_magazin.csv";
}

void prgpfade::ordner_erstellen()
{
    QDir dir_prg(path_prg());
    if(!dir_prg.exists())
    {
        dir_prg.mkdir(path_prg());
    }
    QDir dir_user(path_user());
    if(!dir_user.exists())
    {
        dir_user.mkdir(path_user());
    }
}
void prgpfade::ordner_erstellen_wkz(QString maschine)
{
    QString path;
    path = path_wkz_dir(maschine);

    QDir dir_prg(path);
    if(!dir_prg.exists())
    {
        dir_prg.mkdir(path);
    }
}
//--------------------------------------
QString prgpfade::name_inifile()
{
    return Inifile;
}

//--------------------------------------Programmordner:
QString prgpfade::path_prg()
{
    QString tmp;

#ifdef _WIN32
    //tmp = "C:\\Program Files\\simson";
    tmp = QCoreApplication::applicationDirPath();
#endif //_WIN32

#ifdef __linux__
    tmp = QCoreApplication::applicationDirPath();
#endif //__linux__

#ifdef __APPLE__
    tmp = QCoreApplication::applicationDirPath();
#endif //__APPLE__

    return tmp;
}

//--------------------------------------Nutzerordner:
QString prgpfade::path_user()
{
    QString tmp;

#ifdef _WIN32
    tmp = QDir::homePath();
    tmp += QDir::separator();
    tmp += "AppData";
    tmp += QDir::separator();
    tmp += "Roaming";
    tmp += QDir::separator();
    tmp += ".simson_V1";
#endif //_WIN32

#ifdef __linux__
    tmp = QDir::homePath();
    tmp += QDir::separator();
    tmp += ".simson_V1";
#endif //__linux__

    return tmp;
}
QString prgpfade::path_inifile()
{
    QString tmp;
    tmp = path_user();
    tmp += QDir::separator();
    tmp += Inifile;
    return tmp;
}

QString prgpfade::path_wkz_dir(QString maschine)
{
    QString tmp;
    tmp = path_user();
    tmp += QDir::separator();
    tmp += "Maschinen";
    tmp += QDir::separator();
    tmp += maschine;
    return tmp;
}
QString prgpfade::path_wkz_ini(QString maschine)
{
    QString tmp;
    tmp = path_wkz_dir(maschine);
    tmp += QDir::separator();
    tmp += Wkz;
    return tmp;
}
