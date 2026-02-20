#ifndef RECHTECKTASCHE_H
#define RECHTECKTASCHE_H

#include <QString>

#include "Klassen/text_zw.h"
#include "Defines/def_bearbeitungen.h"
#include "Funktionen/umwandeln.h"
#include "Klassen/geo/punkt3d.h"
#include "Klassen/geo/rechteck.h"


class rechtecktasche
{
public:
    rechtecktasche();
    rechtecktasche(QString text);

    void set_reck(rechteck r);
    void set_laenge(double l);
    void set_laenge(QString l);
    void set_breite(double b);
    void set_breite(QString b);
    void set_tiefe(double t);
    void set_tiefe(QString t);
    void set_mipu(punkt3d p);
    void set_drewi(double wi);
    void set_drewi(QString wi);
    void set_rad(double r);
    void set_rad(QString r);
    void set_zustellmass(double mass);
    void set_zustellmass(QString mass);
    void set_ausraeumen(double ja);
    void set_ausraeumen(QString ja);
    void set_bezug(QString bezugsflaeche);
    void set_afb(QString ausfuehrbedingung);
    void set_wkznum(QString nummer);
    void set_zapfen(bool ist_zapfen);
    void set_zapfen(QString ist_zapfen);

    rechteck reck();
    rechteck *reck_ptr();
    double  laenge();
    QString laenge_qstring();
    double  breite();
    QString breite_qstring();
    double  tiefe();
    QString tiefe_qstring();
    double  x();
    QString x_qstring();
    double  y();
    QString y_qstring();
    double  z();
    QString z_qstring();
    punkt3d mipu();
    double  drewi();
    QString drewi_qstring();
    double  rad();
    QString rad_qstring();
    double  zustellmass();
    QString zustellmass_qstring();
    bool    ausraeumen();
    QString ausraeumen_qstring();
    QString bezug();
    QString afb();
    QString wkznum();
    bool istZapfen();
    QString istZapfen_qstring();

    QString text();
    void set_text(QString text);

private:
    rechteck Rechteck_intern;
    double Tiefe;
    double Zustellmass;
    bool Ausraeumen;
    QString Bezug;
    QString Afb;
    QString Werkzeugnummer;
    bool IstZapfen; //Zapfen fräsen statt Loch Bohren

    void setup();

};

#endif // RECHTECKTASCHE_H
