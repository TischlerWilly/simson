#ifndef STRECKE_H
#define STRECKE_H

#include <QString>
#include <math.h>
#include <QMessageBox>
#include "punkt3d.h"
#include "darstellungsatribute.h"
#include "geofunktionen.h"
#include "Klassen/text_zw.h"
#include "defines_geometrie.h"
#include "bogen.h"

enum strecke_bezugspunkt{strecke_bezugspunkt_start, \
                         strecke_bezugspunkt_mitte, \
                         strecke_bezugspunkt_ende};

class strecke : public darstellungsatribute
{
public:
    strecke();
    strecke(QString geotext);

    //-----------------------------------------set_xy:
    void    set_text(QString geotext);
    void    set_stapu(punkt3d startpunkt);
    void    set_stapu(double x, double y, double z);
    void    set_endpu(punkt3d endpunkt);
    void    set_endpu(double x, double y, double z);

    //-----------------------------------------get_xy:
    inline punkt3d stapu()
    {
        return Stapu;
    }
    inline punkt3d endpu()
    {
        return Endpu;
    }
    punkt3d mipu();
    punkt3d schnittp_zur_senkr_an_p(punkt3d p);

    inline double laenge3d()
    {
        return  laenge3d(Stapu, Endpu);
    }
    double laenge3d(punkt3d sp, punkt3d ep);
    inline double laenge2d()
    {
        return  laenge2d(Stapu, Endpu);
    }
    double laenge2d(punkt3d sp, punkt3d ep);
    inline QString laenge3d_QString()
    {
        return double_to_qstring(laenge3d());
    }
    inline QString laenge2d_QString()
    {
        return double_to_qstring(laenge2d());
    }

    double abst(punkt3d p);
    bool auf_strecke(punkt3d p);
    double wink();
    strecke parallele(bool links, double abstand);
    QString text();


    //-----------------------------------------manipulationen:
    void    richtung_unkehren();
    void    drenen_um_stapu_2d(double drehwi, bool uzs = false);
    void    drenen_um_endpu_2d(double drehwi, bool uzs = false);
    void    drenen_um_mipu_2d(double drehwi, bool uzs = false);
    void    verschieben_um(double xversatz, double yversatz);
    void    set_laenge(double neue_laenge, \
                       strecke_bezugspunkt bezugspunkt = strecke_bezugspunkt_mitte);
    //-----------------------------------------

private:
    punkt3d Stapu, Endpu;

};

//--------------------------------------------------------
//Funktionen außerhalb der Klasse:
bool trimmen(strecke *s1, strecke *s2);
bool trimmen(strecke *s1, bogen *b2);
bool trimmen(bogen *b1, strecke *s2);
double normalize_radiant(double a);
bogen verbindungsbogen(strecke s1, strecke s2);

#endif // STRECKE_H
