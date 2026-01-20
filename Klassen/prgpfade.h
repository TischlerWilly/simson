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

    QString name_inifile();
    QString path_wkz_dir(QString maschine);
    QString path_wkz_ini(QString maschine);

private:
    //Ordner:

    //ini-Dateien:
    QString Inifile;

    //WKZ-Dateien:
    QString Wkz;

};

#endif // PRGPFADE_H
