#include "rechteck.h"

rechteck::rechteck()
{
    set_mipu(0,0,0);
    set_laenge(0);
    set_breite(0);
    set_rad(0);
    set_drewi(0);
}
rechteck::rechteck(QString geotext)
{
    set_text(geotext);
}

//--------------------------------------------------set_xy:
void rechteck::set_text(QString geotext)
{
    text_zw tz;
    tz.set_text(geotext,TRZ_PA_);
    punkt3d p;
    p.set_x(tz.at(1).toDouble());
    p.set_y(tz.at(2).toDouble());
    p.set_z(tz.at(3).toDouble());
    set_mipu(p);
    set_laenge(tz.at(4).toDouble());
    set_breite(tz.at(5).toDouble());
    set_rad(tz.at(6).toDouble());
    set_drewi(tz.at(7).toDouble());
    set_farbe(tz.at(8));
    set_farbe_fuellung(tz.at(9));
    set_linienbreite(tz.at(10).toInt());
    set_stil(tz.at(11));
}
void rechteck::set_mipu(double x, double y, double z=0)
{
    MiPu.set_x(x);
    MiPu.set_y(y);
    MiPu.set_z(z);
}
void rechteck::set_mipu(punkt3d p)
{
    MiPu = p;
}
void rechteck::set_obli(double x, double y, double z)
{
    //Die Funktion beachtet den Drehwinkel nicht.
    //Die Funktion setzt voraus, dass Länge und Breite bereits bekannt sind
    MiPu.set_x( x + l()/2 );
    MiPu.set_y( y - b()/2 );
    MiPu.set_z( z );
}
void rechteck::set_obli(punkt3d p)
{
    set_obli(p.x(), p.y(), p.z());
}
void rechteck::set_obre(double x, double y, double z)
{
    //Die Funktion beachtet den Drehwinkel nicht.
    //Die Funktion setzt voraus, dass Länge und Breite bereits bekannt sind
    MiPu.set_x( x - l()/2 );
    MiPu.set_y( y - b()/2 );
    MiPu.set_z( z );
}
void rechteck::set_obre(punkt3d p)
{
    set_obre(p.x(), p.y(), p.z());
}
void rechteck::set_unli(double x, double y, double z)
{
    //Die Funktion beachtet den Drehwinkel nicht.
    //Die Funktion setzt voraus, dass Länge und Breite bereits bekannt sind
    MiPu.set_x( x + l()/2 );
    MiPu.set_y( y + b()/2 );
    MiPu.set_z( z );
}
void rechteck::set_unli(punkt3d p)
{
    set_unli(p.x(), p.y(), p.z());
}
void rechteck::set_unre(double x, double y, double z)
{
    //Die Funktion beachtet den Drehwinkel nicht.
    //Die Funktion setzt voraus, dass Länge und Breite bereits bekannt sind
    MiPu.set_x( x - l()/2 );
    MiPu.set_y( y + b()/2 );
    MiPu.set_z( z );
}
void rechteck::set_unre(punkt3d p)
{
    set_unre(p.x(), p.y(), p.z());
}
void rechteck::set_li(double x, double y, double z)
{
    //Die Funktion beachtet den Drehwinkel nicht.
    //Die Funktion setzt voraus, dass Länge und Breite bereits bekannt sind
    MiPu.set_x( x + l()/2 );
    MiPu.set_y( y );
    MiPu.set_z( z );
}
void rechteck::set_li(punkt3d p)
{
    set_li(p.x(), p.y(), p.z());
}
void rechteck::set_re(double x, double y, double z)
{
    //Die Funktion beachtet den Drehwinkel nicht.
    //Die Funktion setzt voraus, dass Länge und Breite bereits bekannt sind
    MiPu.set_x( x - l()/2 );
    MiPu.set_y( y );
    MiPu.set_z( z );
}
void rechteck::set_re(punkt3d p)
{
    set_re(p.x(), p.y(), p.z());
}
void rechteck::set_ob(double x, double y, double z)
{
    //Die Funktion beachtet den Drehwinkel nicht.
    //Die Funktion setzt voraus, dass Länge und Breite bereits bekannt sind
    MiPu.set_x( x );
    MiPu.set_y( y - b()/2 );
    MiPu.set_z( z );
}
void rechteck::set_ob(punkt3d p)
{
    set_ob(p.x(), p.y(), p.z());
}
void rechteck::set_un(double x, double y, double z)
{
    //Die Funktion beachtet den Drehwinkel nicht.
    //Die Funktion setzt voraus, dass Länge und Breite bereits bekannt sind
    MiPu.set_x( x );
    MiPu.set_y( y + b()/2 );
    MiPu.set_z( z );
}
void rechteck::set_un(punkt3d p)
{
    set_un(p.x(), p.y(), p.z());
}
void rechteck::set_laenge(double neue_laenge)
{
    Laenge = neue_laenge;
}
void rechteck::set_breite(double neue_breite)
{
    Breite = neue_breite;    
}
void rechteck::set_rad(double neuer_radius)
{
    Eckenradius = neuer_radius;
}
void rechteck::set_drewi(double neuer_drehwinkel)
{
    Drehwinkel = neuer_drehwinkel;
}

//--------------------------------------------------get_xy:
punkt3d rechteck::mipu()
{
    return MiPu;
}
punkt3d rechteck::obli()
{
    punkt3d p = MiPu;
    p.verschieben_um( -l()/2 , b()/2 );
    p.drehen(MiPu, Drehwinkel);
    return p;
}
punkt3d rechteck::obre()
{
    punkt3d p = MiPu;
    p.verschieben_um( l()/2 , b()/2 );
    p.drehen(MiPu, Drehwinkel);
    return p;
}
punkt3d rechteck::unli()
{
    punkt3d p = MiPu;
    p.verschieben_um( -l()/2 , -b()/2 );
    p.drehen(MiPu, Drehwinkel);
    return p;
}
punkt3d rechteck::unre()
{
    punkt3d p = MiPu;
    p.verschieben_um( l()/2 , -b()/2 );
    p.drehen(MiPu, Drehwinkel);
    return p;
}
punkt3d rechteck::ob()
{
    punkt3d p = MiPu;
    p.verschieben_um( 0 , b()/2 );
    p.drehen(MiPu, Drehwinkel);
    return p;
}
punkt3d rechteck::un()
{
    punkt3d p = MiPu;
    p.verschieben_um( 0 , -b()/2 );
    p.drehen(MiPu, Drehwinkel);
    return p;
}
punkt3d rechteck::li()
{
    punkt3d p = MiPu;
    p.verschieben_um( -l()/2 , 0 );
    p.drehen(MiPu, Drehwinkel);
    return p;
}
punkt3d rechteck::re()
{
    punkt3d p = MiPu;
    p.verschieben_um( l()/2 , 0 );
    p.drehen(MiPu, Drehwinkel);
    return p;
}
punkt3d rechteck::obli_li()
{
    punkt3d p = MiPu;
    p.verschieben_um( -l()/2 , b()/2-rad() );
    p.drehen(MiPu, Drehwinkel);
    return p;
}
punkt3d rechteck::obli_ob()
{
    punkt3d p = MiPu;
    p.verschieben_um( -l()/2+rad() , b()/2 );
    p.drehen(MiPu, Drehwinkel);
    return p;
}
punkt3d rechteck::obre_ob()
{
    punkt3d p = MiPu;
    p.verschieben_um( l()/2-rad() , b()/2 );
    p.drehen(MiPu, Drehwinkel);
    return p;
}
punkt3d rechteck::obre_re()
{
    punkt3d p = MiPu;
    p.verschieben_um( l()/2 , b()/2-rad() );
    p.drehen(MiPu, Drehwinkel);
    return p;
}
punkt3d rechteck::unli_li()
{
    punkt3d p = MiPu;
    p.verschieben_um( -l()/2 , -b()/2+rad() );
    p.drehen(MiPu, Drehwinkel);
    return p;
}
punkt3d rechteck::unli_un()
{
    punkt3d p = MiPu;
    p.verschieben_um( -l()/2+rad() , -b()/2 );
    p.drehen(MiPu, Drehwinkel);
    return p;
}
punkt3d rechteck::unre_un()
{
    punkt3d p = MiPu;
    p.verschieben_um( l()/2-rad() , -b()/2 );
    p.drehen(MiPu, Drehwinkel);
    return p;
}
punkt3d rechteck::unre_re()
{
    punkt3d p = MiPu;
    p.verschieben_um( l()/2 , -b()/2+rad() );
    p.drehen(MiPu, Drehwinkel);
    return p;
}

double rechteck::abst(punkt3d p)
{
    punkt3d pol, por, pul, pur;

    pol = obli();
    por = obre();
    pul = unli();
    pur = unre();

    strecke sli, sre, sob, sun;
    sli.set_stapu(pul);
    sli.set_endpu(pol);
    sre.set_stapu(pur);
    sre.set_endpu(por);
    sob.set_stapu(pol);
    sob.set_endpu(por);
    sun.set_stapu(pul);
    sun.set_endpu(pur);
    if(rad() > 0)
    {
        sli.set_laenge( sli.laenge2d() - rad()*2 );
        sre.set_laenge( sli.laenge2d() );
        sob.set_laenge( sob.laenge2d() - rad()*2 );
        sun.set_laenge( sob.laenge2d() );
    }
    double abst_li, abst_re, abst_ob, abst_un;
    abst_li = sli.abst(p);
    abst_re = sre.abst(p);
    abst_ob = sob.abst(p);
    abst_un = sun.abst(p);
    double min = abst_li;
    if(abst_re < min)
    {
        min = abst_re;
    }
    if(abst_ob < min)
    {
        min = abst_ob;
    }
    if(abst_un < min)
    {
        min = abst_un;
    }

    if(rad() > 0)
    {
        rechteck r_innen;//Die Eckpunkte dieses Rechteckes sind die Mittelpunkte der Kreisbögen der Ecken
        r_innen.set_mipu(mipu());
        r_innen.set_drewi(drewi());
        r_innen.set_laenge(l()-rad()*2);
        r_innen.set_breite(b()-rad()*2);

        bogen bobre, bobli, bunli, bunre;
        bobre.set_mipu(r_innen.obre());
        bobli.set_mipu(r_innen.obli());
        bunli.set_mipu(r_innen.unli());
        bunre.set_mipu(r_innen.unre());
        bobre.set_rad(rad());
        bobli.set_rad(rad());
        bunli.set_rad(rad());
        bunre.set_rad(rad());
        bobre.set_swi( drewi() + degToRad(0) );
        bobre.set_ewi( drewi() + degToRad(90) );
        bobli.set_swi( drewi() + degToRad(90) );
        bobli.set_ewi( drewi() + degToRad(180) );
        bunli.set_swi( drewi() + degToRad(180) );
        bunli.set_ewi( drewi() + degToRad(270) );
        bunre.set_swi( drewi() + degToRad(270) );
        bunre.set_ewi( drewi() + degToRad(0) );
        double abst_or, abst_ol, abst_ul, abst_ur;
        abst_or = bobre.abst(p);
        abst_ol = bobli.abst(p);
        abst_ul = bunli.abst(p);
        abst_ur = bunre.abst(p);
        if(abst_or < min)
        {
            min = abst_or;
        }
        if(abst_ol < min)
        {
            min = abst_ol;
        }
        if(abst_ul < min)
        {
            min = abst_ul;
        }
        if(abst_ur < min)
        {
            min = abst_ur;
        }
    }
    return min;
}
//--------------------------------------------------Manipulaionen:
void rechteck::verschieben_um(double xversatz, double yversatz)
{
    MiPu.verschieben_um(xversatz, yversatz);
}

//--------------------------------------------------







