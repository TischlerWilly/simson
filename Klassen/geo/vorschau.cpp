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
     painter.setRenderHint(QPainter::Antialiasing); // Optional: Schöne glatte Linien

    // Hintergrund (ohne Transformation)
    painter.fillRect(rect(), Qt::white);

     // Transformation anwenden
     painter.translate(N.x() - Npv.x(), N.y() - Npv.y());
     painter.scale(Sf * Zf, -Sf * Zf); // Y invertieren für CAD-Koordinaten


    // Geometrien zeichnen - Painter wird mit & übergeben
    for(uint i=0;i<Geotext.count();i++)
    {
        text_zw spalten = Geotext.at(i);
        for(uint ii=0;ii<spalten.count();ii++)
        {
            zeichneGeotext(painter, spalten.at(ii), i);
        }
    }
    //Fräser darstellen:
    for(uint i=0;i<GeoFkon.count();i++)
    {
        text_zw spalten = GeoFkon.at(i);
        for(uint ii=0;ii<spalten.count();ii++)
        {
            zeichneGeotext(painter, spalten.at(ii), i);
        }
    }
}

void vorschau::paintEvent(QPaintEvent *)
{
    update_cad();
}

void vorschau::zeichneGeotext(QPainter &painter, QString geometrieElement, int i)
{
    painter.setBrush(Qt::white);
    painter.setPen(Qt::black);

    text_zw element;
    element.set_text(geometrieElement,TRZ_PA_);

    if(element.text().contains(PUNKT))
    {
        painter.save(); // Speichert den kompletten Zustand (Stift, Pinsel, Transformation)

        punkt3d p;
        p.set_text(element.text());
        QPen pen;
        pen.setWidth(p.linienbreite());
        if(i==Aktuelle_zeilennummer)
        {
            pen.setColor(Qt::red);
            pen.setWidth(p.linienbreite()*2);
        }else
        {
            pen.setColor(set_farbe(p.farbe()));
            pen.setWidth(p.linienbreite());
        }
        pen.setCapStyle(Qt::RoundCap);

        // Verhindert, dass Punkte beim Zoomen riesig werden:
        pen.setCosmetic(true);

        painter.setPen(pen);
        painter.drawPoint(p.x(), p.y());

        painter.restore(); // Stellt den Zustand exakt so wieder her, wie er bei save() war
    }else if(element.text().contains(STRECKE))
    {
        painter.save();
        strecke s;
        s.set_text(element.text());
        QPen pen;
        pen.setCosmetic(true);
        pen.setStyle(set_linienstil(s.stil()));
        if (i == Aktuelle_zeilennummer)
        {
            pen.setColor(Qt::red);
            pen.setWidth(s.linienbreite() * 2);
        }
        else
        {
            pen.setColor(set_farbe(s.farbe()));
            pen.setWidth(s.linienbreite());
        }
        painter.setPen(pen);
        painter.drawLine(QPointF(s.stapu().x(), s.stapu().y()),
                         QPointF(s.endpu().x(), s.endpu().y()));
        painter.restore();
    }else if(element.text().contains(BOGEN))
    {
        painter.save();
        bogen b;
        b.set_text(element.text());

        QPen pen;
        pen.setCosmetic(true);
        if (i == Aktuelle_zeilennummer)
        {
            pen.setColor(Qt::red);
            pen.setWidth(b.linienbreite() * 2);
        }
        else
        {
            pen.setColor(set_farbe(b.farbe()));
            pen.setWidth(b.linienbreite());
        }
        painter.setPen(pen);

        // Das Rechteck für den Bogen (Zentrum - Radius bis Zentrum + Radius)
        QRectF rect(b.mipu().x() - b.rad(),
                    b.mipu().y() - b.rad(),
                    b.rad() * 2,
                    b.rad() * 2);

        // Spannwinkel berechnen
        double spanRad = b.ewi() - b.swi();
        double absSpanDeg = std::abs(spanRad * (180.0 / M_PI));

        // 1. Prüfung auf Vollkreis (nahe 360 Grad)
        if (absSpanDeg >= 359.99)
        {
            painter.drawEllipse(rect);
        }
        // 2. Prüfung auf "Nichts" (nahe 0 Grad)
        else if (absSpanDeg < 0.01)
        {
            // Zeichne nichts oder nur einen Punkt
        }
        // 3. Normaler Bogen
        else
        {
            double stawi16 = b.swi() * (180.0 / M_PI) * 16.0;
            double bogwi16 = spanRad * (180.0 / M_PI) * 16.0;

            // Winkel negieren wegen painter.scale(1, -1)
            painter.drawArc(rect, static_cast<int>(-stawi16), static_cast<int>(-bogwi16));
        }
        painter.restore();
    }else if(element.text().contains(KREIS))
    {
        painter.save();

        kreis k;
        k.set_text(element.text());
        double r = k.radius();

        // Pen (Umriss)
        QPen pen;
        pen.setCosmetic(true);
        pen.setStyle(set_linienstil(k.stil()));
        pen.setCapStyle(Qt::RoundCap);

        if(i == Aktuelle_zeilennummer)
        {
            pen.setColor(Qt::red);
            pen.setWidth(k.linienbreite() * 2);
        }
        else
        {
            pen.setColor(set_farbe(k.farbe()));
            pen.setWidth(k.linienbreite());
        }
        painter.setPen(pen);

        // Brush (Füllung)
        QBrush brush;
        brush.setStyle(Qt::SolidPattern);
        brush.setColor(set_farbe(k.farbe_fuellung()));
        painter.setBrush(brush);

        // Kreis zeichnen: Das Rechteck definiert die äußeren Grenzen (Zentrum +/- Radius)
        QRectF rect(k.mipu().x() - r,
                    k.mipu().y() - r,
                    r * 2.0,
                    r * 2.0);

        painter.drawEllipse(rect);

        painter.restore();
    }else if(element.text().contains(RECHTECK))
    {
        painter.save();

        rechteck r;
        r.set_text(element.text());

        // Pen und Brush konfigurieren
        QPen pen;
        pen.setCosmetic(true);
        pen.setStyle(set_linienstil(r.stil()));
        pen.setCapStyle(Qt::RoundCap);

        if(i == Aktuelle_zeilennummer)
        {
            pen.setColor(Qt::red);
            pen.setWidth(r.linienbreite() * 2);
        }
        else
        {
            pen.setColor(set_farbe(r.farbe()));
            pen.setWidth(r.linienbreite());
        }
        painter.setPen(pen);

        QBrush brush;
        brush.setStyle(Qt::SolidPattern);
        brush.setColor(set_farbe(r.farbe_fuellung()));
        painter.setBrush(brush);

        // Transformation für dieses Rechteck:
        // 1. Zum Mittelpunkt des Rechtecks springen
        painter.translate(r.mipu().x(), r.mipu().y());
        // 2. Um den Winkel drehen (CAD-Winkel sind gegen den Uhrzeigersinn)
        painter.rotate(radToDeg(r.drewi()));

        // Das Rechteck wird nun lokal (Mittelpunkt ist 0,0) gezeichnet
        QRectF rect(-r.l() / 2.0, -r.b() / 2.0, r.l(), r.b());

        if(r.rad() == 0)
        {
            painter.drawRect(rect);
        }
        else
        {
            // Abgerundete Ecken werden direkt von Qt unterstützt
            painter.drawRoundedRect(rect, r.rad(), r.rad());
        }

        painter.restore(); // Setzt Rotation und Verschiebung für das nächste Element zurück
    }
}

void vorschau::slot_aktualisieren(geo_text gt, geo_text fkon, int aktive_zeile)
{
    Geotext = gt;
    GeoFkon = fkon;
    Aktuelle_zeilennummer = aktive_zeile;
    slot_aktualisieren();
}
void vorschau::slot_aktualisieren()
{
    //Zoomfakrotr berechnen:
    int randabstand = 20;
    double maximallaenge = Geotext.max_x() - Geotext.min_x();
    double maximalbreite = Geotext.max_y() - Geotext.min_y();

    if(maximallaenge > 0.001 && maximalbreite > 0.001)
    {
        double bildlaenge = width()-randabstand*2;
        double bildbreite = height()-randabstand*2;

        double faktor_laenge = bildlaenge/maximallaenge;
        double faktor_breite = bildbreite/maximalbreite;

        if(faktor_laenge < faktor_breite)
        {
            set_sf(faktor_laenge);
        }else
        {
            set_sf(faktor_breite);
        }
    }
    else
    {
        set_sf(1.0); // Standardwert
    }

    slot_zf_gleich_eins();
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
        Zf = Zf - Zf/25;
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
    // 1. Panning (Verschieben)
    if(Mrg)
    {
        int dx = event->pos().x() - Maus_pos_alt_x;
        int dy = event->pos().y() - Maus_pos_alt_y;

        Npv.set_x(Npv.x() - dx);
        Npv.set_y(Npv.y() - dy);

        Maus_pos_alt_x = event->pos().x();
        Maus_pos_alt_y = event->pos().y();

        this->update();
    }

    // 2. Real-Koordinaten berechnen (Double nutzen!)
    // Nutze event->position() statt QCursor::pos(), das ist schneller und präziser
    QPointF p = event->position();

    double realX = (p.x() - N.x() + Npv.x()) / (Sf * Zf);
    double realY = (N.y() - Npv.y() - p.y()) / (Sf * Zf);

    // 3. Nur senden, wenn die Änderung signifikant ist (verhindert Flackern)
    // Wir vergleichen mit der letzten gesendeten Position (musst du als Klassenvariable anlegen)
    static double lastX = 0, lastY = 0;
    if (std::abs(realX - lastX) > 0.001 || std::abs(realY - lastY) > 0.001)
    {
        lastX = realX;
        lastY = realY;

        punkt3d reale_pos;
        reale_pos.set_x(realX);
        reale_pos.set_y(realY);
        emit sende_maus_pos(reale_pos);
    }
}
void vorschau::wheelEvent(QWheelEvent *event)
{
    QPointF mauspos = event->position();
    bool istHerauszoomen = (event->angleDelta().y() < 0);
    double idealZf = get_ideal_zf();

    // 1. Wenn wir schon am Limit sind und weiter raus wollen -> Fixieren
    if (istHerauszoomen && Zf <= (idealZf + 0.0001))
    {
        slot_zf_gleich_eins();
        return;
    }

    // 2. CAD-Position VOR dem Zoom
    double wstX_alt = (mauspos.x() - N.x() + Npv.x()) / (Sf * Zf);
    double wstY_alt = (N.y() - Npv.y() - mauspos.y()) / (Sf * Zf);

    // 3. Zoom ausführen
    if (istHerauszoomen)
    {
        zoom(false);
    }
    else
    {
        zoom(true);
    }

    // 4. Nach dem Zoom: Prüfen ob wir das Limit unterschritten haben
    if (istHerauszoomen && Zf < idealZf)
    {
        slot_zf_gleich_eins();
        return;
    }

    // 5. Fokus-Panning (nur wenn nicht zentriert wurde)
    double wstX_neu = (mauspos.x() - N.x() + Npv.x()) / (Sf * Zf);
    double wstY_neu = (N.y() - Npv.y() - mauspos.y()) / (Sf * Zf);

    Npv.set_x(Npv.x() - (wstX_neu - wstX_alt) * Sf * Zf);
    Npv.set_y(Npv.y() + (wstY_neu - wstY_alt) * Sf * Zf);

    this->update();
}
double vorschau::get_ideal_zf()
{
    double minX = Geotext.min_x();
    double minY = Geotext.min_y();
    double maxX = Geotext.max_x();
    double maxY = Geotext.max_y();

    if(minX >= maxX || minY >= maxY) return 1.0 / Sf;

    double breite = maxX - minX;
    double hoehe = maxY - minY;

    double scaleX = (this->width() * 0.9) / breite;
    double scaleY = (this->height() * 0.9) / hoehe;

    return std::min(scaleX, scaleY) / Sf;
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
        m.exec(event->globalPosition().toPoint());
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
    // Aktuelle Widget-Mitte
    N.set_x(this->width() / 2.0);
    N.set_y(this->height() / 2.0);

    double minX = Geotext.min_x();
    double minY = Geotext.min_y();
    double maxX = Geotext.max_x();
    double maxY = Geotext.max_y();

    if(minX > maxX) { minX = 0; maxX = 100; minY = 0; maxY = 100; }

    double breite = maxX - minX;
    double hoehe = maxY - minY;

    // Puffer hinzufügen (10% Rand)
    if(breite < 0.1) breite = 1.0;
    if(hoehe < 0.1) hoehe = 1.0;

    double scaleX = (this->width() * 0.9) / breite;
    double scaleY = (this->height() * 0.9) / hoehe;

    Zf = std::min(scaleX, scaleY) / Sf;

    // Mitte des Werkstücks
    double mitteX = (minX + maxX) / 2.0;
    double mitteY = (minY + maxY) / 2.0;

    // Npv setzen:
    // Der Punkt (mitteX, mitteY) soll auf (N.x, N.y) landen.
    // In update_cad: PainterX = N.x - Npv.x + mitteX * Sf * Zf
    // Damit PainterX = N.x, muss Npv.x = mitteX * Sf * Zf sein.
    Npv.set_x(mitteX * Sf * Zf);
    Npv.set_y(-mitteY * Sf * Zf);

    this->update();
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
    }else if(farbe == FARBE_DUNKELGELB)
    {
        qfarbe = Qt::darkYellow;
    }else if(farbe == FARBE_GRAU)
    {
        qfarbe = Qt::gray;
    }else if(farbe == FARBE_DUNKELGRAU)
    {
        qfarbe = Qt::darkGray;
    }else if(farbe == FARBE_BRAUN)
    {
        qfarbe.setRgb(185,122,87);
    }else if(farbe == FARBE_LILA)
    {
        qfarbe.setRgb(238,130,238);
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


















