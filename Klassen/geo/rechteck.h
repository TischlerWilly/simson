#ifndef RECHTECK3D_H
#define RECHTECK3D_H

#include "darstellungsatribute.h"
#include "punkt3d.h"
#include "strecke.h"
#include "bogen.h"

class rechteck : public darstellungsatribute
{
public:
    rechteck();
    rechteck(QString geotext);

    //--------------------------------------------------set_xy:
    void set_text(QString geotext);
    void set_mipu(double x, double y, double z);
    void set_mipu(punkt3d p);
    void set_obli(double x, double y, double z);
    void set_obli(punkt3d p);
    void set_obre(double x, double y, double z);
    void set_obre(punkt3d p);
    void set_unli(double x, double y, double z);
    void set_unli(punkt3d p);
    void set_unre(double x, double y, double z);
    void set_unre(punkt3d p);
    void set_li(double x, double y, double z);
    void set_li(punkt3d p);
    void set_re(double x, double y, double z);
    void set_re(punkt3d p);
    void set_ob(double x, double y, double z);
    void set_ob(punkt3d p);
    void set_un(double x, double y, double z);
    void set_un(punkt3d p);

    void set_laenge(double neue_laenge);
    void set_breite(double neue_breite);
    void set_rad(double neuer_radius);
    void set_drewi(double neuer_drehwinkel);

    inline  void    set_laenge(QString neue_laenge)
    {
        set_laenge(neue_laenge.toDouble());
    }       
    inline  void    set_breite(QString neue_breite)
    {
        set_breite(neue_breite.toDouble());
    }       
    inline  void    set_rad(QString neuer_radius)
    {
        set_rad(neuer_radius.toDouble());
    }       
    inline  void    set_drewi(QString neuer_drehwinkel)
    {
        set_drewi(neuer_drehwinkel.toDouble());
    }

    //--------------------------------------------------get_xy:
    punkt3d mipu();
    punkt3d obli();
    punkt3d obre();
    punkt3d unli();
    punkt3d unre();

    inline  double  l()
    {
        return Laenge;
    }
    inline  QString l_qstring()
    {
        return double_to_qstring(Laenge);
    }
    inline  double  b()
    {
        return Breite;
    }
    inline  QString b_qstring()
    {
        return double_to_qstring(Breite);
    }
    inline  double  rad()
    {
        return Radius;
    }
    inline  QString rad_qstring()
    {
        return double_to_qstring(Radius);
    }
    inline  double  drewi()
    {
        return Drehwinkel;
    }
    inline  QString drewi_qstring()
    {
        return double_to_qstring(Drehwinkel);
    }

    double abst(punkt3d p);
    //--------------------------------------------------Manipulaionen:
    void    verschieben_um(double xversatz, double yversatz);

private:
    punkt3d MiPu;
    double Laenge, Breite;
    double Radius;
    double Drehwinkel;//in Radiant
};

#endif // RECHTECK3D_H
