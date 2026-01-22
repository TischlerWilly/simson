#ifndef GEOMETRIE_H
#define GEOMETRIE_H

#include <math.h>
#include "punkt3d.h"

double winkel(double endpunkt1_x, double endpunkt1_y,\
              double mittelpunkt_x, double mittelpunkt_y,\
              double endpunkt2_x, double endpunkt2_y);

punkt3d drehen(punkt3d zentrum, punkt3d punkt, double drewi);

double radToDeg(double rad);
double degToRad(double deg);

#endif // GEOMETRIE_H
