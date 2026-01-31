#include "dxf_importklasse.h"

dxf_importklasse::dxf_importklasse()
{
    IstOberseite = true;
    ModusSucheWstgroesse = false;
}
void dxf_importklasse::set_einst_allgem(einstellung_dxf e)
{
    Einst_allgem = e;
}
void dxf_importklasse::set_einst_klassen(einstellung_dxf_klassen e)
{
    Einst_klassen = e;
}

/** Called when header is parsed.  */
void dxf_importklasse::addHeader(const DRW_Header* data)  {}

/** Called for every line Type.  */
void dxf_importklasse::addLType(const DRW_LType& data)  {}
/** Called for every layer. */
void dxf_importklasse::addLayer(const DRW_Layer& data)  {}
/** Called for every dim style. */
void dxf_importklasse::addDimStyle(const DRW_Dimstyle& data)  {}
/** Called for every VPORT table. */
void dxf_importklasse::addVport(const DRW_Vport& data)  {}
/** Called for every text style. */
void dxf_importklasse::addTextStyle(const DRW_Textstyle& data)  {}
/** Called for every AppId entry. */
void dxf_importklasse::addAppId(const DRW_AppId& data)  {}

/**
     * Called for every block. Note: all entities added after this
     * command go into this block until endBlock() is called.
     *
     * @see endBlock()
     */
void dxf_importklasse::addBlock(const DRW_Block& data)  {}

/**
     * In DWG called when the following entities corresponding to a
     * block different from the current. Note: all entities added after this
     * command go into this block until setBlock() is called already.
     *
     * int handle are the value of DRW_Block::handleBlock added with addBlock()
     */
void dxf_importklasse::setBlock(const int handle)  {}

/** Called to end the current block */
void dxf_importklasse::endBlock()  {}

/** Called for every point */
void dxf_importklasse::addPoint(const DRW_Point& data)  {}

/** Called for every line */
void dxf_importklasse::addLine(const DRW_Line& data)
{
    QMessageBox mb;
    mb.setText("Linie gefunden");
    mb.exec();
}

/** Called for every ray */
void dxf_importklasse::addRay(const DRW_Ray& data)  {}

/** Called for every xline */
void dxf_importklasse::addXline(const DRW_Xline& data)  {}

/** Called for every arc */
void dxf_importklasse::addArc(const DRW_Arc& data)
{
    QMessageBox mb;
    mb.setText("Bogen gefunden");
    mb.exec();
}

/** Called for every circle */
void dxf_importklasse::addCircle(const DRW_Circle& data)
{
    if(ModusSucheWstgroesse == false)
    {
        QString klasse = QString::fromUtf8(data.layer.c_str());

        if(  klasse.contains(Einst_klassen.bohr_vert())  )
        {
            bohrung bo;
            bo.set_afb("1");
            QString ti;
            ti = text_rechts(klasse, Einst_klassen.bohr_vert());
            ti = text_rechts(ti, Einst_allgem.paramtren());
            ti.replace(Einst_allgem.dezitren(),".");
            double ti_double = ti.toDouble();
            if(ti_double == Wst->dicke())
            {
                //ti_double = ti_double + 4;
            }
            if(ti_double < 0)
            {
                ti_double = Wst->dicke() - ti_double;
            }
            bo.set_tiefe(ti_double);

            punkt3d mipu;
            mipu.set_x(data.basePoint.x);
            mipu.set_y(data.basePoint.y);
            if(IstOberseite)
            {
                bo.set_bezug(WST_BEZUG_OBSEI);
                bo.set_x(mipu.x());
                bo.set_y(mipu.y());
            }else
            {
                bo.set_bezug(WST_BEZUG_UNSEI);
                if(Einst_allgem.drehtyp_L())
                {
                    bo.set_x(Wst->laenge()-mipu.x());
                    bo.set_y(mipu.y());
                }else //if(Einstellung_dxf.drehtyp_B())
                {
                    bo.set_x(mipu.x());
                    bo.set_y(Wst->breite()-mipu.y());
                }
            }

            bo.set_dm(data.radious*2);
            Wst->bearb_ptr()->add_hi(bo.text());
        }else
        {
            QString msg;
            msg += "Kreis mit Layernamen \"";
            msg += klasse;
            msg += "\" wurde beim Import übersprungen.";
            QMessageBox mb;
            mb.setText(msg);
            mb.setWindowTitle("DXF importieren");
            mb.exec();
        }
    }
}

/** Called for every ellipse */
void dxf_importklasse::addEllipse(const DRW_Ellipse& data)  {}

/** Called for every lwpolyline */
void dxf_importklasse::addLWPolyline(const DRW_LWPolyline& data)
{
    if(ModusSucheWstgroesse == true)
    {
        QString klasse = QString::fromUtf8(data.layer.c_str());

        if(  klasse.contains(Einst_klassen.wst())  )
        {
            QString dicke;
            dicke = text_rechts(klasse, Einst_klassen.wst());
            dicke = text_rechts(dicke, Einst_allgem.paramtren());
            dicke.replace(Einst_allgem.dezitren(),".");
            Wst->set_dicke(dicke);

            if(data.vertlist.empty()) return;

            // Initialisierung mit dem ersten Punkt
            double minX = data.vertlist[0]->x;
            double maxX = data.vertlist[0]->x;
            double minY = data.vertlist[0]->y;
            double maxY = data.vertlist[0]->y;

            // Alle weiteren Eckpunkte vergleichen
            for (const auto& v : data.vertlist)
            {
                if (v->x < minX) minX = v->x;
                if (v->x > maxX) maxX = v->x;
                if (v->y < minY) minY = v->y;
                if (v->y > maxY) maxY = v->y;
            }

            // Berechnung der Dimensionen
            double laenge = maxX - minX;
            double breite = maxY - minY;

            // Werte im Werkstück setzen
            if(Wst)
            {
                Wst->set_laenge(laenge);
                Wst->set_breite(breite);
            }
        }
    }else //ModusSucheWstgroesse == false
    {
        if(data.vertlist.empty()) return;

        QString klasse = QString::fromUtf8(data.layer.c_str());

        if(  klasse.contains(Einst_klassen.rta())  )
        {
            size_t anz_punkte = data.vertlist.size();
            if(anz_punkte == 4)
            {
                //Prüfen ob die Polylinie nur aus Strecken besteht oder auch Bögen enthällt:
                bool nur_strecken = true;
                for (const auto& v : data.vertlist)
                {
                    if(v->bulge != 0)
                    {
                        nur_strecken = false;
                        break; //for
                    }
                }
                if(nur_strecken == true)//rta ist ohne eckenradius definiert
                {
                    bool ist_rechteck = true;

                    // Wir prüfen 3 Ecken (0, 1, 2). Bei einer geschlossenen Polyline (4 Punkte)
                    // reicht das aus, um ein Rechteck zu bestätigen.
                    for (int i = 0; i < 3; ++i)
                    {
                        // Vektoren der zwei angrenzenden Seiten berechnen
                        // Punkt A (i), Punkt B (i+1), Punkt C (i+2)
                        auto* vA = data.vertlist[i];
                        auto* vB = data.vertlist[(i + 1) % anz_punkte];
                        auto* vC = data.vertlist[(i + 2) % anz_punkte];

                        double dx1 = vA->x - vB->x;
                        double dy1 = vA->y - vB->y;
                        double dx2 = vC->x - vB->x;
                        double dy2 = vC->y - vB->y;

                        // Skalarprodukt: (x1*x2 + y1*y2)
                        double dotProduct = dx1 * dx2 + dy1 * dy2;

                        // Prüfung auf 90° (mit kleiner Toleranz für Fließkommafehler)
                        if (std::abs(dotProduct) > 1e-6)
                        {
                            ist_rechteck = false;
                            break;
                        }
                    }

                    if(ist_rechteck)
                    {
                        rechtecktasche rta;
                        punkt3d p0, p1, p2, p3;
                        p0.set_x(data.vertlist[0]->x);
                        p0.set_y(data.vertlist[0]->y);
                        p1.set_x(data.vertlist[1]->x);
                        p1.set_y(data.vertlist[1]->y);
                        p2.set_x(data.vertlist[2]->x);
                        p2.set_y(data.vertlist[2]->y);
                        p3.set_x(data.vertlist[3]->x);
                        p3.set_y(data.vertlist[3]->y);
                        strecke diagon;
                        diagon.set_stapu(p0);
                        diagon.set_endpu(p2);
                        rta.set_mipu( diagon.mipu());
                        strecke s_un, s_re;
                        s_un.set_stapu(p0);
                        s_un.set_endpu(p1);
                        s_re.set_stapu(p1);
                        s_re.set_endpu(p2);
                        rta.set_laenge(s_un.laenge2d());
                        rta.set_breite(s_re.laenge2d());

                        rta.set_drewi(s_un.wink());

                        //Tiefe und wkz einlesen:
                        QString ti;
                        ti = text_rechts(klasse, Einst_klassen.rta());
                        ti = text_rechts(ti, Einst_allgem.paramtren());
                        if(ti.contains(Einst_allgem.kenWKZnr()))
                        {
                            QString wkznr = text_rechts(ti, Einst_allgem.kenWKZnr());
                            rta.set_wkznum(wkznr);
                            ti = text_links(ti, Einst_allgem.kenWKZnr());
                        }
                        ti.replace(Einst_allgem.dezitren(),".");
                        double ti_double = ti.toDouble();
                        if(ti_double == Wst->dicke())
                        {
                            ti_double = ti_double + 2;
                        }
                        if(ti_double < 0)
                        {
                            ti_double = Wst->dicke() - ti_double;
                        }
                        rta.set_tiefe(ti_double);

                        if(IstOberseite == true)
                        {
                            Wst->bearb_ptr()->add_hi(rta.text());
                        }else
                        {
                            rta.set_bezug(WST_BEZUG_UNSEI);
                            rta.set_drewi(-rta.drewi());

                            punkt3d mipu;
                            mipu = rta.mipu();
                            if(Einst_allgem.drehtyp_L())
                            {
                                rta.set_x(Wst->laenge()-mipu.x());
                                //rta.set_y(mipu.y());
                            }else //if(Einstellung_dxf.drehtyp_B())
                            {
                                //rta.set_x(mipu.x());
                                rta.set_y(Wst->breite()-mipu.y());
                            }

                            Wst->bearb_ptr()->add_hi(rta.text());
                        }
                    }
                }
            }else if(anz_punkte == 8)
            {
                double rad = -1.0;
                bool alle_radien_gleich = true;

                for (int i = 0; i < 8; i++)
                {
                    double bul = data.vertlist[i]->bulge;

                    // Jedes zweite Segment (die Ecken) muss ein Bogen sein
                    if (i % 2 != 0)
                    { // Index 1, 3, 5, 7 sind oft die Bögen
                        if (bul == 0)
                        {
                            alle_radien_gleich = false;
                            break;
                        }

                        // Radius berechnen: R = (Sehne / 2) * (1 + b²) / (2 * |b|)
                        auto* vStart = data.vertlist[i];
                        auto* vEnd = data.vertlist[(i + 1) % 8];
                        double dist = std::sqrt(std::pow(vEnd->x - vStart->x, 2) + std::pow(vEnd->y - vStart->y, 2));
                        double currentR = (dist / 2.0) * (1.0 + bul * bul) / (2.0 * std::abs(bul));

                        if (rad < 0)
                        {
                            rad = currentR;
                        }else if (std::abs(rad - currentR) > 1e-4)
                        {
                            alle_radien_gleich = false;
                            break;
                        }
                    }
                }

                if (alle_radien_gleich)
                {
                    // Hilfsfunktion für den Abstand zwischen zwei Punkten
                    auto getDist = [](DRW_Vertex2D* a, DRW_Vertex2D* b)
                    {
                        return std::sqrt(std::pow(b->x - a->x, 2) + std::pow(b->y - a->y, 2));
                    };

                    // Bei 8 Punkten und abwechselnd Gerade/Bogen sind die geraden Segmente:
                    // 0->1, 2->3, 4->5, 6->7 (oder leicht verschoben je nach DXF-Export)
                    // Wir prüfen hier Segment 0->1 (Seite A) und 2->3 (Seite B)
                    double seiteA = getDist(data.vertlist[0], data.vertlist[1]);
                    double seiteB = getDist(data.vertlist[2], data.vertlist[3]);

                    // Die Gesamtaußenmaße inkl. Rundung
                    double dim1 = seiteA + 2.0 * rad;
                    double dim2 = seiteB + 2.0 * rad;

                    double laenge = std::max(dim1, dim2);
                    double breite = std::min(dim1, dim2);

                    // Drehwinkel berechnen (Winkel des ersten geraden Segments zur X-Achse)
                    double dx = data.vertlist[1]->x - data.vertlist[0]->x;
                    double dy = data.vertlist[1]->y - data.vertlist[0]->y;
                    double winkel_rad = std::atan2(dy, dx);
                    double winkel_deg = winkel_rad * 180.0 / M_PI;

                    // Falls die erste gefundene Seite die kürzere war, Winkel korrigieren (+90°)
                    if (dim1 < dim2)
                    {
                        winkel_deg += 90.0;
                    }

                    // Mittelpunkt berechnen (Durchschnitt aller 8 Vertices)
                    double sumX = 0;
                    double sumY = 0;
                    for (const auto& v : data.vertlist)
                    {
                        sumX += v->x;
                        sumY += v->y;
                    }
                    double centerX = sumX / 8.0;
                    double centerY = sumY / 8.0;

                    // Da die Bounding Box bei abgerundeten Ecken die Außenkanten misst:
                    // breite und laenge sind bereits die korrekten Gesamtaußenmaße.
                    // Werte zuweisen
                    rechtecktasche rta;
                    rta.set_rad(rad);
                    punkt3d mipu(centerX, centerY, 0);
                    rta.set_mipu(mipu);
                    rta.set_laenge(laenge);
                    rta.set_breite(breite);
                    rta.set_drewi(degToRad(winkel_deg));

                    //Tiefe und wkz einlesen:
                    QString ti;
                    ti = text_rechts(klasse, Einst_klassen.rta());
                    ti = text_rechts(ti, Einst_allgem.paramtren());
                    if(ti.contains(Einst_allgem.kenWKZnr()))
                    {
                        QString wkznr = text_rechts(ti, Einst_allgem.kenWKZnr());
                        rta.set_wkznum(wkznr);
                        ti = text_links(ti, Einst_allgem.kenWKZnr());
                    }
                    ti.replace(Einst_allgem.dezitren(),".");
                    double ti_double = ti.toDouble();
                    if(ti_double == Wst->dicke())
                    {
                        ti_double = ti_double + 2;
                    }
                    if(ti_double < 0)
                    {
                        ti_double = Wst->dicke() - ti_double;
                    }
                    rta.set_tiefe(ti_double);

                    if(IstOberseite == true)
                    {
                        Wst->bearb_ptr()->add_hi(rta.text());
                    }else
                    {
                        rta.set_bezug(WST_BEZUG_UNSEI);
                        rta.set_drewi(-rta.drewi());

                        punkt3d mipu;
                        mipu = rta.mipu();
                        if(Einst_allgem.drehtyp_L())
                        {
                            rta.set_x(Wst->laenge()-mipu.x());
                            //rta.set_y(mipu.y());
                        }else //if(Einstellung_dxf.drehtyp_B())
                        {
                            //rta.set_x(mipu.x());
                            rta.set_y(Wst->breite()-mipu.y());
                        }

                        Wst->bearb_ptr()->add_hi(rta.text());
                    }
                }
            }
        }else if(  klasse.contains(Einst_klassen.fraes_vert())  )
        {
            size_t anz_punkte = data.vertlist.size();
            if(anz_punkte > 1)
            {
                QString posz;
                posz = text_mitte(klasse, Einst_klassen.fraes_vert(), Einst_allgem.kenWKZnr());
                posz = text_rechts(posz, Einst_allgem.paramtren());
                posz.replace(Einst_allgem.dezitren(),".");
                double fraeserhoehe;
                double fti;
                fraeserhoehe = posz.toDouble();
                if(fraeserhoehe > 0)
                {
                    if(Einst_allgem.bezugTiFkonObSei())
                    {
                        fti = fraeserhoehe;
                        fraeserhoehe = Wst->dicke()-fraeserhoehe;
                    }else
                    {
                        fti = Wst->dicke()-fraeserhoehe;
                    }
                }else
                {
                    fti = fraeserhoehe * -1;
                    fti = fti + Wst->dicke();
                }
                QString werkznr;
                werkznr = text_rechts(klasse, Einst_klassen.fraes_vert());
                werkznr = text_rechts(werkznr, Einst_allgem.kenWKZnr());
                QString radkor = werkznr.at(werkznr.length()-1);
                werkznr = text_links(werkznr,radkor);
                if(radkor == Einst_allgem.kenRadKorLi())
                {
                    radkor = FRKOR_L;
                }else if(radkor == Einst_allgem.kenRadKorRe())
                {
                    radkor = FRKOR_R;
                }else
                {
                    radkor = FRKOR_M;
                }

                punkt3d stapu;
                stapu.set_x(data.vertlist[0]->x);
                stapu.set_y(data.vertlist[0]->y);
                stapu.set_z(posz);

                fraeseraufruf fa;
                fa.set_tiefe(fti);
                fa.set_radkor(radkor);
                fa.set_wkznum(werkznr);

                if(IstOberseite)
                {
                    fa.set_bezug(WST_BEZUG_OBSEI);
                    fa.set_pos(stapu);
                    Wst->bearb_ptr()->add_hi(fa.text());
                }else
                {
                    fa.set_bezug(WST_BEZUG_UNSEI);

                    if(Einst_allgem.drehtyp_L())
                    {
                        stapu.set_x(Wst->laenge()-stapu.x());
                    }else //if(Einstellung_dxf.drehtyp_B())
                    {
                        stapu.set_y(Wst->breite()-stapu.y());
                    }

                    fa.set_pos(stapu);
                    Wst->bearb_ptr()->add_hi(fa.text());
                }

                for (int i = 0; i < anz_punkte; i++)
                {
                    // Bestimmung des Endpunkt-Index (berücksichtigt geschlossene Polylinien)
                    int nextIdx = (i + 1) % anz_punkte;

                    // Wenn die Polyline nicht geschlossen ist, stoppen wir beim vorletzten Punkt
                    if (nextIdx == 0 && !(data.flags & 1))
                    {
                        break;
                    }

                    DRW_Vertex2D* vStart = data.vertlist[i];
                    DRW_Vertex2D* vEnd = data.vertlist[nextIdx];
                    double bul = vStart->bulge;

                    punkt3d spu(vStart->x, vStart->y, fa.pos().z());
                    punkt3d epu(vEnd->x, vEnd->y, fa.pos().z());

                    if(bul == 0)//Strecke
                    {
                        fraesergerade fg;
                        fg.set_tiSta(fa.tiefe());
                        fg.set_tiEnd(fa.tiefe());

                        if(IstOberseite)
                        {
                            fg.set_bezug(WST_BEZUG_OBSEI);
                            fg.set_startpunkt(spu);
                            fg.set_endpunkt(epu);
                            Wst->bearb_ptr()->add_hi(fg.text());
                        }else
                        {
                            fg.set_bezug(WST_BEZUG_UNSEI);

                            if(Einst_allgem.drehtyp_L())
                            {
                                spu.set_x(Wst->laenge()-spu.x());
                                epu.set_x(Wst->laenge()-epu.x());
                            }else //if(Einstellung_dxf.drehtyp_B())
                            {
                                spu.set_y(Wst->breite()-spu.y());
                                epu.set_y(Wst->breite()-epu.y());
                            }

                            fg.set_startpunkt(spu);
                            fg.set_endpunkt(epu);
                            Wst->bearb_ptr()->add_hi(fg.text());
                        }

                    }else //Bogen
                    {
                        fraeserbogen fb;
                        fb.set_tiSta(fa.tiefe());
                        fb.set_tiEnd(fa.tiefe());

                        // Mathematische Parameter für den Bogen berechnen
                        double dx = vEnd->x - vStart->x;
                        double dy = vEnd->y - vStart->y;
                        double d = std::sqrt(dx * dx + dy * dy); // Sehnenlänge

                        // Radius: R = (d/2) * (1 + b²) / (2*|b|)
                        double rad = (d / 2.0) * (1.0 + bul * bul) / (2.0 * std::abs(bul));

                        bool uzs = (bul < 0); // Negativer Bulge = Uhrzeigersinn

                        if(IstOberseite)
                        {
                            fb.set_bezug(WST_BEZUG_OBSEI);
                            bogen b;
                            b.set_bogen(spu, epu, rad, uzs);
                            fb.set_bogen(b);
                            Wst->bearb_ptr()->add_hi(fb.text());
                        }else
                        {
                            fb.set_bezug(WST_BEZUG_UNSEI);

                            if(Einst_allgem.drehtyp_L())
                            {
                                spu.set_x(Wst->laenge()-spu.x());
                                epu.set_x(Wst->laenge()-epu.x());
                            }else //if(Einstellung_dxf.drehtyp_B())
                            {
                                spu.set_y(Wst->breite()-spu.y());
                                epu.set_y(Wst->breite()-epu.y());
                            }

                            uzs = !uzs;

                            bogen b;
                            b.set_bogen(spu, epu, rad, uzs);
                            fb.set_bogen(b);
                            Wst->bearb_ptr()->add_hi(fb.text());
                        }

                    }
                }
            }
        }
    }
}

/** Called for every polyline start */
void dxf_importklasse::addPolyline(const DRW_Polyline& data)
{
    QMessageBox mb;
    mb.setText("Polylinie gefunden");
    mb.exec();
}

/** Called for every spline */
void dxf_importklasse::addSpline(const DRW_Spline* data)  {}

/** Called for every spline knot value */
void dxf_importklasse::addKnot(const DRW_Entity& data)  {}

/** Called for every insert. */
void dxf_importklasse::addInsert(const DRW_Insert& data)  {}

/** Called for every trace start */
void dxf_importklasse::addTrace(const DRW_Trace& data)  {}

/** Called for every 3dface start */
void dxf_importklasse::add3dFace(const DRW_3Dface& data)  {}

/** Called for every solid start */
void dxf_importklasse::addSolid(const DRW_Solid& data)  {}


/** Called for every Multi Text entity. */
void dxf_importklasse::addMText(const DRW_MText& data)  {}

/** Called for every Text entity. */
void dxf_importklasse::addText(const DRW_Text& data)  {}

/**
     * Called for every aligned dimension entity.
     */
void dxf_importklasse::addDimAlign(const DRW_DimAligned *data)  {}
/**
     * Called for every linear or rotated dimension entity.
     */
void dxf_importklasse::addDimLinear(const DRW_DimLinear *data)  {}

/**
     * Called for every radial dimension entity.
     */
void dxf_importklasse::addDimRadial(const DRW_DimRadial *data)  {}

/**
     * Called for every diametric dimension entity.
     */
void dxf_importklasse::addDimDiametric(const DRW_DimDiametric *data)  {}

/**
     * Called for every angular dimension (2 lines version) entity.
     */
void dxf_importklasse::addDimAngular(const DRW_DimAngular *data)  {}

/**
     * Called for every angular dimension (3 points version) entity.
     */
void dxf_importklasse::addDimAngular3P(const DRW_DimAngular3p *data)  {}

/**
     * Called for every ordinate dimension entity.
     */
void dxf_importklasse::addDimOrdinate(const DRW_DimOrdinate *data)  {}

/**
     * Called for every leader start.
     */
void dxf_importklasse::addLeader(const DRW_Leader *data)  {}

/**
     * Called for every hatch entity.
     */
void dxf_importklasse::addHatch(const DRW_Hatch *data)  {}

/**
     * Called for every viewport entity.
     */
void dxf_importklasse::addViewport(const DRW_Viewport& data)  {}

/**
     * Called for every image entity.
     */
void dxf_importklasse::addImage(const DRW_Image *data)  {}

/**
     * Called for every image definition.
     */
void dxf_importklasse::linkImage(const DRW_ImageDef *data)  {}

/**
     * Called for every comment in the DXF file (code 999).
     */
void dxf_importklasse::addComment(const char* comment)  {}

void dxf_importklasse::writeHeader(DRW_Header& data)  {}
void dxf_importklasse::writeBlocks()  {}
void dxf_importklasse::writeBlockRecords()  {}
void dxf_importklasse::writeEntities()  {}
void dxf_importklasse::writeLTypes()  {}
void dxf_importklasse::writeLayers()  {}
void dxf_importklasse::writeTextstyles()  {}
void dxf_importklasse::writeVports()  {}
void dxf_importklasse::writeDimstyles()  {}
void dxf_importklasse::writeAppId()  {}

//-------------------------------------------------
void dxf_importklasse::set_wst(werkstueck *w)
{
    Wst = w;
}
void dxf_importklasse::set_istOberseite(bool istOberseite)
{
    IstOberseite = istOberseite;
}
void dxf_importklasse::set_modusSucheWstgroesse (bool nur_groesse_ermitteln)
{
    ModusSucheWstgroesse = nur_groesse_ermitteln;
}
