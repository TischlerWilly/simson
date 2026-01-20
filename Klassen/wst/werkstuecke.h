#ifndef WERKSTUECKE_H
#define WERKSTUECKE_H

#include <QMessageBox>
#include <QVector>

#include "werkstueck.h"

class werkstuecke
{
public:
    werkstuecke();

    //--------------------------------------------------set_xy:
    bool neu(werkstueck w);
    bool entf(QString werkstueckname);

    inline void set_name(uint zeilennummer,  QString neuer_name)
    {
        Namen.edit(zeilennummer, neuer_name);
    }
    //--------------------------------------------------get_xy:
    bool        ist_bekannt(QString werkstueckname);
    int        get_index(QString werkstueckname);
    werkstueck* wst(uint index);
    QString     name(uint index);
    QString     cad_fehler();

    inline uint             anzahl()
    {
        return Namen.count();
    }
    inline QString          namen()
    {
        return Namen.text();
    }
    inline text_zw namen_tz()
    {
        return Namen;
    }

    //--------------------------------------------------Manipulationen:
    void clear();
    QString stdnamen(text_zw namen_alt, text_zw namen_neu);
    void sortieren();
    //void ersetzen(werkstueck w, uint index);

    //--------------------------------------------------

private:
    //Variabeln:
    text_zw    Namen;        //namen der Wst
    QVector<werkstueck> Wste;         //hier werden alle Werkstücke gespeichert

    //Funktionen:
    //--------------------------------------------------set_xy:

    //--------------------------------------------------get_xy:
    int    index(QString werkstueckname);

    //--------------------------------------------------Manipulationen:

    //--------------------------------------------------

};

#endif // WERKSTUECKE_H
