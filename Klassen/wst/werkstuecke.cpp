#include "werkstuecke.h"


werkstuecke::werkstuecke()
{

}
//#######################################################################
//Public:
//#######################################################################
//--------------------------------------------------set_xy:
bool werkstuecke::neu(werkstueck w)
{
    if(ist_bekannt(w.name()))
    {
        return true;//FEHLER, der Teil gibt es bereits!!!
    }else
    {
        w.unredo_neu();
        Namen.add_hi(w.name());
        Wste.append(w);
    }
    return false;
}
bool werkstuecke::entf(QString werkstueckname)
{
    int wst_index = get_index(werkstueckname);
    if(wst_index >= 0)
    {
        Namen.entf(wst_index, 1);
        Wste.erase(Wste.begin() + wst_index);
        return true;
    }else
    {
        return false;
    }
}
bool werkstuecke::entf_at(uint index)
{
    if(index < (uint)Wste.size())
    {
        Namen.entf(index, 1);
        Wste.erase(Wste.begin() + index);
        return true;
    }else
    {
        return false;
    }
}

//--------------------------------------------------get_xy:
bool werkstuecke::ist_bekannt(QString werkstueckname)
{
    for(uint i=0; i<Namen.count() ;i++)
    {
        if(werkstueckname == Namen.at(i))
        {
            return true;
        }
    }
    return false;
}
int werkstuecke::get_index(QString werkstueckname)
{
    for(uint i=0; i<Namen.count() ;i++)
    {
        if(werkstueckname == Namen.at(i))
        {
            return i;
        }
    }
    return -1;
}
werkstueck* werkstuecke::wst(uint index)
{
    //übergibt eine Kopie des Wst
    //Änderungen an dieser Kopie werden nicht zurück in diese Instanz geschrieben.
    if(index >= 0 && index < Namen.count())
    {
        return &Wste[index];
    }else
    {
        return NULL;
    }
}
QString werkstuecke::name(uint index)
{
    if(index > 0 && index < Namen.count())
    {
        return Namen.at(index);
    }else
    {
        return "";
    }
}
QString werkstuecke::cad_fehler()
{
    QString msg;

    for(uint i=1; i<=anzahl() ;i++)
    {
        werkstueck w = Wste.at(i-1);
        msg += w.cad_fehler();
    }

    return msg;
}

//--------------------------------------------------Manipulationen:
void werkstuecke::clear()
{
    Namen.clear();
    Wste.clear();
}
QString werkstuecke::stdnamen(text_zw namen_alt, text_zw namen_neu)
{
    QString baugruppenname; //Rückgabewert der Funktion
    //erster Durchlauf: Namen tauschen
    for(uint i = 0; i<Namen.count() ;i++)
    {
        QString tmp = Namen.at(i);
        for(uint ii=0; ii<namen_alt.count();ii++)
        {
            if(tmp.contains(namen_alt.at(ii)))
            {
                tmp.replace(namen_alt.at(ii), namen_neu.at(ii));
                Namen.edit(i, tmp);
                break;
            }
        }
    }

    //zweiter Durchlauf: Schranknummer löschen wenn möglich
    QString tmp = Namen.at(0);
    bool schranknummer_wurde_entfernt = false;//wird gebraucht als Prüfung für den 3. Durchlauf
    if(tmp.contains("_"))
    {
        tmp = text_links(tmp, "_");
        bool identisch = true;
        for(uint i = 1; i<Namen.count() ;i++)
        {
            if(  tmp != text_links(Namen.at(i), "_")  )
            {
                identisch = false;
                break;
            }
        }
        bool istbaugruppenname = false;
        if(tmp.at(0) == "S")//Schranknummer
        {
            if(tmp.at(1)=='0' || tmp.at(1)=='1' || tmp.at(1)=='2' || tmp.at(1)=='3' || tmp.at(1)=='4' || tmp.at(1)=='5' || tmp.at(1)=='6' || tmp.at(1)=='7' || tmp.at(1)=='8' || tmp.at(1)=='9')
            {
                istbaugruppenname = true;
            }
        }else if(tmp.at(0)=='#')//Baugruppenbezeichung
        {
            istbaugruppenname = true;
        }else if(tmp.at(0)=='@')//Baugruppenbezeichung nicht im Barcode
        {
            istbaugruppenname = true;
        }

        if(identisch == true && istbaugruppenname == true)
        {
            if(Namen.count() > 0)
            {
                baugruppenname = text_links(Namen.at(0),"_");
                if(baugruppenname.at(0)=='#')
                {
                    baugruppenname = text_rechts(baugruppenname, "#");
                }
            }
            for(uint i = 0; i<Namen.count() ;i++)
            {
                Namen.edit(  i, text_rechts(Namen.at(i),"_")  );
            }
            schranknummer_wurde_entfernt = true;

        }
    }

    //dritter Durchlauf: Nummer hinter Teilenamen löschen wenn möglich
    text_zw bekannte_namen;
    if(schranknummer_wurde_entfernt)
    {
        for(uint i = 0; i<Namen.count() ;i++)//Name für Name durchgehen
        {
            tmp = Namen.at(i);
            QString name_bis_ziffer ="";
            //Namen und Ziffer trennen:
            for(int ii=0; ii<tmp.count() ;ii++)//Namen zeichenweise durchgehen
            {
                if(!ist_ziffer(tmp.at(ii)))
                {
                    name_bis_ziffer += tmp.at(ii);
                }else
                {
                    break;
                }
            }
            //Prüfen, ob es diesen Namen bereits gibt:
            bool bekannt = false;
            for(uint iii=0; iii<bekannte_namen.count() ;iii++)//bekannte Namen nacheinander durchgehen
            {
                if(name_bis_ziffer.length() >0 && bekannte_namen.at(iii) == name_bis_ziffer)
                {
                    bekannt = true;
                    break;
                }
            }
            if(name_bis_ziffer.length() >0 && bekannt == false)//Wenn der Name noch nicht vergeben war
            {
                Namen.edit(i, name_bis_ziffer);
                bekannte_namen.add_hi(name_bis_ziffer);
            }
        }
    }

    //Nameninformatione in den einzenen werkstücken aktualisieren:
    for(uint i=0; i<Namen.count() ;i++)
    {
        werkstueck w = Wste.at(i);
        w.set_name(Namen.at(i));
        Wste.replace(i, w);
    }
    return baugruppenname;
}
void werkstuecke::sortieren()
{
    text_zw    tmp_Namen;
    QVector<werkstueck> tmp_Wste;
    text_zw rankingList;
    rankingList.add_hi("Seite_li");
    rankingList.add_hi("Seite_L");
    rankingList.add_hi("Seite_re");
    rankingList.add_hi("Seite_R");
    rankingList.add_hi("Seite");
    rankingList.add_hi("MS_li");
    rankingList.add_hi("MS_re");
    rankingList.add_hi("MS");
    rankingList.add_hi("OB_li");
    rankingList.add_hi("OB_mi");
    rankingList.add_hi("OB_re");
    rankingList.add_hi("OB");
    rankingList.add_hi("UB_li");
    rankingList.add_hi("UB_mi");
    rankingList.add_hi("UB_re");
    rankingList.add_hi("UB");
    rankingList.add_hi("KB_ob");
    rankingList.add_hi("KB_li");
    rankingList.add_hi("KB_mi");
    rankingList.add_hi("KB_un");
    rankingList.add_hi("KB_re");
    rankingList.add_hi("KB");
    rankingList.add_hi("Trav_ob");
    rankingList.add_hi("Trav_un");
    rankingList.add_hi("Trav_vo");
    rankingList.add_hi("Trav_hi");
    rankingList.add_hi("Trav");
    rankingList.add_hi("Traver_ob");
    rankingList.add_hi("Traver_un");
    rankingList.add_hi("Traver_vo");
    rankingList.add_hi("Traver_hi");
    rankingList.add_hi("Traver");
    rankingList.add_hi("EB_ob");
    rankingList.add_hi("EB_li");
    rankingList.add_hi("EB_mi");
    rankingList.add_hi("EB_un");
    rankingList.add_hi("EB_re");
    rankingList.add_hi("EB");
    rankingList.add_hi("RW_ob");
    rankingList.add_hi("RW_li");
    rankingList.add_hi("RW_mi");
    rankingList.add_hi("RW_un");
    rankingList.add_hi("RW_re");
    rankingList.add_hi("RW");
    rankingList.add_hi("Tuer_li");
    rankingList.add_hi("Tuer_");
    rankingList.add_hi("Tuer_A");
    rankingList.add_hi("Tuer_B");
    rankingList.add_hi("Tuer_C");
    rankingList.add_hi("Tuer_D");
    rankingList.add_hi("Tuer_E");
    rankingList.add_hi("Tuer");
    rankingList.add_hi("Front_li");
    rankingList.add_hi("Front_");
    rankingList.add_hi("Front_A");
    rankingList.add_hi("Front_B");
    rankingList.add_hi("Front_C");
    rankingList.add_hi("Front_D");
    rankingList.add_hi("Front_E");
    rankingList.add_hi("Front");
    rankingList.add_hi("SF_A");
    rankingList.add_hi("SF_B");
    rankingList.add_hi("SF_C");
    rankingList.add_hi("SF_D");
    rankingList.add_hi("SF_E");
    rankingList.add_hi("SF");
    rankingList.add_hi("SS_A");
    rankingList.add_hi("SS_B");
    rankingList.add_hi("SS_C");
    rankingList.add_hi("SS_D");
    rankingList.add_hi("SS_E");
    rankingList.add_hi("SS");
    rankingList.add_hi("SV_A");
    rankingList.add_hi("SV_B");
    rankingList.add_hi("SV_C");
    rankingList.add_hi("SV_D");
    rankingList.add_hi("SV_E");
    rankingList.add_hi("SV");
    rankingList.add_hi("SH_A");
    rankingList.add_hi("SH_B");
    rankingList.add_hi("SH_C");
    rankingList.add_hi("SH_D");
    rankingList.add_hi("SH_E");
    rankingList.add_hi("SH");
    rankingList.add_hi("SB_A");
    rankingList.add_hi("SB_B");
    rankingList.add_hi("SB_C");
    rankingList.add_hi("SB_D");
    rankingList.add_hi("SB_E");
    rankingList.add_hi("SB");
    rankingList.add_hi("Sockel_li");
    rankingList.add_hi("Sockel_mi");
    rankingList.add_hi("Sockel_re");
    rankingList.add_hi("Sockel");
    text_zw kopiert;
    for(uint ii = 0; ii<Namen.count() ;ii++)
    {
        kopiert.add_hi("nein");
    }
    for(uint i = 0; i<rankingList.count() ;i++)
    {
        QString akt_ranking_name =rankingList.at(i);
        for(uint ii = 0; ii<Namen.count() ;ii++)
        {
            QString akt_wst_name = Namen.at(ii);
            if(kopiert.at(ii) != "ja")
            {
                if(akt_wst_name.contains(akt_ranking_name))
                {
                    //kopieren:
                    tmp_Namen.add_hi(akt_wst_name);
                    werkstueck w = Wste.at(ii);
                    w.set_name(akt_wst_name);
                    tmp_Wste.append(w);
                    kopiert.edit(ii, "ja");
                }
            }
        }        
    }

    //2. Durchlauf. Jetzt kommen alle wst die nicht sortierbar waren:
    for(uint i = 0; i<Namen.count() ;i++)
    {
        QString akt_wst_name = Namen.at(i);
        if(kopiert.at(i) != "ja")
        {
            //kopieren:
            tmp_Namen.add_hi(akt_wst_name);
            werkstueck w = Wste.at(i);
            w.set_name(akt_wst_name);
            tmp_Wste.append(w);
            kopiert.edit(i, "ja");
        }
    }
    Namen = tmp_Namen;
    Wste = tmp_Wste;
}

//--------------------------------------------------
//#######################################################################
//Private:
//#######################################################################
//--------------------------------------------------set_xy:

//--------------------------------------------------get_xy:
int werkstuecke::index(QString werkstueckname)
{
    for(uint i=0; i<Namen.count() ;i++)
    {
        if(Namen.at(i)==werkstueckname)
        {
            return i;
        }
    }
    return -1; //Wenn der Name nicht gefunden wurde
}

//--------------------------------------------------Manipulationen:

//--------------------------------------------------



