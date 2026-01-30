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
