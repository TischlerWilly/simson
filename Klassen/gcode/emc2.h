#ifndef EMC2_H
#define EMC2_H

#include "Klassen/wst/werkstueck.h"
#include "Klassen/wkz/maschine.h"

class emc2
{
public:
    emc2(maschine *m, werkstueck *w);
    void set_maschine(maschine *m);
    void set_wst(werkstueck *w);
    void setup();

    QString gcode();
    QString prgkopf();
    QString prgende();
    QString bohr(bohrung bo);





private:
    werkstueck  *Wst;
    maschine    *Maschine;
    double Sicherheitsabstand;

};

#endif // EMC2_H
