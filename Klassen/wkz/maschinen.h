#ifndef MASCHINEN_H
#define MASCHINEN_H

#include <QVector>

#include "maschine.h"

class maschinen
{
public:
    maschinen();

    //--------------------------------------------------set_xy:
    bool neu(maschine masch);
    bool entf(QString masch_name);

    //--------------------------------------------------get_xy:
    bool ist_bekannt(QString masch_name);
    int get_index(QString masch_name);
    maschine* masch(uint index);
    QString name(uint index);

    inline uint anzahl()
    {
        return Namen.count();
    }
    inline QString namen()
    {
        return Namen.text();
    }
    inline text_zw namen_tz()
    {
        return Namen;
    }

    //--------------------------------------------------Manipulationen:
    void clear();

    //--------------------------------------------------

private:
    //Variabeln:
    text_zw    Namen;        //namen der Wst
    QVector<maschine> Maschinen;         //hier werden alle Werkstücke gespeichert

    //Funktionen:
    //--------------------------------------------------set_xy:

    //--------------------------------------------------get_xy:
    int    index(QString masch_name);

    //--------------------------------------------------Manipulationen:

    //--------------------------------------------------
};

#endif // MASCHINEN_H
