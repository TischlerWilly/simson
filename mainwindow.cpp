#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->setWindowTitle("Simson 2026.01.12");
    PrgPfade.ordner_erstellen();
    setup();

    geo_text gt;
    kreis k;
    k.set_mipu(0,0,0);
    k.set_rad(20);
    gt.add_kreis(k);
    strecke s;
    s.set_stapu(-50,0,0);
    s.set_endpu(50,0,0);
    gt.add_strecke(s);
    s.drenen_um_mipu_2d(degToRad(90));
    gt.add_strecke(s);
    gt.zeilenvorschub();

    punkt3d p(500,500,0);
    gt.add_punkt(p);

    vorschaufenster.slot_aktualisieren(gt, 0);

    connect(&vorschaufenster, SIGNAL(sende_maus_pos(QPoint)),\
            this, SLOT(getMausPosXY(QPoint)));
    connect(&vorschaufenster, SIGNAL(sende_zeilennummer(uint, bool)),\
            this, SLOT(get_zeilennummer_bearb(uint, bool)));

    connect(this, SIGNAL(sendEinstellungPfade(einstellung)),\
            &dlg_Einstellung_pfade, SLOT(slot_einstellungen(einstellung)));
    connect(&dlg_Einstellung_pfade, SIGNAL(send_einstellungen(einstellung)),\
            this, SLOT(getEinstellung(einstellung)));

    connect(this, SIGNAL(sendMaschinen(maschinen)),\
            &dlg_Einstellung_maschinen, SLOT(slot_maschinen(maschinen)));
    connect(&dlg_Einstellung_maschinen, SIGNAL(send_maschinen(maschinen)),\
            this, SLOT(getMaschinen(maschinen)));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::setup()
{
    //Schauen ob alle Konfigurationsdateien vorhanden sind:
    bool inifile_gefunden           = false;    //user-Ordner

    QDir user_ordner(PrgPfade.path_user());
    QStringList ordnerinhalt;
    ordnerinhalt = user_ordner.entryList(QDir::Files);
    for(QStringList::iterator it = ordnerinhalt.begin() ; it!=ordnerinhalt.end() ; ++it)
    {
        QString name = *it;
        if(name.contains(PrgPfade.name_inifile()))
        {
            inifile_gefunden = true;
        }
    }

    //Einstellungen aus Konfigurationsdateien übernehmen wo möglich:
    if(inifile_gefunden == false)
    {
        QFile file(PrgPfade.path_inifile());
        if(!file.open(QIODevice::WriteOnly | QIODevice::Text))
        {
            QString tmp = "Fehler beim Dateizugriff!\n";
            tmp += PrgPfade.path_inifile();
            tmp += "\n";
            tmp += "in der Funktion setup";
            QMessageBox::warning(this,"Fehler",tmp,QMessageBox::Ok);
        }else
        {
            file.write(Einstellung.text().toLatin1());
        }
        file.close();
    }else
    {
        QFile file(PrgPfade.path_inifile());
        if(!file.open(QIODevice::ReadOnly | QIODevice::Text))
        {
            QString tmp = "Fehler beim Dateizugriff!\n";
            tmp += PrgPfade.path_inifile();
            tmp += "\n";
            tmp += "in der Funktion setup";
            QMessageBox::warning(this,"Fehler",tmp,QMessageBox::Ok);
        }else
        {
            Einstellung.set_text(file.readAll());
        }
        file.close();
    }

    //Werkzeug einlesen:
    maschinen_einlesen();
}
void MainWindow::schreibe_ini()
{
    QFile file(PrgPfade.path_inifile());
    if(!file.open(QIODevice::WriteOnly | QIODevice::Text))
    {
        QString tmp = "Fehler beim Dateizugriff!\n";
        tmp += PrgPfade.path_inifile();
        tmp += "\n";
        tmp += "in der Funktion schreibe_ini";
        QMessageBox::warning(this,"Fehler",tmp,QMessageBox::Ok);
    }else
    {
        file.write(Einstellung.text().toLatin1());
    }
    file.close();
}
void MainWindow::maschinen_einlesen()
{
    QDir dir(PrgPfade.path_wkz_dir());

    // Nur Verzeichnisse, keine "." und ".."
    QStringList subdirs = dir.entryList(QDir::Dirs | QDir::NoDotAndDotDot,\
                                        QDir::Name );

    Maschinen.clear();
    ui->comboBox_maschinen->clear();

    for (int i = 0; i < subdirs.size(); ++i)
    {
        QString name = subdirs.at(i);
        ui->comboBox_maschinen->addItem(name);

        maschine m;
        m.set_name(name);
        QFile file(PrgPfade.path_masch_ini(name));
        if(file.open(QIODevice::ReadOnly | QIODevice::Text))
        {
            m.set_text(file.readAll());

            QFile file_wkz(PrgPfade.path_wkz_mag(name));
            if(file_wkz.open(QIODevice::ReadOnly | QIODevice::Text))
            {
                wkz_magazin wkz;
                wkz.set_text(file_wkz.readAll());
                m.set_wkzmag(wkz);
            }
            file_wkz.close();
        }
        file.close();
        Maschinen.neu(m);
    }
}
void MainWindow::schreibe_maschinen()
{
    //Maschinenordner erstellen:
    for(uint i=0; i<Maschinen.namen_tz().count();i++)
    {
        QDir dir(PrgPfade.path_masch_dir(Maschinen.namen_tz().at(i)));
        dir.mkpath(dir.path());
    }
    //Maschineneinstellung speichern:
    for(uint i=0; i<Maschinen.namen_tz().count();i++)
    {
        QFile file(PrgPfade.path_masch_ini(Maschinen.namen_tz().at(i)));
        if(!file.open(QIODevice::WriteOnly | QIODevice::Text))
        {
            QString tmp = "Fehler beim Dateizugriff!\n";
            tmp += PrgPfade.path_masch_ini(Maschinen.namen_tz().at(i));
            tmp += "\n";
            tmp += "in der Funktion schreibe_maschinen-Einstellung";
            QMessageBox::warning(this,"Fehler",tmp,QMessageBox::Ok);
        }else
        {
            file.write(Maschinen.masch(i)->text().toLatin1());
        }
        file.close();
    }
    //Werkzeug dieser Maschine speichern:
    for(uint i=0; i<Maschinen.namen_tz().count();i++)
    {
        QFile file(PrgPfade.path_wkz_mag(Maschinen.namen_tz().at(i)));
        if(!file.open(QIODevice::WriteOnly | QIODevice::Text))
        {
            QString tmp = "Fehler beim Dateizugriff!\n";
            tmp += PrgPfade.path_wkz_mag(Maschinen.namen_tz().at(i));
            tmp += "\n";
            tmp += "in der Funktion schreibe_maschinen";
            QMessageBox::warning(this,"Fehler",tmp,QMessageBox::Ok);
        }else
        {
            file.write(Maschinen.masch(i)->wkzmag().text().toLatin1());
        }
        file.close();
    }
}
//------------------------------------------------------
//Grafik und UI:
void MainWindow::resizeEvent(QResizeEvent *event)
{
    //---Vorschaufenster:
    vorschaufenster.setParent(this);
    vorschaufenster.move(5,25);
    vorschaufenster.setFixedWidth(this->width()-270);
    vorschaufenster.setFixedHeight(this->height()-10);
    vorschaufenster.slot_aktualisieren();

    //rechter Bereich
    int x = vorschaufenster.pos().rx()+vorschaufenster.width()+5;
    ui->btn_import->move(x, 5);

    int h = (this->height()-ui->listWidget_dateien->pos().y()-60)/2 - 5;
    int b = this->width() - x - 5;
    ui->listWidget_dateien->move(x, ui->btn_import->pos().y()+ui->btn_import->height()+5);
    ui->listWidget_dateien->setFixedHeight(h);
    ui->listWidget_dateien->setFixedWidth(b);

    ui->listWidget_bearb->move(x, ui->listWidget_dateien->pos().y()+ h + 5);
    ui->listWidget_bearb->setFixedHeight(h);
    ui->listWidget_bearb->setFixedWidth(b);

    int y = ui->listWidget_bearb->pos().y() + h + 2;
    ui->label_mauspos->move(x, y);
    b = b/2-5;
    ui->label_mauspos->setFixedWidth(b);
    ui->label_mauspos->setFixedHeight(20);

    ui->comboBox_maschinen->move(x+b+10, y);
    ui->comboBox_maschinen->setFixedWidth(b);
    ui->comboBox_maschinen->setFixedHeight(20);
}
void MainWindow::getMausPosXY(QPoint p)
{
    QString x = int_to_qstring(p.x());
    QString y = int_to_qstring(p.y());
    QString msg;
    msg  = "X: ";
    msg += x;
    msg += " / Y: ";
    msg += y;
    ui->label_mauspos->setText(msg);
}
void MainWindow::get_zeilennummer_bearb(uint nr, bool bearbeiten)
{
    if(nr <= ui->listWidget_bearb->count())
    {
        ui->listWidget_bearb->setCurrentRow(nr);
    }
}
void MainWindow::update_listwidget_bearb(werkstueck *w)
{
    int currentRow = ui->listWidget_bearb->currentRow();
    ui->listWidget_bearb->clear();
    //Programmkopf als erste Zeile einfügen:
    text_zw pkopf;
    pkopf.set_trenz(TRENNZ_BEARB_PARAM);
    QString param;
    param  = "L=";
    param += w->laenge_qstring();
    pkopf.add_hi(param);
    param  = "B=";
    param += w->breite_qstring();
    pkopf.add_hi(param);
    param  = "D=";
    param += w->dicke_qstring();
    pkopf.add_hi(param);
    ui->listWidget_bearb->addItem(pkopf.text());
    //Bearbeitungen ab 2. Zeile einfügen:
    text_zw *tmp_bearb = w->bearb_ptr();
    for(uint i=0; i<tmp_bearb->count() ;i++)
    {
        QString bearb = tmp_bearb->at(i);
        text_zw zeile;
        zeile.set_text(bearb,TRENNZ_BEARB_PARAM);
        QColor farbe;
        farbe.setRgb(255,255,255);
        int deckkraft = 160;
        QString bezug = zeile.at(1);
        if(zeile.at(0) == BEARBART_BOHR)
        {
            bearb = bohr_zu_prgzei(zeile.text());
            if(bezug == WST_BEZUG_OBSEI)
            {
                farbe.setRgb(0,240,240,deckkraft); //Hellblau
            }else if(bezug == WST_BEZUG_UNSEI)
            {
                farbe.setRgb(255,0,128,deckkraft);//Rose
            }else
            {
                farbe.setRgb(185,122,87,deckkraft);//braun
            }
        }else if(zeile.at(0) == BEARBART_BOHRRASTER)
        {
            bearb = bohrRaster_zu_prgzei(zeile.text());
        }else if(zeile.at(0) == BEARBART_NUT)
        {
            bearb = nut_zu_prgzei(zeile.text());
            farbe.setRgb(145,145,255,deckkraft);//helles lila
        }else if(zeile.at(0) == BEARBART_RTA)
        {
            bearb = rta_zu_prgzei(zeile.text());
            if(bezug == WST_BEZUG_OBSEI || bezug == WST_BEZUG_UNSEI)
            {
                farbe = Qt::darkGray;
                farbe.setAlpha(deckkraft);
            }else
            {
                farbe = Qt::green;
                farbe.setAlpha(deckkraft);
            }
        }else if(zeile.at(0) == BEARBART_FRAESERAUFRUF)
        {
            bearb = fauf_zu_prgzei(zeile.text());
            farbe.setRgb(255,128,0,deckkraft);//orange
        }else if(zeile.at(0) == BEARBART_FRAESERGERADE)
        {
            bearb = fgerade_zu_prgzei(zeile.text());
            farbe.setRgb(255,155,106,deckkraft);//helles orange
        }else if(zeile.at(0) == BEARBART_FRAESERBOGEN)
        {
            bearb = fbogen_zu_prgzei(zeile.text());
            farbe.setRgb(255,155,106,deckkraft);//helles orange
        }
        ui->listWidget_bearb->addItem(bearb);
        ui->listWidget_bearb->item(i+1)->setBackground(farbe);
    }
    ui->listWidget_bearb->addItem("...");
    if(currentRow < ui->listWidget_bearb->count())
    {
        ui->listWidget_bearb->setCurrentRow(currentRow);
    }
}

//------------------------------------------------------
//Einstellungen:
void MainWindow::getEinstellung(einstellung e)
{
    Einstellung = e;
    schreibe_ini();
}
void MainWindow::getMaschinen(maschinen m)
{
    Maschinen = m;
    schreibe_maschinen();

    ui->comboBox_maschinen->clear();
    for(uint i=0; i<Maschinen.anzahl() ;i++)
    {
        ui->comboBox_maschinen->addItem(Maschinen.masch(i)->name());
    }
}
void MainWindow::on_actionPfade_triggered()
{
    emit sendEinstellungPfade(Einstellung);
}
void MainWindow::on_actionCNC_Maschinen_triggered()
{
    emit sendMaschinen(Maschinen);
}

//------------------------------------------------------
//Dateien/Werkstücke/Bearbeitungen:
void MainWindow::on_actionNeu_triggered()
{
    QString name = "Unbekannt";
    if(!Wste.ist_bekannt(name))
    {
        werkstueck w;
        w.set_name(name);
        w.set_laenge(500);
        w.set_breite(300);
        w.set_dicke(19);
        bohrung bo;
        bo.set_dm(25);
        bo.set_x(100);
        bo.set_y(50);
        bo.set_z(19);
        bo.set_tiefe(10);
        bo.set_bezug(WST_BEZUG_OBSEI);
        text_zw bearb;
        bearb.add_hi(bo.text());
        w.set_bearb(bearb);

        Wste.neu(w);

        ui->listWidget_dateien->addItem(name);
    }else
    {
        QString msg;
        msg += "Bitte benenne zuerst das Werkstück \"";
        msg += name;
        msg += "\" um. \n";
        msg += "Danach kannst du ein neues Werkstück erstellen.";
        QMessageBox mb;
        mb.setText(msg);
        mb.setWindowTitle("Neue Datei/neues Werkstück erstellen");
        mb.exec();
    }
}
void MainWindow::on_btn_import_clicked()
{
    QMessageBox mb;
    mb.setText("Diese Funktion ist noch in Arbeit");
    mb.exec();
}
void MainWindow::on_action_oeffnen_triggered()
{
    //QString pfad_lokal = Einstellung.verzeichnis_ziel_lokal();
    //pfad_lokal += QDir::separator();
    //pfad_lokal += "eigen";
    //pfad_lokal.replace("\\", QDir::separator());//linux style
    //pfad_lokal.replace("/", QDir::separator());//windows style
    //if(Pfad_letzte_geoeffnete_ggf_datei.isEmpty())
    //{
    //    Pfad_letzte_geoeffnete_ggf_datei = pfad_lokal;
    //}
    QString Pfad_letzte_geoeffnete_ggf_datei;
    QStringList pfade = QFileDialog::getOpenFileNames(this, tr("Wähle Datei(en)"), \
                                                      Pfad_letzte_geoeffnete_ggf_datei, tr("ewx Dateien (*.ewx)"));
    for(int i=0; i<pfade.size() ;i++)
    {
        /*
        QString aktueller_pfad = pfade.at(i);
        QFile datei(aktueller_pfad);
        QFileInfo finfo(datei);
        Pfad_letzte_geoeffnete_ggf_datei = finfo.path();
        if(!datei.open(QIODevice::ReadOnly | QIODevice::Text))
        {
            QString tmp = "Fehler beim Dateizugriff!\n";
            tmp += aktueller_pfad;
            tmp += "\n";
            tmp += "in der Funktion on_action_oeffnen_triggered";
            QMessageBox::warning(this,"Fehler",tmp,QMessageBox::Ok);
        }else
        {
            QString inhalt = datei.readAll();
            QFileInfo info;
            info.setFile(aktueller_pfad);
            QString wstname = info.fileName();
            QString dateiendung = ".ppf";
            wstname = wstname.left(wstname.length()-dateiendung.length());
            if(wste.import_ppf(wstname, inhalt) == false)
            {
                QString msg;
                msg  = "Die Datei \"";
                msg += wstname;
                msg += "\" konnte nich geöffnet werden, weil bereits ein Bauteil mit diesem Namen in der ";
                msg += "Arbeitsliste vorhanden ist.";
                QMessageBox mb;
                mb.setWindowTitle("Datei öffnen");
                mb.setText(msg);
                mb.exec();
            }
        }
        */
    }
    /*
    werkstueck w;//leeres wst
    emit sendVorschauAktualisieren(w, 0);//leeres wst an vorschau schicken
    update_listwidget_wste();
    emit signal_exporte(wste.namen_tz());
    */
}
void MainWindow::on_listWidget_dateien_currentRowChanged(int currentRow)
{
    uint row = currentRow;
    if(Wste.wst(row))
    {
        werkstueck *w = Wste.wst(row);

        wkz_magazin wkz;
        vorschaufenster.slot_aktualisieren(w->geo(wkz), 0);

        update_listwidget_bearb(w);
    }else
    {
        QString msg;
        msg = "currentRow = ";
        msg += int_to_qstring(currentRow);
        msg += "\n row = ";
        msg += int_to_qstring(row);
        msg += "\nAnz wst = ";
        msg += int_to_qstring(Wste.anzahl());
        QMessageBox mb;
        mb.setText(msg);
        mb.setWindowTitle("Fehler in Funktion MainWindow::on_listWidget_dateien_currentRowChanged(int currentRow)");
        mb.exec();
    }
}
void MainWindow::on_listWidget_bearb_currentRowChanged(int currentRow)
{
    vorschaufenster.slot_aktives_Element_einfaerben(currentRow);
}
void MainWindow::on_actionUmbenennen_triggered()
{
    if(ui->listWidget_dateien->selectedItems().count())
    {
        QString name = ui->listWidget_dateien->currentItem()->text();
        if(!name.isEmpty())
        {
            bool ok;
            QString fenstertitel = "umbenennen";
            QString hinweistext  = "Neuer Name:";
            QString vorgebewert  = name;
            QString neuer_name = QInputDialog::getText(this, fenstertitel,
                                                       hinweistext, QLineEdit::Normal,
                                                       name, &ok);
            if (ok && !neuer_name.isEmpty())
            {
                if(Wste.ist_bekannt(neuer_name))
                {
                    QString msg;
                    msg  = "Das Bauteil kann nicht umbenannt werden.";
                    msg += "\n";
                    msg += "Es gibt bereits ein Bauteil mit diesem Namen ";
                    msg += neuer_name;
                    msg += ".";
                    QMessageBox mb;
                    mb.setText(msg);
                    mb.exec();
                }else
                {
                    int row = ui->listWidget_dateien->currentRow();
                    ui->listWidget_dateien->item(row)->setText(neuer_name);
                    Wste.set_name(row, neuer_name);//Namensliste in wste
                    Wste.wst(row)->set_name(neuer_name);//name des konkreten wst
                    on_listWidget_dateien_currentRowChanged(ui->listWidget_dateien->currentRow());
                }
            }
        }else
        {
            QString msg;
            msg = "Bauteil hat keinen Namen!";
            QMessageBox mb;
            mb.setText(msg);
            mb.setWindowTitle("Werkstück umbenennen");
            mb.exec();
        }
    }else
    {
        QString msg;
        msg = "Es ist kein Bauteil ausgewählt!";
        QMessageBox mb;
        mb.setText(msg);
        mb.setWindowTitle("Werkstück umbenennen");
        mb.exec();
    }
}
void MainWindow::on_listWidget_bearb_itemDoubleClicked(QListWidgetItem *item)
{
    int index = ui->listWidget_bearb->currentRow();
    zeile_bearb_bearbeiten(index);
}
void MainWindow::zeile_bearb_bearbeiten(int zeile)
{
    if(ui->listWidget_dateien->currentRow() < 0)
    {
        QMessageBox mb;
        mb.setText("Es ist kein Werkstück ausgewählt!");
        mb.setWindowTitle("Bearbeitung editieren");
        mb.exec();
        return;
    }

    int index_wst = ui->listWidget_dateien->currentRow();
    wkz_magazin wkz;
    if(ui->comboBox_maschinen->currentIndex() >= 0)
    {
        QString masch_bez = ui->comboBox_maschinen->currentText();
        int index = Maschinen.get_index(masch_bez);
        wkz = Maschinen.masch(index)->wkzmag();
    }

    if(zeile == 0)
    {
        double letzte_wst_l = Wste.wst(index_wst)->laenge();
        double letzte_wst_b = Wste.wst(index_wst)->breite();
        Dialog_bearb_pkopf dlg;
        dlg.setModal(true);
        dlg.set_data(Wste.wst(index_wst));
        dlg.exec();
        text_zw bearb_alt = Wste.wst(index_wst)->bearb();
        text_zw bearb_neu;
        for (uint i = 0; i<bearb_alt.count() ; i++)
        {
            if(i==0)
            {
                double ax = 0, ay = 0, az = 0; //0,0,0 verschiebt die bearb auf die Wst-kanten
                double l_alt = letzte_wst_l;
                double l_neu = Wste.wst(index_wst)->laenge();
                double b_alt = letzte_wst_b;
                double b_neu = Wste.wst(index_wst)->breite();
                QString bearb = verschiebe_bearb_einen(bearb_alt.at(i), \
                                                       ax, ay, az, \
                                                       l_alt, l_neu, b_alt, b_neu);
                bearb_neu.set_text(bearb, bearb_alt.trennz());
            }else
            {
                double ax = 0, ay = 0, az = 0; //0,0,0 verschiebt die bearb auf die Wst-kanten
                double l_alt = letzte_wst_l;
                double l_neu = Wste.wst(index_wst)->laenge();
                double b_alt = letzte_wst_b;
                double b_neu = Wste.wst(index_wst)->breite();
                QString bearb = verschiebe_bearb_einen(bearb_alt.at(i), \
                                                       ax, ay, az, \
                                                       l_alt, l_neu, b_alt, b_neu);
                bearb_neu.add_hi(bearb);
                //0,0,0 verschiebt die bearb auf die Wst-kanten
            }
        }
        Wste.wst(index_wst)->set_bearb(bearb_neu);
        Wste.wst(index_wst)->unredo_neu();
        update_listwidget_bearb(Wste.wst(index_wst));
        vorschaufenster.slot_aktualisieren(Wste.wst(index_wst)->geo(wkz), index_wst);
        return;
    }

    //Zeile Auslesen:
    text_zw bearb;
    bearb.set_text(Wste.wst(index_wst)->bearb_ptr()->at(zeile-1),TRENNZ_BEARB_PARAM);
    //Dialogfenster aufrufen:
    if(bearb.at(0) == BEARBART_RTA)
    {
        Dialog_bearb_rta dlg;
        dlg.setModal(true);
        connect(&dlg, SIGNAL(signal_rta(rechtecktasche)), this, SLOT(slot_rta(rechtecktasche)));
        dlg.set_data(bearb.text(), Wste.wst(index_wst), wkz.magazin());
        dlg.exec();
    }else if(bearb.at(0) == BEARBART_BOHR)
    {
        Dialog_bearb_bohrung dlg;
        dlg.setModal(true);
        connect(&dlg, SIGNAL(signal_bo(bohrung)), this, SLOT(slot_bo(bohrung)));
        dlg.set_data(bearb.text(), Wste.wst(index_wst), wkz.magazin());
        dlg.exec();
    }else if(bearb.at(0) == BEARBART_NUT)
    {
        Dialog_bearb_nut dlg;
        dlg.setModal(true);
        connect(&dlg, SIGNAL(signal_nut(nut)), this, SLOT(slot_nut(nut)));
        dlg.set_data(bearb.text(), Wste.wst(index_wst));
        dlg.exec();
    }else if(bearb.at(0) == BEARBART_FRAESERAUFRUF)
    {
        Dialog_bearb_faufruf dlg;
        dlg.setModal(true);
        connect(&dlg, SIGNAL(signal_faufruf(fraeseraufruf)), this, SLOT(slot_faufruf(fraeseraufruf)));
        dlg.set_data(bearb.text(), Wste.wst(index_wst), wkz.magazin());
        dlg.exec();
    }else if(bearb.at(0) == BEARBART_FRAESERGERADE)
    {
        Dialog_bearb_fgerade dlg;
        dlg.setModal(true);
        connect(&dlg, SIGNAL(signal_fgerade(fraesergerade)), this, SLOT(slot_fgerade(fraesergerade)));
        dlg.set_data(bearb.text(), Wste.wst(index_wst));
        dlg.exec();
    }else if(bearb.at(0) == BEARBART_FRAESERBOGEN)
    {
        Dialog_bearb_fbogen dlg;
        dlg.setModal(true);
        connect(&dlg, SIGNAL(signal_fbogen(fraeserbogen)), this, SLOT(slot_fbogen(fraeserbogen)));
        dlg.set_data(bearb.text(), Wste.wst(index_wst));
        dlg.exec();
    }
}
QString MainWindow::verschiebe_bearb_einen(QString bearb, double ax, double ay, double az,\
                                           double wst_l_alt, double wst_l_neu,\
                                           double wst_b_alt, double wst_b_neu  )
{
    //bei ay == 0 und ay == 0 und az == 0 sorgt
    //diese Funktion dafür dass die Bearbeitungen die sich z.B. auf der Hirnkante
    //des Werkstückes befinden mitwandern wenn das Werkstück in der Größe geändert wird.
    text_zw tz;
    tz.set_text(bearb, TRENNZ_BEARB_PARAM);
    if(tz.at(0) == BEARBART_BOHR)
    {
        bohrung bo;
        bo.set_text(bearb);
        if(bo.bezug() == WST_BEZUG_OBSEI || bo.bezug() == WST_BEZUG_UNSEI)
        {
            bo.set_x(bo.x()+ax);
            bo.set_y(bo.y()+ay);
        }else if(bo.bezug() == WST_BEZUG_LI)
        {
            if(bo.x() != 0)
            {
                bo.set_x(bo.x()+ax);
            }
            bo.set_y(bo.y()+ay);
            bo.set_z(bo.z()+az);
        }else if(bo.bezug() == WST_BEZUG_RE)
        {
            if(bo.x() == wst_l_alt)
            {
                bo.set_x(wst_l_neu);
            }else
            {
                bo.set_x(bo.x()+ax);
            }
            bo.set_y(bo.y()+ay);
            bo.set_z(bo.z()+az);
        }else if(bo.bezug() == WST_BEZUG_VO)
        {
            bo.set_x(bo.x()+ax);
            if(bo.y() != 0)
            {
                bo.set_y(bo.y()+ay);
            }
            bo.set_z(bo.z()+az);
        }else if(bo.bezug() == WST_BEZUG_HI)
        {
            bo.set_x(bo.x()+ax);
            if(bo.y() == wst_b_alt)
            {
                bo.set_y(wst_b_neu);
            }else
            {
                bo.set_y(bo.y()+ay);
            }
            bo.set_z(bo.z()+az);
        }
        bearb = bo.text();
    }else if(tz.at(0) == BEARBART_RTA)
    {
        rechtecktasche rt;
        rt.set_text(bearb);
        if(rt.bezug() == WST_BEZUG_OBSEI || rt.bezug() == WST_BEZUG_UNSEI)
        {
            rt.set_x(rt.x()+ax);
            rt.set_y(rt.y()+ay);
        }else if(rt.bezug() == WST_BEZUG_LI)
        {
            rt.set_x(0);
            rt.set_y(rt.y()+ay);
            rt.set_z(rt.z()+az);
        }else if(rt.bezug() == WST_BEZUG_RE)
        {
            rt.set_x(wst_l_neu);
            rt.set_y(rt.y()+ay);
            rt.set_z(rt.z()+az);
        }else if(rt.bezug() == WST_BEZUG_VO)
        {
            rt.set_x(rt.x()+ax);
            rt.set_y(0);
            rt.set_z(rt.z()+az);
        }else if(rt.bezug() == WST_BEZUG_HI)
        {
            rt.set_x(rt.x()+ax);
            rt.set_y(wst_b_neu);
            rt.set_z(rt.z()+az);
        }
        bearb = rt.text();
    }if(tz.at(0) == BEARBART_NUT)
    {
        nut nu;
        nu.set_text(bearb);
        if(nu.bezug() == WST_BEZUG_OBSEI || nu.bezug() == WST_BEZUG_UNSEI)
        {
            nu.set_xs(nu.xs()+ax);
            nu.set_xe(nu.xe()+ax);
            nu.set_ys(nu.ys()+ay);
            nu.set_ye(nu.ye()+ay);
        }else if(nu.bezug() == WST_BEZUG_LI)
        {
            nu.set_xs(0);
            nu.set_xe(0);
            nu.set_ys(nu.ys()+ay);
            nu.set_ye(nu.ye()+ay);
            nu.set_zs(nu.zs()+az);
            nu.set_ze(nu.ze()+az);
        }else if(nu.bezug() == WST_BEZUG_RE)
        {
            nu.set_xs(wst_l_neu);
            nu.set_xe(wst_l_neu);
            nu.set_ys(nu.ys()+ay);
            nu.set_ye(nu.ye()+ay);
            nu.set_zs(nu.zs()+az);
            nu.set_ze(nu.ze()+az);
        }else if(nu.bezug() == WST_BEZUG_VO)
        {
            nu.set_xs(nu.xs()+ax);
            nu.set_xe(nu.xe()+ax);
            nu.set_ys(0);
            nu.set_ye(0);
            nu.set_zs(nu.zs()+az);
            nu.set_ze(nu.ze()+az);
        }else if(nu.bezug() == WST_BEZUG_HI)
        {
            nu.set_xs(nu.xs()+ax);
            nu.set_xe(nu.xe()+ax);
            nu.set_ys(wst_b_neu);
            nu.set_ye(wst_b_neu);
            nu.set_zs(nu.zs()+az);
            nu.set_ze(nu.ze()+az);
        }
        bearb = nu.text();
    }if(tz.at(0) == BEARBART_FRAESERAUFRUF)
    {
        fraeseraufruf fa;
        fa.set_text(bearb);
        fa.set_x(fa.x()+ax);
        fa.set_y(fa.y()+ay);
        bearb = fa.text();
    }if(tz.at(0) == BEARBART_FRAESERGERADE)
    {
        fraesergerade fg;
        fg.set_text(bearb);
        fg.set_xs(fg.xs()+ax);
        fg.set_xe(fg.xe()+ax);
        fg.set_ys(fg.ys()+ay);
        fg.set_ye(fg.ye()+ay);
        bearb = fg.text();
    }if(tz.at(0) == BEARBART_FRAESERBOGEN)
    {
        fraeserbogen fb;
        fb.set_text(bearb);
        fb.bog_ptr()->verschieben_um(ax, ay);
        bearb = fb.text();
    }
    return bearb;
}
void MainWindow::zeile_aendern(int index_bearb, QString bearb, bool unredor_verwenden)
{
    //index_bearb ist der index der Bearbeitung
    //bearb ist eine Zeile der Bearbeitugen
    int index_dat = ui->listWidget_dateien->currentRow();

    text_zw bearbeitungen = Wste.wst(index_dat)->bearb();
    bearbeitungen.edit(index_bearb, bearb);
    Wste.wst(index_dat)->bearb_ptr()->edit(index_bearb, bearbeitungen.at(index_bearb));
    if(unredor_verwenden == true)
    {
        Wste.wst(index_dat)->unredo_neu();
    }
    wkz_magazin wkz;
    if(ui->comboBox_maschinen->currentIndex() >= 0)
    {
        QString masch_bez = ui->comboBox_maschinen->currentText();
        int index = Maschinen.get_index(masch_bez);
        wkz = Maschinen.masch(index)->wkzmag();
    }
    update_listwidget_bearb(Wste.wst(index_dat));
    vorschaufenster.slot_aktualisieren(Wste.wst(index_dat)->geo(wkz), index_bearb+1);
}
void MainWindow::slot_rta(rechtecktasche rta)
{
    int index = ui->listWidget_bearb->currentRow()-1;//Index-1 weil 1. Zeile WST-Maße sind
    QString bearb = rta.text();
    ui->listWidget_bearb->item(index)->setText(rta_zu_prgzei(bearb));
    zeile_aendern(index, bearb, true);
}
void MainWindow::slot_bo(bohrung bo)
{
    int index = ui->listWidget_bearb->currentRow()-1;//Index-1 weil 1. Zeile WST-Maße sind
    QString bearb = bo.text();
    ui->listWidget_bearb->item(index)->setText(bohr_zu_prgzei(bearb));
    zeile_aendern(index, bearb, true);
}
void MainWindow::slot_nut(nut nu)
{
    int index = ui->listWidget_bearb->currentRow()-1;//Index-1 weil 1. Zeile WST-Maße sind
    QString bearb = nu.text();
    ui->listWidget_bearb->item(index)->setText(nut_zu_prgzei(bearb));
    zeile_aendern(index, bearb, true);
}
void MainWindow::slot_faufruf(fraeseraufruf fa)
{
    int index = ui->listWidget_bearb->currentRow()-1;//Index-1 weil 1. Zeile WST-Maße sind
    QString bearb = fa.text();
    ui->listWidget_bearb->item(index)->setText(fauf_zu_prgzei(bearb));
    text_zw bearbeitungen = Wste.wst(index)->bearb();

    fraeseraufruf fa_alt;
    fa_alt.set_text(bearbeitungen.at(index));
    if(fa_alt.text() != fa.text())
    {
        //Bezug mit ändern
        //Frästiefe ändern
        //Z-Pos ändern
        //XY-Pos der direkten Folgezeile mit ändern
        double tiefe_neu = fa.tiefe();
        double pos_z = fa.pos().z();
        for(uint i=index+1; i<bearbeitungen.count() ;i++)
        {
            text_zw bearb_ff;
            bearb_ff.set_text(bearbeitungen.at(i),TRENNZ_BEARB_PARAM);
            if(bearb_ff.at(0) == BEARBART_FRAESERGERADE)
            {
                fraesergerade fg(bearb_ff.text());
                fg.set_bezug(fa.bezug());
                fg.set_tiSta(tiefe_neu);
                fg.set_tiEnd(tiefe_neu);
                fg.set_zs(pos_z);
                fg.set_ze(pos_z);
                if(i == index+1)//direkte folgezeile vom Fräseraufruf
                {
                    fg.set_xs(fa.pos().x());
                    fg.set_ys(fa.pos().y());
                }
                zeile_aendern(i, fg.text(), false);
            }else if(bearb_ff.at(0) == BEARBART_FRAESERBOGEN)
            {
                fraeserbogen fb(bearb_ff.text());
                if(fa_alt.bezug() != fa.bezug())
                {
                    fb.bog_ptr()->richtung_unkehren();
                    fb.set_bezug(fa.bezug());
                }
                fb.set_tiSta(tiefe_neu);
                fb.set_tiEnd(tiefe_neu);
                fb.set_tiSta(pos_z);
                fb.set_tiEnd(pos_z);
                if(i == index+1)//direkte folgezeile vom Fräseraufruf
                {
                    fb.bog_ptr()->versetze_spu(fa.pos());
                }
                zeile_aendern(i, fb.text(), false);
            }else
            {
                break;//for
            }
        }
    }
    zeile_aendern(index, bearb, true);
}
void MainWindow::slot_fgerade(fraesergerade fg)
{
    int index = ui->listWidget_bearb->currentRow()-1;//Index-1 weil 1. Zeile WST-Maße sind
    QString bearb = fg.text();
    ui->listWidget_bearb->item(index)->setText(fgerade_zu_prgzei(bearb));
    text_zw bearbeitungen = Wste.wst(index)->bearb();
    //Zeile davor mit ändern?:
    if(index-1 >=0)
    {
        text_zw bearb_vor;
        bearb_vor.set_text(bearbeitungen.at(index-1),TRENNZ_BEARB_PARAM);
        if(bearb_vor.at(0) == BEARBART_FRAESERAUFRUF)
        {
            fraeseraufruf fa_vor(bearb_vor.text());
            fa_vor.set_pos(fg.sp());
            fa_vor.set_tiefe(fg.tiSta());
            zeile_aendern(index-1, fa_vor.text(), false);
        }else if(bearb_vor.at(0) == BEARBART_FRAESERGERADE)
        {
            fraesergerade fg_vor(bearb_vor.text());
            fg_vor.set_endpunkt(fg.sp());
            fg_vor.set_tiEnd(fg.tiSta());
            zeile_aendern(index-1, fg_vor.text(), false);
        }else if(bearb_vor.at(0) == BEARBART_FRAESERBOGEN)
        {
            fraeserbogen fb_vor(bearb_vor.text());
            fb_vor.bog_ptr()->versetze_epu(fg.sp());
            fb_vor.set_tiEnd(fg.tiSta());
            zeile_aendern(index-1, fb_vor.text(), false);
        }
    }
    //Folgezeile mit ändern?:
    if(index+1 < bearbeitungen.count())
    {
        text_zw bearb_nach;
        bearb_nach.set_text(bearbeitungen.at(index+1),TRENNZ_BEARB_PARAM);
        if(bearb_nach.at(0) == BEARBART_FRAESERGERADE)
        {
            fraesergerade fg_nach(bearb_nach.text());
            fg_nach.set_startpunkt(fg.ep());
            fg_nach.set_tiSta(fg.tiEnd());
            zeile_aendern(index+1, fg_nach.text(), false);
        }else if(bearb_nach.at(0) == BEARBART_FRAESERBOGEN)
        {
            fraeserbogen fb_nach(bearb_nach.text());
            fb_nach.bog_ptr()->versetze_spu(fg.ep());
            fb_nach.set_tiSta(fg.tiEnd());
            zeile_aendern(index+1, fb_nach.text(), false);
        }
    }
    zeile_aendern(index, bearb, true);
}
void MainWindow::slot_fbogen(fraeserbogen fb)
{
    int index = ui->listWidget_bearb->currentRow()-1;//Index-1 weil 1. Zeile WST-Maße sind
    QString bearb = fb.text();
    ui->listWidget_bearb->item(index)->setText(fbogen_zu_prgzei(bearb));
    text_zw bearbeitungen = Wste.wst(index)->bearb();
    //Zeile davor mit ändern?:
    if(index-1 >=0)
    {
        text_zw bearb_vor;
        bearb_vor.set_text(bearbeitungen.at(index-1),TRENNZ_BEARB_PARAM);
        if(bearb_vor.at(0) == BEARBART_FRAESERAUFRUF)
        {
            fraeseraufruf fa_vor(bearb_vor.text());
            fa_vor.set_pos(fb.sp());
            fa_vor.set_tiefe(fb.tiSta());
            zeile_aendern(index-1, fa_vor.text(), false);
        }else if(bearb_vor.at(0) == BEARBART_FRAESERGERADE)
        {
            fraesergerade fg_vor(bearb_vor.text());
            fg_vor.set_endpunkt(fb.sp());
            fg_vor.set_tiEnd(fb.tiSta());
            zeile_aendern(index-1, fg_vor.text(), false);
        }else if(bearb_vor.at(0) == BEARBART_FRAESERBOGEN)
        {
            fraeserbogen fb_vor(bearb_vor.text());
            fb_vor.bog_ptr()->versetze_epu(fb.sp());
            fb_vor.set_tiEnd(fb.tiSta());
            zeile_aendern(index-1, fb_vor.text(), false);
        }
    }
    //Folgezeile mit ändern?:
    if(index+1 < bearbeitungen.count())
    {
        text_zw bearb_nach;
        bearb_nach.set_text(bearbeitungen.at(index+1),TRENNZ_BEARB_PARAM);
        if(bearb_nach.at(0) == BEARBART_FRAESERGERADE)
        {
            fraesergerade fg_nach(bearb_nach.text());
            fg_nach.set_startpunkt(fb.ep());
            fg_nach.set_tiSta(fb.tiEnd());
            zeile_aendern(index+1, fg_nach.text(), false);
        }else if(bearb_nach.at(0) == BEARBART_FRAESERBOGEN)
        {
            fraeserbogen fb_nach(bearb_nach.text());
            fb_nach.bog_ptr()->versetze_spu(fb.ep());
            fb_nach.set_tiSta(fb.tiEnd());
            zeile_aendern(index+1, fb_nach.text(), false);
        }
    }
    zeile_aendern(index, bearb, true);
}

void MainWindow::slot_make(QString bearb, bool unredor_verwenden)
{
    int index_dat = ui->listWidget_dateien->currentRow();
    if(index_dat < 0)
    {
        return;
    }
    int index_bearb = ui->listWidget_bearb->currentRow();
    //Werte zurück speichern:
    if(index_bearb == 0)
    {
        Wste.wst(index_dat)->bearb_ptr()->add_vo(bearb);
    }else if(index_bearb+1 < ui->listWidget_bearb->count())
    {
        Wste.wst(index_dat)->bearb_ptr()->add_mi(index_bearb-1, bearb);
    }else
    {
        Wste.wst(index_dat)->bearb_ptr()->add_hi(bearb);
    }
    update_listwidget_bearb(Wste.wst(index_dat));
    if(unredor_verwenden == true)
    {
        Wste.wst(index_dat)->unredo_neu();
    }
    wkz_magazin wkz;
    if(ui->comboBox_maschinen->currentIndex() >= 0)
    {
        QString masch_bez = ui->comboBox_maschinen->currentText();
        int index = Maschinen.get_index(masch_bez);
        wkz = Maschinen.masch(index)->wkzmag();
    }
    vorschaufenster.slot_aktualisieren(Wste.wst(index_dat)->geo(wkz), index_dat);
}
void MainWindow::slot_make_bo(bohrung bo)
{
    slot_make(bo.text(), true);
}
void MainWindow::slot_make_rta(rechtecktasche rt)
{
    slot_make(rt.text(), true);
}
void MainWindow::slot_make_nut(nut nu)
{
    slot_make(nu.text(), true);
}
void MainWindow::on_action_make_bohrung_triggered()
{
    int index_dat = ui->listWidget_dateien->currentRow();
    if(index_dat >= 0)
    {
        wkz_magazin wkz;
        if(ui->comboBox_maschinen->currentIndex() >= 0)
        {
            QString masch_bez = ui->comboBox_maschinen->currentText();
            int index = Maschinen.get_index(masch_bez);
            wkz = Maschinen.masch(index)->wkzmag();
        }
        Dialog_bearb_bohrung dlg;
        dlg.setModal(true);
        bohrung bo;//Default-Daten
        dlg.set_data(bo.text(),  Wste.wst(index_dat), wkz.magazin());
        connect(&dlg, SIGNAL(signal_bo(bohrung)), this, SLOT(slot_make_bo(bohrung)));
        dlg.exec();
    }else
    {
        QMessageBox mb;
        mb.setText("Es ist kein aktives Werkstück vorhanden!");
        mb.setWindowTitle("Bohrung einfügen");
        mb.exec();
    }
}
void MainWindow::on_action_make_rta_triggered()
{
    int index_dat = ui->listWidget_dateien->currentRow();
    if(index_dat >= 0)
    {
        wkz_magazin wkz;
        if(ui->comboBox_maschinen->currentIndex() >= 0)
        {
            QString masch_bez = ui->comboBox_maschinen->currentText();
            int index = Maschinen.get_index(masch_bez);
            wkz = Maschinen.masch(index)->wkzmag();
        }
        Dialog_bearb_rta dlg;
        dlg.setModal(true);
        rechtecktasche rt;//Default-Daten
        dlg.set_data(rt.text(),  Wste.wst(index_dat), wkz.magazin());
        connect(&dlg, SIGNAL(signal_rta(rechtecktasche)), this, SLOT(slot_make_rta(rechtecktasche)));
        dlg.exec();
    }else
    {
        QMessageBox mb;
        mb.setText("Es ist kein aktives Werkstück vorhanden!");
        mb.setWindowTitle("Rechtecktasche einfügen");
        mb.exec();
    }
}
void MainWindow::on_action_make_nut_triggered()
{
    int index_dat = ui->listWidget_dateien->currentRow();
    if(index_dat >= 0)
    {
        Dialog_bearb_nut dlg;
        dlg.setModal(true);
        nut nu;//Default-Daten
        dlg.set_data(nu.text(),  Wste.wst(index_dat));
        connect(&dlg, SIGNAL(signal_nut(nut)), this, SLOT(slot_make_nut(nut)));
        dlg.exec();
    }else
    {
        QMessageBox mb;
        mb.setText("Es ist kein aktives Werkstück vorhanden!");
        mb.setWindowTitle("Nut einfügen");
        mb.exec();
    }
}
//------------------------------------------------------









