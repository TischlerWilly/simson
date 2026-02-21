#ifndef BOGEN_H
#define BOGEN_H

#include <math.h>
#include <cmath>
#include <algorithm>
#include "punkt3d.h"
#include "darstellungsatribute.h"

class bogen : public darstellungsatribute
{
public:
    bogen();

    //set:
    void set_text(QString geotext);
    void set_mipu(punkt3d p);
    void set_mipu(double x, double y, double z);
    void set_rad(double rad);
    void set_swi(double w);
    void set_ewi(double w);
    void set_uzs(bool uzs);
    void set_uzs(QString uzs);
    void set_bogen(punkt3d sp, punkt3d ep, double rad, bool uzs);
    void versetze_spu(punkt3d neue_pos);
    void versetze_epu(punkt3d neue_pos);

    //get:
    QString text();
    punkt3d mipu();
    punkt3d mipu_auf_bog(bool oberseite = true);
    double rad();
    QString rad_QString();
    double swi();
    QString swi_QString();
    double ewi();
    QString ewi_QString();
    punkt3d spu();
    punkt3d epu();
    double abst(punkt3d p);
    double spannwinkel();
    bool uzs();
    QString uzs_QString();
    bogen parallele(bool links, double abstand);

    //Manipulationen:
    void    richtung_unkehren();
    void    verschieben_um(double xversatz, double yversatz);
    void    wenden();

private:
    punkt3d Mipu;
    double Rad;
    double Swi;//in Radiant
    double Ewi;//in Radiant
    bool Uzs; //Uhrzeigersinn

    double normalizeAngle(double a);

};

#endif // BOGEN_H
