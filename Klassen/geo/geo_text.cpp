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
    geo.set_trenz(TRZ_PA_);
    geo.add_hi(PUNKT);
    geo.add_hi(p.x_QString());
    geo.add_hi(p.y_QString());
    geo.add_hi(p.z_QString());
    geo.add_hi(p.farbe());
    geo.add_hi(p.linienbreite_qstring());
    add(geo, index);
}
void geo_text::add_strecke(strecke s)
{
    add_strecke(s, AktIndex);
}
void geo_text::add_strecke(strecke s, uint index)
{
    text_zw geo;
    geo.set_trenz(TRZ_PA_);
    geo.add_hi(STRECKE);
    geo.add_hi(s.stapu().x_QString());
    geo.add_hi(s.stapu().y_QString());
    geo.add_hi(s.stapu().z_QString());
    geo.add_hi(s.endpu().x_QString());
    geo.add_hi(s.endpu().y_QString());
    geo.add_hi(s.endpu().z_QString());
    geo.add_hi(s.farbe());
    geo.add_hi(s.linienbreite_qstring());
    geo.add_hi(s.stil());
    add(geo, index);
}
void geo_text::add_bogen(bogen b)
{
    add_bogen(b, AktIndex);
}
void geo_text::add_bogen(bogen b, uint index)
{
    text_zw geo;
    geo.set_trenz(TRZ_PA_);
    geo.add_hi(BOGEN);
    geo.add_hi(b.mipu().x_QString());
    geo.add_hi(b.mipu().y_QString());
    geo.add_hi(b.mipu().z_QString());
    geo.add_hi(b.rad_QString());
    geo.add_hi(b.swi_QString());
    geo.add_hi(b.ewi_QString());
    geo.add_hi(b.farbe());
    geo.add_hi(b.linienbreite_qstring());
    geo.add_hi(b.stil());
    add(geo, index);
}
void geo_text::add_kreis(kreis k)
{
    add_kreis(k, AktIndex);
}
void geo_text::add_kreis(kreis k, uint index)
{
    text_zw geo;
    geo.set_trenz(TRZ_PA_);
    geo.add_hi(KREIS);
    geo.add_hi(k.mipu().x_QString());
    geo.add_hi(k.mipu().y_QString());
    geo.add_hi(k.mipu().z_QString());
    geo.add_hi(k.radius_QString());
    geo.add_hi(k.farbe());
    geo.add_hi(k.farbe_fuellung());
    geo.add_hi(k.linienbreite_qstring());
    geo.add_hi(k.stil());
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
                gt.add_kreis(k);
                strecke s;
                s.set_farbe(k.farbe());
                s.set_stapu(bo.x() - bo.dm()/2 - 2, bo.y(), bo.z());
                s.set_endpu(bo.x() + bo.dm()/2 + 2, bo.y(), bo.z());
                s.verschieben_um(versatz_x, versatz_y);
                gt.add_strecke(s);
                s.drenen_um_mipu_2d(degToRad(90));
                gt.add_strecke(s);
            }else if(bo.bezug() == WST_BEZUG_UNSEI)
            {
                kreis k;
                k.set_farbe(farbe_unterseite);
                k.set_rad(bo.dm()/2);
                k.set_mipu(bo.mipu());
                if(bo.tiefe() > wst_d)
                {
                    k.set_farbe_fuellung(FARBE_WEISS);
                }else
                {
                    k.set_farbe_fuellung(FARBE_HELLBLAU);
                }
                k.verschieben_um(versatz_x, versatz_y);
                gt.add_kreis(k);
                strecke s;
                s.set_farbe(k.farbe());
                s.set_stapu(bo.x() - bo.dm()/2 - 2, bo.y(), bo.z());
                s.set_endpu(bo.x() + bo.dm()/2 + 2, bo.y(), bo.z());
                s.drenen_um_mipu_2d(degToRad(45));
                s.verschieben_um(versatz_x, versatz_y);
                gt.add_strecke(s);
                s.drenen_um_mipu_2d(degToRad(90));
                gt.add_strecke(s);
            }else if(bo.bezug() == WST_BEZUG_LI)
            {
                rechteck r;
                r.set_farbe_fuellung(FARBE_BRAUN);
                r.set_laenge(bo.tiefe());
                r.set_breite(bo.dm());
                r.set_li(bo.x(), bo.y(), 0);
                r.verschieben_um(versatz_x, versatz_y);
                gt.add_rechteck(r);
                kreis k;
                k.set_farbe_fuellung(FARBE_BRAUN);
                k.set_rad(bo.dm()/2);
                k.set_mipu(nullpunkt_li.x()-bo.z(),nullpunkt_li.y()+bo.y(), 0);
                gt.add_kreis(k);
            }else if(bo.bezug() == WST_BEZUG_RE)
            {
                rechteck r;
                r.set_farbe_fuellung(FARBE_BRAUN);
                r.set_laenge(bo.tiefe());
                r.set_breite(bo.dm());
                r.set_re(bo.x(), bo.y(), 0);
                r.verschieben_um(versatz_x, versatz_y);
                gt.add_rechteck(r);
                kreis k;
                k.set_farbe_fuellung(FARBE_BRAUN);
                k.set_rad(bo.dm()/2);
                k.set_mipu(nullpunkt_re.x()+bo.z(),nullpunkt_re.y()+bo.y(), 0);
                gt.add_kreis(k);
            }else if(bo.bezug() == WST_BEZUG_VO)
            {
                rechteck r;
                r.set_farbe_fuellung(FARBE_BRAUN);
                r.set_laenge(bo.dm());
                r.set_breite(bo.tiefe());
                r.set_un(bo.x(), bo.y(), 0);
                r.verschieben_um(versatz_x, versatz_y);
                gt.add_rechteck(r);
                kreis k;
                k.set_farbe_fuellung(FARBE_BRAUN);
                k.set_rad(bo.dm()/2);
                k.set_mipu(nullpunkt_vo.x()+bo.x(),nullpunkt_vo.y()-bo.z(), 0);
                gt.add_kreis(k);
            }else if(bo.bezug() == WST_BEZUG_HI)
            {
                rechteck r;
                r.set_farbe_fuellung(FARBE_BRAUN);
                r.set_laenge(bo.dm());
                r.set_breite(bo.tiefe());
                r.set_ob(bo.x(), bo.y(), 0);
                r.verschieben_um(versatz_x, versatz_y);
                gt.add_rechteck(r);
                kreis k;
                k.set_farbe_fuellung(FARBE_BRAUN);
                k.set_rad(bo.dm()/2);
                k.set_mipu(nullpunkt_hi.x()+bo.x(),nullpunkt_hi.y()+bo.z(), 0);
                gt.add_kreis(k);
            }
        }else if(zeile.at(0) == BEARBART_BOHRRASTER)
        {
            bohrraster bo(zeile.text());
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
                r.set_farbe_fuellung(FARBE_BRAUN);
                r.set_laenge(bo.tiefe());
                r.set_li(0, bo.y(), 0);
                r.verschieben_um(versatz_x, versatz_y);
                kreis k;
                k.set_farbe_fuellung(FARBE_BRAUN);
                k.set_rad(bo.dm()/2);
                k.set_mipu(nullpunkt_li.x()-bo.z(),nullpunkt_li.y()+bo.y(), 0);
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
                r.set_farbe_fuellung(FARBE_BRAUN);
                r.set_laenge(bo.tiefe());
                r.set_breite(bo.dm());
                r.set_re(wst_l, bo.y(), 0);
                r.verschieben_um(versatz_x, versatz_y);
                kreis k;
                k.set_farbe_fuellung(FARBE_BRAUN);
                k.set_rad(bo.dm()/2);
                k.set_mipu(nullpunkt_re.x()+bo.z(),nullpunkt_re.y()+bo.y(), 0);
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
                r.set_farbe_fuellung(FARBE_BRAUN);
                r.set_laenge(bo.dm());
                r.set_breite(bo.tiefe());
                r.set_un(bo.x(), 0, 0);
                r.verschieben_um(versatz_x, versatz_y);
                kreis k;
                k.set_farbe_fuellung(FARBE_BRAUN);
                k.set_rad(bo.dm()/2);
                k.set_mipu(nullpunkt_vo.x()+bo.x(),nullpunkt_vo.y()-bo.z(), 0);
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
                r.set_farbe_fuellung(FARBE_BRAUN);
                r.set_laenge(bo.dm());
                r.set_breite(bo.tiefe());
                r.set_ob(bo.x(), wst_b, 0);
                r.verschieben_um(versatz_x, versatz_y);
                kreis k;
                k.set_farbe_fuellung(FARBE_BRAUN);
                k.set_rad(bo.dm()/2);
                k.set_mipu(nullpunkt_hi.x()+bo.x(),nullpunkt_hi.y()+bo.z(), 0);
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
                }else if(nu.bezug() == WST_BEZUG_RE)
                {
                    r.set_laenge(nu.tiefe());
                    r.set_breite(s.laenge2d());
                    r.set_drewi(0);
                }else if(nu.bezug() == WST_BEZUG_VO)
                {
                    r.set_laenge(s.laenge2d());
                    r.set_breite(nu.tiefe());
                    r.set_drewi(0);
                }else if(nu.bezug() == WST_BEZUG_HI)
                {
                    r.set_laenge(s.laenge2d());
                    r.set_breite(nu.tiefe());
                    r.set_drewi(0);
                }
                r.set_farbe_fuellung(FARBE_GELB);
                punkt3d mipu = s.mipu();
                strecke s_mipu;
                s_mipu.set_stapu(mipu);
                s_mipu.set_endpu(s.endpu());
                s_mipu.set_laenge(nu.tiefe()/2, strecke_bezugspunkt_start);
                s_mipu.drenen_um_stapu_2d(degToRad(90));
                mipu = s_mipu.endpu();
                r.set_mipu(mipu);
                r.verschieben_um(versatz_x, versatz_y);
                //Start anzeigen:
                strecke stmp = s;
                stmp.drenen_um_stapu_2d(degToRad(90));
                stmp.set_laenge(stmp.laenge2d()+nu.tiefe(), strecke_bezugspunkt_ende);
                stmp.set_laenge(30, strecke_bezugspunkt_start);
                kreis k;
                k.set_mipu(stmp.endpu());
                k.set_rad(30);
                k.set_farbe(FARBE_GELB);
                gt.add_kreis(k);
                //---
                gt.add_rechteck(r);
            }
        }else if(zeile.at(0) == BEARBART_RTA)
        {
            rechtecktasche rt(zeile.text());
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
                r.set_laenge(rt.laenge());
                r.set_breite(rt.breite());
                r.set_mipu(rt.x(), rt.y(), rt.z());
                r.set_drewi(rt.drewi());
                r.verschieben_um(versatz_x, versatz_y);
                r.set_rad(eckenrad);
                gt.add_rechteck(r);
                if(rt.ausraeumen() == false)
                {
                    r.set_laenge(r.l()-wkzdm);
                    r.set_breite(r.b()-wkzdm);
                    r.set_rad(eckenrad-wkzdm/2);
                    r.set_farbe_fuellung(FARBE_GRAU);
                    gt.add_rechteck(r);
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
                gt.add_rechteck(r);
                r_kantenansicht.set_rad(eckenrad);
                r_kantenansicht.set_farbe_fuellung(r.farbe_fuellung());
                r_kantenansicht.set_farbe(r.farbe());
                gt.add_rechteck(r_kantenansicht);
            }

        }else if(zeile.at(0) == BEARBART_FRAESERAUFRUF)
        {
            fraeseraufruf fa(zeile.text());
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
            gt.add_punkt(p);
        }else if(zeile.at(0) == BEARBART_FRAESERGERADE)
        {
            fraesergerade fg(zeile.text());
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
            gt.add_strecke(s);
        }else if(zeile.at(0) == BEARBART_FRAESERBOGEN)
        {
            fraeserbogen fb(zeile.text());
            bogen b;
            b.set_startpunkt(fb.stapu());
            b.set_endpunkt(fb.endpu());
            if(fb.bezug() == WST_BEZUG_OBSEI)
            {
                b.set_rad(fb.rad(), fb.uzs());
                b.set_farbe(FARBE_BLAU);
            }else
            {
                b.set_rad(fb.rad(), !fb.uzs());
                b.set_farbe(farbe_unterseite);
                b.set_stil(STIL_GESTRICHELT);
            }
            b.verschieben_um(versatz_x, versatz_y);
            gt.add_bogen(b);
        }
        gt.zeilenvorschub();
    }

    return gt;
}


