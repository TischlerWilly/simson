#include "kreis.h"

kreis::kreis()
{
    set_mipu(0,0,0);
    set_rad(0);
}

kreis::kreis(QString geotext)
{
    set_text(geotext);
}

void kreis::set_text(QString geotext)
{
    text_zw tz;
    tz.set_text(geotext,TRZ_PA_);
    punkt3d p;
    p.set_x(tz.at(1).toDouble());
    p.set_y(tz.at(2).toDouble());
    p.set_z(tz.at(3).toDouble());
    set_mipu(p);
    set_rad(tz.at(4).toDouble());
    set_farbe(tz.at(5));
    set_farbe_fuellung(tz.at(6));
    set_linienbreite(tz.at(7).toInt());
    set_stil(tz.at(8));
}

void kreis::verschieben_um(double xversatz, double yversatz)
{
    MiPu.verschieben_um(xversatz, yversatz);
}

double kreis::abst(punkt3d p)
{
    // Abstand vom Punkt zum Mittelpunkt
    double px = p.x();
    double py = p.y();
    double cx = mipu().x();
    double cy = mipu().y();
    double distCenter = std::sqrt((px - cx) * (px - cx) +
                                  (py - cy) * (py - cy));

    // Abstand zum Kreisrand (nicht negativ)
    double distCircle = distCenter - radius();

    return (distCircle > 0.0) ? distCircle : 0.0;
}
QString kreis::text()
{
    QString msg = KREIS;
    msg += TRZ_PA;
    msg += MiPu.x_QString();
    msg += TRZ_PA;
    msg += MiPu.y_QString();
    msg += TRZ_PA;
    msg += MiPu.z_QString();
    msg += TRZ_PA;
    msg += radius_QString();
    msg += TRZ_PA;
    msg += farbe();
    msg += TRZ_PA;
    msg += farbe_fuellung();
    msg += TRZ_PA;
    msg += linienbreite_qstring();
    msg += TRZ_PA;
    msg += stil();

    return msg;
}
