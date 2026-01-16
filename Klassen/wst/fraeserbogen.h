#ifndef FRAESERBOGEN_H
#define FRAESERBOGEN_H

#include <QString>

#include "Klassen/text_zw.h"
#include "Klassen/geo/punkt3d.h"
#include "Klassen/geo/bogen.h"
#include "Funktionen/umwandeln.h"
#include "Defines/def_bearbeitungen.h"

class fraeserbogen
{
public:
    fraeserbogen();
    fraeserbogen(QString text);


    void set_bogen(bogen b);
    void set_bezug(QString bezugsflaeche);
    void set_afb(QString ausfuehrbedingung);
    void set_tiSta(double ti);
    void set_tiEnd(double ti);

    double  xs();
    QString xs_qstring();
    double  xe();
    QString xe_qstring();
    double  ys();
    QString ys_qstring();
    double  ye();
    QString ye_qstring();
    double  zs();
    QString zs_qstring();
    double  ze();
    QString ze_qstring();
    punkt3d sp();
    punkt3d ep();
    QString bezug();
    QString afb();
    double  rad();
    QString rad_qstring();
    bogen   bog();
    double tiSta();
    QString tiSta_qstring();
    double tiEnd();
    QString tiEnd_qstring();
    punkt3d stapu();
    inline punkt3d endpu();

    QString text();
    void set_text(QString text);

private:
    bogen   Bogen;
    double TiStart;
    double TiEnde;
    QString Afb;
    QString Bezug;

    void setup();

};

#endif // FRAESERBOGEN_H
