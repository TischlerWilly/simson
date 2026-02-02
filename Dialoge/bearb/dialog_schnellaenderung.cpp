#include "dialog_schnellaenderung.h"
#include "ui_dialog_schnellaenderung.h"

Dialog_schnellaenderung::Dialog_schnellaenderung(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialog_schnellaenderung)
{
    ui->setupUi(this);
    Startzeile = 0;
    Menge = 0;
    Param_alle.set_text("Ausführbedingung", '\n');
    Param_bohr = Param_alle;
    Param_bohr.add_hi("Durchmesser");
    Param_bohr.add_hi("Tiefe");
    Param_nut = Param_alle;
    Param_nut.add_hi("Breite");
    Param_nut.add_hi("Tiefe");
    Param_rta = Param_alle;
    Param_rta.add_hi("Länge");
    Param_rta.add_hi("Breite");
    Param_rta.add_hi("Tiefe");
    Param_fauf = Param_alle;
    Param_fauf.add_hi("Anfahrtyp");
    Param_fauf.add_hi("Abfahrtyp");
    Param_fauf.add_hi("Anfahrwert");
    Param_fauf.add_hi("Abfahrwert");
}

Dialog_schnellaenderung::~Dialog_schnellaenderung()
{
    delete ui;
}

void Dialog_schnellaenderung::on_pushButton_ok_clicked()
{
    this->close();
}

void Dialog_schnellaenderung::getData(werkstueck *w, uint start, uint menge)
{
    Wst = w;
    Startzeile = start;
    Menge = menge;
    Dialoge.set_text("ALLE", '\n');


    for(uint i = start; i <= start+menge-1; i++)
    {
        QString programmzeile = Wst->bearb().at(i);
        if(programmzeile.contains(BEARBART_BOHR) || \
           programmzeile.contains(BEARBART_RTA) || \
           programmzeile.contains(BEARBART_NUT) || \
           programmzeile.contains(BEARBART_FRAESERAUFRUF))
        {
            if(!Dialoge.text().contains("Bohrungen"))
            {
                Dialoge.add_hi("Bohrungen");
            }
        }else if(programmzeile.contains(BEARBART_NUT))
        {
            if(!Dialoge.text().contains("Nuten"))
            {
                Dialoge.add_hi("Nuten");
            }
        }else if(programmzeile.contains(BEARBART_RTA))
        {
            if(!Dialoge.text().contains("Rechteckstaschen"))
            {
                Dialoge.add_hi("Rechteckstaschen");
            }
        }else if(programmzeile.contains(BEARBART_FRAESERAUFRUF))
        {
            if(!Dialoge.text().contains("Fräseraufrufe"))
            {
                Dialoge.add_hi("Fräseraufrufe");
            }
        }
    }

    ui->comboBox_dlg->clear();
    for(uint i = 0; i < Dialoge.count() ;i++)
    {
        ui->comboBox_dlg->addItem(Dialoge.at(i));
    }

    this->show();
}

void Dialog_schnellaenderung::on_comboBox_dlg_currentIndexChanged(const QString &arg1)
{
    ui->comboBox_param->clear();
    if(arg1 == "ALLE")
    {
        for(uint i = 0; i < Param_alle.count() ;i++)
        {
            ui->comboBox_param->addItem(Param_alle.at(i));
        }
    }else if(arg1 == "Bohrungen")
    {
        for(uint i = 0; i < Param_bohr.count() ;i++)
        {
            ui->comboBox_param->addItem(Param_bohr.at(i));
        }
    }else if(arg1 == "Nuten")
    {
        for(uint i = 0; i < Param_nut.count() ;i++)
        {
            ui->comboBox_param->addItem(Param_nut.at(i));
        }
    }else if(arg1 == "Rechteckstaschen")
    {
        for(uint i = 0; i < Param_rta.count() ;i++)
        {
            ui->comboBox_param->addItem(Param_rta.at(i));
        }
    }else if(arg1 == "Fräseraufrufe")
    {
        for(uint i = 0; i < Param_fauf.count() ;i++)
        {
            ui->comboBox_param->addItem(Param_fauf.at(i));
        }
    }
}

void Dialog_schnellaenderung::on_comboBox_param_currentIndexChanged()
{
    werte_ermitteln();
}

void Dialog_schnellaenderung::on_comboBox_alt_currentIndexChanged(const QString &arg1)
{
    if(arg1 == "ALLE")
    {
        ui->lineEdit_neu->clear();
    }else
    {
        ui->lineEdit_neu->setText(arg1);
    }

}

void Dialog_schnellaenderung::on_pushButton_werte_aendern_clicked()
{
    QString dlg = ui->comboBox_dlg->currentText();
    QString par = ui->comboBox_param->currentText();
    QString wert_alt = ui->comboBox_alt->currentText();
    QString wert_neu = ui->lineEdit_neu->text();

    if(!wert_neu.isEmpty())
    {
        if(dlg == "ALLE")
        {
            if(par == "Ausführbedingung")
            {
                for(uint i = Startzeile; i <= Startzeile+Menge-1; i++)
                {
                    QString programmzeile = Wst->bearb().at(i);
                    QString parname = "AFB=";
                    QString wert;
                    if(programmzeile.contains(BEARBART_BOHR))
                    {
                        bohrung bo(programmzeile);
                        wert = bo.afb();
                        if(wert == wert_alt)
                        {
                            bo.set_afb(wert_neu);
                            Wst->bearb_ptr()->edit(i, bo.text());
                        }
                    }else if(programmzeile.contains(BEARBART_RTA))
                    {
                        rechtecktasche rta(programmzeile);
                        wert = rta.afb();
                        if(wert == wert_alt)
                        {
                            rta.set_afb(wert_neu);
                            Wst->bearb_ptr()->edit(i, rta.text());
                        }
                    }else if(programmzeile.contains(BEARBART_NUT))
                    {
                        nut n(programmzeile);
                        wert = n.afb();
                        if(wert == wert_alt)
                        {
                            n.set_afb(wert_neu);
                            Wst->bearb_ptr()->edit(i, n.text());
                        }
                    }
                }
            }
        }else if(ui->comboBox_dlg->currentText() == "Bohrungen")
        {
            for(uint i = Startzeile; i <= Startzeile+Menge-1; i++)
            {
                QString programmzeile = Wst->bearb().at(i);
                if(programmzeile.contains(BEARBART_BOHR))
                {
                    bohrung bo(programmzeile);
                    QString wert;// = selektiereEintrag(programmzeile, parname, ENDPAR);
                    if(par == "Ausführbedingung")
                    {
                        wert = bo.afb();
                        if(wert == wert_alt)
                        {
                            bo.set_afb(wert_neu);
                            Wst->bearb_ptr()->edit(i, bo.text());
                        }
                    }else if(par == "Durchmesser")
                    {
                        wert = bo.dm_qstring();
                        if(wert == wert_alt)
                        {
                            bo.set_dm(wert_neu);
                            Wst->bearb_ptr()->edit(i, bo.text());
                        }
                    }else if(par == "Tiefe")
                    {
                        wert = bo.tiefe_qstring();
                        if(wert == wert_alt)
                        {
                            bo.set_tiefe(wert_neu);
                            Wst->bearb_ptr()->edit(i, bo.text());
                        }
                    }
                }
            }
        }else if(ui->comboBox_dlg->currentText() == "Nuten")
        {
            for(uint i = Startzeile; i <= Startzeile+Menge-1; i++)
            {
                QString programmzeile = Wst->bearb().at(i);
                if(programmzeile.contains(BEARBART_NUT))
                {
                    nut n(programmzeile);
                    QString wert;
                    if(par == "Ausführbedingung")
                    {
                        wert = n.afb();
                        if(wert == wert_alt)
                        {
                            n.set_afb(wert_neu);
                            Wst->bearb_ptr()->edit(i, n.text());
                        }
                    }else if(par == "Breite")
                    {
                        wert = n.breite_qstring();
                        if(wert == wert_alt)
                        {
                            n.set_breite(wert_neu);
                            Wst->bearb_ptr()->edit(i, n.text());
                        }
                    }else if(par == "Tiefe")
                    {
                        wert = n.tiefe_qstring();
                        if(wert == wert_alt)
                        {
                            n.set_tiefe(wert_neu);
                            Wst->bearb_ptr()->edit(i, n.text());
                        }
                    }
                }
            }
        }else if(ui->comboBox_dlg->currentText() == "Rechteckstaschen")
        {
            for(uint i = Startzeile; i <= Startzeile+Menge-1; i++)
            {
                QString programmzeile = Wst->bearb().at(i);
                if(programmzeile.contains(BEARBART_RTA))
                {
                    rechtecktasche rta(programmzeile);
                    QString wert;
                    if(par == "Ausführbedingung")
                    {
                        wert = rta.afb();
                        if(wert == wert_alt)
                        {
                            rta.set_afb(wert_neu);
                            Wst->bearb_ptr()->edit(i, rta.text());
                        }
                    }else if(par == "Länge")
                    {
                        wert = rta.laenge_qstring();
                        if(wert == wert_alt)
                        {
                            rta.set_laenge(wert_neu);
                            Wst->bearb_ptr()->edit(i, rta.text());
                        }
                    }else if(par == "Breite")
                    {
                        wert = rta.breite_qstring();
                        if(wert == wert_alt)
                        {
                            rta.set_breite(wert_neu);
                            Wst->bearb_ptr()->edit(i, rta.text());
                        }
                    }else if(par == "Tiefe")
                    {
                        wert = rta.tiefe_qstring();
                        if(wert == wert_alt)
                        {
                            rta.set_tiefe(wert_neu);
                            Wst->bearb_ptr()->edit(i, rta.text());
                        }
                    }
                }
            }
        }else if(ui->comboBox_dlg->currentText() == "Fräseraufrufe")
        {
            for(uint i = Startzeile; i <= Startzeile+Menge-1; i++)
            {
                QString programmzeile = Wst->bearb().at(i);
                if(programmzeile.contains(BEARBART_FRAESERAUFRUF))
                {
                    fraeseraufruf fa(programmzeile);
                    QString wert;
                    if(par == "Anfahrtyp")
                    {
                        wert = fa.anfahrtyp();
                        if(wert == wert_alt)
                        {
                            fa.set_anfahrtyp(wert_neu);
                            Wst->bearb_ptr()->edit(i, fa.text());
                        }
                    }else if(par == "Abfahrtyp")
                    {
                        wert = fa.abfahrtyp();
                        if(wert == wert_alt)
                        {
                            fa.set_abfahrtyp(wert_neu);
                            Wst->bearb_ptr()->edit(i, fa.text());
                        }
                    }else if(par == "Anfahrwert")
                    {
                        wert = fa.anfahrweg_qstring();
                        if(wert == wert_alt)
                        {
                            fa.set_anfahrweg(wert_neu);
                            Wst->bearb_ptr()->edit(i, fa.text());
                        }
                    }else if(par == "Abfahrwert")
                    {
                        wert = fa.abfahrweg_qstring();
                        if(wert == wert_alt)
                        {
                            fa.set_abfahrweg(wert_neu);
                            Wst->bearb_ptr()->edit(i, fa.text());
                        }
                    }
                }
            }
        }

        werte_ermitteln();
    }else
    {
        QString msg;
        msg = "Sie haben keinen neunen Wert definiert!";
        QMessageBox mb;
        mb.setText(msg);
        mb.exec();
    }

}

void Dialog_schnellaenderung::werte_ermitteln()
{
    ui->comboBox_alt->clear(); //Jetige Werte des Paramters
    QString dlg = ui->comboBox_dlg->currentText();
    QString par = ui->comboBox_param->currentText();
    text_zw werte_alt;
    werte_alt.set_text("ALLE", '\n');

    if(ui->comboBox_dlg->currentText() == "ALLE")
    {
        for(uint i = Startzeile; i <= Startzeile+Menge-1; i++)
        {
            QString programmzeile = Wst->bearb().at(i);
            QString parname = "AFB=";
            QString wert;
            if(programmzeile.contains(BEARBART_BOHR))
            {
                bohrung bo(programmzeile);
                wert = bo.afb();
            }else if(programmzeile.contains(BEARBART_RTA))
            {
                rechtecktasche rta(programmzeile);
                wert = rta.afb();
            }else if(programmzeile.contains(BEARBART_NUT))
            {
                nut n(programmzeile);
                wert = n.afb();
            }
            bool vorhanden = false;
            for(uint ii=0; ii< werte_alt.count() ;ii++)
            {
                if(werte_alt.at(ii) == wert)
                {
                    vorhanden = true;
                    break; //for ii
                }
            }
            if(vorhanden == false && !wert.isEmpty())
            {
                werte_alt.add_hi(wert);
            }
        }
    }else if(ui->comboBox_dlg->currentText() == "Bohrungen")
    {
        for(uint i = Startzeile; i <= Startzeile+Menge-1; i++)
        {
            QString programmzeile = Wst->bearb().at(i);
            if(programmzeile.contains(BEARBART_BOHR))
            {
                bohrung bo(programmzeile);
                QString wert;
                if(par == "Ausführbedingung")
                {
                    wert = bo.afb();
                }else if(par == "Durchmesser")
                {
                    wert = bo.dm_qstring();
                }else if(par == "Tiefe")
                {
                    wert = bo.tiefe_qstring();
                }
                bool vorhanden = false;
                for(uint ii=0; ii< werte_alt.count() ;ii++)
                {
                    if(werte_alt.at(ii) == wert)
                    {
                        vorhanden = true;
                        break; //for ii
                    }
                }
                if(vorhanden == false && !wert.isEmpty())
                {
                    werte_alt.add_hi(wert);
                }
            }
        }
    }else if(ui->comboBox_dlg->currentText() == "Nuten")
    {
        for(uint i = Startzeile; i <= Startzeile+Menge-1; i++)
        {
            QString programmzeile = Wst->bearb().at(i);
            if(programmzeile.contains(BEARBART_NUT))
            {
                nut n(programmzeile);
                QString wert;
                if(par == "Ausführbedingung")
                {
                    wert = n.afb();
                }else if(par == "Breite")
                {
                    wert = n.breite_qstring();
                }else if(par == "Tiefe")
                {
                    wert = n.tiefe_qstring();
                }
                bool vorhanden = false;
                for(uint ii=0; ii< werte_alt.count() ;ii++)
                {
                    if(werte_alt.at(ii) == wert)
                    {
                        vorhanden = true;
                        break; //for ii
                    }
                }
                if(vorhanden == false && !wert.isEmpty())
                {
                    werte_alt.add_hi(wert);
                }

            }
        }
    }else if(ui->comboBox_dlg->currentText() == "Rechteckstaschen")
    {        
        for(uint i = Startzeile; i <= Startzeile+Menge-1; i++)
        {
            QString programmzeile = Wst->bearb().at(i);
            if(programmzeile.contains(BEARBART_RTA))
            {
                rechtecktasche rta(programmzeile);
                QString wert;
                if(par == "Ausführbedingung")
                {
                    wert = rta.afb();
                }else if(par == "Länge")
                {
                    wert = rta.laenge_qstring();
                }else if(par == "Breite")
                {
                    wert = rta.breite_qstring();
                }else if(par == "Tiefe")
                {
                    wert = rta.tiefe_qstring();
                }
                bool vorhanden = false;
                for(uint ii=0; ii< werte_alt.count() ;ii++)
                {
                    if(werte_alt.at(ii) == wert)
                    {
                        vorhanden = true;
                        break; //for ii
                    }
                }
                if(vorhanden == false && !wert.isEmpty())
                {
                    werte_alt.add_hi(wert);
                }

            }
        }
    }else if(ui->comboBox_dlg->currentText() == "Fräseraufrufe")
    {
        for(uint i = Startzeile; i <= Startzeile+Menge-1; i++)
        {
            QString programmzeile = Wst->bearb().at(i);
            if(programmzeile.contains(BEARBART_FRAESERAUFRUF))
            {
                fraeseraufruf fa(programmzeile);
                QString wert;
                if(par == "Anfahrtyp")
                {
                    wert = fa.anfahrtyp();
                }else if(par == "Abfahrtyp")
                {
                    wert = fa.abfahrtyp();
                }else if(par == "Anfahrwert")
                {
                    wert = fa.anfahrweg_qstring();
                }else if(par == "Abfahrwert")
                {
                    wert = fa.abfahrweg_qstring();
                }
                bool vorhanden = false;
                for(uint ii=0; ii< werte_alt.count() ;ii++)
                {
                    if(werte_alt.at(ii) == wert)
                    {
                        vorhanden = true;
                        break; //for ii
                    }
                }
                if(vorhanden == false && !wert.isEmpty())
                {
                    werte_alt.add_hi(wert);
                }

            }
        }
    }

    ui->comboBox_alt->clear();
    for(uint i = 0; i < werte_alt.count() ;i++)
    {
        ui->comboBox_alt->addItem(werte_alt.at(i));
    }
}



