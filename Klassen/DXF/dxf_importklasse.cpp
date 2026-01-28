#include "dxf_importklasse.h"

dxf_importklasse::dxf_importklasse()
{
    //Default-Größe:
    Wst.set_laenge(500);
    Wst.set_breite(300);
    Wst.set_dicke(19);
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
void dxf_importklasse::addLine(const DRW_Line& data)  {}

/** Called for every ray */
void dxf_importklasse::addRay(const DRW_Ray& data)  {}

/** Called for every xline */
void dxf_importklasse::addXline(const DRW_Xline& data)  {}

/** Called for every arc */
void dxf_importklasse::addArc(const DRW_Arc& data)  {}

/** Called for every circle */
void dxf_importklasse::addCircle(const DRW_Circle& data)
{
    //erst einmal pauschal Bohrung zum testen:
    bohrung b;
    punkt3d mipu;
    mipu.set_x(data.basePoint.x);
    mipu.set_y(data.basePoint.y);
    mipu.set_z(data.basePoint.z);//fraglich bei 2D-Bohrbildern, muss anhand des Layernamen ober oder Unterseite sein
    b.set_mipu(mipu);
    b.set_dm(data.radious*2);
    Wst.bearb_ptr()->add_hi(b.text());
}

/** Called for every ellipse */
void dxf_importklasse::addEllipse(const DRW_Ellipse& data)  {}

/** Called for every lwpolyline */
void dxf_importklasse::addLWPolyline(const DRW_LWPolyline& data)  {}

/** Called for every polyline start */
void dxf_importklasse::addPolyline(const DRW_Polyline& data)  {}

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
werkstueck dxf_importklasse::wst()
{
    return Wst;
}
