#include "gehezupunkt.h"

gehezupunkt::gehezupunkt()
{
    setup();
}
gehezupunkt::gehezupunkt(QString text)
{
    setup();
    set_text(text);
}

void gehezupunkt::setup()
{
    Afb = "1";
}

void gehezupunkt::set_x(double x)
{
    Position.set_x(x);
}
void gehezupunkt::set_x(QString x)
{
    Position.set_x(x);
}
void gehezupunkt::set_y(double y)
{
    Position.set_y(y);
}
void gehezupunkt::set_y(QString y)
{
    Position.set_y(y);
}
void gehezupunkt::set_z(double z)
{
    Position.set_z(z);
}
void gehezupunkt::set_z(QString z)
{
    Position.set_z(z);
}
void gehezupunkt::set_pos(punkt3d p)
{
    Position = p;
}
void gehezupunkt::set_afb(QString ausfuehrbedingung)
{
    Afb = ausfuehrbedingung;
}

double gehezupunkt::x()
{
    return Position.x();
}
QString gehezupunkt::x_qstring()
{
    return Position.x_QString();
}
double gehezupunkt::y()
{
    return Position.y();
}
QString gehezupunkt::y_qstring()
{
    return Position.y_QString();
}
double gehezupunkt::z()
{
    return Position.z();
}
QString gehezupunkt::z_qstring()
{
    return Position.z_QString();
}
punkt3d gehezupunkt::pos()
{
    return Position;
}
QString gehezupunkt::afb()
{
    return Afb;
}

QString gehezupunkt::text()
{
    QString msg = BEARBART_GEZUPU;    //Wert 0
    msg += TRENNZ_BEARB_PARAM_;
    msg += x_qstring();         //Wert 1
    msg += TRENNZ_BEARB_PARAM_;
    msg += y_qstring();         //Wert 2
    msg += TRENNZ_BEARB_PARAM_;
    msg += z_qstring();         //Wert 3
    msg += TRENNZ_BEARB_PARAM_;
    msg += afb();               //Wert 4

    return msg;
}
void gehezupunkt::set_text(QString text)
{
    text_zw tz;
    tz.set_text(text,TRENNZ_BEARB_PARAM);
    if(tz.at(0) == BEARBART_GEZUPU)
    {
        set_x(tz.at(1));
        set_y(tz.at(2));
        set_z(tz.at(3));
        set_afb(tz.at(4));
    }
}
