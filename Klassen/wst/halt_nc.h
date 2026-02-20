#ifndef HALT_NC_H
#define HALT_NC_H

#include <QString>

#include "Klassen/text_zw.h"
#include "Defines/def_bearbeitungen.h"

class halt_nc
{
public:
    halt_nc();
    halt_nc(QString text);

    void set_afb(QString ausfuehrbedingung);

    QString afb();

    QString text();
    void set_text(QString text);

protected:
    QString Afb;

    void setup();
};

#endif // HALT_NC_H
