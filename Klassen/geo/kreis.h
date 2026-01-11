#ifndef KREIS_H
#define KREIS_H

#include "punkt3d.h"
#include "darstellungsatribute.h"
#include "defines_geometrie.h"
#include "Klassen/text_zw.h"

class kreis : public darstellungsatribute
{
public:
    kreis();
    kreis(QString geotext);
    inline  kreis(punkt3d mittelpunkt, double radius)
    {
        set_mipu(mittelpunkt);
        set_rad(radius);
    }

    void set_text(QString geotext);
    inline  void    set_mipu(punkt3d mittelpunkt)
    {
        MiPu = mittelpunkt;
    }
    inline  void    set_mipu(double x, double y, double z)
    {
        MiPu.set_x(x);
        MiPu.set_y(y);
        MiPu.set_z(z);
    }
    inline  void    set_rad(double radius)
    {
        Rad = radius;
    }
    inline  void    set_rad(QString radius)
    {
        Rad = radius.toDouble();
    }

    inline  punkt3d     mipu()
    {
        return MiPu;
    }
    inline  double      radius()
    {
        return Rad;
    }
    inline  QString     radius_QString()
    {
        return double_to_qstring(Rad);
    }

    void    verschieben_um(double xversatz, double yversatz);

    double abst(punkt3d p);
    QString text();

private:
    punkt3d     MiPu;
    double      Rad;

};

#endif // KREIS_H
