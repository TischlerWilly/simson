#ifndef PRGPFADE_H
#define PRGPFADE_H

#include <QString>
#include <QDir>
#include <QCoreApplication>

class prgpfade
{
public:
    prgpfade();
    void ordner_erstellen();
    void ordner_erstellen_wkz(QString maschine);
    QString path_prg();
    QString path_user();
    QString path_inifile();
    QString path_ini_dxf();
    QString path_ini_dxf_klassen();

    QString name_inifile();
    QString name_ini_dxf();
    QString name_ini_dxf_klassen();
    QString path_wkz_dir();
    QString path_wkz_dir(QString maschine);
    QString path_wkz_mag(QString maschine);
    QString path_masch_dir(QString maschine);
    QString path_masch_ini(QString maschine);

private:
    //Ordner:

    //ini-Dateien:
    QString Inifile;
    QString ini_dxf;
    QString ini_dxf_klassen;

    //WKZ-Dateien:
    QString Wkz;
    QString Masch_ini;

};

#endif // PRGPFADE_H
