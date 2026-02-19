#include "halt_nc.h"

halt_nc::halt_nc()
{
    setup();
}
halt_nc::halt_nc(QString text)
{
    setup();
    set_text(text);
}

void halt_nc::set_afb(QString ausfuehrbedingung)
{
    Afb = ausfuehrbedingung;
}

QString halt_nc::afb()
{
    return Afb;
}

void halt_nc::setup()
{
    Afb = "1";
}

QString halt_nc::text()
{
    QString msg = BEARBART_HALT;        //Wert 0
    msg += TRENNZ_BEARB_PARAM_;
    msg += afb();                       //Wert 1

    return msg;
}
void halt_nc::set_text(QString text)
{
    text_zw tz;
    tz.set_text(text,TRENNZ_BEARB_PARAM);
    if(tz.at(0) == BEARBART_HALT)
    {
        set_afb(tz.at(1));
    }
}
