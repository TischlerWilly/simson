#include "maschinen.h"

maschinen::maschinen()
{

}
//--------------------------------------------------set_xy:
bool maschinen::neu(maschine masch)
{
    if(ist_bekannt(masch.name()))
    {
        return true;//FEHLER, der Teil gibt es bereits!!!
    }else
    {
        Namen.add_hi(masch.name());
        Maschinen.append(masch);
    }
    return false;
}
bool maschinen::entf(QString masch_name)
{
    int wst_index = get_index(masch_name);
    if(wst_index >= 0)
    {
        Namen.entf(wst_index, 1);
        Maschinen.erase(Maschinen.begin() + wst_index);
        return true;
    }else
    {
        return false;
    }
}

//--------------------------------------------------get_xy:
bool maschinen::ist_bekannt(QString masch_name)
{
    for(uint i=0; i<Namen.count() ;i++)
    {
        if(masch_name == Namen.at(i))
        {
            return true;
        }
    }
    return false;
}
int maschinen::get_index(QString masch_name)
{
    for(uint i=0; i<Namen.count() ;i++)
    {
        if(masch_name == Namen.at(i))
        {
            return i;
        }
    }
    return -1;
}
maschine* maschinen::masch(uint index)
{
    //übergibt eine Kopie der Maschine
    //Änderungen an dieser Kopie werden nicht zurück in diese Instanz geschrieben.
    if(index >= 0 && index < Namen.count())
    {
        return &Maschinen[index];
    }else
    {
        return NULL;
    }
}
QString maschinen::name(uint index)
{
    if(index > 0 && index < Namen.count())
    {
        return Namen.at(index);
    }else
    {
        return "";
    }
}

//--------------------------------------------------Manipulationen:
void maschinen::clear()
{
    Namen.clear();
    Maschinen.clear();
}






