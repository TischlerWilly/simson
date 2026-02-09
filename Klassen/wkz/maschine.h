#ifndef MASCHINE_H
#define MASCHINE_H

#include <QString>

#include "Funktionen/umwandeln.h"
#include "wkz_magazin.h"

class maschine
{
public:
    maschine();
    maschine(QString neuer_name);

    //--------------------------------------------------set_xy:
    void set_text(QString t);
    void set_laenge(double l);
    void set_laenge(QString l);
    void set_breite(double b);
    void set_breite(QString b);
    void set_ausgabeformat(QString f);
    void set_name(QString neuer_name);
    void set_wkzmag(wkz_magazin wkzmag);

    //--------------------------------------------------get_xy:
    QString text();

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
    inline QString  ausgabeformat() const
    {
        return Ausgabeformat;
    }
    inline QString  name()
    {
        return Name;
    }
    inline wkz_magazin wkzmag()
    {
        return Wkzmag;
    }

private:
    //Variabeln:
    QString Name;
    double Laenge;  //X-Wert
    double Breite;  //Y-Wert
    QString Ausgabeformat; //kein|emc2
    wkz_magazin Wkzmag;

};

#endif // MASCHINE_H
