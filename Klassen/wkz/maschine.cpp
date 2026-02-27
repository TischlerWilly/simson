#include "maschine.h"

maschine::maschine()
{
    Laenge  = 700;
    Breite  = 700;
    Ausgabeformat = "kein";
    DrehzExportieren = false;
    Prgendpos.set_x(100);
    Prgendpos.set_y(100);
    Prgendpos.set_z(100);
    Zugabe_DuBoTi = 0.3;
    Zugabe_DuTaTi = 0.3;
    WkzWechselText = "\nM0 (Bitte Werkzeug: \"[WKZNR]\" einwechseln)\n";
}
maschine::maschine(QString neuer_name)
{
    Name    = neuer_name;
    Laenge  = 700;
    Breite  = 700;
    Ausgabeformat = "kein";
    DrehzExportieren = false;
    Prgendpos.set_x(100);
    Prgendpos.set_y(100);
    Prgendpos.set_z(100);
    Zugabe_DuBoTi = 0.3;
    Zugabe_DuTaTi = 0.3;
    WkzWechselText = "M0 (Bitte Werkzeug: \" [WKZNR] \" einwechseln)\n";
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
        }else if(spalten.at(0) == "PrgEndPos X:")
        {
            set_prgenpos_x(spalten.at(1));
        }else if(spalten.at(0) == "PrgEndPos Y:")
        {
            set_prgenpos_y(spalten.at(1));
        }else if(spalten.at(0) == "PrgEndPos Z:")
        {
            set_prgenpos_z(spalten.at(1));
        }else if(spalten.at(0) == "Zugabe DuBoTi:")
        {
            set_zugabe_duboti(spalten.at(1));
        }else if(spalten.at(0) == "Zugabe DuTaTi:")
        {
            set_zugabe_dutati(spalten.at(1));
        }else if(spalten.at(0) == "Ausgabeformat:")
        {
            set_ausgabeformat(spalten.at(1));
        }else if(spalten.at(0) == "DrehzExportieren:")
        {
            set_drehzExportieren(spalten.at(1));
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
    set_breite(ausdruck_auswerten(l).toDouble());
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
    set_breite(ausdruck_auswerten(b).toDouble());
}
void maschine::set_prgenpos_x(double pos)
{
    if(pos>0 && pos<5000)
    {
        Prgendpos.set_x(pos);
    }
}
void maschine::set_prgenpos_x(QString pos)
{
    set_prgenpos_x(ausdruck_auswerten(pos).toDouble());
}
void maschine::set_prgenpos_y(double pos)
{
    if(pos>0 && pos<5000)
    {
        Prgendpos.set_y(pos);
    }
}
void maschine::set_prgenpos_y(QString pos)
{
    set_prgenpos_y(ausdruck_auswerten(pos).toDouble());
}
void maschine::set_prgenpos_z(double pos)
{
    if(pos>0 && pos<500)
    {
        Prgendpos.set_z(pos);
    }
}
void maschine::set_prgenpos_z(QString pos)
{
    set_prgenpos_z(ausdruck_auswerten(pos).toDouble());
}
void maschine::set_zugabe_duboti(double zugabe)
{
    if(zugabe>0 && zugabe<10)
    {
        Zugabe_DuBoTi = zugabe;
    }
}
void maschine::set_zugabe_duboti(QString zugabe)
{
    set_zugabe_duboti(ausdruck_auswerten(zugabe).toDouble());
}
void maschine::set_zugabe_dutati(double zugabe)
{
    if(zugabe>0 && zugabe<10)
    {
        Zugabe_DuTaTi = zugabe;
    }
}
void maschine::set_zugabe_dutati(QString zugabe)
{
    set_zugabe_dutati(ausdruck_auswerten(zugabe).toDouble());
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
void maschine::set_wkzWechselText(QString text)
{
    WkzWechselText = text;
}
void maschine::set_drehzExportieren(bool jn)
{
    DrehzExportieren = jn;
}
void maschine::set_drehzExportieren(QString jn)
{
    if(jn == "ja")
    {
        DrehzExportieren = true;
    }else
    {
        DrehzExportieren = false;
    }
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

    text += "PrgEndPos X:";
    text += "\t";
    text += prgenpos_x_qstring();
    text += "\n";

    text += "PrgEndPos Y:";
    text += "\t";
    text += prgenpos_y_qstring();
    text += "\n";

    text += "PrgEndPos Z:";
    text += "\t";
    text += prgenpos_z_qstring();
    text += "\n";

    text += "Zugabe DuBoTi:";
    text += "\t";
    text += zugabe_duboti_qstring();
    text += "\n";

    text += "Zugabe DuTaTi:";
    text += "\t";
    text += zugabe_dutati_qstring();
    text += "\n";

    text += "Ausgabeformat:";
    text += "\t";
    text += ausgabeformat();
    text += "\n";

    text += "DrehzExportieren:";
    text += "\t";
    if(drehzExportieren() == true)
    {
        text += "ja";
    }else
    {
        text += "nein";
    }
    text += "\n";

    return text;
}
