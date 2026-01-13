#ifndef GEO_TEXT_H
#define GEO_TEXT_H

#include <QString>
#include <QMessageBox>
#include "Klassen/text_zw.h"
#include "Funktionen/umwandeln.h"
#include "punkt3d.h"
#include "strecke.h"
#include "bogen.h"
#include "kreis.h"
#include "rechteck.h"
#include "defines_geometrie.h"
#include "geofunktionen.h"
#include "Klassen/wkz/wkz_magazin.h"
#include "Defines/def_bearbeitungen.h"
#include "Klassen/wst/bohrung.h"
#include "Klassen/wst/bohrraster.h"
#include "Klassen/wst/nut.h"
#include "Klassen/wst/rechtecktasche.h"
#include "Klassen/wst/fraeseraufruf.h"
#include "Klassen/wst/fraesergerade.h"
#include "Klassen/wst/fraeserbogen.h"

class geo_text
{
public:
    geo_text();
    void clear();
    void zeilenvorschub();
    //--------------------------------------set:
    void add_leerzeile();
    void add_punkt(punkt3d p);
    void add_punkt(punkt3d p, uint index);
    void add_strecke(strecke s);
    void add_strecke(strecke s, uint index);
    void add_bogen(bogen b);
    void add_bogen(bogen b, uint index);
    void add_kreis(kreis k);
    void add_kreis(kreis k, uint index);
    void add_rechteck(rechteck rec);
    void add_rechteck(rechteck rec, uint index);
    //--------------------------------------get:
    uint akt_index();
    uint count();
    text_zw at();
    text_zw at(uint index);
    double max_x();
    double min_x();
    double max_y();
    double min_y();

private:
    void add(text_zw geometrie, uint index);    

    std::vector<text_zw>    Daten;
    uint                    AktIndex;
    bool                    Zeivor;//Zeilenvorschub

};

//Funktionen außerhalb der Klasse:

geo_text geo_ermitteln(text_zw bearb, double wst_l, double wst_b, double wst_d, wkz_magazin wkzm);


#endif // GEO_TEXT_H
