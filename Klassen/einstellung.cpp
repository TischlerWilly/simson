#include "einstellung.h"

einstellung::einstellung()
{
    Entwicklermodus             = false;
    Verzeichnis_quelle          = "./";
    Verzeichnis_ziel_server     = "./";
    Verzeichnis_ziel_lokal      = "./";
}

//----------------------------------------set:
void einstellung::set_text(QString t)
{
    text_zw tz;
    tz.set_text(t,'\n');
    for(uint i=0; i<tz.count() ;i++)
    {
        text_zw spalten;
        spalten.set_text(tz.at(i),'\t');
        if(spalten.at(0) == "Entwicklermodus:")
        {
            set_entwicklermodus(spalten.at(1));
        }else if(spalten.at(0) == "verzeichnis_quelle:")
        {
            set_verzeichnis_quelle(spalten.at(1));
        }else if(spalten.at(0) == "verzeichnis_ziel_server:")
        {
            set_verzeichnis_ziel_server(spalten.at(1));
        }else if(spalten.at(0) == "verzeichnis_ziel_lokal:")
        {
            set_verzeichnis_ziel_lokal(spalten.at(1));
        }
    }
}
void einstellung::set_entwicklermodus(bool ja)
{
    Entwicklermodus = ja;
}
void einstellung::set_entwicklermodus(QString jn)
{
    if(jn == "ja")
    {
        set_entwicklermodus(true);
    }else
    {
        set_entwicklermodus(false);
    }
}
void einstellung::set_verzeichnis_quelle(QString v)
{
    Verzeichnis_quelle = v;
}
void einstellung::set_verzeichnis_ziel_server(QString v)
{
    Verzeichnis_ziel_server = v;
}
void einstellung::set_verzeichnis_ziel_lokal(QString v)
{
    Verzeichnis_ziel_lokal = v;
}

//----------------------------------------get:
QString einstellung::text()
{
    QString text;

    text += "Entwicklermodus:";
    text += "\t";
    if(entwicklermodus() == true)
    {
        text += "ja";
    }else
    {
        text += "nein";
    }
    text += "\n";

    text += "verzeichnis_quelle:";
    text += "\t";
    text += verzeichnis_quelle();
    text += "\n";

    text += "verzeichnis_ziel_server:";
    text += "\t";
    text += verzeichnis_ziel_server();
    text += "\n";

    text += "verzeichnis_ziel_lokal:";
    text += "\t";
    text += verzeichnis_ziel_lokal();
    text += "\n";    

    return text;
}
bool einstellung::entwicklermodus()
{
    return Entwicklermodus;
}
QString einstellung::verzeichnis_quelle()
{
    return Verzeichnis_quelle;
}
QString einstellung::verzeichnis_ziel_server()
{
    return Verzeichnis_ziel_server;
}
QString einstellung::verzeichnis_ziel_lokal()
{
    return Verzeichnis_ziel_lokal;
}
