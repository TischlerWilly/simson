#include "emc2.h"

emc2::emc2(maschine *m, werkstueck *w)
{
    //Default-Wert:
    Sicherheitsabstand = 20;

    set_maschine(m);
    set_wst(w);
    setup();
}
void emc2::set_maschine(maschine *m)
{
    Maschine = m;
    setup();
}
void emc2::set_wst(werkstueck *w)
{
    Wst = w;
    setup();
}
void emc2::setup()
{
    if(Maschine != nullptr && Wst != nullptr)
    {
        Sicherheitsabstand = 20;//Später noch als Parameter in maschine und wst ergänzen
        //Wenn bei wst AUTO dann nimm den Wert von der Maschine
    }
}

QString emc2::gcode()
{
    QString code;

    if(Maschine != nullptr && Wst != nullptr)
    {
        QTextStream stream(&code);

        stream << prgkopf();

        for(uint i=0; i<Wst->bearb().count() ;i++)
        {

        }

        stream << prgende();
    }

    return code;
}
QString emc2::prgkopf()
{
    QString prgkopf;

    if(Maschine != nullptr && Wst != nullptr)
    {
        QTextStream stream(&prgkopf);

        stream << "( --- Ausgabeformat: emc2 --- )\n";
        if(Maschine->manWkzWechsel() == true)
        {
            stream << "(   -> Manueller Werkzeugwechsel )\n";
        }
        if(Maschine->drehzExportieren() == false)
        {
            stream << "(   -> Export ohne Spindeldrehzahlen )\n";
        }

        stream << "\n( --- Programmkopf --- )\n";

        // G21: Millimeter
        // G90: Absolut-Koordinaten
        // G64 P0.1: Flüssige Bewegungen (Path Blending)
        // G17: XY-Ebene
        stream << "G21 G90 G64 P0.1 G17\n";

        // Sicherheitsabstand beim Start anfahren
        // Wir gehen davon aus, dass Z=0 die Werkstück-Unterseite (Tisch) ist
        stream << "G0 Z" << (Wst->dicke() + Sicherheitsabstand) << " (SICHERHEITSHOEHE)\n";

        // Hinweis für den Bediener
        stream << "( BITTE SPINDEL MANUELL EINSCHALTEN )\n";
        stream << "M0 (PROGRAMM PAUSE - START DRUECKEN WENN SPINDEL LAEUFT)\n\n";
    }

    return prgkopf;
}

QString emc2::prgende()
{
    QString prgende;

    if(Maschine != nullptr && Wst != nullptr)
    {
        QTextStream stream(&prgende);

        stream << "\n( --- Programmende --- )\n";

        // 1. Fräser sicher aus dem Material fahren
        stream << "G0 Z" << (Wst->dicke() + Sicherheitsabstand) << "\n";

        // 2. Parkposition anfahren
        double x,y,z;
        if(Wst->prgend_x() == "AUTO")
        {
            x = Maschine->prgenpos_x();
        }else
        {
            x = ausdruck_auswerten(Wst->prgend_x()).toDouble();
        }
        if(Wst->prgend_y() == "AUTO")
        {
            y = Maschine->prgenpos_y();
        }else
        {
            y = ausdruck_auswerten(Wst->prgend_y()).toDouble();
        }
        if(Wst->prgend_z() == "AUTO")
        {
            z = Maschine->prgenpos_z();
        }else
        {
            z = ausdruck_auswerten(Wst->prgend_z()).toDouble();
        }
        stream << "G0 X" << x << " Y" << y << " Z" << z << "\n";

        // 3. Programm-Ende
        // M2 oder M30 setzt die Steuerung zurück
        stream << "M30\n";
    }

    return prgende;
}

QString emc2::bohr(bohrung bo)
{
    // Parameter definieren
    double wst_dicke = Wst->dicke();
    double bohrtiefe = bo.tiefe();
    double zustellmass = 5.0;
    double austritt = 0.0;
    double toleranz = 0.01;

    // Prüfen ob Durchgangsbohrung
    if (bohrtiefe >= (wst_dicke - toleranz))
    {
        // Falls die Bohrung durchgehen soll, 2mm Sicherheitszugabe
        austritt = 2.0;
    }

    // Z-Werte berechnen
    double sicherheits_z = wst_dicke + 5.0;
    double ziel_z = (wst_dicke - bohrtiefe) - austritt;
    double rueckzug_r = wst_dicke + 1.0;

    QString gcode;
    QTextStream stream(&gcode);

    // G-Code generieren
    if (bohrtiefe > 0)
    {
        // 1. Positionierung
        stream << "G0 X" << bo.x() << " Y" << bo.y() << "\n";
        stream << "G0 Z" << sicherheits_z << "\n";

        // 2. Zyklus wählen
        // Wenn die Tiefe sehr gering ist, reicht G81 (ohne Spanbrechen)
        if (bohrtiefe <= zustellmass)
        {
            stream << "G81 Z" << ziel_z
                   << " R" << rueckzug_r
                   << " F200" << "\n";
        }
        else
        {
            // G83 für tiefere Bohrungen mit Entspanen
            stream << "G83 Z" << ziel_z
                   << " R" << rueckzug_r
                   << " Q" << zustellmass
                   << " F200" << "\n";
        }

        // 3. Abschluss
        stream << "G80" << "\n";
        stream << "G0 Z" << sicherheits_z << "\n";
    }
}
