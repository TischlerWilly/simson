#include "geo_text.h"

geo_text::geo_text()
{
    clear();
}

void geo_text::clear()
{
    Daten.clear();
    AktIndex = 0;
    Zeivor = false;
}
void geo_text::zeilenvorschub()
{
    Zeivor = true;
}
//--------------------------------------set:
void geo_text::add(text_zw geometrie, uint index)
{
    text_zw geometrieen;
    geometrieen.set_trenz(TRZ_EL_);
    if(Zeivor == true)
    {
        //Zeile hinten anhängen:
        geometrieen.add_hi(geometrie.text());
        Daten.push_back(geometrieen);
        AktIndex++;
        Zeivor = false;
    }else
    {
        if(index + 1 > Daten.size())
        {
            //Zeile hinten anhängen:
            geometrieen.add_hi(geometrie.text());
            Daten.push_back(geometrieen);
        }else
        {
            //Zeile verlängern
            geometrieen = Daten.at(index);
            geometrieen.add_hi(geometrie.text());
            Daten.at(index) = geometrieen;
        }
    }
}
void geo_text::add_leerzeile()
{
    text_zw geo;
    geo.set_text("leerzeile", TRZ_PA_);
    add(geo, akt_index());
}
void geo_text::add_punkt(punkt3d p)
{
    add_punkt(p, AktIndex);
}
void geo_text::add_punkt(punkt3d p, uint index)
{
    text_zw geo;
    geo.set_text(p.text(), TRZ_PA_);
    add(geo, index);
}
void geo_text::add_strecke(strecke s)
{
    add_strecke(s, AktIndex);
}
void geo_text::add_strecke(strecke s, uint index)
{
    text_zw geo;
    geo.set_text(s.text(), TRZ_PA_);
    add(geo, index);
}
void geo_text::add_bogen(bogen b)
{
    add_bogen(b, AktIndex);
}
void geo_text::add_bogen(bogen b, uint index)
{
    text_zw geo;
    geo.set_text(b.text(), TRZ_PA_);
    add(geo, index);
}
void geo_text::add_kreis(kreis k)
{
    add_kreis(k, AktIndex);
}
void geo_text::add_kreis(kreis k, uint index)
{
    text_zw geo;
    geo.set_text(k.text(), TRZ_PA_);
    add(geo, index);
}
void geo_text::add_rechteck(rechteck rec)
{
    add_rechteck(rec, AktIndex);
}
void geo_text::add_rechteck(rechteck rec, uint index)
{
    text_zw geo;
    geo.set_trenz(TRZ_PA_);
    geo.add_hi(RECHTECK);
    geo.add_hi(rec.mipu().x_QString());
    geo.add_hi(rec.mipu().y_QString());
    geo.add_hi(rec.mipu().z_QString());
    geo.add_hi(rec.l_qstring());
    geo.add_hi(rec.b_qstring());
    geo.add_hi(rec.rad_qstring());
    geo.add_hi(rec.drewi_qstring());
    geo.add_hi(rec.farbe());
    geo.add_hi(rec.farbe_fuellung());
    geo.add_hi(rec.linienbreite_qstring());
    geo.add_hi(rec.stil());
    add(geo, index);
}
//--------------------------------------get:
uint geo_text::akt_index()
{
    return AktIndex;
}
uint geo_text::count()
{
    return Daten.size();
}
text_zw geo_text::at()
{
    return at(AktIndex);
}
text_zw geo_text::at(uint index)
{
    text_zw ret;
    if(index < Daten.size())
    {
        ret = Daten.at(index);
    }else
    {
        ret.set_text("Fehler in Funktion geo_text::at(...)! Zeilennummer zu hoch.",'\n');
    }
    return ret;
}
double geo_text::max_x()
{
    double max = 0;
    for(uint i=0 ; i < count() ; i++)
    {
        text_zw spalten = Daten.at(i);
        for(uint ii=0;ii<spalten.count();ii++)
        {
            text_zw element;
            element.set_text(spalten.at(ii),TRZ_PA_);
            if(element.text().contains(PUNKT))
            {
                punkt3d p;
                p.set_text(element.text());
                if(p.x() > max)
                {
                    max = p.x();
                }
            }else if(element.text().contains(STRECKE))
            {
                strecke s;
                s.set_text(element.text());
                if(s.stapu().x() > max)
                {
                    max = s.stapu().x();
                }
                if(s.endpu().x() > max)
                {
                    max = s.endpu().x();
                }
            }else if(element.text().contains(BOGEN))
            {
                bogen b;
                b.set_text(element.text());
                if(b.spu().x() > max)
                {
                    max = b.spu().x();
                }
                if(b.epu().x() > max)
                {
                    max = b.epu().x();
                }
            }else if(element.text().contains(KREIS))
            {
                kreis k;
                k.set_text(element.text());
                double x = k.mipu().x() + k.radius();
                if(x > max)
                {
                    max = x;
                }
            }else if(element.text().contains(RECHTECK))
            {
                rechteck r;
                r.set_text(element.text());
                double x = r.obre().x();
                if(x > max)
                {
                    max = x;
                }
                x = r.obli().x();
                if(x > max)
                {
                    max = x;
                }
                x = r.unli().x();
                if(x > max)
                {
                    max = x;
                }
                x = r.unre().x();
                if(x > max)
                {
                    max = x;
                }
            }
        }
    }
    return max;
}
double geo_text::min_x()
{
    double min = 0;
    for(uint i=0 ; i < count() ; i++)
    {
        text_zw spalten = Daten.at(i);
        for(uint ii=0;ii<spalten.count();ii++)
        {
            text_zw element;
            element.set_text(spalten.at(ii),TRZ_PA_);
            if(element.text().contains(PUNKT))
            {
                punkt3d p;
                p.set_text(element.text());
                if(p.x() < min)
                {
                    min = p.x();
                }
            }else if(element.text().contains(STRECKE))
            {
                strecke s;
                s.set_text(element.text());
                if(s.stapu().x() < min)
                {
                    min = s.stapu().x();
                }
                if(s.endpu().x() < min)
                {
                    min = s.endpu().x();
                }
            }else if(element.text().contains(BOGEN))
            {
                bogen b;
                b.set_text(element.text());
                if(b.spu().x() < min)
                {
                    min = b.spu().x();
                }
                if(b.epu().x() < min)
                {
                    min = b.epu().x();
                }
            }else if(element.text().contains(KREIS))
            {
                kreis k;
                k.set_text(element.text());
                double x = k.mipu().x() - k.radius();
                if(x < min)
                {
                    min = x;
                }
            }else if(element.text().contains(RECHTECK))
            {
                rechteck r;
                r.set_text(element.text());
                double x = r.obre().x();
                if(x < min)
                {
                    min = x;
                }
                x = r.obli().x();
                if(x < min)
                {
                    min = x;
                }
                x = r.unli().x();
                if(x < min)
                {
                    min = x;
                }
                x = r.unre().x();
                if(x < min)
                {
                    min = x;
                }
            }
        }
    }
    return min;
}
double geo_text::max_y()
{
    double max = 0;
    for(uint i=0 ; i < count() ; i++)
    {
        text_zw spalten = Daten.at(i);
        for(uint ii=0;ii<spalten.count();ii++)
        {
            text_zw element;
            element.set_text(spalten.at(ii),TRZ_PA_);
            if(element.text().contains(PUNKT))
            {
                punkt3d p;
                p.set_text(element.text());
                if(p.y() > max)
                {
                    max = p.y();
                }
            }else if(element.text().contains(STRECKE))
            {
                strecke s;
                s.set_text(element.text());
                if(s.stapu().y() > max)
                {
                    max = s.stapu().y();
                }
                if(s.endpu().y() > max)
                {
                    max = s.endpu().y();
                }
            }else if(element.text().contains(BOGEN))
            {
                bogen b;
                b.set_text(element.text());
                if(b.spu().y() > max)
                {
                    max = b.spu().y();
                }
                if(b.epu().y() > max)
                {
                    max = b.epu().y();
                }
            }else if(element.text().contains(KREIS))
            {
                kreis k;
                k.set_text(element.text());
                double y = k.mipu().y() + k.radius();
                if(y > max)
                {
                    max = y;
                }
            }else if(element.text().contains(RECHTECK))
            {
                rechteck r;
                r.set_text(element.text());
                double y = r.obre().y();
                if(y > max)
                {
                    max = y;
                }
                y = r.obli().y();
                if(y > max)
                {
                    max = y;
                }
                y = r.unli().y();
                if(y > max)
                {
                    max = y;
                }
                y = r.unre().y();
                if(y > max)
                {
                    max = y;
                }
            }
        }
    }
    return max;
}
double geo_text::min_y()
{
    double min = 0;
    for(uint i=0 ; i < count() ; i++)
    {
        text_zw spalten = Daten.at(i);
        for(uint ii=0;ii<spalten.count();ii++)
        {
            text_zw element;
            element.set_text(spalten.at(ii),TRZ_PA_);
            if(element.text().contains(PUNKT))
            {
                punkt3d p;
                p.set_text(element.text());
                if(p.y() < min)
                {
                    min = p.y();
                }
            }else if(element.text().contains(STRECKE))
            {
                strecke s;
                s.set_text(element.text());
                if(s.stapu().y() < min)
                {
                    min = s.stapu().y();
                }
                if(s.endpu().y() < min)
                {
                    min = s.endpu().y();
                }
            }else if(element.text().contains(BOGEN))
            {
                bogen b;
                b.set_text(element.text());
                if(b.spu().y() < min)
                {
                    min = b.spu().y();
                }
                if(b.epu().y() < min)
                {
                    min = b.epu().y();
                }
            }else if(element.text().contains(KREIS))
            {
                kreis k;
                k.set_text(element.text());
                double y = k.mipu().y() - k.radius();
                if(y < min)
                {
                    min = y;
                }
            }else if(element.text().contains(RECHTECK))
            {
                rechteck r;
                r.set_text(element.text());
                double y = r.obre().y();
                if(y < min)
                {
                    min = y;
                }
                y = r.obli().y();
                if(y < min)
                {
                    min = y;
                }
                y = r.unli().y();
                if(y < min)
                {
                    min = y;
                }
                y = r.unre().y();
                if(y < min)
                {
                    min = y;
                }
            }
        }
    }
    return min;
}
//--------------------------------------
//Funktionen außerhalb der Klasse:
geo_text geo_ermitteln(text_zw bearb, double wst_l, double wst_b, double wst_d, double versatz_x, double versatz_y, \
                       wkz_magazin wkzm)
{
    double abst_darstel_hirnansicht = 20;
    //-------------------------------------------
    geo_text gt;
    //------------------------------
    //Nullpunkt darstellen:
    punkt3d nullpunkt(0,0,0);
    nullpunkt.set_linienbreite(15);
    gt.add_punkt(nullpunkt);
    //------------------------------
    //wst darstellen:
    rechteck rec;
    rec.set_laenge(wst_l);
    rec.set_breite(wst_b);
    rec.set_unli(versatz_x,versatz_y,0);
    rec.set_farbe_fuellung(FARBE_GRAU);
    gt.add_rechteck(rec);
    //------------------------------
    //WST-Hirnseite vorne:
    punkt3d nullpunkt_vo(0,-abst_darstel_hirnansicht,0);
    nullpunkt_vo.set_linienbreite(10);
    gt.add_punkt(nullpunkt_vo);
    rechteck rec_vo;
    rec_vo.set_laenge(wst_l);
    rec_vo.set_breite(wst_d);
    rec_vo.set_obli(nullpunkt_vo.x(),nullpunkt_vo.y(),0);
    rec_vo.set_farbe_fuellung(FARBE_GRAU);
    gt.add_rechteck(rec_vo);
    //------------------------------
    //WST-Hirnseite hinten:
    punkt3d nullpunkt_hi(0,wst_b+versatz_y+abst_darstel_hirnansicht,0);
    nullpunkt_hi.set_linienbreite(10);
    gt.add_punkt(nullpunkt_hi);
    rechteck rec_hi;
    rec_hi.set_laenge(wst_l);
    rec_hi.set_breite(wst_d);
    rec_hi.set_unli(nullpunkt_hi.x(),nullpunkt_hi.y(),0);
    rec_hi.set_farbe_fuellung(FARBE_GRAU);
    gt.add_rechteck(rec_hi);
    //------------------------------
    //WST-Hirnseite links:
    punkt3d nullpunkt_li(-abst_darstel_hirnansicht,versatz_y,0);
    nullpunkt_li.set_linienbreite(10);
    gt.add_punkt(nullpunkt_li);
    rechteck rec_li;
    rec_li.set_laenge(wst_d);
    rec_li.set_breite(wst_b);
    rec_li.set_unre(nullpunkt_li.x(),nullpunkt_li.y(),0);
    rec_li.set_farbe_fuellung(FARBE_GRAU);
    gt.add_rechteck(rec_li);
    //------------------------------
    //WST-Hirnseite rechts:
    punkt3d nullpunkt_re(wst_l+versatz_x+abst_darstel_hirnansicht,versatz_y,0);
    nullpunkt_re.set_linienbreite(10);
    gt.add_punkt(nullpunkt_re);
    rechteck rec_re;
    rec_re.set_laenge(wst_d);
    rec_re.set_breite(wst_b);
    rec_re.set_unli(nullpunkt_re.x(),nullpunkt_re.y(),0);
    rec_re.set_farbe_fuellung(FARBE_GRAU);
    gt.add_rechteck(rec_re);
    //------------------------------
    gt.zeilenvorschub();
    //------------------------------
    //Bearbeitungen darstellen:
    QString farbe_unterseite = FARBE_ROSE;
    for(uint i=0; i<bearb.count() ;i++)
    {
        text_zw zeile;
        zeile.set_text(bearb.at(i),TRENNZ_BEARB_PARAM);
        if(zeile.at(0) == BEARBART_BOHR)
        {
            bohrung bo(zeile.text());
            double afb = ausdruck_auswerten(bo.afb()).toDouble();
            if(bo.bezug() == WST_BEZUG_OBSEI)
            {
                kreis k;                
                k.set_rad(bo.dm()/2);
                k.set_mipu(bo.mipu());
                k.verschieben_um(versatz_x, versatz_y);
                double wkzdm = wkzm.dm(bo.wkznum()).toDouble();
                if(bo.istZapfen() == false)
                {
                    k.set_farbe(FARBE_SCHWARZ);
                    if(bo.tiefe() > wst_d)
                    {
                        k.set_farbe_fuellung(FARBE_WEISS);
                    }else
                    {
                        k.set_farbe_fuellung(FARBE_HELLBLAU);
                    }
                }else
                {
                    k.set_farbe(FARBE_BLAU);
                    k.set_farbe_fuellung(FARBE_KEINE);
                    if(wkzdm > 0)
                    {
                        kreis k_au = k;
                        k_au.set_rad(k.radius()+wkzdm);
                        if(afb <= 0)
                        {
                            k_au.set_farbe(FARBE_DUNKELGRAU);
                            k_au.set_farbe_fuellung(FARBE_KEINE);
                        }
                        gt.add_kreis(k_au);
                    }
                }

                if(afb <= 0)
                {
                    k.set_farbe(FARBE_DUNKELGRAU);
                    k.set_farbe_fuellung(FARBE_KEINE);
                }

                gt.add_kreis(k);
                strecke s;
                s.set_farbe(k.farbe());
                s.set_stapu(bo.x() - bo.dm()/2 - 2, bo.y(), bo.z());
                s.set_endpu(bo.x() + bo.dm()/2 + 2, bo.y(), bo.z());
                s.verschieben_um(versatz_x, versatz_y);
                if(bo.istZapfen() && wkzdm > 0)
                {
                    s.set_laenge(bo.dm()+wkzdm*2);
                }
                gt.add_strecke(s);
                s.drenen_um_mipu_2d(degToRad(90));
                gt.add_strecke(s);
            }else if(bo.bezug() == WST_BEZUG_UNSEI)
            {
                kreis k;
                k.set_farbe(farbe_unterseite);
                k.set_rad(bo.dm()/2);
                k.set_mipu(bo.mipu());
                double wkzdm = wkzm.dm(bo.wkznum()).toDouble();
                if(bo.istZapfen() == false)
                {
                    if(bo.tiefe() > wst_d)
                    {
                        k.set_farbe_fuellung(FARBE_WEISS);
                    }else
                    {
                        k.set_farbe_fuellung(FARBE_HELLBLAU);
                    }
                }else
                {
                    k.set_farbe_fuellung(FARBE_KEINE);
                    if(wkzdm > 0)
                    {
                        kreis k_au = k;
                        k_au.set_rad(k.radius()+wkzdm);
                        if(afb <= 0)
                        {
                            k_au.set_farbe(FARBE_DUNKELGRAU);
                            k_au.set_farbe_fuellung(FARBE_KEINE);
                        }
                        gt.add_kreis(k_au);
                    }
                }

                if(afb <= 0)
                {
                    k.set_farbe(FARBE_DUNKELGRAU);
                    k.set_farbe_fuellung(FARBE_KEINE);
                }

                k.verschieben_um(versatz_x, versatz_y);
                gt.add_kreis(k);
                strecke s;
                s.set_farbe(k.farbe());
                s.set_stapu(bo.x() - bo.dm()/2 - 2, bo.y(), bo.z());
                s.set_endpu(bo.x() + bo.dm()/2 + 2, bo.y(), bo.z());
                s.drenen_um_mipu_2d(degToRad(45));
                s.verschieben_um(versatz_x, versatz_y);
                if(bo.istZapfen() && wkzdm > 0)
                {
                    s.set_laenge(bo.dm()+wkzdm*2);
                }
                gt.add_strecke(s);
                s.drenen_um_mipu_2d(degToRad(90));
                gt.add_strecke(s);
            }else if(bo.bezug() == WST_BEZUG_LI)
            {
                rechteck r;
                r.set_laenge(bo.tiefe());
                r.set_breite(bo.dm());
                r.set_li(bo.x(), bo.y(), 0);
                r.verschieben_um(versatz_x, versatz_y);
                if(afb <= 0)
                {
                    r.set_farbe(FARBE_DUNKELGRAU);
                    r.set_farbe_fuellung(FARBE_KEINE);
                }else
                {
                    r.set_farbe_fuellung(FARBE_BRAUN);
                }
                gt.add_rechteck(r);

                kreis k;
                k.set_rad(bo.dm()/2);
                k.set_mipu(nullpunkt_li.x()-bo.z(),nullpunkt_li.y()+bo.y(), 0);
                if(afb <= 0)
                {
                    k.set_farbe(FARBE_DUNKELGRAU);
                    k.set_farbe_fuellung(FARBE_KEINE);
                }else
                {
                    k.set_farbe_fuellung(FARBE_BRAUN);
                }
                gt.add_kreis(k);
            }else if(bo.bezug() == WST_BEZUG_RE)
            {
                rechteck r;
                r.set_laenge(bo.tiefe());
                r.set_breite(bo.dm());
                r.set_re(bo.x(), bo.y(), 0);
                r.verschieben_um(versatz_x, versatz_y);
                if(afb <= 0)
                {
                    r.set_farbe(FARBE_DUNKELGRAU);
                    r.set_farbe_fuellung(FARBE_KEINE);
                }else
                {
                    r.set_farbe_fuellung(FARBE_BRAUN);
                }
                gt.add_rechteck(r);

                kreis k;
                k.set_rad(bo.dm()/2);
                k.set_mipu(nullpunkt_re.x()+bo.z(),nullpunkt_re.y()+bo.y(), 0);
                if(afb <= 0)
                {
                    k.set_farbe(FARBE_DUNKELGRAU);
                    k.set_farbe_fuellung(FARBE_KEINE);
                }else
                {
                    k.set_farbe_fuellung(FARBE_BRAUN);
                }
                gt.add_kreis(k);
            }else if(bo.bezug() == WST_BEZUG_VO)
            {
                rechteck r;
                r.set_laenge(bo.dm());
                r.set_breite(bo.tiefe());
                r.set_un(bo.x(), bo.y(), 0);
                r.verschieben_um(versatz_x, versatz_y);
                if(afb <= 0)
                {
                    r.set_farbe(FARBE_DUNKELGRAU);
                    r.set_farbe_fuellung(FARBE_KEINE);
                }else
                {
                    r.set_farbe_fuellung(FARBE_BRAUN);
                }
                gt.add_rechteck(r);

                kreis k;
                k.set_rad(bo.dm()/2);
                k.set_mipu(nullpunkt_vo.x()+bo.x(),nullpunkt_vo.y()-bo.z(), 0);
                if(afb <= 0)
                {
                    k.set_farbe(FARBE_DUNKELGRAU);
                    k.set_farbe_fuellung(FARBE_KEINE);
                }else
                {
                    k.set_farbe_fuellung(FARBE_BRAUN);
                }
                gt.add_kreis(k);
            }else if(bo.bezug() == WST_BEZUG_HI)
            {
                rechteck r;
                r.set_laenge(bo.dm());
                r.set_breite(bo.tiefe());
                r.set_ob(bo.x(), bo.y(), 0);
                r.verschieben_um(versatz_x, versatz_y);
                if(afb <= 0)
                {
                    r.set_farbe(FARBE_DUNKELGRAU);
                    r.set_farbe_fuellung(FARBE_KEINE);
                }else
                {
                    r.set_farbe_fuellung(FARBE_BRAUN);
                }
                gt.add_rechteck(r);

                kreis k;
                k.set_rad(bo.dm()/2);
                k.set_mipu(nullpunkt_hi.x()+bo.x(),nullpunkt_hi.y()+bo.z(), 0);
                if(afb <= 0)
                {
                    k.set_farbe(FARBE_DUNKELGRAU);
                    k.set_farbe_fuellung(FARBE_KEINE);
                }else
                {
                    k.set_farbe_fuellung(FARBE_BRAUN);
                }
                gt.add_kreis(k);
            }
        }else if(zeile.at(0) == BEARBART_BOHRRASTER)
        {
            bohrraster bo(zeile.text());
            double afb = ausdruck_auswerten(bo.afb()).toDouble();
            if(bo.bezug() == WST_BEZUG_OBSEI)
            {
                kreis k;
                k.set_farbe(FARBE_SCHWARZ);
                k.set_rad(bo.dm()/2);
                k.set_mipu(bo.mipu());
                k.verschieben_um(versatz_x, versatz_y);
                if(bo.tiefe() > wst_d)
                {
                    k.set_farbe_fuellung(FARBE_WEISS);
                }else
                {
                    k.set_farbe_fuellung(FARBE_HELLBLAU);
                }
                if(afb <= 0)
                {
                    k.set_farbe(FARBE_DUNKELGRAU);
                    k.set_farbe_fuellung(FARBE_KEINE);
                }
                strecke s1;
                s1.set_farbe(k.farbe());
                s1.set_stapu(bo.x() - bo.dm()/2 - 2, bo.y(), bo.z());
                s1.set_endpu(bo.x() + bo.dm()/2 + 2, bo.y(), bo.z());
                s1.verschieben_um(versatz_x, versatz_y);
                strecke s2 = s1;
                s2.drenen_um_mipu_2d(degToRad(90));
                for(uint i=0; i<bo.anz_x() ;i++)
                {
                    kreis tmp_k = k;
                    tmp_k.verschieben_um(i*bo.raster_x(), 0);
                    strecke tmp_s1 = s1;
                    tmp_s1.verschieben_um(i*bo.raster_x(), 0);
                    strecke tmp_s2 = s2;
                    tmp_s2.verschieben_um(i*bo.raster_x(), 0);
                    for(uint ii=0; ii<bo.anz_y() ;ii++)
                    {
                        kreis tmp_kk = tmp_k;
                        strecke tmp_s11 = tmp_s1;
                        strecke tmp_s22 = tmp_s2;
                        tmp_kk.verschieben_um(0, ii*bo.raster_y());
                        tmp_s11.verschieben_um(0, ii*bo.raster_y());
                        tmp_s22.verschieben_um(0, ii*bo.raster_y());
                        gt.add_kreis(tmp_kk);
                        gt.add_strecke(tmp_s11);
                        gt.add_strecke(tmp_s22);
                    }
                }
            }else if(bo.bezug() == WST_BEZUG_UNSEI)
            {
                kreis k;
                k.set_farbe(farbe_unterseite);
                k.set_rad(bo.dm()/2);
                k.set_mipu(bo.mipu());
                k.verschieben_um(versatz_x, versatz_y);
                if(bo.tiefe() > wst_d)
                {
                    k.set_farbe_fuellung(FARBE_WEISS);
                }else
                {
                    k.set_farbe_fuellung(FARBE_HELLBLAU);
                }
                if(afb <= 0)
                {
                    k.set_farbe(FARBE_DUNKELGRAU);
                    k.set_farbe_fuellung(FARBE_KEINE);
                }
                strecke s1;
                s1.set_farbe(k.farbe());
                s1.set_stapu(bo.x() - bo.dm()/2 - 2, bo.y(), bo.z());
                s1.set_endpu(bo.x() + bo.dm()/2 + 2, bo.y(), bo.z());
                s1.drenen_um_mipu_2d(degToRad(45));
                s1.verschieben_um(versatz_x, versatz_y);
                strecke s2 = s1;
                s2.drenen_um_mipu_2d(degToRad(90));
                for(uint i=0; i<bo.anz_x() ;i++)
                {
                    kreis tmp_k = k;
                    tmp_k.verschieben_um(i*bo.raster_x(), 0);
                    strecke tmp_s1 = s1;
                    tmp_s1.verschieben_um(i*bo.raster_x(), 0);
                    strecke tmp_s2 = s2;
                    tmp_s2.verschieben_um(i*bo.raster_x(), 0);
                    for(uint ii=0; ii<bo.anz_y() ;ii++)
                    {
                        kreis tmp_kk = tmp_k;
                        strecke tmp_s11 = tmp_s1;
                        strecke tmp_s22 = tmp_s2;
                        tmp_kk.verschieben_um(0, ii*bo.raster_y());
                        tmp_s11.verschieben_um(0, ii*bo.raster_y());
                        tmp_s22.verschieben_um(0, ii*bo.raster_y());
                        gt.add_kreis(tmp_kk);
                        gt.add_strecke(tmp_s11);
                        gt.add_strecke(tmp_s22);
                    }
                }
            }else if(bo.bezug() == WST_BEZUG_LI)
            {
                rechteck r;
                r.set_laenge(bo.tiefe());
                r.set_li(0, bo.y(), 0);
                r.verschieben_um(versatz_x, versatz_y);
                if(afb <= 0)
                {
                    r.set_farbe(FARBE_DUNKELGRAU);
                    r.set_farbe_fuellung(FARBE_KEINE);
                }else
                {
                    r.set_farbe_fuellung(FARBE_BRAUN);
                }

                kreis k;                
                k.set_rad(bo.dm()/2);
                k.set_mipu(nullpunkt_li.x()-bo.z(),nullpunkt_li.y()+bo.y(), 0);
                if(afb <= 0)
                {
                    k.set_farbe(FARBE_DUNKELGRAU);
                    k.set_farbe_fuellung(FARBE_KEINE);
                }else
                {
                    k.set_farbe_fuellung(FARBE_BRAUN);
                }

                for(uint i=0; i<bo.anz_y() ;i++)
                {
                    rechteck tmp_r = r;
                    tmp_r.verschieben_um(0, i*bo.raster_y());
                    gt.add_rechteck(tmp_r);
                    kreis tmp_k = k;
                    tmp_k.verschieben_um(0, i*bo.raster_y());
                    gt.add_kreis(tmp_k);
                }
            }else if(bo.bezug() == WST_BEZUG_RE)
            {
                rechteck r;
                r.set_laenge(bo.tiefe());
                r.set_breite(bo.dm());
                r.set_re(wst_l, bo.y(), 0);
                r.verschieben_um(versatz_x, versatz_y);
                if(afb <= 0)
                {
                    r.set_farbe(FARBE_DUNKELGRAU);
                    r.set_farbe_fuellung(FARBE_KEINE);
                }else
                {
                    r.set_farbe_fuellung(FARBE_BRAUN);
                }

                kreis k;
                k.set_rad(bo.dm()/2);
                k.set_mipu(nullpunkt_re.x()+bo.z(),nullpunkt_re.y()+bo.y(), 0);
                if(afb <= 0)
                {
                    k.set_farbe(FARBE_DUNKELGRAU);
                    k.set_farbe_fuellung(FARBE_KEINE);
                }else
                {
                    k.set_farbe_fuellung(FARBE_BRAUN);
                }
                for(uint i=0; i<bo.anz_y() ;i++)
                {
                    rechteck tmp_r = r;
                    tmp_r.verschieben_um(0, i*bo.raster_y());
                    gt.add_rechteck(tmp_r);
                    kreis tmp_k = k;
                    tmp_k.verschieben_um(0, i*bo.raster_y());
                    gt.add_kreis(tmp_k);
                }
            }else if(bo.bezug() == WST_BEZUG_VO)
            {
                rechteck r;
                r.set_laenge(bo.dm());
                r.set_breite(bo.tiefe());
                r.set_un(bo.x(), 0, 0);
                r.verschieben_um(versatz_x, versatz_y);
                if(afb <= 0)
                {
                    r.set_farbe(FARBE_DUNKELGRAU);
                    r.set_farbe_fuellung(FARBE_KEINE);
                }else
                {
                    r.set_farbe_fuellung(FARBE_BRAUN);
                }

                kreis k;
                k.set_rad(bo.dm()/2);
                k.set_mipu(nullpunkt_vo.x()+bo.x(),nullpunkt_vo.y()-bo.z(), 0);
                if(afb <= 0)
                {
                    k.set_farbe(FARBE_DUNKELGRAU);
                    k.set_farbe_fuellung(FARBE_KEINE);
                }else
                {
                    k.set_farbe_fuellung(FARBE_BRAUN);
                }

                for(uint i=0; i<bo.anz_x() ;i++)
                {
                    rechteck tmp_r = r;
                    tmp_r.verschieben_um(i*bo.raster_x(), 0);
                    gt.add_rechteck(tmp_r);
                    kreis tmp_k = k;
                    tmp_k.verschieben_um(i*bo.raster_x(), 0);
                    gt.add_kreis(tmp_k);
                }
            }else if(bo.bezug() == WST_BEZUG_HI)
            {
                rechteck r;
                r.set_laenge(bo.dm());
                r.set_breite(bo.tiefe());
                r.set_ob(bo.x(), wst_b, 0);
                r.verschieben_um(versatz_x, versatz_y);
                if(afb <= 0)
                {
                    r.set_farbe(FARBE_DUNKELGRAU);
                    r.set_farbe_fuellung(FARBE_KEINE);
                }else
                {
                    r.set_farbe_fuellung(FARBE_BRAUN);
                }

                kreis k;
                k.set_rad(bo.dm()/2);
                k.set_mipu(nullpunkt_hi.x()+bo.x(),nullpunkt_hi.y()+bo.z(), 0);
                if(afb <= 0)
                {
                    k.set_farbe(FARBE_DUNKELGRAU);
                    k.set_farbe_fuellung(FARBE_KEINE);
                }else
                {
                    k.set_farbe_fuellung(FARBE_BRAUN);
                }
                for(uint i=0; i<bo.anz_x() ;i++)
                {
                    rechteck tmp_r = r;
                    tmp_r.verschieben_um(i*bo.raster_x(), 0);
                    gt.add_rechteck(tmp_r);
                    kreis tmp_k = k;
                    tmp_k.verschieben_um(i*bo.raster_x(), 0);
                    gt.add_kreis(tmp_k);
                }
            }
        }else if(zeile.at(0) == BEARBART_NUT)
        {
            nut nu(zeile.text());
            double afb = ausdruck_auswerten(nu.afb()).toDouble();
            strecke s;
            s.set_stapu(nu.xs(), nu.ys(), 0);
            s.set_endpu(nu.xe(), nu.ye(), 0);
            rechteck r;
            if(  (nu.bezug() == WST_BEZUG_OBSEI) ||  (nu.bezug() == WST_BEZUG_UNSEI)  )
            {
                if(nu.bezug() == WST_BEZUG_OBSEI)
                {
                    r.set_farbe_fuellung(FARBE_BLAU);
                }else
                {
                    r.set_farbe_fuellung(farbe_unterseite);
                    r.set_stil(STIL_GESTRICHELT);
                }
                if(afb <= 0)
                {
                    r.set_farbe(FARBE_DUNKELGRAU);
                    r.set_farbe_fuellung(FARBE_KEINE);
                }
                r.set_laenge(s.laenge2d());
                r.set_breite(nu.breite());
                r.set_mipu(s.mipu());
                r.set_drewi(s.wink());
                r.verschieben_um(versatz_x, versatz_y);
                gt.add_rechteck(r);
            }else
            {
                if(nu.bezug() == WST_BEZUG_LI)
                {
                    r.set_laenge(nu.tiefe());
                    r.set_breite(s.laenge2d());
                    r.set_drewi(0);
                    r.set_li(s.mipu());
                }else if(nu.bezug() == WST_BEZUG_RE)
                {
                    r.set_laenge(nu.tiefe());
                    r.set_breite(s.laenge2d());
                    r.set_drewi(0);
                    r.set_re(s.mipu());
                }else if(nu.bezug() == WST_BEZUG_VO)
                {
                    r.set_laenge(s.laenge2d());
                    r.set_breite(nu.tiefe());
                    r.set_drewi(0);
                    r.set_un(s.mipu());
                }else if(nu.bezug() == WST_BEZUG_HI)
                {
                    r.set_laenge(s.laenge2d());
                    r.set_breite(nu.tiefe());
                    r.set_drewi(0);
                    r.set_ob(s.mipu());
                }
                r.set_farbe_fuellung(FARBE_GELB);
                punkt3d mipu = s.mipu();
                strecke s_mipu;
                s_mipu.set_stapu(mipu);
                s_mipu.set_endpu(s.endpu());
                s_mipu.set_laenge(nu.tiefe()/2, strecke_bezugspunkt_start);
                s_mipu.drenen_um_stapu_2d(degToRad(90));
                mipu = s_mipu.endpu();
                r.verschieben_um(versatz_x, versatz_y);
                //Start anzeigen:
                strecke stmp = s;
                stmp.drenen_um_stapu_2d(degToRad(-90));
                stmp.set_laenge(30, strecke_bezugspunkt_start);
                stmp.set_laenge(9999, strecke_bezugspunkt_ende);
                stmp.set_laenge(stmp.laenge2d()-nu.tiefe(), strecke_bezugspunkt_start);
                kreis k;
                k.set_mipu(stmp.endpu());
                k.set_rad(30);
                k.set_farbe(FARBE_GELB);
                if(afb <= 0)
                {
                    k.set_farbe(FARBE_DUNKELGRAU);
                    k.set_farbe_fuellung(FARBE_KEINE);
                    r.set_farbe(FARBE_DUNKELGRAU);
                    r.set_farbe_fuellung(FARBE_KEINE);
                }
                gt.add_kreis(k);
                gt.add_rechteck(r);
            }
        }else if(zeile.at(0) == BEARBART_RTA)
        {
            rechtecktasche rt(zeile.text());
            double afb = ausdruck_auswerten(rt.afb()).toDouble();
            rechteck r;
            double wkzdm = wkzm.dm(rt.wkznum()).toDouble();
            double eckenrad;
            if(wkzdm > rt.rad())
            {
                eckenrad = wkzdm/2;
            }else
            {
                eckenrad = rt.rad();
            }

            if(  (rt.bezug() == WST_BEZUG_OBSEI) ||  (rt.bezug() == WST_BEZUG_UNSEI)  )
            {
                if(rt.tiefe() >= wst_d)
                {
                    r.set_farbe_fuellung(FARBE_WEISS);
                }else
                {
                    if(rt.bezug() == WST_BEZUG_OBSEI)
                    {
                        r.set_farbe_fuellung(FARBE_DUNKELGRAU);
                    }else
                    {
                        r.set_farbe_fuellung(farbe_unterseite);
                        r.set_stil(STIL_GESTRICHELT);
                    }
                }
                if(rt.istZapfen() == true)
                {
                    r.set_farbe_fuellung(FARBE_KEINE);
                    if(rt.bezug() == WST_BEZUG_OBSEI)
                    {
                        r.set_farbe(FARBE_BLAU);
                    }else
                    {
                        r.set_farbe(farbe_unterseite);
                    }
                }
                if(afb <= 0)
                {
                    r.set_farbe(FARBE_DUNKELGRAU);
                    r.set_farbe_fuellung(FARBE_KEINE);
                }
                r.set_laenge(rt.laenge());
                r.set_breite(rt.breite());
                r.set_mipu(rt.x(), rt.y(), rt.z());
                r.set_drewi(rt.drewi());
                r.verschieben_um(versatz_x, versatz_y);
                r.set_rad(eckenrad);
                if(rt.istZapfen() && wkzdm > 0)
                {
                    rechteck r_au = r;
                    r_au.set_laenge(r.l()+wkzdm*2);
                    r_au.set_breite(r.b()+wkzdm*2);
                    r_au.set_rad(r.rad()+wkzdm);
                    gt.add_rechteck(r_au);
                }
                gt.add_rechteck(r);
                if(rt.ausraeumen() == false  && rt.istZapfen() == false)
                {
                    double innen_l = r.l() - wkzdm*2;
                    double innen_b = r.b() - wkzdm*2;
                    if(innen_l > 0 && innen_b > 0)
                    {
                        r.set_laenge(innen_l);
                        r.set_breite(innen_b);
                        r.set_rad(eckenrad-wkzdm);
                        r.set_farbe_fuellung(FARBE_GRAU);
                        gt.add_rechteck(r);
                    }
                }
            }else
            {
                rechteck r_kantenansicht;
                if(rt.bezug() == WST_BEZUG_LI)
                {
                    r.set_laenge(rt.tiefe());
                    r.set_breite(rt.laenge());
                    r_kantenansicht.set_laenge(rt.breite());
                    r_kantenansicht.set_breite(rt.laenge());
                    r_kantenansicht.set_mipu(nullpunkt_li.x()-rt.z(),nullpunkt_li.y()+rt.y(), 0);
                    r.set_li(rt.x(), rt.y(), rt.z());
                }else if(rt.bezug() == WST_BEZUG_RE)
                {
                    r.set_laenge(rt.tiefe());
                    r.set_breite(rt.laenge());
                    r_kantenansicht.set_laenge(rt.breite());
                    r_kantenansicht.set_breite(rt.laenge());
                    r_kantenansicht.set_mipu(nullpunkt_re.x()+rt.z(),nullpunkt_re.y()+rt.y(), 0);
                    r.set_re(rt.x(), rt.y(), rt.z());
                }else if(rt.bezug() == WST_BEZUG_VO)
                {
                    r.set_laenge(rt.laenge());
                    r.set_breite(rt.tiefe());
                    r_kantenansicht.set_laenge(rt.laenge());
                    r_kantenansicht.set_breite(rt.breite());
                    r_kantenansicht.set_mipu(nullpunkt_vo.x()+rt.x(),nullpunkt_vo.y()-rt.z(), 0);
                    r.set_un(rt.x(), rt.y(), rt.z());
                }else if(rt.bezug() == WST_BEZUG_HI)
                {
                    r.set_laenge(rt.laenge());
                    r.set_breite(rt.tiefe());
                    r_kantenansicht.set_laenge(rt.laenge());
                    r_kantenansicht.set_breite(rt.breite());
                    r_kantenansicht.set_mipu(nullpunkt_hi.x()+rt.x(),nullpunkt_hi.y()+rt.z(), 0);
                    r.set_ob(rt.x(), rt.y(), rt.z());
                }
                r.set_farbe_fuellung(FARBE_GRUEN);
                r.verschieben_um(versatz_x, versatz_y);
                //hier darf nicht der rad gesetzt werden weil Draufsicht
                if(afb <= 0)
                {
                    r.set_farbe(FARBE_DUNKELGRAU);
                    r.set_farbe_fuellung(FARBE_KEINE);
                }
                gt.add_rechteck(r);
                r_kantenansicht.set_rad(eckenrad);
                r_kantenansicht.set_farbe_fuellung(r.farbe_fuellung());
                r_kantenansicht.set_farbe(r.farbe());
                gt.add_rechteck(r_kantenansicht);
            }

        }else if(zeile.at(0) == BEARBART_FRAESERAUFRUF)
        {
            fraeseraufruf fa(zeile.text());
            double afb = ausdruck_auswerten(fa.afb()).toDouble();
            punkt3d p(fa.x(), fa.y(), fa.z());
            p.set_linienbreite(10);
            p.verschieben_um(versatz_x, versatz_y);
            if(fa.bezug() == WST_BEZUG_OBSEI)
            {
                p.set_farbe(FARBE_BLAU);
            }else
            {
                p.set_farbe(farbe_unterseite);
            }
            if(afb <= 0)
            {
                p.set_farbe(FARBE_DUNKELGRAU);
            }
            gt.add_punkt(p);
        }else if(zeile.at(0) == BEARBART_FRAESERGERADE)
        {
            fraesergerade fg(zeile.text());
            double afb = ausdruck_auswerten(fg.afb()).toDouble();
            strecke s;
            s = fg.strecke_();
            s.verschieben_um(versatz_x, versatz_y);
            if(fg.bezug() == WST_BEZUG_OBSEI)
            {
                s.set_farbe(FARBE_BLAU);
            }else
            {
                s.set_farbe(farbe_unterseite);
                s.set_stil(STIL_GESTRICHELT);
            }
            if(afb <= 0)
            {
                s.set_farbe(FARBE_DUNKELGRAU);
                s.set_farbe_fuellung(FARBE_KEINE);
            }
            gt.add_strecke(s);
        }else if(zeile.at(0) == BEARBART_FRAESERBOGEN)
        {
            fraeserbogen fb(zeile.text());
            double afb = ausdruck_auswerten(fb.afb()).toDouble();
            bogen b = fb.bog();
            if(fb.bezug() == WST_BEZUG_OBSEI)
            {
                b.set_farbe(FARBE_DUNKELGELB);
            }else
            {
                b.set_farbe(farbe_unterseite);
                b.set_stil(STIL_GESTRICHELT);
            }
            if(afb <= 0)
            {
                b.set_farbe(FARBE_DUNKELGRAU);
                b.set_farbe_fuellung(FARBE_KEINE);
            }
            b.verschieben_um(versatz_x, versatz_y);
            gt.add_bogen(b);
        }else if(zeile.at(0) == BEARBART_KOMMENTAR)
        {
            gt.add_leerzeile();
        }else if(zeile.at(0) == BEARBART_GEZUPU)
        {
            gehezupunkt gzp(zeile.text());
            double afb = ausdruck_auswerten(gzp.afb()).toDouble();
            punkt3d p = gzp.pos();
            p.set_linienbreite(15);
            p.verschieben_um(versatz_x, versatz_y);
            if(afb <= 0)
            {
                p.set_farbe(FARBE_DUNKELGRAU);
            }else
            {
                p.set_farbe(FARBE_LILA);
            }
            gt.add_punkt(p);
        }else if(zeile.at(0) == BEARBART_HALT)
        {
            gt.add_leerzeile();
        }
        gt.zeilenvorschub();
    }

    return gt;
}
geo_text geo_ermitteln_akt_fkon(text_zw bearb, double versatz_x, double versatz_y, wkz_magazin wkzm)
{
    //-------------------------------------------
    geo_text gt;
    gt.add_leerzeile();//Werkstück
    gt.zeilenvorschub();
    //------------------------------
    //fkon darstellen:
    QString radkor;
    double fraeserdm = 10;
    QString farbe_unterseite = FARBE_ROSE;
    fraeseraufruf letzter_fa;
    for(uint i=0; i<bearb.count() ;i++)
    {
        text_zw zeile;
        zeile.set_text(bearb.at(i),TRENNZ_BEARB_PARAM);
        if(zeile.at(0) == BEARBART_FRAESERAUFRUF)
        {
            fraeseraufruf fa(zeile.text());
            letzter_fa = fa;//wird gebraucht für die Berechnung der Abfahrwege
            radkor = fa.radkor();
            gt.add_leerzeile();
            QString wkznr = fa.wkznum();
            fraeserdm = wkzm.dm(wkznr).toDouble();
            if(i+1 < bearb.count())
            {
                double anweg;
                if(fa.anfahrweg_qstring() == FAUFRUF_ANABWEG_AUTO)
                {
                    anweg = fraeserdm;
                }else
                {
                    anweg = fa.anfahrweg();
                }
                text_zw folzei;//Folgezeile
                folzei.set_text(bearb.at(i+1),TRENNZ_BEARB_PARAM);
                punkt3d pfa;//Punkt Fräseraufruf
                pfa.set_x(fa.x());
                pfa.set_y(fa.y());
                punkt3d pein;//Eintauchpunkt
                if(folzei.at(0) == BEARBART_FRAESERGERADE)
                {
                    fraesergerade fg(folzei.text());
                    if(fa.anfahrtyp() == FAUFRUF_ANABTYP_GARADE)
                    {
                        strecke s = fg.strecke_();
                        strecke_bezugspunkt sb;
                        sb = strecke_bezugspunkt_ende;
                        s.set_laenge(s.laenge2d()+anweg, sb);
                        sb = strecke_bezugspunkt_start;
                        s.set_laenge(anweg, sb);
                        if(fa.bezug() == WST_BEZUG_OBSEI)
                        {
                            s.set_farbe(FARBE_BLAU);
                        }else
                        {
                            s.set_farbe(farbe_unterseite);
                            s.set_stil(STIL_GESTRICHELT);
                        }
                        s.verschieben_um(versatz_x, versatz_y);
                        gt.add_strecke(s);
                    }else if(fa.anfahrtyp() == FAUFRUF_ANABTYP_BOGEN)
                    {
                        bogen b;
                        //Hilfs-Strecke:
                        strecke s = fg.strecke_();
                        strecke_bezugspunkt sb;
                        sb = strecke_bezugspunkt_ende;
                        s.set_laenge(s.laenge2d()+anweg, sb);
                        sb = strecke_bezugspunkt_start;
                        s.set_laenge(anweg, sb);
                        //---
                        if(fa.bezug() == WST_BEZUG_OBSEI)
                        {
                            b.set_farbe(FARBE_BLAU);
                            if(fa.radkor() == FRKOR_L)
                            {
                                s.drenen_um_endpu_2d(radToDeg(90), true);
                                s.drenen_um_stapu_2d(radToDeg(90), true);
                                //b.set_startpunkt(s.endpu());
                                //b.set_endpunkt(fa.pos());
                                //b.set_radius(anweg, false);
                                b.set_bogen(s.endpu(), fa.pos(), anweg, false);
                            }else if(fa.radkor() == FRKOR_R)
                            {
                                s.drenen_um_endpu_2d(radToDeg(90), false);
                                s.drenen_um_stapu_2d(radToDeg(90), false);
                                //b.set_startpunkt(s.endpu());
                                //b.set_endpunkt(fa.pos());
                                //b.set_radius(anweg, true);
                                b.set_bogen(s.endpu(), fa.pos(), anweg, true);
                            }
                        }else
                        {
                            b.set_farbe(farbe_unterseite);
                            b.set_stil(STIL_GESTRICHELT);
                            if(fa.radkor() == FRKOR_L)
                            {
                                s.drenen_um_endpu_2d(radToDeg(90), false);
                                s.drenen_um_stapu_2d(radToDeg(90), false);
                                //b.set_startpunkt(s.endpu());
                                //b.set_endpunkt(fa.pos());
                                //b.set_radius(anweg, true);
                                b.set_bogen(s.endpu(), fa.pos(), anweg, true);
                            }else if(fa.radkor() == FRKOR_R)
                            {
                                s.drenen_um_endpu_2d(radToDeg(90), true);
                                s.drenen_um_stapu_2d(radToDeg(90), true);
                                //b.set_startpunkt(s.endpu());
                                //b.set_endpunkt(fa.pos());
                                //b.set_radius(anweg, false);
                                b.set_bogen(s.endpu(), fa.pos(), anweg, false);
                            }
                        }
                        b.verschieben_um(versatz_x, versatz_y);
                        gt.add_bogen(b);
                    }
                }else if(folzei.at(0) == BEARBART_FRAESERBOGEN)
                {
                    fraeserbogen fb(folzei.text());
                    if(fa.anfahrtyp() == FAUFRUF_ANABTYP_GARADE)
                    {
                        strecke s;
                        s.set_stapu(fb.bog().mipu());
                        s.set_endpu(fb.bog().spu());
                        strecke_bezugspunkt sb;
                        sb = strecke_bezugspunkt_ende;
                        s.set_laenge(anweg, sb);
                        if(fa.bezug() == WST_BEZUG_OBSEI)
                        {
                            s.set_farbe(FARBE_BLAU);
                            s.drenen_um_endpu_2d(radToDeg(90), fb.bog().uzs());
                        }else
                        {
                            s.set_farbe(farbe_unterseite);
                            s.set_stil(STIL_GESTRICHELT);
                            s.drenen_um_endpu_2d(radToDeg(180), true);
                        }
                        s.verschieben_um(versatz_x, versatz_y);
                        gt.add_strecke(s);
                    }else if(fa.anfahrtyp() == FAUFRUF_ANABTYP_BOGEN)
                    {
                        bogen b;
                        if(fa.bezug() == WST_BEZUG_OBSEI)
                        {
                            strecke s;
                            s.set_stapu(fb.bog().mipu());
                            s.set_endpu(fb.bog().spu());
                            strecke_bezugspunkt sb;
                            sb = strecke_bezugspunkt_ende;
                            s.set_laenge(anweg, sb);
                            b.set_farbe(FARBE_BLAU);
                            s.drenen_um_endpu_2d(radToDeg(90), fb.bog().uzs());
                            if(fa.radkor() == FRKOR_L)
                            {
                                if(fb.bog().uzs())
                                {
                                    s.drenen_um_endpu_2d(radToDeg(90), true);
                                    s.drenen_um_stapu_2d(radToDeg(90), true);
                                    //b.set_startpunkt(s.endpu());
                                    //b.set_endpunkt(fa.pos());
                                    //b.set_radius(anweg, false);
                                    b.set_bogen(s.endpu(), fa.pos(), anweg, false);
                                }else
                                {
                                    s.drenen_um_stapu_2d(radToDeg(90), fb.bog().uzs());
                                    //b.set_startpunkt(s.endpu());
                                    //b.set_endpunkt(fa.pos());
                                    //b.set_radius(anweg, false);
                                    b.set_bogen(s.endpu(), fa.pos(), anweg, false);
                                }
                            }else if(fa.radkor() == FRKOR_R)
                            {
                                if(fb.bog().uzs())
                                {
                                    s.drenen_um_endpu_2d(radToDeg(90), false);
                                    s.drenen_um_stapu_2d(radToDeg(90), false);
                                    //b.set_startpunkt(s.endpu());
                                    //b.set_endpunkt(fa.pos());
                                    //b.set_radius(anweg, true);
                                    b.set_bogen(s.endpu(), fa.pos(), anweg, true);
                                }else
                                {
                                    s.drenen_um_stapu_2d(radToDeg(90), true);
                                    //b.set_startpunkt(s.endpu());
                                    //b.set_endpunkt(fa.pos());
                                    //b.set_radius(anweg, true);
                                    b.set_bogen(s.endpu(), fa.pos(), anweg, true);
                                }
                            }
                        }else
                        {
                            strecke s;
                            s.set_stapu(fb.bog().mipu());
                            s.set_endpu(fb.bog().epu());
                            strecke_bezugspunkt sb;
                            sb = strecke_bezugspunkt_ende;
                            s.set_laenge(anweg, sb);
                            strecke sehne;
                            sehne.set_stapu(fb.bog().spu());
                            sehne.set_endpu(fb.bog().epu());
                            punkt3d strecke_sp = drehen(sehne.mipu(), s.stapu(), radToDeg(180));
                            punkt3d strecke_ep = drehen(sehne.mipu(), s.endpu(), radToDeg(180));
                            s.set_stapu(strecke_sp);
                            s.set_endpu(strecke_ep);
                            b.set_farbe(farbe_unterseite);
                            b.set_stil(STIL_GESTRICHELT);
                            if(fa.radkor() == FRKOR_L)
                            {
                                if(fb.bog().uzs())
                                {
                                    s.drenen_um_endpu_2d(radToDeg(180), true);
                                    s.drenen_um_stapu_2d(radToDeg(90), false);
                                    //b.set_startpunkt(s.endpu());
                                    //b.set_endpunkt(fa.pos());
                                    //b.set_radius(anweg, true);
                                    b.set_bogen(s.endpu(), fa.pos(), anweg, true);
                                }else
                                {
                                    s.drenen_um_stapu_2d(radToDeg(90), false);
                                    //b.set_startpunkt(s.endpu());
                                    //b.set_endpunkt(fa.pos());
                                    //b.set_radius(anweg, true);
                                    b.set_bogen(s.endpu(), fa.pos(), anweg, true);
                                }
                            }else if(fa.radkor() == FRKOR_R)
                            {
                                if(fb.bog().uzs())
                                {
                                    s.drenen_um_stapu_2d(radToDeg(90), true);
                                    //b.set_startpunkt(s.endpu());
                                    //b.set_endpunkt(fa.pos());
                                    //b.set_radius(anweg, false);
                                    b.set_bogen(s.endpu(), fa.pos(), anweg, false);
                                }else
                                {
                                    s.drenen_um_endpu_2d(radToDeg(180), false);
                                    s.drenen_um_stapu_2d(radToDeg(90), true);
                                    //b.set_startpunkt(s.endpu());
                                    //b.set_endpunkt(fa.pos());
                                    //b.set_radius(anweg, false);
                                    b.set_bogen(s.endpu(), fa.pos(), anweg, false);
                                }
                            }
                        }
                        b.verschieben_um(versatz_x, versatz_y);
                        gt.add_bogen(b);
                    }
                }
            }
        }else if(zeile.at(0) == BEARBART_FRAESERGERADE)
        {
            fraesergerade fg(zeile.text());
            strecke s;
            s = fg.strecke_();
            s.verschieben_um(versatz_x, versatz_y);
            kreis k;
            if(fg.bezug() == WST_BEZUG_OBSEI)
            {
                k.set_farbe(FARBE_BLAU);
            }else
            {
                k.set_farbe(farbe_unterseite);
                k.set_stil(STIL_GESTRICHELT);
            }
            k.set_rad(fraeserdm/2);
            if(radkor == FRKOR_M)
            {
                k.set_mipu(s.mipu());
            }else
            {
                strecke stmp = s;
                stmp.drenen_um_mipu_2d(degToRad(90), true);
                stmp.set_laenge(fraeserdm, strecke_bezugspunkt_mitte);
                if(radkor == FRKOR_L)
                {
                    if(fg.bezug() == WST_BEZUG_OBSEI)
                    {
                        k.set_mipu(stmp.stapu());
                    }else
                    {
                        k.set_mipu(stmp.endpu());
                    }
                }else //if(radkor == FRKOR_R)
                {
                    if(fg.bezug() == WST_BEZUG_OBSEI)
                    {
                        k.set_mipu(stmp.endpu());
                    }else
                    {
                        k.set_mipu(stmp.stapu());
                    }
                }
            }
            gt.add_kreis(k);
            //Abfahrweg:
            double abweg;
            if(letzter_fa.abfahrweg_qstring() == FAUFRUF_ANABWEG_AUTO)
            {
                abweg = fraeserdm;
            }else
            {
                abweg = letzter_fa.abfahrweg();
            }
            bool ist_konturende = false;
            if(i+1 == bearb.count())
            {
                ist_konturende = true;
            }
            if(i+1 < bearb.count())
            {
                text_zw folzei;//Folgezeile
                folzei.set_text(bearb.at(i+1),TRENNZ_BEARB_PARAM);
                if(folzei.at(0) != BEARBART_FRAESERGERADE  &&  folzei.at(0) != BEARBART_FRAESERBOGEN)
                {
                    ist_konturende = true;
                }
            }
            if(ist_konturende == true)
            {
                if(letzter_fa.abfahrtyp() == FAUFRUF_ANABTYP_GARADE)
                {
                    strecke s = fg.strecke_();
                    strecke_bezugspunkt sb;
                    sb = strecke_bezugspunkt_start;
                    s.set_laenge(s.laenge2d()+abweg, sb);
                    sb = strecke_bezugspunkt_ende;
                    s.set_laenge(abweg, sb);
                    if(letzter_fa.bezug() == WST_BEZUG_OBSEI)
                    {
                        s.set_farbe(FARBE_BLAU);
                    }else
                    {
                        s.set_farbe(farbe_unterseite);
                        s.set_stil(STIL_GESTRICHELT);
                    }
                    s.verschieben_um(versatz_x, versatz_y);
                    gt.add_strecke(s);
                }else if(letzter_fa.abfahrtyp() == FAUFRUF_ANABTYP_BOGEN)
                {
                    bogen b;
                    //Hilfs-Strecke:
                    strecke s = fg.strecke_();
                    strecke_bezugspunkt sb;
                    sb = strecke_bezugspunkt_start;
                    s.set_laenge(s.laenge2d()+abweg, sb);
                    sb = strecke_bezugspunkt_ende;
                    s.set_laenge(abweg, sb);
                    //---
                    if(letzter_fa.bezug() == WST_BEZUG_OBSEI)
                    {
                        b.set_farbe(FARBE_BLAU);
                        if(letzter_fa.radkor() == FRKOR_L)
                        {
                            s.drenen_um_endpu_2d(radToDeg(90), false);
                            s.drenen_um_stapu_2d(radToDeg(90), false);
                            //b.set_startpunkt(fg.ep());
                            //b.set_endpunkt(s.stapu());
                            //b.set_radius(abweg, false);
                            b.set_bogen(fg.ep(), s.stapu(), abweg, false);
                        }else if(letzter_fa.radkor() == FRKOR_R)
                        {
                            s.drenen_um_endpu_2d(radToDeg(90), true);
                            s.drenen_um_stapu_2d(radToDeg(90), true);
                            //b.set_startpunkt(fg.ep());
                            //b.set_endpunkt(s.stapu());
                            //b.set_radius(abweg, true);
                            b.set_bogen(fg.ep(), s.stapu(), abweg, true);
                        }
                    }else
                    {
                        b.set_farbe(farbe_unterseite);
                        b.set_stil(STIL_GESTRICHELT);
                        if(letzter_fa.radkor() == FRKOR_L)
                        {
                            s.drenen_um_endpu_2d(radToDeg(90), true);
                            s.drenen_um_stapu_2d(radToDeg(90), true);
                            //b.set_startpunkt(fg.ep());
                            //b.set_endpunkt(s.stapu());
                            //b.set_radius(abweg, true);
                            b.set_bogen(fg.ep(), s.stapu(), abweg, true);
                        }else if(letzter_fa.radkor() == FRKOR_R)
                        {
                            s.drenen_um_endpu_2d(radToDeg(90), false);
                            s.drenen_um_stapu_2d(radToDeg(90), false);
                            //b.set_startpunkt(fg.ep());
                            //b.set_endpunkt(s.stapu());
                            //b.set_radius(abweg, false);
                            b.set_bogen(fg.ep(), s.stapu(), abweg, false);
                        }
                    }
                    b.verschieben_um(versatz_x, versatz_y);
                    gt.add_bogen(b);
                }
            }
        }else if(zeile.at(0) == BEARBART_FRAESERBOGEN)
        {
            fraeserbogen fb(zeile.text());
            bogen b = fb.bog();
            b.verschieben_um(versatz_x, versatz_y);
            kreis k;
            k.set_rad(fraeserdm/2);
            if(fb.bezug() == WST_BEZUG_OBSEI)
            {
                k.set_farbe(FARBE_BLAU);
            }else
            {
                k.set_farbe(farbe_unterseite);
                k.set_stil(STIL_GESTRICHELT);
            }
            strecke s;
            s.set_stapu(b.mipu());
            s.set_endpu(b.mipu_auf_bog());
            if(radkor == FRKOR_M)
            {
                k.set_mipu(b.mipu_auf_bog());
            }else
            {
                if(fb.bezug() == WST_BEZUG_OBSEI)
                {
                    if(radkor == FRKOR_L)
                    {
                        if(b.uzs())
                        {
                            s.set_laenge(b.rad()+fraeserdm/2, strecke_bezugspunkt_start);
                        }else
                        {
                            s.set_laenge(b.rad()-fraeserdm/2, strecke_bezugspunkt_start);
                        }
                    }else //if(radkor == FRKOR_R)
                    {
                        if(b.uzs())
                        {
                            s.set_laenge(b.rad()-fraeserdm/2, strecke_bezugspunkt_start);
                        }else
                        {
                            s.set_laenge(b.rad()+fraeserdm/2, strecke_bezugspunkt_start);
                        }
                    }
                }else if(fb.bezug() == WST_BEZUG_UNSEI)
                {
                    if(radkor == FRKOR_R)
                    {
                        if(b.uzs())
                        {
                            s.set_laenge(b.rad()+fraeserdm/2, strecke_bezugspunkt_start);
                        }else
                        {
                            s.set_laenge(b.rad()-fraeserdm/2, strecke_bezugspunkt_start);
                        }
                    }else //if(radkor == FRKOR_L)
                    {
                        if(b.uzs())
                        {
                            s.set_laenge(b.rad()-fraeserdm/2, strecke_bezugspunkt_start);
                        }else
                        {
                            s.set_laenge(b.rad()+fraeserdm/2, strecke_bezugspunkt_start);
                        }
                    }
                }

                k.set_mipu(s.endpu());
            }
            gt.add_kreis(k);
            //Abfahrweg:
            double abweg;
            if(letzter_fa.abfahrweg_qstring() == FAUFRUF_ANABWEG_AUTO)
            {
                abweg = fraeserdm;
            }else
            {
                abweg = letzter_fa.abfahrweg();
            }
            bool ist_konturende = false;
            if(i+1 == bearb.count())
            {
                ist_konturende = true;
            }
            if(i+1 < bearb.count())
            {
                text_zw folzei;//Folgezeile
                folzei.set_text(bearb.at(i+1),TRENNZ_BEARB_PARAM);
                if(folzei.at(0) != BEARBART_FRAESERGERADE  &&  folzei.at(0) != BEARBART_FRAESERBOGEN)
                {
                    ist_konturende = true;
                }
            }
            if(ist_konturende == true)
            {
                if(letzter_fa.abfahrtyp() == FAUFRUF_ANABTYP_GARADE)
                {
                    strecke s;
                    s.set_stapu(fb.bog().epu());
                    s.set_endpu(fb.bog().mipu());
                    strecke_bezugspunkt sb;
                    sb = strecke_bezugspunkt_start;
                    s.set_laenge(abweg, sb);
                    if(letzter_fa.bezug() == WST_BEZUG_OBSEI)
                    {
                        s.set_farbe(FARBE_BLAU);
                        s.drenen_um_stapu_2d(radToDeg(90), !fb.bog().uzs());
                    }else
                    {
                        s.set_farbe(farbe_unterseite);
                        s.set_stil(STIL_GESTRICHELT);
                        s.drenen_um_stapu_2d(radToDeg(180), true);
                    }
                    s.verschieben_um(versatz_x, versatz_y);
                    gt.add_strecke(s);
                }else if(letzter_fa.anfahrtyp() == FAUFRUF_ANABTYP_BOGEN)
                {
                    bogen b;
                    if(letzter_fa.bezug() == WST_BEZUG_OBSEI)
                    {
                        s.set_stapu(fb.bog().epu());
                        s.set_endpu(fb.bog().mipu());
                        strecke_bezugspunkt sb;
                        sb = strecke_bezugspunkt_start;
                        s.set_laenge(abweg, sb);
                        b.set_farbe(FARBE_BLAU);
                        s.drenen_um_stapu_2d(90, !fb.bog().uzs());
                        s.drenen_um_endpu_2d(90, !fb.bog().uzs());
                        if(letzter_fa.radkor() == FRKOR_L)
                        {
                            if(fb.bog().uzs())
                            {
                                s.drenen_um_endpu_2d(radToDeg(180), false);
                            }
                            //b.set_startpunkt(fb.endpu());
                            //b.set_endpunkt(s.stapu());
                            //b.set_radius(abweg, false);
                            b.set_bogen(fb.bog().epu(), s.stapu(), abweg, false);
                        }else if(letzter_fa.radkor() == FRKOR_R)
                        {
                            if(!fb.bog().uzs())
                            {
                                s.drenen_um_endpu_2d(radToDeg(180), false);
                            }
                            //b.set_startpunkt(fb.endpu());
                            //b.set_endpunkt(s.stapu());
                            //b.set_radius(abweg, true);
                            b.set_bogen(fb.bog().epu(), s.stapu(), abweg, true);
                        }
                    }else
                    {
                        s.set_stapu(fb.bog().spu());
                        s.set_endpu(fb.bog().mipu());
                        strecke sehne;
                        sehne.set_stapu(fb.bog().spu());
                        sehne.set_endpu(fb.bog().epu());
                        punkt3d strecke_sp = drehen(sehne.mipu(), s.stapu(),radToDeg(180));
                        punkt3d strecke_ep = drehen(sehne.mipu(), s.endpu(), radToDeg(180));
                        s.set_stapu(strecke_sp);
                        s.set_endpu(strecke_ep);
                        strecke_bezugspunkt sb;
                        sb = strecke_bezugspunkt_start;
                        s.set_laenge(abweg, sb);
                        b.set_farbe(farbe_unterseite);
                        b.set_stil(STIL_GESTRICHELT);
                        if(letzter_fa.radkor() == FRKOR_L)
                        {
                            if(fb.bog().uzs())
                            {
                                s.drenen_um_stapu_2d(radToDeg(90), fb.bog().uzs());
                                s.drenen_um_endpu_2d(radToDeg(90), !fb.bog().uzs());
                                //b.set_startpunkt(fb.epu());
                                //b.set_endpunkt(s.stapu());
                                //b.set_radius(abweg, fb.bog().uzs());
                                b.set_bogen(fb.bog().epu(), s.stapu(), abweg, fb.bog().uzs());
                            }else
                            {
                                s.drenen_um_endpu_2d(radToDeg(90), !fb.bog().uzs());
                                //b.set_startpunkt(fb.endpu());
                                //b.set_endpunkt(s.stapu());
                                //b.set_radius(abweg, !fb.bog().im_uzs());
                                b.set_bogen(fb.bog().epu(), s.stapu(), abweg, fb.bog().uzs());
                            }
                        }else if(letzter_fa.radkor() == FRKOR_R)
                        {
                            if(fb.bog().uzs())
                            {
                                s.drenen_um_endpu_2d(radToDeg(90), !fb.bog().uzs());
                                //b.set_startpunkt(fb.endpu());
                                //b.set_endpunkt(s.stapu());
                                //b.set_radius(abweg, !fb.bog().im_uzs());
                                b.set_bogen(fb.bog().epu(), s.stapu(), abweg, fb.bog().uzs());
                            }else
                            {
                                s.drenen_um_stapu_2d(radToDeg(90), fb.bog().uzs());
                                s.drenen_um_endpu_2d(radToDeg(90), !fb.bog().uzs());
                                //b.set_startpunkt(fb.endpu());
                                //b.set_endpunkt(s.stapu());
                                //b.set_radius(abweg, fb.bog().im_uzs());
                                b.set_bogen(fb.bog().epu(), s.stapu(), abweg, fb.bog().uzs());
                            }
                        }
                    }
                    b.verschieben_um(versatz_x, versatz_y);
                    gt.add_bogen(b);
                }
            }
        }else
        {
            gt.add_leerzeile();
        }
        gt.zeilenvorschub();
    }
    return gt;
}












