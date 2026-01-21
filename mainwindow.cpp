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
//Dateien/Werkstücke:
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

//------------------------------------------------------



