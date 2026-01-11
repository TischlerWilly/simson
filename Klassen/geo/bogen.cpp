#include "bogen.h"

bogen::bogen()
{

}

//---------------------------------set:
void bogen::set_text(QString geotext)
{
    text_zw tz;
    tz.set_text(geotext,TRZ_PA_);
    punkt3d p;
    p.set_x(tz.at(1).toDouble());
    p.set_y(tz.at(2).toDouble());
    p.set_z(tz.at(3).toDouble());
    set_mipu(p);
    set_rad(tz.at(4).toDouble());
    set_swi(tz.at(5).toDouble());
    set_ewi(tz.at(6).toDouble());
    set_farbe(tz.at(7));
    set_linienbreite(tz.at(8).toInt());
    set_stil(tz.at(9));
}
void bogen::set_mipu(punkt3d p)
{
    Mipu = p;
}
void bogen::set_mipu(double x, double y, double z)
{
    Mipu.set_x(x);
    Mipu.set_y(y);
    Mipu.set_z(z);
}
void bogen::set_rad(double rad)
{
    if(rad >= 0)
    {
        Rad = rad;
    }else
    {
        Rad = rad * -1;
    }

}
void bogen::set_swi(double w)
{
    Swi = w;
}
void bogen::set_ewi(double w)
{
    Ewi = w;
}

//---------------------------------get:
QString bogen::text()
{
    QString msg = BOGEN;
    msg += TRZ_PA;
    msg += Mipu.x_QString();
    msg += TRZ_PA;
    msg += Mipu.y_QString();
    msg += TRZ_PA;
    msg += Mipu.z_QString();
    msg += TRZ_PA;
    msg += rad_QString();
    msg += TRZ_PA;
    msg += swi_QString();
    msg += TRZ_PA;
    msg += ewi_QString();
    msg += farbe();
    msg += TRZ_PA;
    msg += linienbreite_qstring();
    msg += TRZ_PA;
    msg += stil();

    return msg;
}
punkt3d bogen::mipu()
{
    return Mipu;
}
double bogen::rad()
{
    return Rad;
}
QString bogen::rad_QString()
{
    return double_to_qstring(rad());
}
double bogen::swi()
{
    return Swi;
}
QString bogen::swi_QString()
{
    return double_to_qstring(swi());
}
double bogen::ewi()
{
    return Ewi;
}
QString bogen::ewi_QString()
{
    return double_to_qstring(ewi());
}
punkt3d bogen::spu()
{
    punkt3d p;
    p.set_x(mipu().x() + rad() * std::cos(swi()));
    p.set_y(mipu().y() + rad() * std::sin(swi()));
    return p;
}
punkt3d bogen::epu()
{
    punkt3d p;
    p.set_x(mipu().x() + rad() * std::cos(ewi()));
    p.set_y(mipu().y() + rad() * std::sin(ewi()));
    return p;
}
double bogen::abst(punkt3d p)
{
    // Punkt relativ zum Mittelpunkt
    double vx = p.x() - mipu().x();
    double vy = p.y() - mipu().y();

    // Abstand Punkt → Mittelpunkt
    double distCenter = std::sqrt(vx*vx + vy*vy);

    // Winkel des Punktes relativ zum Mittelpunkt
    double angle = std::atan2(vy, vx);

    // Winkel normalisieren auf [0, 2π)
    auto norm = [](double a) {
        while (a < 0) a += 2*M_PI;
        while (a >= 2*M_PI) a -= 2*M_PI;
        return a;
    };

    double startAngle = swi();
    double endAngle = ewi();
    angle = norm(angle);
    startAngle = norm(startAngle);
    endAngle   = norm(endAngle);

    // Prüfen, ob angle im Bogenbereich liegt
    auto angleInArc = [&](double a) {
        if (startAngle <= endAngle)
            return a >= startAngle && a <= endAngle;
        else
            return a >= startAngle || a <= endAngle; // Bogen über 0°-Grenze
    };

    double radius = rad();
    if (angleInArc(angle)) {
        // Projektion liegt auf dem Kreisbogen
        return std::fabs(distCenter - radius);
    }

    // Projektion liegt außerhalb → Abstand zu den Endpunkten
    double sx = mipu().x() + radius * std::cos(startAngle);
    double sy = mipu().y() + radius * std::sin(startAngle);
    double ex = mipu().x() + radius * std::cos(endAngle);
    double ey = mipu().y() + radius * std::sin(endAngle);

    double ds = std::hypot(p.x() - sx, p.y() - sy);
    double de = std::hypot(p.x() - ex, p.y() - ey);

    return std::min(ds, de);
}
//---------------------------------Manipulationen:
void bogen::richtung_unkehren()
{
    double tmp = swi();
    set_swi(ewi());
    set_ewi(tmp);
}






