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
                double x = r.obr().x();
                if(x > max)
                {
                    max = x;
                }
                x = r.obl().x();
                if(x > max)
                {
                    max = x;
                }
                x = r.unl().x();
                if(x > max)
                {
                    max = x;
                }
                x = r.unr().x();
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
                double x = r.obr().x();
                if(x < min)
                {
                    min = x;
                }
                x = r.obl().x();
                if(x < min)
                {
                    min = x;
                }
                x = r.unl().x();
                if(x < min)
                {
                    min = x;
                }
                x = r.unr().x();
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
                double y = r.obr().y();
                if(y > max)
                {
                    max = y;
                }
                y = r.obl().y();
                if(y > max)
                {
                    max = y;
                }
                y = r.unl().y();
                if(y > max)
                {
                    max = y;
                }
                y = r.unr().y();
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
                double y = r.obr().y();
                if(y < min)
                {
                    min = y;
                }
                y = r.obl().y();
                if(y < min)
                {
                    min = y;
                }
                y = r.unl().y();
                if(y < min)
                {
                    min = y;
                }
                y = r.unr().y();
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


