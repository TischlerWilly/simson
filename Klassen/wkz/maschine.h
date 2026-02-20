#ifndef MASCHINE_H
#define MASCHINE_H

#include <QString>

#include "Funktionen/umwandeln.h"
#include "wkz_magazin.h"
#include "Klassen/geo/punkt3d.h"
#include "Funktionen/myfunktion.h"

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
    void set_prgenpos_x(double pos);
    void set_prgenpos_x(QString pos);
    void set_prgenpos_y(double pos);
    void set_prgenpos_y(QString pos);
    void set_prgenpos_z(double pos);
    void set_prgenpos_z(QString pos);
    void set_zugabe_duboti(double zugabe);
    void set_zugabe_duboti(QString zugabe);
    void set_zugabe_dutati(double zugabe);
    void set_zugabe_dutati(QString zugabe);
    void set_ausgabeformat(QString f);
    void set_name(QString neuer_name);
    void set_wkzmag(wkz_magazin wkzmag);
    void set_manWkzWechsel(bool jn);
    void set_manWkzWechsel(QString jn);
    void set_drehzExportieren(bool jn);
    void set_drehzExportieren(QString jn);

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
    inline double   prgenpos_x()
    {
        return Prgendpos.x();
    }
    inline QString   prgenpos_x_qstring()
    {
        return Prgendpos.x_QString();
    }
    inline double   prgenpos_y()
    {
        return Prgendpos.y();
    }
    inline QString   prgenpos_y_qstring()
    {
        return Prgendpos.y_QString();
    }
    inline double   prgenpos_z()
    {
        return Prgendpos.z();
    }
    inline QString   prgenpos_z_qstring()
    {
        return Prgendpos.z_QString();
    }
    inline double   zugabe_duboti()
    {
        return Zugabe_DuBoTi;
    }
    inline QString   zugabe_duboti_qstring()
    {
        return double_to_qstring(Zugabe_DuBoTi);
    }
    inline double   zugabe_dutati()
    {
        return Zugabe_DuTaTi;
    }
    inline QString   zugabe_dutati_qstring()
    {
        return double_to_qstring(Zugabe_DuTaTi);
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
    inline bool manWkzWechsel()
    {
        return ManWkzWechsel;
    }
    inline bool drehzExportieren()
    {
        return DrehzExportieren;
    }

private:
    //Variabeln:
    QString Name;
    double Laenge;  //X-Wert
    double Breite;  //Y-Wert
    punkt3d Prgendpos;
    double Zugabe_DuBoTi; //Zugabe für Durchgangsbohrungen bei denen gilt BoTi == WST-Dicke
    double Zugabe_DuTaTi; //Zugabe für Durchgangs-Taschen bei denen gilt TaTi == WST-Dicke
    QString Ausgabeformat; //kein|emc2
    bool ManWkzWechsel; //Wkz wird von Hand gewechselt | Hat Wechselteller
    bool DrehzExportieren; //Drehzahlen exportieren | Händisch an der Spindel einstellen
    wkz_magazin Wkzmag;

};

#endif // MASCHINE_H
