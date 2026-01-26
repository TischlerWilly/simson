#include "dxf.h"

werkstueck import_dxf(QString dateipfad)
{
    dxf_importklasse import;

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
