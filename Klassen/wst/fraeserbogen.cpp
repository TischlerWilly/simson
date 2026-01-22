#include "fraeserbogen.h"

fraeserbogen::fraeserbogen()
{
    setup();
}

fraeserbogen::fraeserbogen(QString text)
{
    setup();
    set_text(text);
}

void fraeserbogen::setup()
{
    Afb = "1";
    Bezug = WST_BEZUG_OBSEI;
}

void fraeserbogen::set_bogen(bogen b)
{
    Bogen = b;
}
void fraeserbogen::set_bezug(QString bezugsflaeche)
{
    Bezug = bezugsflaeche;
}
void fraeserbogen::set_afb(QString ausfuehrbedingung)
{
    Afb = ausfuehrbedingung;
}
void fraeserbogen::set_tiSta(double ti)
{
    TiStart = ti;
}
void fraeserbogen::set_tiEnd(double ti)
{
    TiEnde = ti;
}

bogen fraeserbogen::bog()
{
    return Bogen;
}
bogen *fraeserbogen::bog_ptr()
{
    return &Bogen;
}
double fraeserbogen::xs()
{
    return Bogen.spu().x();
}
QString fraeserbogen::xs_qstring()
{
    return Bogen.spu().x_QString();
}
double fraeserbogen::xe()
{
    return Bogen.epu().x();
}
QString fraeserbogen::xe_qstring()
{
    return Bogen.epu().x_QString();
}
double fraeserbogen::ys()
{
    return Bogen.spu().y();
}
QString fraeserbogen::ys_qstring()
{
    return Bogen.spu().y_QString();
}
double fraeserbogen::ye()
{
    return Bogen.epu().y();
}
QString fraeserbogen::ye_qstring()
{
    return Bogen.epu().y_QString();
}
double fraeserbogen::zs()
{
    return Bogen.spu().z();
}
QString fraeserbogen::zs_qstring()
{
    return Bogen.spu().z_QString();
}
double fraeserbogen::ze()
{
    return Bogen.epu().z();
}
QString fraeserbogen::ze_qstring()
{
    return Bogen.epu().z_QString();
}
punkt3d fraeserbogen::sp()
{
    return bog().spu();
}
punkt3d fraeserbogen::ep()
{
    return bog().epu();
}
QString fraeserbogen::bezug()
{
    return Bezug;
}
QString fraeserbogen::afb()
{
    return Afb;
}
double fraeserbogen::rad()
{
    return bog().rad();
}
QString fraeserbogen::rad_qstring()
{
    return bog().rad_QString();
}
double fraeserbogen::tiSta()
{
    return TiStart;
}
QString fraeserbogen::tiSta_qstring()
{
    return double_to_qstring(TiStart);
}
double fraeserbogen::tiEnd()
{
    return TiEnde;
}
QString fraeserbogen::tiEnd_qstring()
{
    return double_to_qstring(TiEnde);
}


QString fraeserbogen::text()
{
    QString msg = BEARBART_FRAESERBOGEN;     //Zeile 0
    msg += TRENNZ_BEARB_PARAM_;
    msg += bezug();                      //Zeile 1
    msg += TRENNZ_BEARB_PARAM_;
    msg += bog().mipu().x_QString();     //Zeile 2
    msg += TRENNZ_BEARB_PARAM_;
    msg += bog().mipu().y_QString();     //Zeile 3
    msg += TRENNZ_BEARB_PARAM_;
    msg += tiSta_qstring();              //Zeile 4
    msg += TRENNZ_BEARB_PARAM_;
    msg += tiEnd_qstring();              //Zeile 5
    msg += TRENNZ_BEARB_PARAM_;
    msg += rad_qstring();                //Zeile 6
    msg += TRENNZ_BEARB_PARAM_;
    msg += bog().swi_QString();          //Zeile 7
    msg += TRENNZ_BEARB_PARAM_;
    msg += bog().ewi_QString();          //Zeile 8
    msg += TRENNZ_BEARB_PARAM_;
    msg += bog().uzs_QString();          //Zeile 9
    msg += TRENNZ_BEARB_PARAM_;
    msg += afb();                        //Zeile 10

    return msg;
}
void fraeserbogen::set_text(QString text)
{
    text_zw tz;
    tz.set_text(text,TRENNZ_BEARB_PARAM);
    if(tz.at(0) == BEARBART_FRAESERBOGEN)
    {
        set_bezug(tz.at(1));                    //Zeile 1
        double mipu_x = tz.at(2).toDouble();    //Zeile 2
        double mipu_y = tz.at(3).toDouble();    //Zeile 3
        set_tiSta(tz.at(4).toDouble());         //Zeile 4
        set_tiEnd(tz.at(5).toDouble());         //Zeile 5
        double rad = tz.at(6).toDouble();       //Zeile 6
        double swi = tz.at(7).toDouble();       //Zeile 7
        double ewi = tz.at(8).toDouble();       //Zeile 8
        Bogen.set_uzs(tz.at(9));                //Zeile 9
        set_afb(tz.at(10));                     //Zeile 10

        Bogen.set_mipu(mipu_x, mipu_y,0);
        Bogen.set_rad(rad);
        Bogen.set_swi(swi);
        Bogen.set_ewi(ewi);
    }
}




















