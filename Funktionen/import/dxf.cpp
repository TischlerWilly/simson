#include "dxf.h"

werkstueck import_dxf(QString dateipfad, einstellung_dxf algemein, einstellung_dxf_klassen klassen)
{
    dxf_importklasse import;
    import.set_einst_allgem(algemein);
    import.set_einst_klassen(klassen);

    dxfRW reader(dateipfad.toStdString().c_str());
    if (reader.read(&import, false))
    {
        return import.wst();
    }else
    {
        werkstueck w;
        return w;
    }
}
