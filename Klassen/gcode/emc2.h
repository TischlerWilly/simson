#ifndef EMC2_H
#define EMC2_H

#include <QSet>

#include "Klassen/wst/werkstueck.h"
#include "Klassen/wkz/maschine.h"
#include "Funktionen/funktionen_prgtext.h"

class emc2
{
public:
    emc2(maschine *m, werkstueck *w);    
    void setup();

    QString gcode();


private:
    void set_maschine(maschine *m);
    void set_wst(werkstueck *w);
    QString prgkopf();
    QString prgende();
    QString wkz_wechsel(QString tnummer);
    QString bohr(bohrung bo);
    QString kommentar(kommentar_nc ko);
    QString halt(halt_nc ha);
    QString gezupu(gehezupunkt gzp);

    werkstueck  *Wst;
    maschine    *Maschine;
    double Sicherheitsabstand;
    double Masszugabe_duboti;//Durchgangsbohrung Tiefenzugabe
    double Masszugabe_dutati;//Durchgangs-Tasche Tiefenzugabe
    QString Akt_wkz;
    QSet<QString> Verwendete_wkz;

};

#endif // EMC2_H
