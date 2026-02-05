#include "rechtecktasche.h"

rechtecktasche::rechtecktasche()
{
    setup();
}

rechtecktasche::rechtecktasche(QString text)
{
    setup();
    set_text(text);
}

void rechtecktasche::setup()
{
    Laenge = 0;
    Breite = 0;
    Tiefe = 0;
    Drewinkel = 0;
    Eckenradius = 0;
    Zustellmass = 0;
    Ausraeumen = true;
    Bezug = WST_BEZUG_OBSEI;
    Afb = "1";
    Werkzeugnummer = "void";
    IstZapfen = false;
}

void rechtecktasche::set_laenge(double l)
{
    if(l > 0)
    {
        Laenge = l;
    }
}
void rechtecktasche::set_laenge(QString l)
{
    set_laenge(l.toDouble());
}
void rechtecktasche::set_breite(double b)
{
    if(b > 0)
    {
        Breite = b;
    }
}
void rechtecktasche::set_breite(QString b)
{
    set_breite(b.toDouble());
}
void rechtecktasche::set_tiefe(double t)
{
    if(t > 0)
    {
        Tiefe = t;
    }
}
void rechtecktasche::set_tiefe(QString t)
{
    set_tiefe(t.toDouble());
}
void rechtecktasche::set_x(double x)
{
    Mipu.set_x(x);
}
void rechtecktasche::set_x(QString x)
{
    Mipu.set_x(x);
}
void rechtecktasche::set_y(double y)
{
    Mipu.set_y(y);
}
void rechtecktasche::set_y(QString y)
{
    Mipu.set_y(y);
}
void rechtecktasche::set_z(double z)
{
    Mipu.set_z(z);
}
void rechtecktasche::set_z(QString z)
{
    Mipu.set_z(z);
}
void rechtecktasche::set_mipu(punkt3d p)
{
    Mipu = p;
}
void rechtecktasche::set_drewi(double wi)
{
    while(wi > 180)
    {
        wi = wi -180;
    }
    Drewinkel = wi;
}
void rechtecktasche::set_drewi(QString wi)
{
    set_drewi(wi.toDouble());
}
void rechtecktasche::set_rad(double r)
{
    if(r >= 0)
    {
        Eckenradius = r;
    }
}
void rechtecktasche::set_rad(QString r)
{
    set_rad(r.toDouble());
}
void rechtecktasche::set_zustellmass(double mass)
{
    if(mass >= 0)
    {
        Zustellmass = mass;
    }
}
void rechtecktasche::set_zustellmass(QString mass)
{
    set_zustellmass(mass.toDouble());
}
void rechtecktasche::set_ausraeumen(double ja)
{
    if(ja > 0)
    {
        Ausraeumen = true;
    }else
    {
        Ausraeumen = false;
    }
}
void rechtecktasche::set_ausraeumen(QString ja)
{
    set_ausraeumen(ja.toDouble());
}
void rechtecktasche::set_bezug(QString bezugsflaeche)
{
    Bezug = bezugsflaeche;
}
void rechtecktasche::set_afb(QString ausfuehrbedingung)
{
    Afb = ausfuehrbedingung;
}
void rechtecktasche::set_wkznum(QString nummer)
{
    Werkzeugnummer = nummer;
}
void rechtecktasche::set_zapfen(bool ist_zapfen)
{
    IstZapfen = ist_zapfen;
}
void rechtecktasche::set_zapfen(QString ist_zapfen)
{
    if(ist_zapfen == "ja")
    {
        IstZapfen = true;
    }else
    {
        IstZapfen = false;
    }
}


double rechtecktasche::laenge()
{
    return Laenge;
}
QString rechtecktasche::laenge_qstring()
{
    return double_to_qstring(Laenge);
}
double rechtecktasche::breite()
{
    return Breite;
}
QString rechtecktasche::breite_qstring()
{
    return double_to_qstring(Breite);
}
double rechtecktasche::tiefe()
{
    return Tiefe;
}
QString rechtecktasche::tiefe_qstring()
{
    return double_to_qstring(Tiefe);
}
double rechtecktasche::x()
{
    return Mipu.x();
}
QString rechtecktasche::x_qstring()
{
    return Mipu.x_QString();
}
double rechtecktasche::y()
{
    return Mipu.y();
}
QString rechtecktasche::y_qstring()
{
    return Mipu.y_QString();
}
double rechtecktasche::z()
{
    return Mipu.z();
}
QString rechtecktasche::z_qstring()
{
    return Mipu.z_QString();
}
punkt3d rechtecktasche::mipu()
{
    return  Mipu;
}
double rechtecktasche::drewi()
{
    return Drewinkel;
}
QString rechtecktasche::drewi_qstring()
{
    return double_to_qstring(Drewinkel);
}
double rechtecktasche::rad()
{
    return Eckenradius;
}
QString rechtecktasche::rad_qstring()
{
    return double_to_qstring(Eckenradius);
}
double rechtecktasche::zustellmass()
{
    return Zustellmass;
}
QString rechtecktasche::zustellmass_qstring()
{
    return double_to_qstring(Zustellmass);
}
bool rechtecktasche::ausraeumen()
{
    return Ausraeumen;
}
QString rechtecktasche::ausraeumen_qstring()
{
    if(Ausraeumen == true)
    {
        return "1";
    }else
    {
        return "0";
    }
}
QString rechtecktasche::bezug()
{
    return Bezug;
}
QString rechtecktasche::afb()
{
    return Afb;
}
QString rechtecktasche::wkznum()
{
    return Werkzeugnummer;
}
bool rechtecktasche::istZapfen()
{
    return IstZapfen;
}
QString rechtecktasche::istZapfen_qstring()
{
    if(IstZapfen == true)
    {
        return "ja";
    }else
    {
        return "nein";
    }
}

QString rechtecktasche::text()
{
    QString msg = BEARBART_RTA;  //Wert 0
    msg += TRENNZ_BEARB_PARAM_;
    msg += bezug();             //Wert 1
    msg += TRENNZ_BEARB_PARAM_;
    msg += laenge_qstring();    //Wert 2
    msg += TRENNZ_BEARB_PARAM_;
    msg += breite_qstring();    //Wert 3
    msg += TRENNZ_BEARB_PARAM_;
    msg += tiefe_qstring();     //Wert 4
    msg += TRENNZ_BEARB_PARAM_;
    msg += x_qstring();         //Wert 5
    msg += TRENNZ_BEARB_PARAM_;
    msg += y_qstring();         //Wert 6
    msg += TRENNZ_BEARB_PARAM_;
    msg += z_qstring();         //Wert 7
    msg += TRENNZ_BEARB_PARAM_;
    msg += drewi_qstring();     //Wert 8
    msg += TRENNZ_BEARB_PARAM_;
    msg += rad_qstring();       //Wert 9
    msg += TRENNZ_BEARB_PARAM_;
    msg += ausraeumen_qstring();//Wert 10
    msg += TRENNZ_BEARB_PARAM_;
    msg += afb();               //Wert 11
    msg += TRENNZ_BEARB_PARAM_;
    msg += zustellmass_qstring();//Wert 12
    msg += TRENNZ_BEARB_PARAM_;
    msg += wkznum();            //Wert 13
    msg += TRENNZ_BEARB_PARAM_;
    msg += istZapfen_qstring(); //Wert 14

    return msg;
}
void rechtecktasche::set_text(QString text)
{
    text_zw tz;
    tz.set_text(text,TRENNZ_BEARB_PARAM);
    if(tz.at(0) == BEARBART_RTA)
    {
        set_bezug(tz.at(1));
        set_laenge(tz.at(2));
        set_breite(tz.at(3));
        set_tiefe(tz.at(4));
        set_x(tz.at(5));
        set_y(tz.at(6));
        set_z(tz.at(7));
        set_drewi(tz.at(8));
        set_rad(tz.at(9));
        set_ausraeumen(tz.at(10));
        set_afb(tz.at(11));
        set_zustellmass(tz.at(12));
        set_wkznum(tz.at(13));
        set_zapfen(tz.at(14));
    }
}













