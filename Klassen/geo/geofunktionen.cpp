#include "geofunktionen.h"

double winkel(double endpunkt1_x, double endpunkt1_y,\
              double mittelpunkt_x, double mittelpunkt_y,\
              double endpunkt2_x, double endpunkt2_y)
{
    // Struktur für einen 2D-Punkt
    struct Point {
        double x;
        double y;
    };
    //Point sp = {1.0, 0.0};
    //Point mp = {0.0, 0.0};
    //Point ep = {0.0, 1.0};
    Point sp = {endpunkt1_x, endpunkt1_y};
    Point mp = {mittelpunkt_x, mittelpunkt_y};
    Point ep = {endpunkt2_x, endpunkt2_y};

    // Vektoren von mp zu sp und mp zu ep
    double v1_x = sp.x - mp.x;
    double v1_y = sp.y - mp.y;
    double v2_x = ep.x - mp.x;
    double v2_y = ep.y - mp.y;

    // Skalarprodukt und Längen der Vektoren
    double dot = v1_x * v2_x + v1_y * v2_y;
    double len1 = std::sqrt(v1_x * v1_x + v1_y * v1_y);
    double len2 = std::sqrt(v2_x * v2_x + v2_y * v2_y);

    if (len1 == 0.0 || len2 == 0.0) {
        // Ungültige Eingabe, ein Punkt ist identisch mit mp
        return 0.0;
    }

    // Winkel im Bogenmaß
    double cos_angle = dot / (len1 * len2);
    // Korrektur für numerische Fehler
    if (cos_angle > 1.0) cos_angle = 1.0;
    if (cos_angle < -1.0) cos_angle = -1.0;
    double angle_rad = std::acos(cos_angle);
    return angle_rad;
}

punkt3d drehen(punkt3d punkt, punkt3d zentrum, double drewi)
{
    // Schritt 1: Punkt relativ zum Zentrum verschieben
    double dx = punkt.x() - zentrum.x();
    double dy = punkt.y() - zentrum.y();

    // Schritt 2: Rotation anwenden
    double rotatedX = dx * std::cos(drewi) - dy * std::sin(drewi);
    double rotatedY = dx * std::sin(drewi) + dy * std::cos(drewi);

    // Schritt 3: Zurückverschieben
    punkt3d ret;
    ret.set_x(rotatedX + zentrum.x());
    ret.set_y(rotatedY + zentrum.y());
    return ret;
}

double radToDeg(double rad)
{
    return rad * (180.0 / M_PI);
}

double degToRad(double deg)
{
    return deg * (M_PI / 180.0);
}



