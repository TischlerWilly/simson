#include "strecke.h"

strecke::strecke()
{

}

strecke::strecke(QString geotext)
{
    set_text(geotext);
}

//-----------------------------------------set_xy:
void strecke::set_text(QString geotext)
{
    text_zw tz;
    tz.set_text(geotext,TRZ_PA_);
    punkt3d p;
    p.set_x(tz.at(1).toDouble());
    p.set_y(tz.at(2).toDouble());
    p.set_z(tz.at(3).toDouble());
    set_stapu(p);
    p.set_x(tz.at(4).toDouble());
    p.set_y(tz.at(5).toDouble());
    p.set_z(tz.at(6).toDouble());
    set_endpu(p);
    set_farbe(tz.at(7));
    set_linienbreite(tz.at(8).toInt());
    set_stil(tz.at(9));
}
void strecke::set_stapu(punkt3d startpunkt)
{
    Stapu = startpunkt;
}
void strecke::set_stapu(double x, double y, double z)
{
    punkt3d p;
    p.set_x(x);
    p.set_y(y);
    p.set_z(z);
    Stapu = p;
}
void strecke::set_endpu(punkt3d endpunkt)
{
    Endpu = endpunkt;
}
void strecke::set_endpu(double x, double y, double z)
{
    punkt3d p;
    p.set_x(x);
    p.set_y(y);
    p.set_z(z);
    Endpu = p;
}
//-----------------------------------------get_xy:
punkt3d strecke::mipu()
{
    return Stapu + ((Endpu-Stapu)/2);
}
punkt3d strecke::schnittp_zur_senkr_an_p(punkt3d p)
{
    //Die Strecke wird unendlich verlängert zu einer Geraden
    //es wird eine Senkrechte zur Gerade gezeichnet die durch den Punkt p verläuft
    //Diese Funktoin liefet den Schnittpunkt der Senkechten auf der Gerade zurück

    //Strecke in Vektorenform bringen:
    //Gerade = ortsvektor + (var * Richtungsvektor)
    //Gerade =     sp     + (var * (ep-sp))
    punkt3d ov; //Ortsvektor
    ov = stapu();
    punkt3d rv; //Richtungsvektor
    rv.set_x(  endpu().x() - stapu().x()  );
    rv.set_y(  endpu().y() - stapu().y()  );
    rv.set_z(  endpu().z() - stapu().z()  );
    // Gerade = ov   + (var * rv  )
    //          ov.x          rv.x   --> ov.x + (var * rv.x)
    //          ov.y + (var * rv.y)  --> ov.y + (var * rv.y)
    //          ov.z          rv.z   --> ov.z + (var * rv.z)

    //  rv   * (Gerade - p  ) = 0
    //  rv.x             p.x
    //  rv.y * (Gerade - p.y) = 0
    //  rv.z             p.z

    // Ausmultiplizienen
    //((rv.x * gerade.x) + (rv.y * gerade.y) + (rv.z * gerade.z)) - ( (rv.x * p.x) + (rv.y * p.y) + (rv.z * p.z) ) = 0
    //((rv.x * gerade.x) + (rv.y * gerade.y) + (rv.z * gerade.z)) - (                    hans                    ) = 0
    double hans = ( rv.x() * p.x() ) + (rv.y() * p.y() ) + (rv.z() * p.z() );

    //((rv.x * gerade.x) + (rv.y * gerade.y) + (rv.z * gerade.z)) - hans = 0
    //     Beispielzahlen:    3  * (  2  +   3 * s )
    //(rv.x * gerade.x) --> rv.x * (ov.x + rv.x*var)
    //                             6 +  9s
    //rv.x * ov.x + (rv.x*rv.x*var)
    //(rv.x * ov.x + (rv.x*rv.x*var)) + (rv.y * ov.y + (rv.y*rv.y*var)) + (rv.z * ov.z + (rv.z*rv.z*var)) - hans = 0
    //    a        + (rv.x*rv.x*var)) + (      b     + (rv.y*rv.y*var)) + (     c      + (rv.z*rv.z*var)) - hans = 0
    //    a        + (    d    *var)) + (      b     + (     e *  var)) + (     c      + (     f *  var)) - hans = 0
    //    a + (d * var) + b + (e *  var) + c + (f *  var) - hans = 0
    //    a + b + c - hans  + ((d+e+f)*var )  = 0
    //          g           +  (   h  *var )  = 0   /-g
    //                    h * var = -g              /:h
    //                        var = (-g)/h
    double a = rv.x() * ov.x();
    double b = rv.y() * ov.y();
    double c = rv.z() * ov.z();
    double d = rv.x() * rv.x();
    double e = rv.y() * rv.y();
    double f = rv.z() * rv.z();
    double g = a + b + c - hans;
    double h = d + e + f;
    double var = (-1 * g)/h;

    //var in gerade einsetzen
    // Gerade = ov   + (var * rv  )
    punkt3d pp; //Punkt auf der Geraden an dem die Senkrechte linie zum Punkt P beginnt
    pp.set_x(  ov.x() + (var * rv.x())  );
    pp.set_y(  ov.y() + (var * rv.y())  );
    pp.set_z(  ov.z() + (var * rv.z())  );

    return pp;
}
double strecke::laenge3d(punkt3d sp, punkt3d ep)
{
    punkt3d p3 = ep - sp;
    return sqrt(p3.x()*p3.x() + p3.y()*p3.y() + p3.z()*p3.z());
                //Wurzel aus (a²+b²+c²)
}
double strecke::laenge2d(punkt3d sp, punkt3d ep)
{//Z-Werte werden ignoriert
    punkt3d p3;
    p3.set_x(ep.x() - sp.x());
    p3.set_y(ep.y() - sp.y());
    return sqrt(p3.x()*p3.x() + p3.y()*p3.y());
    //Wurzel aus (a²+b²)
}

double strecke::abst(punkt3d p)
{
    double abst = 999999;

    if(auf_strecke(p))
    {
        abst = 0;
    }else
    {
        punkt3d pp;
        pp = schnittp_zur_senkr_an_p(p);

        if(auf_strecke(pp))
        {
            abst = laenge3d(p, pp);
        }else
        {
            double abst_sp = laenge3d(stapu(),p);
            double abst_ep = laenge3d(endpu(),p);
            if(abst_sp < abst_ep)
            {
                abst = abst_sp;
            }else
            {
                abst = abst_ep;
            }
        }
    }
    return abst;
}
bool strecke::auf_strecke(punkt3d p)
{
    bool ergebnis;

    punkt3d ov; //Ortsvektor
    ov = stapu();
    punkt3d rv; //Richtungsvektor
    rv.set_x(  endpu().x() - stapu().x()  );
    rv.set_y(  endpu().y() - stapu().y()  );
    rv.set_z(  endpu().z() - stapu().z()  );

    // Gerade = ov   + (var * rv  )
    //Punkt = Gerade in die Gleichung setzen
    //   p    = ov   + (var * rv  )
    //   p.x  = ov.x          rv.x   --> p.x  = ov.x + (var * rv.x)
    //   p.y  = ov.y + (var * rv.y)  --> p.y  = ov.y + (var * rv.y)
    //   p.z  = ov.z          rv.z   --> p.z  = ov.z + (var * rv.z)

    //Der Punkt liegt auf der Gerade wenn alle 3 Gleichungen der gleiche Ergebnis haben
    //   p.x  = ov.x + (var * rv.x)  /-ov.x
    //   p.x - ov.x = var * rv.x     /:rv.x
    //   (p.x - ov.x):rv.x = var
    double var_x = (p.x() - ov.x()) / rv.x();
    double var_y = (p.y() - ov.y()) / rv.y();
    double var_z = (p.z() - ov.z()) / rv.z();

    if(var_x == var_y == var_z)
    {
        double var = var_x;
        if(  (0 <= var)  && (var <= 1)  )
        {
            ergebnis = true; //Punkt liegt auf der Strecke
        }else
        {
            ergebnis = false; //Punkt liegt zwar auf der Geraden, jedoch vor dem Start oder nach dem Endpunkt
        }
    }else
    {
        ergebnis = false; //Punkt liegt nicht auf der Geraden und somit auch nicht auf der Strecke
    }
    return ergebnis;
}
double strecke::wink()
{
    // Berechnet den Winkel (in Radiant) zwischen der Horizontalen
    // und der Linie vom Zentrumspunkt (cx, cy) zum Schenkelpunkt (px, py).
    //double cx = stapu().x();
    //double cy = stapu().y();
    //double px = endpu().x();
    //double py = endpu().y();

    double dx = endpu().x() - stapu().x();//px - cx
    double dy = endpu().y() - stapu().y();//py - cy

    return std::atan2(dy, dx);  // Ergebnis in Radiant
}
strecke strecke::parallele(bool links, double abstand)
{
    // 1. Richtungsvektor der Originalstrecke berechnen
    double dx = Endpu.x() - Stapu.x();
    double dy = Endpu.y() - Stapu.y();

    // 2. Länge der Strecke berechnen (für die Normierung)
    double laenge = std::sqrt(dx * dx + dy * dy);

    // Sicherheitsabfrage: Falls die Punkte identisch sind,
    // kann keine Parallele berechnet werden.
    if (laenge < 0.0000001) {
        strecke s;
        s.set_stapu(Stapu);
        s.set_endpu(Endpu);
        return s;
    }

    // 3. Normalenvektor berechnen (senkrecht auf der Strecke)
    // Bei (dx, dy) ist (-dy, dx) der Vektor nach links
    // und (dy, -dx) der Vektor nach rechts.
    double nx, ny;
    if (links) {
        nx = -dy / laenge;
        ny =  dx / laenge;
    } else {
        nx =  dy / laenge;
        ny = -dx / laenge;
    }

    // 4. Verschiebevektor mit dem Abstand multiplizieren
    double vx = nx * abstand;
    double vy = ny * abstand;

    // 5. Neue Punkte berechnen (Z bleibt unverändert)
    punkt3d sp(Stapu.x() + vx, Stapu.y() + vy, Stapu.z());
    punkt3d ep(Endpu.x() + vx, Endpu.y() + vy, Endpu.z());

    // 6. Neue Strecke zurückgeben
    strecke s;
    s.set_stapu(sp);
    s.set_endpu(ep);
    return s;
}
QString strecke::text()
{
    QString msg = STRECKE;
    msg += TRZ_PA;
    msg += stapu().x_QString();
    msg += TRZ_PA;
    msg += stapu().y_QString();
    msg += TRZ_PA;
    msg += stapu().z_QString();
    msg += TRZ_PA;
    msg += endpu().x_QString();
    msg += TRZ_PA;
    msg += endpu().y_QString();
    msg += TRZ_PA;
    msg += endpu().z_QString();
    msg += TRZ_PA;
    msg += farbe();
    msg += TRZ_PA;
    msg += linienbreite_qstring();
    msg += TRZ_PA;
    msg += stil();

    return msg;
}

//-----------------------------------------manipulationen:
void strecke::richtung_unkehren()
{
    punkt3d tmp = Stapu;
    Stapu = Endpu;
    Endpu = tmp;
}
void strecke::drenen_um_stapu_2d(double drehwi, bool uzs)
{
    //drewi muss in Radiant angegeben werden!!
    if(uzs == true)
    {
        drehwi = -drehwi;
    }
    punkt3d sp(Stapu);
    punkt3d ep(Endpu);
    ep = drehen(sp, ep, drehwi);
    punkt3d tmp;
    tmp.set_x(ep.x());
    tmp.set_y(ep.y());
    tmp.set_z(Endpu.z());
    set_endpu(tmp);
}

void strecke::drenen_um_endpu_2d(double drehwi, bool uzs)
{
    //drewi muss in Radiant angegeben werden!!
    if(uzs == true)
    {
        drehwi = -drehwi;
    }
    punkt3d sp(Stapu);
    punkt3d ep(Endpu);
    sp = drehen(ep, sp, drehwi);
    punkt3d tmp;
    tmp.set_x(sp.x());
    tmp.set_y(sp.y());
    tmp.set_z(Stapu.z());
    set_stapu(tmp);
}
void strecke::drenen_um_mipu_2d(double drehwi, bool uzs)
{
    //drewi muss in Radiant angegeben werden!!
    if(uzs == true)
    {
        drehwi = -drehwi;
    }
    punkt3d sp(Stapu);
    punkt3d ep(Endpu);
    punkt3d mp = mipu();
    sp = drehen(mp, sp, drehwi);
    ep = drehen(mp, ep,drehwi);
    set_stapu(sp);
    set_endpu(ep);
}
void strecke::verschieben_um(double xversatz, double yversatz)
{
    Stapu.verschieben_um(xversatz, yversatz);
    Endpu.verschieben_um(xversatz, yversatz);
}
void strecke::set_laenge(double neue_laenge, strecke_bezugspunkt bezugspunkt)
{
    double skalfakt = neue_laenge/laenge2d(stapu(), endpu());

    switch(bezugspunkt)
    {
    case strecke_bezugspunkt_start:
    {
        punkt3d sp(Stapu);
        punkt3d ep(Endpu);
        punkt3d p2;
        punkt3d p3;
        p2 = ep-sp;
        p2 = p2*skalfakt;
        ep = sp+p2;
        p3.set_x(ep.x());
        p3.set_y(ep.y());
        p3.set_z(Endpu.z());
        set_endpu(p3);
    }
    break;
    case strecke_bezugspunkt_mitte:
    {
        punkt3d mp = mipu();
        punkt3d sp(Stapu);
        punkt3d ep(Endpu);

        punkt3d p2e;//Endpunkt
        punkt3d p3e;//Endpunkt
        p2e = ep-mp;
        p2e = p2e*skalfakt;
        ep = mp+p2e;
        p3e.set_x(ep.x());
        p3e.set_y(ep.y());
        p3e.set_z(Endpu.z());
        set_endpu(p3e);

        punkt3d p2s; //Startpunkt
        punkt3d p3s; //Startpunkt
        p2s = sp-mp;
        p2s = p2s*skalfakt;
        sp = mp+p2s;
        p3s.set_x(sp.x());
        p3s.set_y(sp.y());
        p3s.set_z(Stapu.z());
        set_stapu(p3s);
    }
    break;
    case strecke_bezugspunkt_ende:
    {
        punkt3d sp(Stapu);
        punkt3d ep(Endpu);
        punkt3d p2;
        punkt3d p3;
        p2 = sp-ep;
        p2 = p2*skalfakt;
        sp = ep+p2;
        p3.set_x(sp.x());
        p3.set_y(sp.y());
        p3.set_z(Stapu.z());
        set_stapu(p3);
    }
    break;
    }
}
//-----------------------------------------

//--------------------------------------------------------
//Funktionen außerhalb der Klasse:
bool trimmen(strecke *s1, strecke *s2)
{
    //Diese Funktion ist für Innenecken gedacht
    //Trimmt den Endpunkt von s1 und den Startpunkt von s2
    //wenn die Operation erfolgreich ausgeführt werden kann gibt die Funkintion true zurück
    //wenn sich die Strechen nicht schneiden gibt die Funktion false zurück

    // Richtungsvektoren
    double dx1 = s1->endpu().x() - s1->stapu().x();
    double dy1 = s1->endpu().y() - s1->stapu().y();
    double dx2 = s2->endpu().x() - s2->stapu().x();
    double dy2 = s2->endpu().y() - s2->stapu().y();

    // Determinante
    double nenner = dx1 * dy2 - dy1 * dx2;

    // Parallelitätsprüfung mit etwas größerem Epsilon für numerische Stabilität
    if (std::abs(nenner) < 1e-7)
    {
        return false;
    }

    // Vektor vom Start s1 zum Start s2
    double ox = s2->stapu().x() - s1->stapu().x();
    double oy = s2->stapu().y() - s1->stapu().y();

    // t = Wie weit auf s1 (0.0 bis 1.0)
    // u = Wie weit auf s2 (0.0 bis 1.0)
    double t = (ox * dy2 - oy * dx2) / nenner;
    double u = (ox * dy1 - oy * dx1) / nenner;

    // TOLERANZ-CHECK:
    // Wir erlauben ein winziges Überstehen (1e-6), um Rundungsfehler
    // bei exakten 45°/90° Winkeln abzufangen.
    const double eps = 1e-6;

    // WICHTIG: Wenn du NUR KÜRZEN willst (Innenecke):
    // Der Schnittpunkt muss VOR dem Ende von s1 liegen (t <= 1.0)
    // UND NACH dem Anfang von s2 liegen (u >= 0.0)

    // Falls deine Radienkorrektur die Strecken jedoch so weit verschiebt,
    // dass sie sich erst NACH ihrem Ende treffen, ist es eine Außenecke.

    if (t > -eps && t < 1.0 + eps && u > -eps && u < 1.0 + eps)
    {
        double px = s1->stapu().x() + t * dx1;
        double py = s1->stapu().y() + t * dy1;

        // Z-Wert: Wir nehmen den Durchschnitt oder den Wert von s1
        punkt3d schnittp(px, py, s1->endpu().z());

        s1->set_endpu(schnittp);
        s2->set_stapu(schnittp);
        return true;
    }

    // Wenn die Bedingung oben nicht zutrifft, liegt der Schnittpunkt
    // außerhalb der Segmente. Das bedeutet: Lücke (Außenecke) oder
    // die Strecken liegen komplett voneinander weg.
    return false;

    // Wenn der Schnittpunkt außerhalb liegt (Außenecke / Lücke),
    // liefern wir false zurück, damit später ein Bogen eingefügt werden kann.
}
bool trimmen(strecke *s1, bogen *b2)
{
    //Trimmt den Endpunkt von s1 und den Startpunkt von b2
    //wenn die Operation erfolgreich ausgeführt werden kann gibt die Funkintion true zurück
    //wenn sich die Geometrien nicht schneiden gibt die Funktion false zurück

    // 1. Grunddaten (Winkel sind in Radiant)
    punkt3d m = b2->mipu();
    double r  = b2->rad();
    double swi_alt = b2->swi(); // Startwinkel in Radiant

    double x1 = s1->stapu().x();
    double y1 = s1->stapu().y();
    double dx = s1->endpu().x() - x1;
    double dy = s1->endpu().y() - y1;
    double d_len = std::sqrt(dx * dx + dy * dy);

    if (d_len < 1e-7) return false;

    // 2. Quadratische Gleichung (Abstand t vom Startpunkt s1)
    double ux = dx / d_len;
    double uy = dy / d_len;
    double ox = x1 - m.x();
    double oy = y1 - m.y();

    double B = 2 * (ox * ux + oy * uy);
    double C = ox * ox + oy * oy - r * r;
    double diskriminante = B * B - 4 * C;

    double t_final;
    if (diskriminante < 0)
    {
        // Lotfußpunkt als bestmögliche Annäherung, falls kein Schnittpunkt existiert
        t_final = -(ox * ux + oy * uy);
    } else
    {
        double sqrtD = std::sqrt(diskriminante);
        double t1 = (-B + sqrtD) / 2.0;
        double t2 = (-B - sqrtD) / 2.0;

        // Wir berechnen die Winkel für beide t-Lösungen in Radiant
        auto get_w = [&](double t)
        {
            return std::atan2(y1 + t * uy - m.y(), x1 + t * ux - m.x());
        };

        double w1 = get_w(t1);
        double w2 = get_w(t2);

        // Wir wählen den t-Wert, dessen Winkel näher am ursprünglichen Startwinkel liegt.
        // Die Normalisierung ist hier zwingend erforderlich!
        if (std::abs(normalize_radiant(w2 - swi_alt)) < std::abs(normalize_radiant(w1 - swi_alt)))
        {
            t_final = t2;
        } else
        {
            t_final = t1;
        }
    }

    // 3. Geometrie aktualisieren
    double w_final = std::atan2(y1 + t_final * uy - m.y(), x1 + t_final * ux - m.x());

    // Nur den Startwinkel des Bogens anpassen
    b2->set_swi(w_final);

    // Strecke exakt auf den neuen Bogenstartpunkt setzen (Erzwingt Punktgleichheit)
    s1->set_endpu(b2->spu());

    return true;
}
bool trimmen(bogen *b1, strecke *s2)
{
    //Trimmt den Endpunkt von b1 und den Startpunkt von s1
    //wenn die Operation erfolgreich ausgeführt werden kann gibt die Funkintion true zurück
    //wenn sich die Geometrien nicht schneiden gibt die Funktion false zurück

    // 1. Daten des Bogens (Mittelpunkt und Radius bleiben fix)
    punkt3d m = b1->mipu();
    double r  = b1->rad();
    double ewi_alt = b1->ewi(); // Der aktuelle Endwinkel in Radiant

    // 2. Daten der Folgestrecke s2
    double x3 = s2->stapu().x();
    double y3 = s2->stapu().y();
    double dx = s2->endpu().x() - x3;
    double dy = s2->endpu().y() - y3;
    double d_len = std::sqrt(dx * dx + dy * dy);

    if (d_len < 1e-7) return false;

    // 3. Quadratische Gleichung Gerade-Kreis
    double ux = dx / d_len;
    double uy = dy / d_len;
    double ox = x3 - m.x();
    double oy = y3 - m.y();

    double B = 2 * (ox * ux + oy * uy);
    double C = ox * ox + oy * oy - r * r;
    double diskriminante = B * B - 4 * C;

    double t_final;
    if (diskriminante < 0)
    {
        // Lotfußpunkt als bestmögliche Annäherung
        t_final = -(ox * ux + oy * uy);
    } else
    {
        double sqrtD = std::sqrt(diskriminante);
        double t1 = (-B + sqrtD) / 2.0;
        double t2 = (-B - sqrtD) / 2.0;

        // Wir wählen den t-Wert, dessen Winkel näher am ursprünglichen Endwinkel (ewi_alt) liegt
        auto get_w = [&](double t)
        {
            return std::atan2(y3 + t * uy - m.y(), x3 + t * ux - m.x());
        };

        double w1 = get_w(t1);
        double w2 = get_w(t2);

        // Vergleich mit Normalisierung (Radiant)
        if (std::abs(normalize_radiant(w2 - ewi_alt)) < std::abs(normalize_radiant(w1 - ewi_alt)))
        {
            t_final = t2;
        } else
        {
            t_final = t1;
        }
    }

    // 4. Geometrie aktualisieren
    double w_final = std::atan2(y3 + t_final * uy - m.y(), x3 + t_final * ux - m.x());

    // Nur den ENDWINKEL des ersten Bogens anpassen
    b1->set_ewi(w_final);

    // Den STARTPUNKT der Strecke exakt auf das neue Bogenende setzen (Punktgleichheit)
    s2->set_stapu(b1->epu());

    return true;
}
double normalize_radiant(double a)
{
    while (a > M_PI)  a -= 2.0 * M_PI;
    while (a < -M_PI) a += 2.0 * M_PI;
    return a;
}

bogen verbindungsbogen(strecke s1, strecke s2)
{
    //Diese funkttion ist für Außenecken gedacht
    //s1 und s2 schneiden sich nicht
    //s1 und s2 würden sich schneiden wenn man sie verlängern würde
    //die Lücke zwischen s1 und s2 wird mit dem Bogen geschlossen
    //Die Funktion gibt einen Bogen mir Radius 0 zurück wenn sie nicht erfolgreich ist

    bogen b;
    b.set_rad(0);

    // 1. Endpunkte der versetzten Bahnen
    punkt3d p_start = s1.endpu();
    punkt3d p_ende  = s2.stapu();

    // 2. Richtungsvektoren der versetzten Bahnen
    double dx1 = s1.endpu().x() - s1.stapu().x();
    double dy1 = s1.endpu().y() - s1.stapu().y();
    double dx2 = s2.endpu().x() - s2.stapu().x();
    double dy2 = s2.endpu().y() - s2.stapu().y();

    // 3. Drehrichtung bestimmen (Knick der Kontur)
    double det = dx1 * dy2 - dy1 * dx2;
    // Wenn det fast 0 ist, sind die Strecken parallel -> kein Bogen nötig
    if (std::abs(det) < 1e-7) return b;

    bool im_uhrzeigersinn = (det < 0);

    // 4. Den Mittelpunkt (M) des Bogens finden
    // Der Mittelpunkt ist der Schnittpunkt der Normalen, die an den
    // Endpunkten (p_start, p_ende) senkrecht auf den Strecken stehen.

    // Normale auf s1 an p_start
    double nx1 = -dy1;
    double ny1 = dx1;
    // Normale auf s2 an p_ende
    double nx2 = -dy2;
    double ny2 = dx2;

    // Wir berechnen den Schnittpunkt der beiden Geraden:
    // G1: p_start + r * n1
    // G2: p_ende + s * n2
    double nenner_m = nx1 * (-ny2) - ny1 * (-nx2);

    if (std::abs(nenner_m) < 1e-9) return b;

    double t = ((p_ende.x() - p_start.x()) * (-ny2) - (p_ende.y() - p_start.y()) * (-nx2)) / nenner_m;

    punkt3d m;
    m.set_x(p_start.x() + t * nx1);
    m.set_y(p_start.y() + t * ny1);
    m.set_z(p_start.z());

    // 5. Radius berechnen (Abstand vom Mittelpunkt zum Startpunkt)
    double radius = std::sqrt(std::pow(m.x() - p_start.x(), 2) + std::pow(m.y() - p_start.y(), 2));

    // 6. Bogen erzeugen
    if (radius > 0.0001)
    {
        b.set_bogen(p_start, p_ende, radius, im_uhrzeigersinn);
        // Da set_bogen intern manchmal den Mittelpunkt neu wählt,
        // setzen wir zur Sicherheit den exakt berechneten Mittelpunkt:
        b.set_mipu(m);
    }

    return b;
}













