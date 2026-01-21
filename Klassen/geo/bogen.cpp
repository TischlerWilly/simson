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
void bogen::set_bogen(punkt3d sp, punkt3d ep, double rad, bool uzs)
{
    punkt3d P0 = sp;
    punkt3d P1 = ep;
    double dx = P1.x() - P0.x();
    double dy = P1.y() - P0.y();
    double d = std::sqrt(dx*dx + dy*dy);

    if (d > 2*rad)
        throw std::runtime_error("Kein Kreis möglich: Abstand > 2*Radius");

    // Mittelpunkt der Sehne
    punkt3d C;
    C.set_x( (P0.x() + P1.x()) * 0.5 );
    C.set_y( (P0.y() + P1.y()) * 0.5 );

    // Normalisierte Richtung
    double ux = dx / d;
    double uy = dy / d;

    // Normale
    double nx = -uy;
    double ny = ux;

    // Abstand vom Sehnenmittelpunkt zum Kreismittelpunkt
    double h = std::sqrt(rad*rad - (d*0.5)*(d*0.5));

    // Zwei mögliche Mittelpunkte
    punkt3d M1;
    M1.set_x( C.x() + nx * h );
    M1.set_y( C.y() + ny * h );

    punkt3d M2;
    M2.set_x( C.x() - nx * h );
    M2.set_y( C.y() - ny * h );

    // Auswahl abhängig von Drehrichtung
    punkt3d M = uzs ? M1 : M2;

    // Winkel berechnen
    double startAngle = std::atan2( P0.y() - M.y(), P0.x() - M.x() );
    double endAngle   = std::atan2( P1.y() - M.y(), P1.x() - M.x() );

    // Winkelrichtung anpassen
    if (uzs) {
        if (endAngle > startAngle)
            endAngle -= 2 * M_PI;
    } else {
        if (endAngle < startAngle)
            endAngle += 2 * M_PI;
    }
    set_mipu(M);
    set_rad(rad);
    set_swi(startAngle);
    set_ewi(endAngle);
}
void bogen::versetze_spu(punkt3d neue_pos)
{
    //Der alte Endpunkt bleibt fix.
    //Der neue Startpunkt wird übergeben.
    //Der Winkelspan bleibt derselbe.
    //Mittelpunkt und Radius werden so berechnet, dass alter
    //Endpunkt und neuer Startpunkt garantiert auf dem Kreis liegen.

    // 1. Distanz zwischen Start- und Endpunkt
    punkt3d oldEndPoint( epu() );
    punkt3d newStartPoint = neue_pos;
    double dx = oldEndPoint.x() - newStartPoint.x();
    double dy = oldEndPoint.y() - newStartPoint.y();
    double d = std::sqrt(dx*dx + dy*dy);

    // 2. Mittelpunkt der Strecke
    punkt3d mid;
    mid.set_x( (newStartPoint.x() + oldEndPoint.x()) / 2.0 );
    mid.set_y( (newStartPoint.y() + oldEndPoint.y()) / 2.0 );

    // 3. Abstand des Kreismittelpunkts von der Mitte
    double oldAngleSpan = spannwinkel();
    double h = d / (2.0 * std::tan(oldAngleSpan / 2.0));

    // 4. Normale zur Strecke (linksseitig)
    double nx = -dy / d;
    double ny =  dx / d;

    // 5. Mittelpunkt berechnen
    punkt3d C;
    C.set_x( mid.x() + nx * h );
    C.set_y( mid.y() + ny * h );

    // 6. Radius
    double r = std::sqrt( (newStartPoint.x() - C.x())*(newStartPoint.x() - C.x()) +\
                          (newStartPoint.y() - C.y())*(newStartPoint.y() - C.y()) );

    // 7. Winkel
    double startAngle = std::atan2( newStartPoint.y() - C.y(),
                                    newStartPoint.x() - C.x() );

    double endAngle = std::atan2( oldEndPoint.y() - C.y(),
                                  oldEndPoint.x() - C.x() );

    // Ergebnis setzen
    set_mipu(C);
    set_rad(r);
    set_swi(startAngle);
    set_ewi(endAngle);
}
void bogen::versetze_epu(punkt3d neue_pos)
{
    //Der alte Startpunkt bleibt fix.
    //Der neue Endpunkt wird übergeben.
    //Der Winkelspan bleibt derselbe.
    //Mittelpunkt und Radius werden so berechnet, dass alter
    //Startpunkt und neuer Endpunkt garantiert auf dem Kreis liegen.

    // 1. Distanz zwischen Start- und Endpunkt
    punkt3d oldStartPoint( spu() );
    punkt3d newEndPoint = neue_pos;
    double dx = newEndPoint.x() - oldStartPoint.x();
    double dy = newEndPoint.y() - oldStartPoint.y();
    double d = std::sqrt(dx*dx + dy*dy);

    // 2. Mittelpunkt der Strecke
    punkt3d mid;
    mid.set_x( (oldStartPoint.x() + newEndPoint.x()) / 2.0 );
    mid.set_y( (oldStartPoint.y() + newEndPoint.y()) / 2.0 );

    // 3. Abstand des Kreismittelpunkts von der Mitte
    double oldAngleSpan = spannwinkel();
    double h = d / (2.0 * std::tan(oldAngleSpan / 2.0));

    // 4. Normale zur Strecke (linksseitig)
    double nx = -dy / d;
    double ny =  dx / d;

    // 5. Mittelpunkt berechnen
    punkt3d C;
    C.set_x( mid.x() + nx * h );
    C.set_y( mid.y() + ny * h );

    // 6. Radius
    double r = std::sqrt( (oldStartPoint.x() - C.x())*(oldStartPoint.x() - C.x()) +\
                          (oldStartPoint.y() - C.y())*(oldStartPoint.y() - C.y()) );

    // 7. Winkel
    double startAngle = std::atan2( oldStartPoint.y() - C.y(),
                                    oldStartPoint.x() - C.x() );

    double endAngle = std::atan2( newEndPoint.y() - C.y(),
                                  newEndPoint.x() - C.x() );

    // Ergebnis setzen
    set_mipu(C);
    set_rad(r);
    set_swi(startAngle);
    set_ewi(endAngle);
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
double bogen::spannwinkel()
{
    // Gibt den Winkelspan des Bogens zurück
    double s = normalizeAngle(Swi);
    double e = normalizeAngle(Ewi);

    double span = e - s;
    if (span < 0.0)
        span += 2.0 * M_PI;

    return span;
}
bool bogen::uzs()
{
    const double TWO_PI = 2.0 * M_PI;

    // Winkel in [0, 2π) normalisieren
    auto norm = [&](double a) {
        a = fmod(a, TWO_PI);
        if (a < 0) a += TWO_PI;
        return a;
    };

    double startAngle = Swi;
    double endAngle = Ewi;
    startAngle = norm(startAngle);
    endAngle   = norm(endAngle);

    // CCW-Differenz berechnen
    double delta = fmod(endAngle - startAngle + TWO_PI, TWO_PI);

    // Wenn der CCW-Weg größer als π ist, ist der kürzere Weg CW
    return delta > M_PI;
}
//---------------------------------Manipulationen:
void bogen::richtung_unkehren()
{
    double tmp = swi();
    set_swi(ewi());
    set_ewi(tmp);
}
void bogen::verschieben_um(double xversatz, double yversatz)
{
    Mipu.verschieben_um(xversatz, yversatz);
}
void bogen::wenden()
{
    // Mittelpunkt an der Sehne spiegeln
    Mipu.spiegeln(spu(), epu());

    // Start- und Endwinkel tauschen
    double tmp = swi();
    Swi = ewi();
    Ewi = tmp;
}




//---------------------------------Privat:

double bogen::normalizeAngle(double a)
{
    // Normalisiert einen Winkel in den Bereich [0, 2π)
    const double TWO_PI = 2.0 * M_PI;
    a = fmod(a, TWO_PI);
    if (a < 0.0)
        a += TWO_PI;
    return a;
}


