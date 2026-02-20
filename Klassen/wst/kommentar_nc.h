#ifndef KOMMENTAR_NC_H
#define KOMMENTAR_NC_H

#include <QString>

#include "Klassen/text_zw.h"
#include "Defines/def_bearbeitungen.h"

class kommentar_nc
{
public:
    kommentar_nc();
    kommentar_nc(QString text);

    void set_kom(QString kom);
    void set_afb(QString ausfuehrbedingung);

    QString kom();
    QString afb();

    QString text();
    void set_text(QString text);

protected:
    QString Kommentar;
    QString Afb;

    void setup();

};

#endif // KOMMENTAR_NC_H
