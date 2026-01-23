#include "werkstueck.h"

werkstueck::werkstueck()
{
    Laenge  = 0;
    Breite  = 0;
    Dicke   = 0;
    Versatz_x = 0;
    Versatz_y = 0;
    Versatz_z = 0;
}
werkstueck::werkstueck(QString neuer_name)
{
    Name    = neuer_name;
    Laenge  = 0;
    Breite  = 0;
    Dicke   = 0;
    Versatz_x = 0;
    Versatz_y = 0;
    Versatz_z = 0;
}
//#######################################################################
//Public:
//#######################################################################
//--------------------------------------------------set_xy:
void werkstueck::set_laenge(double l)
{
    if(l>0 && l<5000)
    {
        Laenge = l;
    }
}
void werkstueck::set_laenge(QString l)
{
    set_laenge(l.toDouble());
}
void werkstueck::set_breite(double b)
{
    if(b>0 && b<5000)
    {
        Breite = b;
    }
}
void werkstueck::set_breite(QString b)
{
    set_breite(b.toDouble());
}
void werkstueck::set_dicke(double d)
{
    if(d>0 && d<200)
    {
        Dicke = d;
    }
}
void werkstueck::set_dicke(QString d)
{
    set_dicke(d.toDouble());
}
void werkstueck::neue_bearbeitung(QString text)
{
    bool bereits_vorhanden = false;
    for(uint i=0; i<Bearb.count() ;i++)
    {
        if(Bearb.at(i) == text)
        {
            text_zw zeile;//Folgezeile
            zeile.set_text(text, TRENNZ_BEARB_PARAM);
            if(zeile.at(0) == BEARBART_BOHR)//Nur doppelte Bohrungen unterdrücken, Doppelte Fräsbahnen können gewollt sein!
            {
                bereits_vorhanden = true;
                break;
            }
        }
    }
    if(bereits_vorhanden == false)
    {
        Bearb.add_hi(text);
    }
}
void werkstueck::set_bearb(text_zw b)
{
    Bearb = b;
}
void werkstueck::set_name(QString neuer_name)
{
    Name = neuer_name;
}
void werkstueck::set_versatz_x(double versatz)
{
    Versatz_x = versatz;
}
void werkstueck::set_versatz_y(double versatz)
{
    Versatz_y = versatz;
}
void werkstueck::set_versatz_z(double versatz)
{
    Versatz_z = versatz;
}
//--------------------------------------------------get_xy:

QString werkstueck::cad_fehler(bool kurz)
{
    QString msg;
    if(Name.contains("Tuer") || \
       Name.contains("Tur")      )
    {
        uint anz_asd = 0; //Anzahl Aufschlagdämpfer

        for(uint i=0; i<Bearb.count() ;i++)
        {
            if(Bearb.at(i).contains(BEARBART_BOHR))
            {
                bohrung bo(Bearb.at(i));
                if(bo.dm() == 6 && bo.tiefe() < dicke())
                {
                    anz_asd++;
                }else if(bo.dm() == 5 && bo.tiefe() == 14)
                {
                    anz_asd++;
                }
            }
        }
        if(anz_asd < 2)
        {
            if(kurz)
            {
                if(!msg.isEmpty())
                {
                    msg += " / ";
                }
                msg += double_to_qstring(anz_asd);
                msg += " Aufschlagdämpfer";
            }else
            {
                msg += name();
                msg += ": ";
                msg += double_to_qstring(anz_asd);
                msg += " Aufschlagdämpfer";
                msg += "\n";
            }
        }
    }
    if(Name.contains("SchubFront") || \
       Name.contains("Schubfront") || \
       Name.contains("SF")                )
    {
        uint anz_asd = 0; //Anzahl Aufschlagdämpfer

        for(uint i=0; i<Bearb.count() ;i++)
        {
            if(Bearb.at(i).contains(BEARBART_BOHR))
            {
                bohrung bo(Bearb.at(i));
                if(bo.dm() == 6 && bo.tiefe() < dicke())
                {
                    anz_asd++;
                }else if(bo.dm() == 5 && bo.tiefe() == 14)
                {
                    anz_asd++;
                }
            }
        }
        if(anz_asd < 4)
        {
            if(kurz)
            {
                if(!msg.isEmpty())
                {
                    msg += " / ";
                }
                msg += double_to_qstring(anz_asd);
                msg += " Aufschlagdämpfer";
            }else
            {
                msg += name();
                msg += ": ";
                msg += double_to_qstring(anz_asd);
                msg += " Aufschlagdämpfer";
                msg += "\n";
            }
        }
    }
    return msg;
}
geo_text werkstueck::geo(wkz_magazin wkzm)
{
    geo_text gt = geo_ermitteln(Bearb, Laenge, Breite, Dicke, Versatz_x, Versatz_y, wkzm);
    return gt;
}
geo_text werkstueck::geo_aktfkon(wkz_magazin wkzm)
{
    geo_text fkon = geo_ermitteln_akt_fkon(Bearb, Versatz_x, Versatz_y, wkzm);
    return fkon;
}

//--------------------------------------------------Manipulationen:
void werkstueck::undo()
{
    Laenge = UnReDo_L.undo();
    Breite = UnReDo_B.undo();
    Dicke = UnReDo_D.undo();
    Bearb = UnReDo.undo();
}
void werkstueck::redo()
{
    Laenge = UnReDo_L.redo();
    Breite = UnReDo_B.redo();
    Dicke = UnReDo_D.redo();
    Bearb = UnReDo.redo();
}
void werkstueck::unredo_neu()
{
    UnReDo_L.neu(Laenge);
    UnReDo_B.neu(Breite);
    UnReDo_D.neu(Dicke);
    UnReDo.neu(Bearb);
}
void werkstueck::unredo_clear()
{
    UnReDo_L.clear();
    UnReDo_B.clear();
    UnReDo_D.clear();
    UnReDo.clear();
}

//#######################################################################
//Private:
//#######################################################################





