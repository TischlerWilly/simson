#ifndef UNDO_REDO_TZ_H
#define UNDO_REDO_TZ_H

#include <vector>
#include "Klassen/text_zw.h"


template<typename T>
class undo_redo
{
public:
    undo_redo()
    {
        set_groesse_max(100);
        clear();
    }
    void neu(T t_neu)
    {
        // Falls wir nach einem Undo in der Mitte des Stacks sind:
        // Alles löschen, was nach der aktuellen Position kommt
        if ((uint)Aktuelle_position + 1 < Vector_t.size())
        {
            Vector_t.erase(Vector_t.begin() + Aktuelle_position + 1, Vector_t.end());
            Erstes_aktives_element_t.erase(Erstes_aktives_element_t.begin() + Aktuelle_position + 1, Erstes_aktives_element_t.end());
            Anz_aktive_elemente_t.erase(Anz_aktive_elemente_t.begin() + Aktuelle_position + 1, Anz_aktive_elemente_t.end());
        }

        // Wenn Maximum erreicht: Erstes Element entfernen
        if (Vector_t.size() >= Max_anzahl && Max_anzahl > 0)
        {
            Vector_t.erase(Vector_t.begin());
            Erstes_aktives_element_t.erase(Erstes_aktives_element_t.begin());
            Anz_aktive_elemente_t.erase(Anz_aktive_elemente_t.begin());
        }

        // Neuen Wert hinzufügen
        Vector_t.push_back(t_neu);

        // WICHTIG: Die Index-Vektoren müssen immer befüllt werden!
        Erstes_aktives_element_t.push_back(0);
        Anz_aktive_elemente_t.push_back(1);

        Aktuelle_position = Vector_t.size() - 1;
    }

    int pos() // Neue Hilfsfunktion für den Dirty-Check
    {
        return Aktuelle_position;
    }

    //----------------------------------------set_xy:
    void set_groesse_max(uint maximale_anzahl_an_wiederrufenschritten)
    {
        if(maximale_anzahl_an_wiederrufenschritten > Vector_t.size())
        {
            Max_anzahl = maximale_anzahl_an_wiederrufenschritten;
        }
        if(maximale_anzahl_an_wiederrufenschritten < Vector_t.size())
        {
            Vector_t.clear();
            Max_anzahl = maximale_anzahl_an_wiederrufenschritten;
        }
    }

    //----------------------------------------get_xy:
    uint groesse_max()
    {
        return Max_anzahl;
    }
    uint groesse()
    {
        return Vector_t.size();
    }
    int  erstes_aktives_element()
    {
        return Erstes_aktives_element_t.at(Aktuelle_position);
    }
    int  anz_aktive_elemente()
    {
        return Vector_t.size(); // Gibt die Anzahl der gespeicherten Schritte zurück
    }
    T akt_elem()
    {
        if(Aktuelle_position >= 0)
        {
            return Vector_t.at(Aktuelle_position);
        }else
        {
            T ein_leeres_etwas;
            return ein_leeres_etwas;
        }
    }
    int akt_pos()
    {
        return Aktuelle_position;
    }

    //----------------------------------------Manipulationen:
    void             clear()
    {
        Aktuelle_position = -1;
        Vector_t.clear();
    }
    T undo()
    {
        if(Aktuelle_position > 0)
        {
            Aktuelle_position--;
            return Vector_t.at(Aktuelle_position);
        }else
        {
            return Vector_t.at(0);
        }
    }
    T redo()
    {
        if((uint)Aktuelle_position+1 < Vector_t.size())
        {
            Aktuelle_position++;
            return Vector_t.at(Aktuelle_position);
        }else
        {
            return Vector_t.at(Aktuelle_position);
        }
    }

    //----------------------------------------

private:

    std::vector<T> Vector_t;
    std::vector<int>              Erstes_aktives_element_t;
    std::vector<int>              Anz_aktive_elemente_t;

    int  Aktuelle_position;
    uint Max_anzahl;




};



#endif // UNDO_REDO_TZ_H
