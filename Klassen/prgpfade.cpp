#include "prgpfade.h"

prgpfade::prgpfade()
{
    Inifile             = "einstellungen.ini";
    ini_dxf             = "dxf.ini";
    ini_dxf_klassen     = "dxf_klassen.ini";
    Wkz                 = "wkz_magazin.csv";
    Masch_ini           = "maschine.ini";

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
    QDir dir_wkz(path_wkz_dir());
    if(!dir_wkz.exists())
    {
        dir_wkz.mkdir(path_wkz_dir());
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
QString prgpfade::name_ini_dxf()
{
    return ini_dxf;
}
QString prgpfade::name_ini_dxf_klassen()
{
    return ini_dxf_klassen;
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
QString prgpfade::path_ini_dxf()
{
    QString tmp;
    tmp = path_user();
    tmp += QDir::separator();
    tmp += ini_dxf;
    return tmp;
}
QString prgpfade::path_ini_dxf_klassen()
{
    QString tmp;
    tmp = path_user();
    tmp += QDir::separator();
    tmp += ini_dxf_klassen;
    return tmp;
}
QString prgpfade::path_wkz_dir()
{
    QString tmp;
    tmp = path_user();
    tmp += QDir::separator();
    tmp += "Maschinen";
    return tmp;
}
QString prgpfade::path_wkz_dir(QString maschine)
{
    QString tmp;
    tmp = path_wkz_dir();
    tmp += QDir::separator();
    tmp += maschine;
    return tmp;
}
QString prgpfade::path_wkz_mag(QString maschine)
{
    QString tmp;
    tmp = path_wkz_dir(maschine);
    tmp += QDir::separator();
    tmp += Wkz;
    return tmp;
}
QString prgpfade::path_masch_dir(QString maschine)
{
    return path_wkz_dir(maschine);
}
QString prgpfade::path_masch_ini(QString maschine)
{
    QString tmp;
    tmp = path_masch_dir(maschine);
    tmp += QDir::separator();
    tmp += Masch_ini;
    return tmp;
}
