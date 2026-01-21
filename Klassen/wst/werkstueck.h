#ifndef WERKSTUECK_H
#define WERKSTUECK_H

#include <QString>
#include <QMessageBox>

#include "Defines/werkzeug.h"
#include "Funktionen/umwandeln.h"
#include "Funktionen/runden.h"
#include "Funktionen/myfunktion.h"
#include "Klassen/text_zw.h"
#include "Klassen/wkz/wkz_magazin.h"
#include "bohrung.h"
#include "rechtecktasche.h"
#include "nut.h"
#include "fraeseraufruf.h"
#include "fraesergerade.h"
#include "fraeserbogen.h"
#include "bohrraster.h"
#include "Defines/dateinamen.h"
#include "Klassen/geo/geo_text.h"
#include "Klassen/undo_redo.h"


class werkstueck
{
public:
    werkstueck();
    werkstueck(QString neuer_name);

    //--------------------------------------------------set_xy:
    void set_laenge(double l);
    void set_laenge(QString l);
    void set_breite(double b);
    void set_breite(QString b);
    void set_dicke(double d);
    void set_dicke(QString d);
    void neue_bearbeitung(QString text);
    void set_bearb(text_zw b);
    void set_name(QString neuer_name);
    void set_versatz_x(double versatz);
    void set_versatz_y(double versatz);

    //--------------------------------------------------get_xy:
    QString cad_fehler(bool kurz = false);
    geo_text geo(wkz_magazin wkzm);
    inline double   laenge() const
    {
        return Laenge;
    }
    inline QString  laenge_qstring() const
    {
        return double_to_qstring(Laenge);
    }
    inline double   breite() const
    {
        return Breite;
    }
    inline QString  breite_qstring() const
    {
        return double_to_qstring(Breite);
    }
    inline double   dicke() const
    {
        return Dicke;
    }
    inline QString  dicke_qstring() const
    {
        return double_to_qstring(Dicke);
    }
    inline QString  name()
    {
        return Name;
    }
    inline text_zw bearb()
    {
        return Bearb;
    }
    inline text_zw *bearb_ptr()
    {
        return &Bearb;
    }

    //--------------------------------------------------Manipulationen:
    void undo();
    void redo();
    void unredo_neu();
    void unredo_clear();

    //--------------------------------------------------

private:
    //Variabeln:
    double Laenge;  //X-Wert
    double Breite;  //Y-Wert
    double Dicke;   //Z-Wert
    double Versatz_x;
    double Versatz_y;
    text_zw Bearb;
    QString Name;

    undo_redo<text_zw> UnReDo;
    undo_redo<double> UnReDo_L;
    undo_redo<double> UnReDo_B;
    undo_redo<double> UnReDo_D;

};

#endif // WERKSTUECK_H
