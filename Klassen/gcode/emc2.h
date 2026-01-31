#ifndef EMC2_H
#define EMC2_H

#include "Klassen/wst/werkstueck.h"
#include "Klassen/wkz/maschine.h"

class emc2
{
public:
    emc2();
    QString header();
    QString footer();
    QString bohr(bohrung bo);



private:
    werkstueck  *Wst;
    maschine    *Masch;

};

#endif // EMC2_H
