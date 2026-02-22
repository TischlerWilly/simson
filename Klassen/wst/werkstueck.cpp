#include "werkstueck.h"

werkstueck::werkstueck()
{
    Laenge  = 0;
    Breite  = 0;
    Dicke   = 0;
    Versatz_x = 0;
    Versatz_y = 0;
    Versatz_z = 0;
    Prgend_x = "AUTO";
    Prgend_y = "AUTO";
    Prgend_z = "AUTO";
    SichAbst = 40;
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
    Prgend_x = "AUTO";
    Prgend_y = "AUTO";
    Prgend_z = "AUTO";
    SichAbst = 40;
}
//#######################################################################
//Public:
//#######################################################################
//--------------------------------------------------set_xy:
void werkstueck::set_text(QString text)
{
    text_zw pkopf;
    pkopf.set_text(text_mitte(text, "[Programmkopf]\n", "[ENDE_Programmkopf]"),'\n');
    for(uint i=0 ; i< pkopf.count() ;i++)
    {
        QString zeile = pkopf.at(i);
        if(zeile.contains("Name:"))
        {
            set_name(text_rechts(zeile, "Name:"));
        }else if(zeile.contains("WST-Laenge:"))
        {
            set_laenge(text_rechts(zeile, "WST-Laenge:"));
        }else if(zeile.contains("WST-Breite:"))
        {
            set_breite(text_rechts(zeile, "WST-Breite:"));
        }else if(zeile.contains("WST-Dicke:"))
        {
            set_dicke(text_rechts(zeile, "WST-Dicke:"));
        }else if(zeile.contains("Versatz X:"))
        {
            set_versatz_x(text_rechts(zeile, "Versatz X:").toDouble());
        }else if(zeile.contains("Versatz Y:"))
        {
            set_versatz_y(text_rechts(zeile, "Versatz Y:").toDouble());
        }else if(zeile.contains("Versatz Z:"))
        {
            set_versatz_z(text_rechts(zeile, "Versatz Z:").toDouble());
        }else if(zeile.contains("Programmende X:"))
        {
            set_prgend_x(text_rechts(zeile, "Programmende X:"));
        }else if(zeile.contains("Programmende Y:"))
        {
            set_prgend_y(text_rechts(zeile, "Programmende Y:"));
        }else if(zeile.contains("Programmende Z:"))
        {
            set_prgend_z(text_rechts(zeile, "Programmende Z:"));
        }else if(zeile.contains("Sicherheitsabstand:"))
        {
            set_sichabst(text_rechts(zeile, "Sicherheitsabstand:"));
        }
    }

    Bearb.set_text(text_mitte(text, "[Bearbeitungen]\n", "[ENDE_Bearbeitungen]"),'\n');
}
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
    if(d>0 && d<300)
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
void werkstueck::set_dateipfad(QString pfad)
{
    if(!pfad.isEmpty())
    {
        Dateipfad = pfad;
        QFileInfo info(pfad);
        set_name(info.baseName());
    }
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
void werkstueck::set_prgend_x(QString endpos)
{
    Prgend_x = endpos;
}
void werkstueck::set_prgend_y(QString endpos)
{
    Prgend_y = endpos;
}
void werkstueck::set_prgend_z(QString endpos)
{
    Prgend_z = endpos;
}
void werkstueck::set_sichabst(double abst)
{
    if(abst>0 && abst<300)
    {
        SichAbst = abst;
    }
}
void werkstueck::set_sichabst(QString abst)
{
    set_sichabst(abst.toDouble());
}
void werkstueck::wurde_gespeichert()
{
    unredo_clear();
    unredo_neu();
}
//--------------------------------------------------get_xy:

QString werkstueck::text()
{
    QString msg;

    msg += "[Programmkopf]\n";

    msg += "Name:";
    msg += name();
    msg += "\n";

    msg += "WST-Laenge:";
    msg += laenge_qstring();
    msg += "\n";

    msg += "WST-Breite:";
    msg += breite_qstring();
    msg += "\n";

    msg += "WST-Dicke:";
    msg += dicke_qstring();
    msg += "\n";

    msg += "Versatz X:";
    msg += versatz_x_qstring();
    msg += "\n";

    msg += "Versatz Y:";
    msg += versatz_y_qstring();
    msg += "\n";

    msg += "Versatz Z:";
    msg += versatz_z_qstring();
    msg += "\n";

    msg += "Programmende X:";
    msg += prgend_x();
    msg += "\n";

    msg += "Programmende Y:";
    msg += prgend_y();
    msg += "\n";

    msg += "Programmende Z:";
    msg += prgend_z();
    msg += "\n";

    msg += "Sicherheitsabstand:";
    msg += sichabst_qstring();
    msg += "\n";

    msg += "[ENDE_Programmkopf]";

    msg += "\n";
    msg += "---------------------------------------------------";
    msg += "\n";

    msg += "[Bearbeitungen]\n";
    msg += bearb().text();
    msg += "\n";
    msg += "[ENDE_Bearbeitungen]";

    return msg;
}
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
geo_text werkstueck::geo_leitliniefkon(wkz_magazin wkzm)
{
    geo_text fkon = geo_ermitteln_leitlinie_fkon(Bearb, Versatz_x, Versatz_y, wkzm);
    return fkon;
}
//--------------------------------------------------Manipulationen:
void werkstueck::undo()
{
    Laenge = UnReDo_L.undo();
    Breite = UnReDo_B.undo();
    Dicke = UnReDo_D.undo();
    Bearb = UnReDo.undo();
    Versatz_x = UnReDo_versatz_x.undo();
    Versatz_y = UnReDo_versatz_y.undo();
    Versatz_z = UnReDo_versatz_z.undo();
    Prgend_x = UnReDo_prgend_x.undo();
    Prgend_y = UnReDo_prgend_y.undo();
    Prgend_z = UnReDo_prgend_z.undo();
    SichAbst = UnReDo_sichabst.undo();
}
void werkstueck::redo()
{
    Laenge = UnReDo_L.redo();
    Breite = UnReDo_B.redo();
    Dicke = UnReDo_D.redo();
    Bearb = UnReDo.redo();
    Versatz_x = UnReDo_versatz_x.redo();
    Versatz_y = UnReDo_versatz_y.redo();
    Versatz_z = UnReDo_versatz_z.redo();
    Prgend_x = UnReDo_prgend_x.redo();
    Prgend_y = UnReDo_prgend_y.redo();
    Prgend_z = UnReDo_prgend_z.redo();
    SichAbst = UnReDo_sichabst.redo();
}
void werkstueck::unredo_neu()
{
    bool anders = false;
    if(UnReDo_L.akt_elem() != Laenge)
    {
        anders = true;
    }
    if(UnReDo_B.akt_elem() != Breite)
    {
        anders = true;
    }
    if(UnReDo_D.akt_elem() != Dicke)
    {
        anders = true;
    }
    if(UnReDo.akt_elem().text() != Bearb.text())
    {
        anders = true;
    }
    if(UnReDo_versatz_x.akt_elem() != Versatz_x)
    {
        anders = true;
    }
    if(UnReDo_versatz_y.akt_elem() != Versatz_y)
    {
        anders = true;
    }
    if(UnReDo_versatz_z.akt_elem() != Versatz_z)
    {
        anders = true;
    }
    if(UnReDo_prgend_x.akt_elem() != Prgend_x)
    {
        anders = true;
    }
    if(UnReDo_prgend_y.akt_elem() != Prgend_y)
    {
        anders = true;
    }
    if(UnReDo_prgend_z.akt_elem() != Prgend_z)
    {
        anders = true;
    }
    if(UnReDo_sichabst.akt_elem() != SichAbst)
    {
        anders = true;
    }

    if(anders == true)
    {
        UnReDo_L.neu(Laenge);
        UnReDo_B.neu(Breite);
        UnReDo_D.neu(Dicke);
        UnReDo.neu(Bearb);
        UnReDo_versatz_x.neu(Versatz_x);
        UnReDo_versatz_y.neu(Versatz_y);
        UnReDo_versatz_z.neu(Versatz_z);
        UnReDo_prgend_x.neu(Prgend_x);
        UnReDo_prgend_y.neu(Prgend_y);
        UnReDo_prgend_z.neu(Prgend_z);
        UnReDo_sichabst.neu(SichAbst);
    }
}
void werkstueck::unredo_clear()
{
    UnReDo_L.clear();
    UnReDo_B.clear();
    UnReDo_D.clear();
    UnReDo.clear();
    UnReDo_versatz_x.clear();
    UnReDo_versatz_y.clear();
    UnReDo_versatz_z.clear();
    UnReDo_prgend_x.clear();
    UnReDo_prgend_y.clear();
    UnReDo_prgend_z.clear();
    UnReDo_sichabst.clear();
}

//#######################################################################
//Private:
//#######################################################################





