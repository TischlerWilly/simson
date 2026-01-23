#ifndef VORSCHAU_H
#define VORSCHAU_H

#include <QWidget>
#include <QPainter>
#include <QPainterPath>
#include <QMessageBox>
#include <QStatusBar>
#include <QMouseEvent>
#include <QWheelEvent>
#include <QMenu>
#include <math.h>
#include "geo_text.h"



class vorschau : public QWidget
{
    Q_OBJECT
public:
    explicit vorschau(QWidget *parent = 0);

protected:
    void paintEvent(QPaintEvent *);

private:
    void zeichneGeotext(QString geometrieElement, int i);
    void update_cad();
    float sf();
    void set_sf(float neuer_faktor);
    void mouseMoveEvent(QMouseEvent *event);
    void wheelEvent(QWheelEvent *event);
    void mousePressEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
    void zoom(bool dichter);
    punkt3d mauspos_vom_np();
    uint zeile_von_Mauspos();

    QColor set_farbe(QString farbe);
    Qt::PenStyle set_linienstil(QString stil);


    punkt3d     N;    //Nullpunkt
    punkt3d     Npv;  //Nullpunkt-Verschiebung(Verschiebung des sichtbaren Ausschnittes)
    float       Sf;   //Skalierungsfaktor
    float       Zf;   //Zoomfaktor
    int         Aktuelle_zeilennummer;
    geo_text    Geotext;
    geo_text    GeoFkon;
    int         Maus_pos_alt_x;
    int         Maus_pos_alt_y;
    uint        Zeile_von_maus_pos;//Zum Zwischenspeichern damit Ergebnis nicht verfälscht wird wenn Maus sich bewegt
    bool        Mrg;  //Mausrad gedrückt
    
signals:
    void sende_maus_pos(QPoint p);
    void sende_zeilennummer(uint nr, bool bearbeiten);

public slots:
    void slot_aktualisieren(geo_text gt, geo_text fkon, int aktive_zeile);
    void slot_aktualisieren();
    void slot_aktives_Element_einfaerben(int zeilennummer);

private slots:
    void slot_zf_gleich_eins();
    void slot_tunix();
    void slot_sende_zeilennummer();

};

#endif // VORSCHAU_H
