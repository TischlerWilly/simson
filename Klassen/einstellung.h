#ifndef EINSTELLUNG_H
#define EINSTELLUNG_H

#include <QString>
#include "text_zw.h"
#include "Funktionen/umwandeln.h"

class einstellung
{
public:
    einstellung();

    //set:
    void set_text(QString t);
    void set_entwicklermodus(bool ja);
    void set_entwicklermodus(QString jn);
    void set_verzeichnis_quelle(QString v);
    void set_verzeichnis_ziel_server(QString v);
    void set_verzeichnis_ziel_lokal(QString v);

    //get:    
    QString text();
    bool entwicklermodus();
    QString verzeichnis_quelle();
    QString verzeichnis_ziel_server();
    QString verzeichnis_ziel_lokal();

private:
    bool Entwicklermodus;
    QString Verzeichnis_quelle;
    QString Verzeichnis_ziel_server;
    QString Verzeichnis_ziel_lokal;

};

#endif // EINSTELLUNG_H
