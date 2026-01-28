#ifndef DXF_IMPORTKLASSE_H
#define DXF_IMPORTKLASSE_H

#ifdef _WIN32
#include <libdxfrw.h>
#include <drw_interface.h>
#endif //_WIN32

#ifdef __linux__
#include <libdxfrw/libdxfrw.h>
#include <libdxfrw/drw_interface.h>
#endif //__linux__

#include "Klassen/wst/werkstueck.h"
#include "Klassen/einstellung_dxf.h"
#include "Klassen/einstellung_dxf_klassen.h"


class dxf_importklasse : public DRW_Interface
{
public:
    dxf_importklasse();
    void set_einst_allgem(einstellung_dxf e);
    void set_einst_klassen(einstellung_dxf_klassen e);

    /** Called when header is parsed.  */
    void addHeader(const DRW_Header* data);

    /** Called for every line Type.  */
    void addLType(const DRW_LType& data);
    /** Called for every layer. */
    void addLayer(const DRW_Layer& data);
    /** Called for every dim style. */
    void addDimStyle(const DRW_Dimstyle& data);
    /** Called for every VPORT table. */
    void addVport(const DRW_Vport& data);
    /** Called for every text style. */
    void addTextStyle(const DRW_Textstyle& data);
    /** Called for every AppId entry. */
    void addAppId(const DRW_AppId& data);

    /**
     * Called for every block. Note: all entities added after this
     * command go into this block until endBlock() is called.
     *
     * @see endBlock()
     */
    void addBlock(const DRW_Block& data);

    /**
     * In DWG called when the following entities corresponding to a
     * block different from the current. Note: all entities added after this
     * command go into this block until setBlock() is called already.
     *
     * int handle are the value of DRW_Block::handleBlock added with addBlock()
     */
    void setBlock(const int handle);

    /** Called to end the current block */
    void endBlock();

    /** Called for every point */
    void addPoint(const DRW_Point& data);

    /** Called for every line */
    void addLine(const DRW_Line& data);

    /** Called for every ray */
    void addRay(const DRW_Ray& data);

    /** Called for every xline */
    void addXline(const DRW_Xline& data);

    /** Called for every arc */
    void addArc(const DRW_Arc& data);

    /** Called for every circle */
    void addCircle(const DRW_Circle& data);

    /** Called for every ellipse */
    void addEllipse(const DRW_Ellipse& data);

    /** Called for every lwpolyline */
    void addLWPolyline(const DRW_LWPolyline& data);

    /** Called for every polyline start */
    void addPolyline(const DRW_Polyline& data);

    /** Called for every spline */
    void addSpline(const DRW_Spline* data);

    /** Called for every spline knot value */
    void addKnot(const DRW_Entity& data);

    /** Called for every insert. */
    void addInsert(const DRW_Insert& data);

    /** Called for every trace start */
    void addTrace(const DRW_Trace& data);

    /** Called for every 3dface start */
    void add3dFace(const DRW_3Dface& data);

    /** Called for every solid start */
    void addSolid(const DRW_Solid& data);


    /** Called for every Multi Text entity. */
    void addMText(const DRW_MText& data);

    /** Called for every Text entity. */
    void addText(const DRW_Text& data);

    /**
     * Called for every aligned dimension entity.
     */
    void addDimAlign(const DRW_DimAligned *data);
    /**
     * Called for every linear or rotated dimension entity.
     */
    void addDimLinear(const DRW_DimLinear *data);

    /**
     * Called for every radial dimension entity.
     */
    void addDimRadial(const DRW_DimRadial *data);

    /**
     * Called for every diametric dimension entity.
     */
    void addDimDiametric(const DRW_DimDiametric *data);

    /**
     * Called for every angular dimension (2 lines version) entity.
     */
    void addDimAngular(const DRW_DimAngular *data);

    /**
     * Called for every angular dimension (3 points version) entity.
     */
    void addDimAngular3P(const DRW_DimAngular3p *data);

    /**
     * Called for every ordinate dimension entity.
     */
    void addDimOrdinate(const DRW_DimOrdinate *data);

    /**
     * Called for every leader start.
     */
    void addLeader(const DRW_Leader *data);

    /**
     * Called for every hatch entity.
     */
    void addHatch(const DRW_Hatch *data);

    /**
     * Called for every viewport entity.
     */
    void addViewport(const DRW_Viewport& data);

    /**
     * Called for every image entity.
     */
    void addImage(const DRW_Image *data);

    /**
     * Called for every image definition.
     */
    void linkImage(const DRW_ImageDef *data);

    /**
     * Called for every comment in the DXF file (code 999).
     */
    void addComment(const char* comment);

    void writeHeader(DRW_Header& data);
    void writeBlocks();
    void writeBlockRecords();
    void writeEntities();
    void writeLTypes();
    void writeLayers();
    void writeTextstyles();
    void writeVports();
    void writeDimstyles();
    void writeAppId();

    //-------------------------------------------------
    werkstueck wst();

private:
    werkstueck Wst;
    einstellung_dxf Einst_allgem;
    einstellung_dxf_klassen Einst_klassen;
};

#endif // DXF_IMPORTKLASSE_H
