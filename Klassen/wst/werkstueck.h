#ifndef WERKSTUECK_H
#define WERKSTUECK_H

#include <QString>
#include <QMessageBox>
#include <QFileInfo>

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
    void set_text(QString text);
    void set_laenge(double l);
    void set_laenge(QString l);
    void set_breite(double b);
    void set_breite(QString b);
    void set_dicke(double d);
    void set_dicke(QString d);
    void neue_bearbeitung(QString text);
    void set_bearb(text_zw b);
    void set_name(QString neuer_name);
    void set_dateipfad(QString pfad);
    void set_versatz_x(double versatz);
    void set_versatz_y(double versatz);
    void set_versatz_z(double versatz);
    void set_prgend_x(QString endpos);
    void set_prgend_y(QString endpos);
    void set_prgend_z(QString endpos);
    void wurde_gespeichert();

    //--------------------------------------------------get_xy:
    QString text();
    QString cad_fehler(bool kurz = false);
    geo_text geo(wkz_magazin wkzm);
    geo_text geo_aktfkon(wkz_magazin wkzm);
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
    inline double verastz_x()
    {
        return Versatz_x;
    }
    inline QString versatz_x_qstring()
    {
        return double_to_qstring(Versatz_x);
    }
    inline double verastz_y()
    {
        return Versatz_y;
    }
    inline QString versatz_y_qstring()
    {
        return double_to_qstring(Versatz_y);
    }
    inline double verastz_z()
    {
        return Versatz_z;
    }
    inline QString versatz_z_qstring()
    {
        return double_to_qstring(Versatz_z);
    }
    inline QString prgend_x()
    {
        return Prgend_x;
    }
    inline QString prgend_y()
    {
        return Prgend_y;
    }
    inline QString prgend_z()
    {
        return Prgend_z;
    }
    inline QString  name()
    {
        return Name;
    }
    inline QString dateipfad()
    {
        return Dateipfad;
    }
    inline text_zw bearb()
    {
        return Bearb;
    }
    inline text_zw *bearb_ptr()
    {
        return &Bearb;
    }
    inline bool hat_aenderungen()
    {
        if(UnReDo.anz_aktive_elemente() > 1)
        {
            return true;
        }else
        {
            return false;
        }
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
    double Versatz_z;
    QString Prgend_x;
    QString Prgend_y;
    QString Prgend_z;
    text_zw Bearb;
    QString Name;
    QString Dateipfad;

    undo_redo<text_zw> UnReDo;
    undo_redo<double> UnReDo_L;
    undo_redo<double> UnReDo_B;
    undo_redo<double> UnReDo_D;
    undo_redo<double> UnReDo_versatz_x;
    undo_redo<double> UnReDo_versatz_y;
    undo_redo<double> UnReDo_versatz_z;
    undo_redo<QString> UnReDo_prgend_x;
    undo_redo<QString> UnReDo_prgend_y;
    undo_redo<QString> UnReDo_prgend_z;

};

#endif // WERKSTUECK_H
