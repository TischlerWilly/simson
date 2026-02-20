#ifndef FUNKTIONEN_PRGTEXT_H
#define FUNKTIONEN_PRGTEXT_H

#include <QString>

#include "Defines/def_bearbeitungen.h"
#include "Klassen/wst/rechtecktasche.h"
#include "Klassen/wst/bohrung.h"
#include "Klassen/wst/bohrraster.h"
#include "Klassen/wst/nut.h"
#include "Klassen/wst/fraeseraufruf.h"
#include "Klassen/wst/fraesergerade.h"
#include "Klassen/wst/fraeserbogen.h"
#include "Klassen/wst/kommentar_nc.h"
#include "Klassen/wst/halt_nc.h"
#include "Klassen/wst/gehezupunkt.h"


QString bezug(QString b);
QString rta_zu_prgzei(QString text);
QString bohr_zu_prgzei(QString text);
QString bohrRaster_zu_prgzei(QString text);
QString nut_zu_prgzei(QString text);
QString fauf_zu_prgzei(QString text);
QString fgerade_zu_prgzei(QString text);
QString fbogen_zu_prgzei(QString text);
QString kom_zu_prgzei(QString text);
QString halt_zu_prgzei(QString text);
QString gezupu_zu_prgzei(QString text);

#endif // FUNKTIONEN_PRGTEXT_H


