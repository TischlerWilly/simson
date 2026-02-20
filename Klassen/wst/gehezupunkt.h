#ifndef GEHEZUPUNKT_H
#define GEHEZUPUNKT_H
#include <QString>

#include "Klassen/text_zw.h"
#include "Defines/def_bearbeitungen.h"
#include "Funktionen/umwandeln.h"
#include "Klassen/geo/punkt3d.h"

class gehezupunkt
{
public:
    gehezupunkt();
    gehezupunkt(QString text);

    void set_x(double x);
    void set_x(QString x);
    void set_y(double y);
    void set_y(QString y);
    void set_z(double z);
    void set_z(QString z);
    void set_pos(punkt3d p);
    void set_afb(QString ausfuehrbedingung);

    double  x();
    QString x_qstring();
    double  y();
    QString y_qstring();
    double  z();
    QString z_qstring();
    punkt3d  pos();
    QString afb();

    QString text();
    void set_text(QString text);

protected:
    punkt3d Position;
    QString Afb;

    void setup();
};

#endif // GEHEZUPUNKT_H
