#include "kommentar_nc.h"



kommentar_nc::kommentar_nc()
{
    setup();
}
kommentar_nc::kommentar_nc(QString text)
{
    setup();
    set_text(text);
}

void kommentar_nc::set_kom(QString kom)
{
    Kommentar = kom;
}
void kommentar_nc::set_afb(QString ausfuehrbedingung)
{
    Afb = ausfuehrbedingung;
}

QString kommentar_nc::kom()
{
    return Kommentar;
}
QString kommentar_nc::afb()
{
    return Afb;
}

void kommentar_nc::setup()
{
    Kommentar.clear();
    Afb = "1";
}

QString kommentar_nc::text()
{
    QString msg = BEARBART_KOMMENTAR;   //Wert 0
    msg += TRENNZ_BEARB_PARAM_;
    msg += kom();                       //Wert 1
    msg += TRENNZ_BEARB_PARAM_;
    msg += afb();                       //Wert 2

    return msg;
}
void kommentar_nc::set_text(QString text)
{
    text_zw tz;
    tz.set_text(text,TRENNZ_BEARB_PARAM);
    if(tz.at(0) == BEARBART_KOMMENTAR)
    {
        set_kom(tz.at(1));
        set_afb(tz.at(2));
    }
}
