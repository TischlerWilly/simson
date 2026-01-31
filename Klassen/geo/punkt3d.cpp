#include "punkt3d.h"

punkt3d::punkt3d()
{
    set_x(0);
    set_y(0);
    set_z(0);
}
punkt3d::punkt3d(QString geotext)
{
    set_text(geotext);
}

void punkt3d::set_text(QString geotext)
{
    text_zw tz;
    tz.set_text(geotext,TRZ_PA_);
    set_x(tz.at(1).toDouble());
    set_y(tz.at(2).toDouble());
    set_z(tz.at(3).toDouble());
    set_farbe(tz.at(4));
    set_linienbreite(tz.at(5).toInt());
}
QString punkt3d::text()
{
    QString msg = PUNKT;
    msg += TRZ_PA;
    msg += x_QString();
    msg += TRZ_PA;
    msg += y_QString();
    msg += TRZ_PA;
    msg += z_QString();
    msg += TRZ_PA;
    msg += farbe();
    msg += TRZ_PA;
    msg += linienbreite_qstring();
    return msg;
}

void punkt3d::verschieben_um(double xversatz, double yversatz)
{
    X_param += xversatz;
    Y_param += yversatz;
}

void punkt3d::drehen(punkt3d zentrum, double drewi)
{
    //es wird gegen den Uhrzeigersinn gedreht

    // Schritt 1: Punkt relativ zum Zentrum verschieben
    double dx = X_param - zentrum.x();
    double dy = Y_param - zentrum.y();

    // Schritt 2: Rotation anwenden
    double rotatedX = dx * std::cos(drewi) - dy * std::sin(drewi);
    double rotatedY = dx * std::sin(drewi) + dy * std::cos(drewi);

    // Schritt 3: Zurückverschieben
    X_param = rotatedX + zentrum.x();
    Y_param = rotatedY + zentrum.y();
}

void punkt3d::spiegeln(punkt3d sp_achse, punkt3d ep_achse)
{
    //sp_achse und ep_achse definieren die Achse über die der Punkt gespiegelt wird

    // Richtungsvektor der Sehne
    double d_x = ep_achse.x() - sp_achse.x();
    double d_y = ep_achse.y() - sp_achse.y();

    // Normalenvektor (senkrecht zu d)
    double n_x = -d_y;
    double n_y = d_x;

    // Normieren
    double len = std::sqrt(n_x*n_x + n_y*n_y);
    n_x /= len;
    n_y /= len;

    // Abstand des Punkts zur Geraden in Normalenrichtung
    double C_x = x();
    double C_y = y();
    double dist = (C_x - sp_achse.x()) * n_x + (C_y - sp_achse.y()) * n_y;

    // Spiegelung
    X_param = C_x - 2 * dist * n_x;
    Y_param = C_y - 2 * dist * n_y;
}

//-------------------Funktionen nicht innerhalb der Klasse:
punkt3d operator +(punkt3d p1, punkt3d p2)
{
    punkt3d p3;
    p3.set_x(p1.x() + p2.x());
    p3.set_y(p1.y() + p2.y());
    p3.set_z(p1.z() + p2.z());
    return p3;
}

punkt3d operator -(punkt3d p1, punkt3d p2)
{
    punkt3d p3;
    p3.set_x(p1.x() - p2.x());
    p3.set_y(p1.y() - p2.y());
    p3.set_z(p1.z() - p2.z());
    return p3;
}

punkt3d operator *(punkt3d p1, double fakror)
{
    punkt3d p2;
    p2.set_x(p1.x() * fakror);
    p2.set_y(p1.y() * fakror);
    p2.set_z(p1.z() * fakror);
    return p2;
}

punkt3d operator *(double fakror, punkt3d p1)
{
    punkt3d p2;
    p2.set_x(p1.x() * fakror);
    p2.set_y(p1.y() * fakror);
    p2.set_z(p1.z() * fakror);
    return p2;
}

punkt3d operator /(punkt3d p1, double divisor)
{
    punkt3d p2;
    p2.set_x(p1.x() / divisor);
    p2.set_y(p1.y() / divisor);
    p2.set_z(p1.z() / divisor);
    return p2;
}

bool operator ==(punkt3d p1, punkt3d p2)
{
    bool returnbool = true;
    if(p1.x() != p2.x())
    {
        returnbool = false;
    }
    if(p1.y() != p2.y())
    {
        returnbool = false;
    }
    if(p1.z() != p2.z())
    {
        returnbool = false;
    }
    return returnbool;
}

bool operator !=(punkt3d p1, punkt3d p2)
{
    bool returnbool = true;
    if(p1.x() != p2.x())
    {
        returnbool = false;
    }
    if(p1.y() != p2.y())
    {
        returnbool = false;
    }
    if(p1.z() != p2.z())
    {
        returnbool = false;
    }
    if(returnbool == true)
    {
        return false;
    }else
    {
        return true;
    }
}







