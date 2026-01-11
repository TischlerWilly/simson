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

    //get:
    QString text();
    punkt3d mipu();
    double rad();
    QString rad_QString();
    double swi();
    QString swi_QString();
    double ewi();
    QString ewi_QString();
    punkt3d spu();
    punkt3d epu();
    double abst(punkt3d p);

    //Manipulationen:
    void    richtung_unkehren();

private:
    punkt3d Mipu;
    double Rad;
    double Swi;//in Radiant
    double Ewi;//in Radiant

};

#endif // BOGEN_H
