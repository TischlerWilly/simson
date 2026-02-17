#include "emc2.h"

emc2::emc2(maschine *m, werkstueck *w)
{
    //Default-Wert:
    Sicherheitsabstand = 20;
    Masszugabe_duboti = 0.3;
    Masszugabe_dutati = 0.3;

    set_maschine(m);
    set_wst(w);
    setup();
}
void emc2::set_maschine(maschine *m)
{
    Maschine = m;
}
void emc2::set_wst(werkstueck *w)
{
    Wst = w;
}
void emc2::setup()
{
    if(Maschine != nullptr && Wst != nullptr)
    {
        Sicherheitsabstand = Wst->sichabst();
        Masszugabe_duboti = 0.3;//Später noch als Parameter in maschine
        Masszugabe_dutati = 0.3;//Später noch als Parameter in maschine
    }
}

QString emc2::gcode()
{
    QString code;
    Akt_wkz.clear();
    Verwendete_wkz.clear();

    if(Maschine != nullptr && Wst != nullptr)
    {
        QTextStream stream(&code);

        stream << prgkopf();

        text_zw zeile;
        for(uint i=0; i<Wst->bearb().count() ;i++)
        {
            zeile.set_text(Wst->bearb().at(i),TRENNZ_BEARB_PARAM);
            if(zeile.at(0) == BEARBART_BOHR)
            {
                bohrung bo(zeile.text());
                stream << bohr(bo);
            }
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

        stream << "( Verwendete Werkzeuge: )\n";
        QStringList sortierteListe = Verwendete_wkz.values();
        sortierteListe.sort();
        for (const QString &nr : sortierteListe)
        {
            stream << "( " << nr  << " )\n";
        }
    }

    return prgende;
}

QString emc2::wkz_wechsel(QString tnummer)
{
    QString gcode;

    if(Akt_wkz != tnummer)
    {
        QTextStream stream(&gcode);
        stream << "\n" << "M0 (Bitte Werkzeug: \"" << tnummer << "\" einwechseln)\n";
        stream << "( BITTE SPINDEL MANUELL EINSCHALTEN )\n";
    }

    Akt_wkz = tnummer;
    Verwendete_wkz.insert(tnummer);//wird nur eingefügt wenn der Wert nicht bereits vorhanden ist in der Liste

    return gcode;
}

QString emc2::bohr(bohrung bo)
{
    QString gcode;

    if(ausdruck_auswerten(bo.afb()).toDouble() <= 0)
    {
        return gcode;//leerer String
    }

    if(Maschine != nullptr && Wst != nullptr)
    {
        QTextStream stream(&gcode);
        QString bezug = bo.bezug();
        if(!bo.istZapfen())
        {
            QString tnummer = Maschine->wkzmag().wkznummer(WKZ_TYP_BOHRER, bo.dm(), bo.tiefe(), Wst->dicke(), bezug);
            if(!tnummer.isEmpty())
            {
                stream << wkz_wechsel(tnummer);

                //Werkzeug wurde gefunden, Bohrung kann gebohrt werden:
                if(bezug == WST_BEZUG_OBSEI)
                {
                    // Parameter definieren
                    double wst_dicke = Wst->dicke();
                    double bohrtiefe = bo.tiefe();
                    double zustellmass = bo.zustellmass();
                    double austritt = 0.0;
                    double toleranz = 0.01;
                    double vorschub = Maschine->wkzmag().vorschub(tnummer).toDouble();

                    // Prüfen ob Durchgangsbohrung
                    if (bohrtiefe >= (wst_dicke - toleranz))
                    {
                        // Falls die Bohrung durchgehen soll -> Sicherheitszugabe
                        austritt = Masszugabe_duboti;
                    }

                    // Z-Werte berechnen
                    double sicherheits_z = wst_dicke + Sicherheitsabstand;
                    double ziel_z = (wst_dicke - bohrtiefe) - austritt;
                    double rueckzug_r = wst_dicke + 1.0;

                    // G-Code generieren
                    if (bohrtiefe > 0)
                    {
                        stream << "\n";
                        stream << "( " << bohr_zu_prgzei(bo.text()) << " )\n";
                        // 1. Positionierung
                        stream << "G0 X" << bo.x() << " Y" << bo.y() << "\n";
                        stream << "G0 Z" << sicherheits_z << "\n";

                        // 2. Zyklus wählen

                        if (bohrtiefe <= zustellmass)
                        {   // Wenn die Tiefe sehr gering ist, reicht G81 (ohne Spanbrechen)
                            stream << "G81 Z" << ziel_z
                                   << " R" << rueckzug_r
                                   << " F"<< vorschub << "\n";
                        }else
                        {   // G83 für tiefere Bohrungen mit Entspanen
                            stream << "G83 Z" << ziel_z
                                   << " R" << rueckzug_r
                                   << " Q" << zustellmass
                                   << " F"<< vorschub << "\n";
                        }

                        // 3. Abschluss
                        stream << "G80" << "\n";
                        stream << "G0 Z" << sicherheits_z << "\n";
                    }
                }
            }else
            {
                //Kein Werkzeug wurde gefunden.
                //Kann Bohrung als Kreistasche gefräst werden?:

                //Ist direkt ein WKZ definiert?:
                if(bo.bezug() == WST_BEZUG_OBSEI  ||  bo.bezug() == WST_BEZUG_UNSEI)
                {
                    tnummer = Maschine->wkzmag().wkznummer_von_alias(bo.wkznum(), WKZ_VERT);
                }else
                {
                    tnummer = Maschine->wkzmag().wkznummer_von_alias(bo.wkznum(), WKZ_HORI);
                }

                //Oder ist ein geeignetes WKZ verfügbar?:
                if(tnummer.isEmpty())
                {
                    tnummer = Maschine->wkzmag().wkznummer(WKZ_TYP_FRAESER, bo.dm(), bo.tiefe(), Wst->dicke(), bezug);
                }

                if(!tnummer.isEmpty())
                {
                    //Es kann eine Kreistasche verwendet werden

                    double wst_dicke = Wst->dicke();
                    double bohrtiefe = bo.tiefe();
                    double zustellmass = Maschine->wkzmag().zustmasvert(tnummer).toDouble();
                    double austritt = 0.0;
                    double toleranz = 0.01;
                    double vorschub = Maschine->wkzmag().vorschub(tnummer).toDouble();
                    double wkzdm = Maschine->wkzmag().dm(tnummer).toDouble();//Durchmesser des Fräsers
                    double ueberlappung = 1.0;//Überlappung der Fräsbahnen zueinander

                    // Prüfen ob Durchgangs-Tasche
                    if (bohrtiefe >= (wst_dicke - toleranz))
                    {
                        // Falls die Tasche durchgehen soll -> Sicherheitszugabe
                        austritt = Masszugabe_dutati;
                    }

                    // Z-Werte berechnen
                    double sicherheits_z = wst_dicke + Sicherheitsabstand;
                    double ziel_z = (wst_dicke - bohrtiefe) - austritt;

                    stream << wkz_wechsel(tnummer);

                    if(Maschine->wkzmag().kann_bohrend_eintauchen(tnummer))
                    {//bohrendes eintauchen (spart etwas Zeit)
                        if (bohrtiefe > 0)
                        {
                            stream << "\n";
                            stream << "( Kreistasche bohrend eintauchen: )\n";
                            stream << "( " << bohr_zu_prgzei(bo.text()) << " )\n";

                            double schlichtzugabe = 0.5;
                            double taschen_radius = bo.dm() / 2.0;
                            double fraeser_radius = wkzdm / 2.0;

                            // Radius für das grobe Ausräumen (mit Aufmaß)
                            double max_ausraeum_radius = taschen_radius - fraeser_radius - schlichtzugabe;
                            // Radius für das finale Schlichten
                            double final_schlicht_radius = taschen_radius - fraeser_radius;

                            // Sicherheitscheck: Ist die Tasche groß genug für Schlichten?
                            if (max_ausraeum_radius < 0.1)
                            {
                                max_ausraeum_radius = final_schlicht_radius;
                                schlichtzugabe = 0;
                            }

                            stream << "G0 X" << bo.x() << " Y" << bo.y() << "\n";
                            stream << "G0 Z" << sicherheits_z << "\n";

                            // --- SCHRITT 1: Räumen der Tasche mit Z-Zustellungen ---
                            double aktuelle_z = wst_dicke;
                            while (aktuelle_z > ziel_z + toleranz)
                            {
                                aktuelle_z -= zustellmass;
                                if (aktuelle_z < ziel_z)
                                {
                                    aktuelle_z = ziel_z;
                                }

                                // Zustellung in Z
                                stream << "G1 Z" << aktuelle_z << " F" << vorschub / 2.0 << "\n";

                                // Radiales Räumen bis zum Schlichtaufmaß
                                double aktueller_radius = 0.0;
                                double schrittweite = wkzdm - ueberlappung;

                                while (aktueller_radius < max_ausraeum_radius - toleranz)
                                {
                                    aktueller_radius += schrittweite;
                                    if (aktueller_radius > max_ausraeum_radius)
                                    {
                                        aktueller_radius = max_ausraeum_radius;
                                    }

                                    stream << "G1 X" << (bo.x() + aktueller_radius) << " F" << vorschub << "\n";
                                    stream << "G2 X" << (bo.x() + aktueller_radius) << " Y" << bo.y()
                                           << " I" << -aktueller_radius << " J0" << "\n";
                                }

                                // Nach jeder Z-Ebene kurz zur Mitte zurück, um Freiraum zu schaffen
                                stream << "G1 X" << bo.x() << " Y" << bo.y() << "\n";
                            }

                            // --- SCHRITT 2: Finales Schlichten in voller Tiefe ---
                            if (schlichtzugabe > 0)
                            {
                                stream << "( Finales Schlichten der Wandung in voller Tiefe )\n";
                                // Der Fräser steht bereits auf ziel_z in der Mitte
                                stream << "G1 X" << (bo.x() + final_schlicht_radius) << " F" << vorschub * 0.7 << "\n";
                                stream << "G2 X" << (bo.x() + final_schlicht_radius) << " Y" << bo.y()
                                       << " I" << -final_schlicht_radius << " J0" << "\n";

                                // Nach dem Schlichten wieder zur Mitte fahren
                                stream << "G1 X" << bo.x() << " Y" << bo.y() << "\n";
                            }

                            // --- SCHRITT 3: Rückzug ---
                            stream << "G0 Z" << sicherheits_z << "\n";
                        }
                    }else
                    {//spiralförmiges eintauchen
                        if (bohrtiefe > 0)
                        {
                            stream << "\n";
                            stream << "( Kreistasche helikal eintauchen: )\n";
                            stream << "( " << bohr_zu_prgzei(bo.text()) << " )\n";

                            double schlichtzugabe = 0.5;
                            double taschen_radius = bo.dm() / 2.0;
                            double fraeser_radius = wkzdm / 2.0;
                            double max_ausraeum_radius = taschen_radius - fraeser_radius - schlichtzugabe;
                            double final_schlicht_radius = taschen_radius - fraeser_radius;

                            if (max_ausraeum_radius < 0.1)
                            {
                                max_ausraeum_radius = final_schlicht_radius;
                                schlichtzugabe = 0;
                            }

                            // --- SCHRITT 0: Anfahren ---
                            stream << "G0 X" << bo.x() << " Y" << bo.y() << "\n";
                            stream << "G0 Z" << sicherheits_z << "\n";

                            double aktuelle_z = wst_dicke;
                            // Radius für die Eintauch-Helix (kleiner Kreis, um Platz zu schaffen)
                            double helix_radius = std::min(max_ausraeum_radius, wkzdm * 0.4);

                            while (aktuelle_z > ziel_z + toleranz)
                            {
                                double start_z = aktuelle_z;
                                aktuelle_z -= zustellmass;
                                if (aktuelle_z < ziel_z)
                                {
                                    aktuelle_z = ziel_z;
                                }

                                // --- SCHRITT 1: Helikales Eintauchen ---
                                if (helix_radius > 0.1)
                                {
                                    // 1a. Anfahren auf Startpunkt der Helix (X-Versatz)
                                    stream << "G1 X" << (bo.x() + helix_radius) << " Z" << start_z << " F" << vorschub << "\n";

                                    // 1b. Helix fahren (Vollkreis mit Z-Zustellung)
                                    // G2 X... Y... Z... I... J...
                                    stream << "G2 X" << (bo.x() + helix_radius) << " Y" << bo.y()
                                           << " Z" << aktuelle_z << " I" << -helix_radius << " J0 F" << vorschub / 2.0 << "\n";

                                    // 1c. Ein Ebener Kreis auf Ziel-Tiefe, um den Boden zu glätten
                                    stream << "G2 X" << (bo.x() + helix_radius) << " Y" << bo.y()
                                           << " I" << -helix_radius << " J0\n";
                                }
                                else
                                {
                                    // Falls die Tasche zu klein für eine Helix ist (Notfall: Rampe oder langsames Eintauchen)
                                    stream << "G1 Z" << aktuelle_z << " F" << vorschub / 5.0 << "\n";
                                }

                                // --- SCHRITT 2: Radiales Räumen auf dieser Ebene ---
                                double aktueller_radius = helix_radius;
                                double schrittweite = wkzdm - ueberlappung;

                                while (aktueller_radius < max_ausraeum_radius - toleranz)
                                {
                                    aktueller_radius += schrittweite;
                                    if (aktueller_radius > max_ausraeum_radius)
                                    {
                                        aktueller_radius = max_ausraeum_radius;
                                    }

                                    stream << "G1 X" << (bo.x() + aktueller_radius) << " F" << vorschub << "\n";
                                    stream << "G2 X" << (bo.x() + aktueller_radius) << " Y" << bo.y()
                                           << " I" << -aktueller_radius << " J0\n";
                                }

                                // Zurück zur Mitte
                                stream << "G1 X" << bo.x() << " Y" << bo.y() << "\n";
                            }

                            // --- SCHRITT 3: Finales Schlichten in voller Tiefe ---
                            if (schlichtzugabe > 0)
                            {
                                stream << "( Schlichten Wandung )\n";
                                stream << "G1 X" << (bo.x() + final_schlicht_radius) << " F" << vorschub * 0.7 << "\n";
                                stream << "G2 X" << (bo.x() + final_schlicht_radius) << " Y" << bo.y()
                                       << " I" << -final_schlicht_radius << " J0\n";
                                stream << "G1 X" << bo.x() << " Y" << bo.y() << "\n";
                            }

                            stream << "G0 Z" << sicherheits_z << "\n";
                        }
                    }
                }else
                {
                    //Fehlermeldung:
                    stream << "\n";
                    stream << "( >>>>>>>>>> Bohrung übersprungen weil kein geeignetes Werkzeug gefunden wurde! <<<<<<<<<<)\n";
                    stream << "( " << bohr_zu_prgzei(bo.text()) << " )\n\n";
                }
            }
        }else //bo.istZapfen()
        {

        }
    }
    return gcode;
}
