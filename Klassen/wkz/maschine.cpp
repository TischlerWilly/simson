#include "maschine.h"

maschine::maschine()
{
    Laenge  = 700;
    Breite  = 700;
    Ausgabeformat = "kein";
}
maschine::maschine(QString neuer_name)
{
    Name    = neuer_name;
    Laenge  = 700;
    Breite  = 700;
    Ausgabeformat = "kein";
}

//--------------------------------------------------set_xy:
void maschine::set_text(QString t)
{
    text_zw tz;
    tz.set_text(t,'\n');
    for(uint i=0; i<tz.count() ;i++)
    {
        text_zw spalten;
        spalten.set_text(tz.at(i),'\t');
        if(spalten.at(0) == "Laenge:")
        {
            set_laenge(spalten.at(1));
        }else if(spalten.at(0) == "Breite:")
        {
            set_breite(spalten.at(1));
        }else if(spalten.at(0) == "Ausgabeformat:")
        {
            set_ausgabeformat(spalten.at(1));
        }
    }
}
void maschine::set_laenge(double l)
{
    if(l>0 && l<5000)
    {
        Laenge = l;
    }
}
void maschine::set_laenge(QString l)
{
    set_laenge(l.toDouble());
}
void maschine::set_breite(double b)
{
    if(b>0 && b<5000)
    {
        Breite = b;
    }
}
void maschine::set_breite(QString b)
{
    set_breite(b.toDouble());
}
void maschine::set_ausgabeformat(QString f)
{
    Ausgabeformat = f;
}
void maschine::set_name(QString neuer_name)
{
    Name = neuer_name;
}
void maschine::set_wkzmag(wkz_magazin wkzmag)
{
    Wkzmag = wkzmag;
}

//--------------------------------------------------get_xy:
QString maschine::text()
{
    QString text;

    text += "Laenge:";
    text += "\t";
    text += laenge_qstring();
    text += "\n";

    text += "Breite:";
    text += "\t";
    text += breite_qstring();
    text += "\n";

    text += "Ausgabeformat:";
    text += "\t";
    text += ausgabeformat();
    text += "\n";

    return text;
}
