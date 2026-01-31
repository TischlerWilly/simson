#include "dxf.h"

void import_dxf(werkstueck *w,
                bool istOberseite,
                const QString &dateipfad,
                const einstellung_dxf &algemein,
                const einstellung_dxf_klassen &klassen)
{
    /*
    In der libdxfrw-Dokumentation (oder im GitHub Repository von libdxfrw) wird oft empfohlen,
    für den zweiten Durchlauf ein neues dxfRW-Objekt zu erstellen oder sicherzustellen,
    dass der Dateizeiger zurückgesetzt wird (was durch ein neues Objekt automatisch passiert).
    */
    //---------------------------------------------------------------
    //WST-Größe einlesen:
    {
        dxf_importklasse *import = new dxf_importklasse();

        import->set_einst_allgem(algemein);
        import->set_einst_klassen(klassen);
        import->set_wst(w);
        import->set_istOberseite(istOberseite);

        std::string path_std = dateipfad.toStdString();
        dxfRW reader(path_std.c_str());
        import->set_modusSucheWstgroesse(true);
        reader.read(import, false);

        delete import;
    }
    //---------------------------------------------------------------
    //Bearbeitung einlesen:
    {
        dxf_importklasse *import = new dxf_importklasse();

        import->set_einst_allgem(algemein);
        import->set_einst_klassen(klassen);
        import->set_wst(w);
        import->set_istOberseite(istOberseite);

        std::string path_std = dateipfad.toStdString();
        dxfRW reader(path_std.c_str());
        import->set_modusSucheWstgroesse(false);
        reader.read(import, false);

        delete import;
    }
    //---------------------------------------------------------------
}
