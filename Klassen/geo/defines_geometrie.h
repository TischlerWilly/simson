#ifndef DEFINES_GEOMETRIE_H
#define DEFINES_GEOMETRIE_H

#define TRZ_PA         "/"     //Trennt einzelne Parameter
#define TRZ_PA_        '/'
#define TRZ_EL         ";"     //Trennt Geometrieelemente
#define TRZ_EL_        ';'

#define PUNKT       "cadPunkt"
/*
 2) x
 3) y
 4) z
 5) Farbe des Punktes
 6) Breite des Punktes = Punktgröße
*/

#define STRECKE     "cadStrecke"
/*
 2) x-Start
 3) y-Start
 4) z-Start
 5) x-Ende
 6) y-Ende
 7) z-Ende
 8) Farbe der Linie
 9) Breite der Linie
10) Stil der Linie
*/

#define BOGEN     "cadBogen"
/*
 2) x-Mittelpunkt
 3) y-Mittelpunkt
 4) z-Mittelpunkt
 5) Radius
 6) Startwinkel (in Radiant)
 7) Endwinkel (in Radiant)
 8) Uhrzeigersinn
 9) Farbe der Linie
10) Breite der Linie
11) Stil der Linie
*/
#define KREIS       "cadKreis"
/*
 2) x-Mitte
 3) y-Mitte
 4) z-Mitte
 5) Radius
 6) Farbe der Linie
 7) Farbe der Füllung
 8) Breite der Linie
 9) Stil der Linie
*/
#define RECHTECK  "cadRechteck"
/*
 2) x-Mittelpunkt
 3) y-Mittelpunkt
 4) z-Mittelpunkt
 5) Länge
 6) Breite
 7) Eckenradius
 8) Drehwinkel (in Radiant)
 9) Farbe der Linie
10) Farbe der Füllung
11) Breite der Linie
12) Stil der Linie
*/

#endif // DEFINES_GEOMETRIE_H
