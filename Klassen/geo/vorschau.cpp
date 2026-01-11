#include "vorschau.h"

vorschau::vorschau(QWidget *parent) :
    QWidget(parent)
{
    this->resize(500, 500);
    this->setWindowTitle("Vorschau");
    this->setMouseTracking(true);
    Zf = 1;
    Mrg = false;
    this->setCursor(Qt::CrossCursor);
}

void vorschau::update_cad()
{
    QPainter painter(this);

    //Hintergrund:
    painter.setBrush( Qt::white);
    painter.setPen(Qt::black);
    painter.drawRect(0, 0, width(), height());

    //Geometrieen darstellen:
    for(uint i=0;i<Geotext.count();i++)
    {
        text_zw spalten = Geotext.at(i);
        for(uint ii=0;ii<spalten.count();ii++)
        {
            zeichneGeotext(spalten.at(ii), i);
        }
    }
    this->update();
}

void vorschau::paintEvent(QPaintEvent *)
{
    update_cad();
}

void vorschau::zeichneGeotext(QString geometrieElement, int i)
{
    QPainter painter(this);
    painter.setBrush( Qt::white);
    painter.setPen(Qt::black);

    text_zw element;
    element.set_text(geometrieElement,TRZ_PA_);

    if(element.text().contains(PUNKT))
    {
        punkt3d p;
        p.set_text(element.text());
        p.set_x(p.x()*Sf*Zf);
        p.set_y(p.y()*Sf*Zf);
        QPen pen, pen_alt;
        pen_alt = painter.pen();
        pen.setWidth(p.linienbreite());
        if(i==Aktuelle_zeilennummer)
        {
            pen.setColor(Qt::red);
            pen.setWidth(pen.width()*2);
        }else
        {
            pen.setColor(set_farbe(p.farbe()));
        }
        pen.setCapStyle(Qt::RoundCap);
        painter.setPen(pen);
        painter.drawPoint(N.x()-Npv.x()+p.x(), \
                          N.y()-Npv.y()-p.y());
        painter.setPen(pen_alt);
    }else if(element.text().contains(STRECKE))
    {
        strecke s;
        s.set_text(element.text());
        punkt3d startpunkt, endpunkt;
        startpunkt.set_x(s.stapu().x()*Sf*Zf);
        startpunkt.set_y(s.stapu().y()*Sf*Zf);
        endpunkt.set_x(s.endpu().x()*Sf*Zf);
        endpunkt.set_y(s.endpu().y()*Sf*Zf);

        QPen pen, pen_alt;
        pen_alt = painter.pen();
        pen.setWidth(s.linienbreite());
        pen.setStyle(set_linienstil(s.stil()));
        if(i==Aktuelle_zeilennummer)
        {
            pen.setColor(Qt::red);
            pen.setWidth(pen.width()*2);
        }else
        {
            pen.setColor(set_farbe(s.farbe()));
        }
        pen.setCapStyle(Qt::RoundCap);
        painter.setPen(pen);

        painter.drawLine(N.x()-Npv.x()+startpunkt.x(), \
                         N.y()-Npv.y()-startpunkt.y(), \
                         N.x()-Npv.x()+endpunkt.x(), \
                         N.y()-Npv.y()-endpunkt.y());

        painter.setPen(pen_alt);
    }else if(element.text().contains(BOGEN))
    {
        bogen b;
        b.set_text(element.text());

        double rad = b.rad()*Sf*Zf;
        punkt3d mipu;
        mipu.set_x(b.mipu().x()*Sf*Zf);//Mittelpunkt in X
        mipu.set_y(b.mipu().y()*Sf*Zf);//Mittelpunkt in Y
        punkt3d obli;
        obli.set_x(mipu.x()-rad);
        obli.set_y(mipu.y()+rad);

        double stawi = qRadiansToDegrees(b.swi());
        double endwi = qRadiansToDegrees(b.ewi());
        int stawi16 = static_cast<int>(stawi * 16.0);
        int bogwi16 = static_cast<int>((endwi - stawi) * 16.0);

        QPen pen, pen_alt;
        pen_alt = painter.pen();
        pen.setWidth(b.linienbreite());
        pen.setStyle(set_linienstil(b.stil()));
        if(i==Aktuelle_zeilennummer)
        {
            pen.setColor(Qt::red);
            pen.setWidth(pen.width()*2);
        }else
        {
            pen.setColor(set_farbe(b.farbe()));
        }
        pen.setCapStyle(Qt::RoundCap);
        painter.setPen(pen);

        painter.drawArc(N.x()-Npv.x()+obli.x(),\
                        N.y()-Npv.y()-obli.y(),\
                        rad*2,\
                        rad*2,\
                        stawi16,\
                        bogwi16);

        painter.setPen(pen_alt);
    }else if(element.text().contains(KREIS))
    {
        kreis k;
        k.set_text(element.text());
        double rad = k.radius()*Sf*Zf;
        QPen pen, pen_alt;
        pen_alt = painter.pen();
        pen.setWidth(k.linienbreite());
        pen.setStyle(set_linienstil(k.stil()));
        if(i==Aktuelle_zeilennummer)
        {
            pen.setColor(Qt::red);
            pen.setWidth(pen.width()*2);
        }else
        {
            pen.setColor(set_farbe(k.farbe()));
        }
        pen.setCapStyle(Qt::RoundCap);
        painter.setPen(pen);

        QBrush brush = painter.brush();
        QBrush brush_alt = painter.brush();
        brush.setColor(set_farbe(k.farbe_fuellung()));
        painter.setBrush(brush);

        painter.drawEllipse(N.x()-Npv.x()+(k.mipu().x()*Sf*Zf)-rad,\
                            N.y()-Npv.y()-(k.mipu().y()*Sf*Zf)-rad,\
                            rad*2,\
                            rad*2);

        painter.setPen(pen_alt);
        painter.setBrush(brush_alt);
    }else if(element.text().contains(RECHTECK))
    {
        rechteck r;
        r.set_text(element.text());

        r.set_mipu(N.x()-Npv.x()+r.mipu().x()*Sf*Zf,\
                   N.y()-Npv.y()-r.mipu().y()*Sf*Zf,\
                   r.mipu().z()*Sf*Zf);
        r.set_laenge(r.l()*Sf*Zf);
        r.set_breite(r.b()*Sf*Zf);
        r.set_rad(r.rad()*Sf*Zf);

        QPen pen, pen_alt;
        pen_alt = painter.pen();
        pen.setWidth(r.linienbreite());
        pen.setStyle(set_linienstil(r.stil()));
        pen.setCapStyle(Qt::RoundCap);
        if(i==Aktuelle_zeilennummer)
        {
            pen.setColor(Qt::red);
            pen.setWidth(pen.width()*2);
        }else
        {
            pen.setColor(set_farbe(r.farbe()));
        }
        painter.setPen(pen);

        QBrush brush = painter.brush();
        QBrush brush_alt = painter.brush();
        brush.setColor(set_farbe(r.farbe_fuellung()));
        painter.setBrush(brush);

        if(r.rad()==0)
        {
            //Rechteck mit eckigen Ecken:
            QPainterPath pp;
            pp.moveTo(r.unl().x(),\
                      r.unl().y());
            pp.lineTo(r.unr().x(),\
                      r.unr().y());
            pp.lineTo(r.obr().x(),\
                      r.obr().y());
            pp.lineTo(r.obl().x(),\
                      r.obl().y());
            pp.closeSubpath();
            painter.drawPath(pp);

        }else
        {
            //Rechteck runden Ecken:
            punkt3d mitpu = r.mipu();
            rechteck r_innen = r;//Die Eckpunkte dieses Rechteckes sind die Mittelpunkte der Kreisbögen der Ecken
            r_innen.set_laenge(r.l()-r.rad()*2);
            r_innen.set_breite(r.b()-r.rad()*2);
            rechteck r_ecke;//Rechteck um je eine der abgerundeten Ecken
            r_ecke.set_laenge(r.rad()*2);
            r_ecke.set_breite(r.rad()*2);
            QPainterPath pp;

            //Startpunkt:
                //Startpunkt ist unten links am Anfang der Rundung
                //wir zeichnen gegen den Uhrzeigersinn
                //Nullpunkt des Fensters ist oben links, desshalb ist Y invertiert und oben ist unten
            punkt3d unli_1;
            unli_1.set_x( mitpu.x() - r.l()/2 );
            unli_1.set_y( mitpu.y() + r.b()/2 - r.rad() );
            unli_1.drehen(mitpu, r.drewi());
            pp.moveTo(unli_1.x(), unli_1.y());
            //Rundung links unten:
            r_ecke.set_mipu(r_innen.obl());
            pp.arcTo(r_ecke.unl().x(), r_ecke.unl().y(), r_ecke.l(), r_ecke.b(), \
                     qRadiansToDegrees(r.drewi())+180, 90);
            //Linie nach rechts unten:            
            punkt3d unre_1;
            unre_1.set_x( mitpu.x() + r.l()/2 - r.rad() );
            unre_1.set_y( mitpu.y() + r.b()/2 );
            unre_1.drehen(mitpu, r.drewi());
            pp.lineTo(unre_1.x(), unre_1.y());
            //Rundung rechts unten:
            r_ecke.set_mipu(r_innen.obr());
            pp.arcTo(r_ecke.unl().x(), r_ecke.unl().y(), r_ecke.l(), r_ecke.b(), \
                     qRadiansToDegrees(r.drewi())+270, 90);
            //Linie nach rechts oben:
            punkt3d obre_1;
            obre_1.set_x( mitpu.x() + r.l()/2 );
            obre_1.set_y( mitpu.y() - r.b()/2 + r.rad() );
            obre_1.drehen(mitpu, r.drewi());
            pp.lineTo(obre_1.x(), obre_1.y());
            //Rundung rechts oben
            r_ecke.set_mipu(r_innen.unr());
            pp.arcTo(r_ecke.unl().x(), r_ecke.unl().y(), r_ecke.l(), r_ecke.b(), \
                     qRadiansToDegrees(r.drewi())+0, 90);
            //Linie nach links oben:
            punkt3d obli_1;
            obli_1.set_x( mitpu.x() - r.l()/2 + r.rad() );
            obli_1.set_y( mitpu.y() - r.b()/2 );
            obli_1.drehen(mitpu, r.drewi());
            pp.lineTo(obli_1.x(), obli_1.y());
            //Rundung links oben
            r_ecke.set_mipu(r_innen.unl());
            pp.arcTo(r_ecke.unl().x(), r_ecke.unl().y(), r_ecke.l(), r_ecke.b(), \
                     qRadiansToDegrees(r.drewi())+90, 90);
            //Kontur schließen:
            pp.closeSubpath();

            painter.drawPath(pp);
        }

        painter.setPen(pen_alt);
        painter.setBrush(brush_alt);
    }
}

void vorschau::slot_aktualisieren(geo_text gt, int aktive_zeile)
{
    Geotext = gt;
    Aktuelle_zeilennummer = aktive_zeile;
    slot_aktualisieren();
}
void vorschau::slot_aktualisieren()
{
    //Zoomfakrotr berechnen:
    int randabstand = 20;
    float maximallaenge = Geotext.max_x() - Geotext.min_x();
    float maximalbreite = Geotext.max_y() - Geotext.min_y();

    float bildlaenge = width()-randabstand*2;
    float bildbreite = height()-randabstand*2;

    float faktor_laenge = bildlaenge/maximallaenge;
    float faktor_breite = bildbreite/maximalbreite;

    if(faktor_laenge < faktor_breite)
    {
        set_sf(faktor_laenge);
    }else
    {
        set_sf(faktor_breite);
    }

    double basispunkt_x = randabstand;
    double basispunkt_y = height()-randabstand;

    N.set_x( basispunkt_x - Geotext.min_x()*Sf * Zf );
    N.set_y( basispunkt_y + Geotext.min_y()*Sf * Zf );

    this->update();
}

void vorschau::slot_aktives_Element_einfaerben(int zeilennummer)
{
    //Element aus dieser Zeile farbig markieren
    Aktuelle_zeilennummer = zeilennummer;
    this->update();
}

//-------------------------------------------------------------
float vorschau::sf()
{
    return Sf;
}

//-------------------------------------------------------------
void vorschau::set_sf(float neuer_faktor)
{
    Sf = neuer_faktor;
}
void vorschau::zoom(bool dichter)
{
    if(dichter == true)
    {
        Zf = Zf + Zf/25;
    }else
    {
        if(Zf > 1)
        {
            Zf = Zf - Zf/25;
        }else
        {
            //Npv.x = 0;
            //Npv.y = 0;
        }
    }

}
punkt3d vorschau::mauspos_vom_np()
{
    QPoint p = this->mapFromGlobal(QCursor::pos());
    int abst_nullp_x = p.x() - N.x() + Npv.x();
    int abst_nullp_y = p.y() - N.y() + Npv.y();

    punkt3d p2d;
    p2d.set_x(abst_nullp_x/Sf/Zf);
    p2d.set_y(abst_nullp_y/Sf*-1/Zf);
    return p2d;
}
uint vorschau::zeile_von_Mauspos()
{
    uint zeile = 0;
    double abst = 9999999999;    
    geo_text geotext = Geotext;

    for(uint i=0;i<geotext.count();i++)
    {
        text_zw spalten = geotext.at(i);
        for(uint ii=0;ii<spalten.count();ii++)
        {
            text_zw element;
            element.set_text(spalten.at(ii),TRZ_PA_);

            if(element.text().contains(PUNKT))
            {
                strecke s; //nehmen wir für Längenberechnung/Abstandsberechnung
                s.set_stapu(mauspos_vom_np());
                punkt3d ep;
                ep.set_text(element.text());
                s.set_endpu(ep);
                double l = s.laenge2d();
                if(l < abst)
                {
                    abst = l;
                    zeile = i;
                }
            }else if(element.text().contains(STRECKE))
            {
                strecke s2;
                s2.set_text(element.text());
                double l = s2.abst(mauspos_vom_np());
                if(l < abst)
                {
                    abst = l;
                    zeile = i;
                }
            }else if(element.text().contains(BOGEN))
            {
                bogen b;
                b.set_text(element.text());
                double l = b.abst(mauspos_vom_np());
                if(l < abst)
                {
                    abst = l;
                    zeile = i;
                }
            }else if(  element.text().contains(KREIS)  )
            {
                kreis k;
                k.set_text(element.text());
                double l = k.abst(mauspos_vom_np());
                if(l < abst)
                {
                    abst = l;
                    zeile = i;
                }
            }else if(  element.text().contains(RECHTECK)  )
            {
                rechteck r;
                r.set_text(element.text());
                double l = r.abst(mauspos_vom_np());
                if(l < abst)
                {
                    abst = l;
                    zeile = i;
                }
            }
        }
    }
    return zeile;
}
void vorschau::mouseMoveEvent(QMouseEvent *event)
{
    if(Mrg)
    {
        Npv.set_x( Npv.x() + Maus_pos_alt_x - event->pos().x() );
        Npv.set_y( Npv.y() + Maus_pos_alt_y - event->pos().y() );
        Maus_pos_alt_x = event->pos().x();
        Maus_pos_alt_y = event->pos().y();
        this->update();
    }

    QPoint p = this->mapFromGlobal(QCursor::pos());
    int abst_nullp_x = p.x() - N.x()+Npv.x();
    int abst_nullp_y = p.y() - N.y()+Npv.y();

    QPoint p_real;
    p_real.setX(abst_nullp_x/Sf/Zf);
    p_real.setY(abst_nullp_y/Sf*-1/Zf);

    emit sende_maus_pos(p_real);
}
void vorschau::wheelEvent(QWheelEvent *event)
{
    QPointF mauspos = event->position();
    mauspos.setY(this->height()-mauspos.y());
    QPoint wstpos;
    wstpos.setX(  (mauspos.x() - N.x()+Npv.x())  /Sf/Zf  );
    wstpos.setY(  (N.x()+Npv.y() - mauspos.y())  /Sf/Zf  );

    int i = event->angleDelta().y();
    if(i<0)
    {
        zoom(false);
    }else
    {
        zoom(true);
    }

    QPoint wstposneu;
    wstposneu.setX(  (mauspos.x() - N.x()+Npv.x())  /Sf/Zf  );
    wstposneu.setY(  (N.x()+Npv.y() - mauspos.y())  /Sf/Zf  );
    int xversmm = wstposneu.x() - wstpos.x();
    int yversmm = wstposneu.y() - wstpos.y();
    int xverspixel = xversmm*Sf*Zf;
    int yverspixel = yversmm*Sf*Zf;
    Npv.set_x(Npv.x()-xverspixel);
    Npv.set_y(Npv.y()-yverspixel);
    if(Zf<=1)
    {
        Npv.set_x(0);
        Npv.set_y(0);
    }
    this->update();
}
void vorschau::mousePressEvent(QMouseEvent *event)
{
    if(event->button() == Qt::MiddleButton)
    {
        Mrg = true;
        Maus_pos_alt_x = event->pos().x();
        Maus_pos_alt_y = event->pos().y();
    }else if(event->button() == Qt::RightButton)
    {
        punkt3d p;
        p = mauspos_vom_np();
        QString msg_pos;
        msg_pos += "(X: ";
        msg_pos += p.x_QString();
        msg_pos += " / Y: ";
        msg_pos += p.y_QString();
        msg_pos += ")";

        uint zeile = zeile_von_Mauspos();
        Zeile_von_maus_pos = zeile;
        slot_aktives_Element_einfaerben(zeile);

        emit sende_zeilennummer(Zeile_von_maus_pos, false);

        QMenu m(this);
        m.addAction("Ansicht einpassen", 0, this, SLOT(slot_zf_gleich_eins()));
        m.addAction(msg_pos, 0, this, SLOT(slot_tunix())) ;
        m.exec(this->mapFrom(this, QCursor::pos()));

    }else if(event->button() == Qt::LeftButton)
    {
        uint zeile = zeile_von_Mauspos();
        Zeile_von_maus_pos = zeile;
        slot_aktives_Element_einfaerben(zeile);
        sende_zeilennummer(Zeile_von_maus_pos, false);
    }
}
void vorschau::slot_sende_zeilennummer()
{
    uint zeile = Zeile_von_maus_pos;
    slot_aktives_Element_einfaerben(zeile);
    emit sende_zeilennummer(zeile, true);
}
void vorschau::mouseReleaseEvent(QMouseEvent *event)
{
    if(event->button() == Qt::MiddleButton)
    {
        Mrg = false;
    }
}
void vorschau::slot_zf_gleich_eins()
{
    Zf = 1;
    Npv.set_x(0);
    Npv.set_y(0);
    update_cad();
}
void vorschau::slot_tunix()
{

}
QColor vorschau::set_farbe(QString farbe)
{
    QColor qfarbe = Qt::black;
    if(farbe == FARBE_KEINE)
    {
        qfarbe = Qt::transparent;
    }else if(farbe == FARBE_SCHWARZ)
    {
        qfarbe = Qt::black;
    }else if(farbe == FARBE_WEISS)
    {
        qfarbe = Qt::white;
    }else if(farbe == FARBE_BLAU)
    {
        qfarbe = Qt::blue;
    }else if(farbe == FARBE_HELLBLAU)
    {
        qfarbe.setRgb(0,240,240);
    }else if(farbe == FARBE_ROT)
    {
        qfarbe = Qt::red;
    }else if(farbe == FARBE_ROSE)
    {
        qfarbe.setRgb(255,0,128);
    }else if(farbe == FARBE_GRUEN)
    {
        qfarbe = Qt::green;
    }else if(farbe == FARBE_GELB)
    {
        qfarbe = Qt::yellow;
    }else if(farbe == FARBE_GRAU)
    {
        qfarbe = Qt::gray;
    }else if(farbe == FARBE_DUNKELGRAU)
    {
        qfarbe = Qt::darkGray;
    }else if(farbe == FARBE_BRAUN)
    {
        qfarbe.setRgb(185,122,87);
    }
    return qfarbe;
}
Qt::PenStyle vorschau::set_linienstil(QString stil)
{
    Qt::PenStyle style = Qt::SolidLine;
    if(stil == STIL_TRANSPARENT)
    {
        style = Qt::NoPen;
    }else if(stil == STIL_TRANSPARENT)
    {
        style = Qt::NoPen;
    }else if(stil == STIL_DURCHGEHEND)
    {
        style = Qt::SolidLine;
    }else if(stil == STIL_GESTRICHELT)
    {
        style = Qt::DashLine;
    }else if(stil == STIL_GEPUNKTET)
    {
        style = Qt::DotLine;
    }else if(stil == STIL_STRICHP)
    {
        style = Qt::DashDotLine;
    }else if(stil == STIL_STRICHPP)
    {
        style = Qt::DashDotDotLine;
    }

    return style;
}


















