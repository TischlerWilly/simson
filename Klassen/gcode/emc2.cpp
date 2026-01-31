#include "emc2.h"

emc2::emc2()
{

}
QString emc2::header()
{
    QString h;
    QTextStream stream(&h);

    stream << "( GEGENERIERTER G-CODE - MANUELLER WECHSEL )\n";

    // G21: Millimeter
    // G90: Absolut-Koordinaten
    // G64 P0.1: Flüssige Bewegungen (Path Blending)
    // G17: XY-Ebene
    stream << "G21 G90 G64 P0.1 G17\n";

    // Sicherheitsabstand beim Start anfahren
    // Wir gehen davon aus, dass Z=0 die Werkstück-Unterseite (Tisch) ist
    stream << "G0 Z" << (Wst->dicke() + 20.0) << " (SICHERHEITSHOEHE)\n";

    // Hinweis für den Bediener
    stream << "( BITTE SPINDEL MANUELL EINSCHALTEN )\n";
    stream << "M0 (PROGRAMM PAUSE - START DRUECKEN WENN SPINDEL LAEUFT)\n";

    return h;
}

QString emc2::footer()
{
    QString f;
    QTextStream stream(&f);

    // 1. Fräser sicher aus dem Material fahren
    stream << "G0 Z" << (Wst->dicke() + 20.0) << "\n";

    // 2. Parkposition anfahren (z.B. X0 Y-Maximum, damit das Bett frei ist)
    // Hier kannst du Werte wählen, die für deine Maschine passen
    stream << "G0 X0 Y0\n";

    // 3. Programm-Ende
    // M2 oder M30 setzt die Steuerung zurück
    stream << "M30\n";

    return f;
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
