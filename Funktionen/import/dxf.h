#ifndef DXF_H
#define DXF_H

#include "Klassen/DXF/dxf_importklasse.h"

void import_dxf(werkstueck *w, bool istOberseite, const QString &dateipfad,
                const einstellung_dxf &algemein, const einstellung_dxf_klassen &klassen);


#endif // DXF_H
