#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "ToDo.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    Programmversion_simson = PROGRAMMVERSION;
    aktualisiere_fendtertitel();
    PrgPfade.ordner_erstellen();
    setup();

    set_vorschaufenster_default();

    connect(&vorschaufenster, SIGNAL(sende_maus_pos(punkt3d)),\
            this, SLOT(getMausPosXY(punkt3d)));
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

    connect(this, SIGNAL(sendEinstellungDxf(einstellung_dxf)),\
            &dlg_einstellung_dxf, SLOT(slot_einstellung(einstellung_dxf)));
    connect(&dlg_einstellung_dxf, SIGNAL(send_einstellung(einstellung_dxf)),\
            this, SLOT(getEinstellungDxf(einstellung_dxf )));
    connect(this, SIGNAL(sendEinstellungDxfKlassen(einstellung_dxf, einstellung_dxf_klassen)),\
            &dlg_einstellung_dxf_klassen, SLOT(slot_einstellung(einstellung_dxf, einstellung_dxf_klassen)));
    connect(&dlg_einstellung_dxf_klassen, SIGNAL(send_einstellung(einstellung_dxf_klassen)),\
            this, SLOT(getEinstellungDxfKlassen(einstellung_dxf_klassen )));

    // Installiere den Shortcut-Filter NUR für das spezifische ListWidget
    ui->listWidget_bearb->installEventFilter(this);

    this->setWindowState(Qt::WindowMaximized);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::setup()
{
    //Schauen ob alle Konfigurationsdateien vorhanden sind:
    bool inifile_gefunden           = false;    //user-Ordner
    bool ini_dxf_gefunden           = false;    //user-Ordner
    bool ini_dxf_klassen_gefunden   = false;    //user-Ordner

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
        if(name.contains(PrgPfade.name_ini_dxf()))
        {
            ini_dxf_gefunden = true;
        }
        if(name.contains(PrgPfade.name_ini_dxf_klassen()))
        {
            ini_dxf_klassen_gefunden = true;
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
            Pfad_letzte_geoeffnete_datei = Einstellung.verzeichnis_zuletzt_geoefnet();
        }
        file.close();
    }

    if(ini_dxf_gefunden == false)
    {
        QFile file(PrgPfade.path_ini_dxf());
        if(!file.open(QIODevice::WriteOnly | QIODevice::Text))
        {
            QString tmp = "Fehler beim Dateizugriff!\n";
            tmp += PrgPfade.path_ini_dxf();
            tmp += "\n";
            tmp += "in der Funktion setup";
            QMessageBox::warning(this,"Fehler",tmp,QMessageBox::Ok);
        }else
        {
            file.write(Einstellung_dxf.text().toLatin1());
        }
        file.close();
    }else
    {
        QFile file(PrgPfade.path_ini_dxf());
        if(!file.open(QIODevice::ReadOnly | QIODevice::Text))
        {
            QString tmp = "Fehler beim Dateizugriff!\n";
            tmp += PrgPfade.path_ini_dxf();
            tmp += "\n";
            tmp += "in der Funktion setup";
            QMessageBox::warning(this,"Fehler",tmp,QMessageBox::Ok);
        }else
        {
            Einstellung_dxf.set_text(file.readAll());
        }
        file.close();
    }

    if(ini_dxf_klassen_gefunden == false)
    {
        QFile file(PrgPfade.path_ini_dxf_klassen());
        if(!file.open(QIODevice::WriteOnly | QIODevice::Text))
        {
            QString tmp = "Fehler beim Dateizugriff!\n";
            tmp += PrgPfade.path_ini_dxf_klassen();
            tmp += "\n";
            tmp += "in der Funktion setup";
            QMessageBox::warning(this,"Fehler",tmp,QMessageBox::Ok);
        }else
        {
            file.write(Einstellung_dxf_klassen.text().toLatin1());
        }
        file.close();
    }else
    {
        QFile file(PrgPfade.path_ini_dxf_klassen());
        if(!file.open(QIODevice::ReadOnly | QIODevice::Text))
        {
            QString tmp = "Fehler beim Dateizugriff!\n";
            tmp += PrgPfade.path_ini_dxf_klassen();
            tmp += "\n";
            tmp += "in der Funktion setup";
            QMessageBox::warning(this,"Fehler",tmp,QMessageBox::Ok);
        }else
        {
            Einstellung_dxf_klassen.set_text(file.readAll());
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
//Grafik und UI allgemein:
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
    ui->btn_quick_import->move(x, 5);

    int h = (this->height()-ui->listWidget_dateien->pos().y()-60)/2 - 25;
    int b = this->width() - x - 5;
    ui->listWidget_dateien->move(x, ui->btn_quick_import->pos().y()+ui->btn_quick_import->height()+5);
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
void MainWindow::closeEvent(QCloseEvent *event)
{
    bool abbrechen = false;

    for (int i = 0; i < Wste.anzahl(); i++)
    {
        werkstueck* wst = Wste.wst(i);
        if (wst->hat_aenderungen())
        {
            QString msg = "Das Bauteil '" + wst->name() + "' wurde geändert. Möchten Sie speichern?";
            QMessageBox::StandardButton res = QMessageBox::warning(this, "Beenden", msg,
                                                                   QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel);

            if (res == QMessageBox::Save)
            {
                // Wir wählen das Element im Listwidget aus, damit die Speicher-Funktion weiß, wen sie speichern soll
                ui->listWidget_dateien->setCurrentRow(i);
                on_actionSpeichern_triggered();
            }else if (res == QMessageBox::Cancel)
            {
                abbrechen = true;
                break;
            }
        }
    }

    if (abbrechen)
    {
        event->ignore(); // Programm bleibt offen
    }else
    {
        event->accept(); // Programm schließt
    }
}
void MainWindow::set_vorschaufenster_default()
{
    geo_text gt;
    geo_text fkon;
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

    vorschaufenster.slot_aktualisieren(gt, fkon, 0);
}
void MainWindow::getMausPosXY(punkt3d p)
{
    QString x = double_to_qstring(p.x(), 1);
    QString y = double_to_qstring(p.y(), 1);
    QString msg;
    msg  = "X: ";
    msg += x;
    msg += " / Y: ";
    msg += y;
    ui->label_mauspos->setText(msg);
}
void MainWindow::aktualisiere_fendtertitel()
{
    if(ui->listWidget_dateien->selectedItems().count())
    {
        if(Wste.wst(ui->listWidget_dateien->currentRow()) != nullptr)
        {
            if(!Wste.wst(ui->listWidget_dateien->currentRow())->dateipfad().isEmpty())
            {
                QString titel;
                titel = Wste.wst(ui->listWidget_dateien->currentRow())->dateipfad();
                if(Wste.wst(ui->listWidget_dateien->currentRow())->hat_aenderungen())
                {
                    titel += "*";
                }
                this->setWindowTitle(titel);
            }else
            {
                this->setWindowTitle(Programmversion_simson);
            }
        }else
        {
            this->setWindowTitle(Programmversion_simson);
        }
    }else
    {
        this->setWindowTitle(Programmversion_simson);
    }
}
bool MainWindow::eventFilter(QObject *obj, QEvent *event) {
    // 1. Prüfen, ob das Event vom richtigen Widget kommt
    if (obj == ui->listWidget_bearb)
    {
        // 2. Prüfen, ob es ein Tastendruck ist
        if (event->type() == QEvent::KeyPress)
        {
            QKeyEvent *keyEvent = static_cast<QKeyEvent*>(event);

            // 3. Prüfen, ob es die Leertaste ist
            if (keyEvent->key() == Qt::Key_Space)
            {
                if(ui->listWidget_dateien->currentRow() >= 0)
                {
                    for(int i=auswahl_erster(); i<=auswahl_letzter() ; i++)
                    {
                        zeile_bearb_afb_umkehren(i-1);
                    }
                }
                return true; // Event stoppen (nicht an das Widget weiterleiten)
            }else if (keyEvent->key() == Qt::Key_Return || keyEvent->key() == Qt::Key_Enter)
            {
                if(ui->listWidget_dateien->currentRow() >= 0)
                {
                    int index_bearb = ui->listWidget_bearb->currentRow();
                    zeile_bearb_bearbeiten(index_bearb);
                }
                return true; // Event stoppen (nicht an das Widget weiterleiten)
            }
        }
    }
    // Andere Events/Widgets normal weiterverarbeiten
    return QMainWindow::eventFilter(obj, event);
}
void MainWindow::update_vorschau()
{
    int index_wst = ui->listWidget_dateien->currentRow();
    int index_liwid = ui->listWidget_bearb->currentRow();

    if(index_wst >= 0)
    {
        if(index_liwid < 0)
        {
            index_liwid = 0;
        }
        Wste.wst(index_wst)->unredo_neu();
        ui->listWidget_bearb->setCurrentRow(index_liwid);

        wkz_magazin wkz;
        if(ui->comboBox_maschinen->currentIndex() >= 0)
        {
            QString masch_bez = ui->comboBox_maschinen->currentText();
            int index = Maschinen.get_index(masch_bez);
            wkz = Maschinen.masch(index)->wkzmag();
        }
        vorschaufenster.slot_aktualisieren(Wste.wst(index_wst)->geo(wkz), \
                                           Wste.wst(index_wst)->geo_aktfkon(wkz), index_liwid);
    }

}
void MainWindow::update_listwid_bearb()
{
    int index_wst = ui->listWidget_dateien->currentRow();
    if(index_wst >= 0)
    {
        update_listwidget_bearb(Wste.wst(index_wst));
    }
}
//Datei:
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
        //bohrung bo;
        //bo.set_dm(25);
        //bo.set_x(100);
        //bo.set_y(50);
        //bo.set_z(19);
        //bo.set_tiefe(10);
        //bo.set_bezug(WST_BEZUG_OBSEI);
        //text_zw bearb;
        //bearb.add_hi(bo.text());
        //w.set_bearb(bearb);

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
void MainWindow::on_action_oeffnen_triggered()
{
    QDir d(Pfad_letzte_geoeffnete_datei);
    if(!d.exists())//z.B. wenn der ordner zwischenzeitlich umbenannt wurde
    {
        Pfad_letzte_geoeffnete_datei = Einstellung.verzeichnis_quelle();
    }
    QString filter = tr("Alle unterstützten Dateien (*.ewx *.sim);;ewx Dateien (*.ewx);;sim Dateien (*.sim)");
    QStringList pfade = QFileDialog::getOpenFileNames(this, tr("Wähle Datei(en)"), \
                                                      Pfad_letzte_geoeffnete_datei, filter, \
                                                      nullptr, QFileDialog::DontUseNativeDialog);

    //-----------------------------Dateien einlesen:
    for(int i=0; i<pfade.size() ;i++)
    {
        QString aktueller_pfad = pfade.at(i);
        QFile datei(aktueller_pfad);
        QFileInfo finfo(datei);
        Pfad_letzte_geoeffnete_datei = finfo.path();
        Einstellung.set_verzeichnis_zuletzt_geoefnet(Pfad_letzte_geoeffnete_datei);
        schreibe_ini();

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
            QString wstname = finfo.fileName();
            QString dateiendung =  finfo.suffix();
            wstname = finfo.baseName();

            if(Wste.ist_bekannt(wstname))
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
            }else if(dateiendung == "ewx")
            {
                werkstueck w = import_ewx(inhalt);
                w.set_name(wstname);
                Wste.neu(w);
            }else if(dateiendung == "sim")
            {
                werkstueck w;
                w.set_text(inhalt);
                w.set_dateipfad(aktueller_pfad);//setzt auch den wst-namen
                Wste.neu(w);
            }
        }
    }
    //-----------------------------
    //-----------------------------UI aktualisieren:
    if(Wste.wst(0))
    {
        ui->listWidget_dateien->clear();
        for(uint i=0; i<Wste.anzahl();i++)
        {
            ui->listWidget_dateien->addItem(Wste.namen_tz().at(i));
        }
        ui->listWidget_dateien->setCurrentRow(Wste.anzahl()-1);
    }
    //-----------------------------
}
void MainWindow::on_btn_quick_import_clicked()
{
    on_action_quick_import_triggered();
}
void MainWindow::on_action_quick_import_triggered()
{
    QString ordnerPfad = Einstellung.verzeichnis_quelle();

    // NameFilter definieren (Wildcards sind erlaubt)
    QStringList filter;
    filter << "*.ewx";

    // QDirIterator ist speicherschonend und schnell
    // Flags:
    //      QDir::NoDotAndDotDot (schließt "." und ".." aus)
    //      QDir::Files (nur Dateien, keine Ordner)
    QDirIterator it(ordnerPfad, filter, QDir::Files | QDir::NoDotAndDotDot);

    QStringList pfade;
    while (it.hasNext())
    {
        // it.next() gibt den vollständigen Pfad zurück und springt zum nächsten Element
        pfade.append(it.next());
    }

    //-----------------------------Dateien einlesen:
    Wste.clear();
    for(int i=0; i<pfade.size() ;i++)
    {
        QString aktueller_pfad = pfade.at(i);
        QFile datei(aktueller_pfad);
        QFileInfo finfo(datei);

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
            QString wstname = finfo.fileName();
            QString dateiendung = finfo.suffix();
            wstname = wstname.left(wstname.length()-dateiendung.length());
            if(dateiendung == "ewx")
            {
                if(Wste.ist_bekannt(wstname))
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
                }else
                {
                    werkstueck w = import_ewx(inhalt);
                    w.set_name(wstname);
                    Wste.neu(w);
                }
            }
        }
    }
    Wste.sortieren();
    //-----------------------------
    //-----------------------------UI aktualisieren:
    if(Wste.wst(0))
    {
        ui->listWidget_dateien->clear();
        for(uint i=0; i<Wste.anzahl();i++)
        {
            ui->listWidget_dateien->addItem(Wste.namen_tz().at(i));

        }
        ui->listWidget_dateien->setCurrentRow(0);
    }
    //-----------------------------
}
void MainWindow::on_action_import_dxf_triggered()
{
    Pfad_letzte_geoeffnete_datei = Einstellung.verzeichnis_zuletzt_geoefnet();
    QDir d(Pfad_letzte_geoeffnete_datei);
    if(!d.exists())//z.B. wenn der ordner zwischenzeitlich umbenannt wurde
    {
        Pfad_letzte_geoeffnete_datei = Einstellung.verzeichnis_quelle();
    }

    QStringList pfade = QFileDialog::getOpenFileNames(this, tr("Wähle Datei(en)"), \
                                                      Pfad_letzte_geoeffnete_datei, tr("dxf Dateien (*.dxf)"), \
                                                      nullptr, QFileDialog::DontUseNativeDialog);

    //-----------------------------Dateien einlesen:
    for(int i=0; i<pfade.size() ;i++)
    {
        QString aktueller_pfad = pfade.at(i);
        QFile datei(aktueller_pfad);
        QFileInfo finfo(datei);
        Pfad_letzte_geoeffnete_datei = finfo.path();
        Einstellung.set_verzeichnis_zuletzt_geoefnet(Pfad_letzte_geoeffnete_datei);
        schreibe_ini();

        //QString nam_ohn_end = dateien_alle.at(i).left(dateien_alle.at(i).length()-dxf.length());
        QString wstname = finfo.fileName();
        QString dateiendung = finfo.suffix();
        wstname = wstname.left(wstname.length()-dateiendung.length()-1);
        QString kenOb = Einstellung_dxf.kenObsei();
        QString kenUn = Einstellung_dxf.kenUnsei();
        QString nam_ohn_pref;
        bool ist_oberseite = true;
        if(wstname.right(kenOb.length()) == kenOb)
        {
            nam_ohn_pref = wstname.left(wstname.length()-kenOb.length());
            ist_oberseite = true;
        }else if(wstname.right(kenUn.length()) == kenUn)
        {
            nam_ohn_pref = wstname.left(wstname.length()-kenUn.length());
            ist_oberseite = false;
        }else
        {
            nam_ohn_pref = wstname;
            ist_oberseite = true;
        }

        if(!datei.open(QIODevice::ReadOnly | QIODevice::Text))
        {
            QString tmp = "Fehler beim Dateizugriff!\n";
            tmp += aktueller_pfad;
            tmp += "\n";
            tmp += "in der Funktion on_action_import_dxf_triggered";
            QMessageBox::warning(this,"Fehler",tmp,QMessageBox::Ok);
        }else
        {
            datei.close();

            if(ist_oberseite)
            {
                werkstueck w;
                w.set_name(nam_ohn_pref);
                w.set_laenge(400);//Default-Wert
                w.set_breite(250);//Default-Wert
                w.set_dicke(20);//Default-Wert

                import_dxf(&w, ist_oberseite, aktueller_pfad, Einstellung_dxf, Einstellung_dxf_klassen);
                Wste.neu(w);
            }else// istUnterseite
            {
                int wstindex = Wste.get_index(nam_ohn_pref);
                if(wstindex >= 0)
                {
                    if(Wste.wst(wstindex))//Doppelte Sicherheit
                    {
                        werkstueck *w = Wste.wst(wstindex);//Zeiger auf wst innerhalb von wste
                        //Bearbeitungen ergänzen im bereits vorhandenne wst:
                        import_dxf(w, ist_oberseite, aktueller_pfad, Einstellung_dxf, Einstellung_dxf_klassen);
                        Wste.wst(wstindex)->unredo_clear();
                        Wste.wst(wstindex)->unredo_neu();
                    }
                }else
                {
                    //Die Datei wurde wahrscheinlich versehentlich falsch benannt und es
                    //gibt zu diesem Zeitpunkt noch keine Oberseitenbearbeitung
                    ist_oberseite = true;
                    werkstueck w;
                    w.set_name(nam_ohn_pref);
                    w.set_laenge(400);//Default-Wert
                    w.set_breite(250);//Default-Wert
                    w.set_dicke(20);//Default-Wert
                    import_dxf(&w, ist_oberseite, aktueller_pfad, Einstellung_dxf, Einstellung_dxf_klassen);
                    Wste.neu(w);
                }

            }
        }
    }
    //-----------------------------
    //-----------------------------UI aktualisieren:
    if(Wste.wst(0))
    {
        werkstueck *w = Wste.wst(0);
        ui->listWidget_dateien->clear();
        for(uint i=0; i<Wste.anzahl();i++)
        {
            ui->listWidget_dateien->addItem(Wste.namen_tz().at(i));
            ui->listWidget_dateien->setCurrentRow(0);
        }
        wkz_magazin wkz;
        if(ui->comboBox_maschinen->currentIndex() >= 0)
        {
            QString masch_bez = ui->comboBox_maschinen->currentText();
            int index_masch = Maschinen.get_index(masch_bez);
            wkz = Maschinen.masch(index_masch)->wkzmag();
        }
        vorschaufenster.slot_aktualisieren(w->geo(wkz), w->geo_aktfkon(wkz), 0);
    }
    //-----------------------------
}
//---
void MainWindow::on_actionUmbenennen_triggered()
{
    if(ui->listWidget_dateien->selectedItems().count())
    {
        QString name = ui->listWidget_dateien->currentItem()->text();
        if(!name.isEmpty())
        {
            if(Wste.wst(ui->listWidget_dateien->currentRow()))
            {
                QString dateipfad = Wste.wst(ui->listWidget_dateien->currentRow())->dateipfad();
                if(!dateipfad.isEmpty())
                {
                    QString msg;
                    msg  = "Das Bauteil wurde bereits unter diesem Namen gespeichert!\n";
                    msg += "Bitte Verwende \"Speichern unter\" zum Umbenennen des Bauteils";
                    QMessageBox mb;
                    mb.setText(msg);
                    mb.setWindowTitle("Bauteil umbenennen");
                    mb.exec();
                    return;
                }else
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
void MainWindow::on_actionSpeichern_triggered()
{
    if(ui->listWidget_dateien->selectedItems().count())
    {
        QString fileName;
        fileName = Wste.wst(ui->listWidget_dateien->currentRow())->dateipfad();
        if(fileName.isEmpty())
        {
            QString vorschlagName = Pfad_letzte_geoeffnete_datei;
            vorschlagName += QDir::separator();
            vorschlagName += Wste.wst(ui->listWidget_dateien->currentRow())->name();
            vorschlagName += DATEIENDUNG_EIGENE;
            //Dialog öffnen zum Wählen des Speicherortes und des Namens:
            fileName = QFileDialog::getSaveFileName(this, tr("Datei Speichern"), \
                                                    vorschlagName, tr("sim Dateien (*.sim)"));

            fileName = validiere_dateipfad(fileName);
            if (!fileName.isEmpty())
            {
                QFileInfo info(fileName);
                Pfad_letzte_geoeffnete_datei = info.absolutePath();

                if(speichern(fileName, Wste.wst(ui->listWidget_dateien->currentRow())))
                {
                    Wste.wst(ui->listWidget_dateien->currentRow())->set_dateipfad(fileName);
                    aktualisiere_listwidget_dateien(ui->listWidget_dateien->currentRow());
                }
            }
        }else
        {
            speichern(fileName, Wste.wst(ui->listWidget_dateien->currentRow()));
        }
    }else
    {
        QString msg;
        msg = "Es ist kein Bauteil ausgewählt!";
        QMessageBox mb;
        mb.setText(msg);
        mb.setWindowTitle("Speichern");
        mb.exec();
    }
}
void MainWindow::on_actionSpeichern_unter_triggered()
{
    if(ui->listWidget_dateien->selectedItems().count())
    {
        QString fileName;
        //Dialog öffnen zum Wählen des Speicherortes und des Namens:
        fileName = QFileDialog::getSaveFileName(this, tr("Datei Speichern"), \
                                                Pfad_letzte_geoeffnete_datei, tr("sim Dateien (*.sim)"));
        fileName = validiere_dateipfad(fileName);
        if (!fileName.isEmpty())
        {
            QFileInfo info(fileName);
            if(Wste.ist_bekannt(info.baseName()))
            {
                QString msg;
                msg  = "Es gibt bereits ein Bauteil mit diesem Namen in der Bauteilliste!\n";
                msg += "Bitte speichere dieses Bauteil unter einem anderen Namen oder schließe das andere Bauteil vorab.";
                QMessageBox mb;
                mb.setText(msg);
                mb.setWindowTitle("Bauteil umbenennen");
                mb.exec();
            }else
            {
                if(speichern(fileName, Wste.wst(ui->listWidget_dateien->currentRow())))
                {
                    Wste.wst(ui->listWidget_dateien->currentRow())->set_dateipfad(fileName);
                    Wste.wst(ui->listWidget_dateien->currentRow())->set_name(info.baseName());
                    aktualisiere_listwidget_dateien(ui->listWidget_dateien->currentRow());
                }
            }
        }
    }else
    {
        QString msg;
        msg = "Es ist kein Bauteil ausgewählt!";
        QMessageBox mb;
        mb.setText(msg);
        mb.setWindowTitle("Speichern unter");
        mb.exec();
    }
}
bool MainWindow::speichern(QString dateipfad, werkstueck *wst)
{
    // 1. Validitätsprüfung
    if (dateipfad.isEmpty() || wst == nullptr)
    {
        return false;
    }

    // 2. Datei zum Schreiben öffnen
    QFile file(dateipfad);

    // QIODevice::WriteOnly | QIODevice::Text reicht völlig aus.
    // Wenn die Datei existiert, wird sie automatisch überschrieben (Truncate).
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
    {
        QMessageBox::critical(this, tr("Fehler"), tr("Datei konnte nicht zum Schreiben geöffnet werden."));
        return false;
    }

    // 3. Inhalt schreiben
    QString dateiInhalt = wst->text();

    // Prüfen, ob das Schreiben erfolgreich war
    if (file.write(dateiInhalt.toUtf8()) == -1)
    {
        QMessageBox::critical(this, tr("Fehler"), tr("Inhalt konnte nicht geschrieben werden."));
        file.close();
        return false;
    }

    file.close();
    wst->wurde_gespeichert();
    aktualisiere_fendtertitel();
    return true; // Erfolgreich gespeichert
}
QString MainWindow::validiere_dateipfad(QString pfad)
{
    if (pfad.isEmpty())
    {
        return "";
    }

    QFileInfo info(pfad);
    // 1. Prüfen ob überhaupt ein Name vor dem Punkt steht
    if (info.baseName().isEmpty())
    {
        QMessageBox::warning(this, "Speichern", "Bitte geben Sie einen gültigen Dateinamen ein.");
        return "";
    }

    // 2. Endung prüfen (case-insensitive)
    // endsWith ist hier am sichersten
    if (!pfad.endsWith(DATEIENDUNG_EIGENE, Qt::CaseInsensitive))
    {
        // Wenn gar keine Endung da ist, einfach anhängen
        if (info.suffix().isEmpty())
        {
            pfad += DATEIENDUNG_EIGENE;
        }else
        {
            // Wenn eine falsche Endung da ist (z.B. .txt)
            QMessageBox::warning(this, "Speichern", "Die angegebene Dateiendung ist ungültig!");
            return "";
        }
    }
    return pfad;
}
void MainWindow::on_actionGCode_Export_triggered()
{
    if(ui->listWidget_dateien->selectedItems().count())
    {
        int index = ui->listWidget_dateien->currentRow();
        werkstueck* wst = Wste.wst(index);
        if(wst->dateipfad().isEmpty())
        {
            QString msg;
            msg = "Das Bauteil muss zuerst gespeichert werden!";
            QMessageBox mb;
            mb.setText(msg);
            mb.setWindowTitle("GCode exportieren");
            mb.exec();
            return;
        }
        if (wst->hat_aenderungen())
        {
            QString msg;
            msg = "Die Änderungen am Bauteil müssen zuerst gespeichert werden!";
            QMessageBox mb;
            mb.setText(msg);
            mb.setWindowTitle("GCode exportieren");
            mb.exec();
            return;
        }
        maschine *masch;
        if(ui->comboBox_maschinen->currentIndex() >= 0)
        {
            QString masch_bez = ui->comboBox_maschinen->currentText();
            int index_masch = Maschinen.get_index(masch_bez);
            masch = Maschinen.masch(index_masch);
        }else
        {
            QString msg;
            msg = "Bitte zuerst eine CNC-Maschine erstellen und auswählen!";
            QMessageBox mb;
            mb.setText(msg);
            mb.setWindowTitle("GCode exportieren");
            mb.exec();
            return;
        }
        Dialog_GCode dlg;
        dlg.set_maschine(masch);
        dlg.set_wst(wst);
        dlg.exec();
    }else
    {
        QString msg;
        msg = "Es ist kein Bauteil ausgewählt!";
        QMessageBox mb;
        mb.setText(msg);
        mb.setWindowTitle("GCode exportieren");
        mb.exec();
    }
}
//---
void MainWindow::on_action_schliessen_triggered()
{
    if (ui->listWidget_dateien->selectedItems().count())
    {
        int index = ui->listWidget_dateien->currentRow();
        werkstueck* wst = Wste.wst(index);

        if (wst->hat_aenderungen())
        {
            QString msg = "Das Bauteil '" + wst->name() + "' wurde geändert. Möchten Sie vor dem Schließen speichern?";
            QMessageBox::StandardButton res = QMessageBox::warning(this, "Datei schließen", msg,
                                                                   QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel);

            if (res == QMessageBox::Save)
            {
                on_actionSpeichern_triggered();
                // Falls speichern() fehlschlug oder abgebrochen wurde, darf nicht gelöscht werden
                if (Wste.wst(index)->hat_aenderungen())
                {
                    return;
                }
            }else if (res == QMessageBox::Cancel)
            {
                return;
            }
        }

        Wste.entf_at(index);

        // Nach dem Löschen Index 0 wählen, falls noch Dateien da sind
        int neuerIndex = 0;
        if(Wste.anzahl() == 0)
        {
            neuerIndex = -1;
        }
        aktualisiere_listwidget_dateien(neuerIndex);
    }else
    {
        QString msg = "Es ist kein Bauteil ausgewählt!";
        QMessageBox mb;
        mb.setText(msg);
        mb.setWindowTitle("Datei/Bauteil schließen");
        mb.exec();
    }
}
//Bearbeiten:
void MainWindow::on_actionUndo_triggered()
{
    process_undo_redo(true);
}
void MainWindow::on_actionRedo_triggered()
{
    process_undo_redo(false);
}
void MainWindow::process_undo_redo(bool isUndo)
{
    if (ui->listWidget_dateien->selectedItems().count())
    {
        int index_wst = ui->listWidget_dateien->currentRow();
        werkstueck* wst = Wste.wst(index_wst);

        if (isUndo)
        {
            wst->undo();
        }else
        {
            wst->redo();
        }

        // UI aktualisieren
        update_listwidget_bearb(wst);
        update_vorschau();
        aktualisiere_fendtertitel();
    }else
    {
        QMessageBox::information(this, "Rückgängig", "Es ist kein Bauteil ausgewählt!");
    }
}
//---
void MainWindow::on_actionKopieren_triggered()
{
    if(ui->listWidget_dateien->selectedItems().count())
    {
        int index_liwid = ui->listWidget_bearb->currentRow();
        int index_bearb = index_liwid-1;
        int index_wst = ui->listWidget_dateien->currentRow();

        if((ui->listWidget_bearb->currentIndex().isValid())  &&  \
            (ui->listWidget_bearb->currentItem()->isSelected())    )
        {
            QList<QListWidgetItem*> items = ui->listWidget_bearb->selectedItems();
            int items_menge = items.count();
            int row_erstes_liwid = 0;//Nummer des ersten Elementes
            for(int i=0; i<ui->listWidget_bearb->count() ;i++)
            {
                if(ui->listWidget_bearb->item(i)->isSelected())
                {
                    row_erstes_liwid = i;
                    break;
                }
            }
            int row_erstes_bearb = row_erstes_liwid-1;
            if(items_menge==1)
            {
                if(index_liwid > 0  &&  index_liwid+1 < ui->listWidget_bearb->count())
                {
                    KopierterEintrag = Wste.wst(index_wst)->bearb_ptr()->at(index_bearb);
                }
            }else
            {
                if(row_erstes_liwid == 0)//Programmkopf
                {
                    row_erstes_liwid = 1;
                    row_erstes_bearb = row_erstes_liwid-1;
                    items_menge = items_menge-1;
                }
                if(row_erstes_liwid+items_menge >= ui->listWidget_bearb->count())
                {
                    items_menge = ui->listWidget_bearb->count()-row_erstes_liwid-1;
                }
                QString tmp = Wste.wst(index_wst)->bearb_ptr()->at(row_erstes_bearb, items_menge);
                KopierterEintrag = tmp;
            }
        }else
        {
            QMessageBox mb;
            mb.setWindowTitle("Bearbeitung kopieren");
            mb.setText("Sie haben noch nichts ausgewaelt was kopiert werden kann!");
            mb.exec();
        }
    }else
    {
        QString msg;
        msg = "Es ist kein Bauteil ausgewählt!";
        QMessageBox mb;
        mb.setText(msg);
        mb.setWindowTitle("Bearbeitung kopieren");
        mb.exec();
    }
}
void MainWindow::on_actionEinfuegen_triggered()
{
    if(ui->listWidget_dateien->selectedItems().count())
    {
        if(!KopierterEintrag.isEmpty())
        {
            int index_liwid = ui->listWidget_bearb->currentRow();
            int index_wst = ui->listWidget_dateien->currentRow();

            if(index_liwid == 0)//Programmkopf
            {
                index_liwid = 1;
            }
            if(index_liwid == 1)
            {
                Wste.wst(index_wst)->bearb_ptr()->add_vo(KopierterEintrag);
            }else if(index_liwid == ui->listWidget_bearb->count()-1)
            {
                Wste.wst(index_wst)->bearb_ptr()->add_hi(KopierterEintrag);
            }else
            {
                Wste.wst(index_wst)->bearb_ptr()->add_mi(index_liwid-2, KopierterEintrag);
            }
            update_listwidget_bearb(Wste.wst(index_wst));
            Wste.wst(index_wst)->unredo_neu();
            ui->listWidget_bearb->setCurrentRow(index_liwid);

            update_listwid_bearb();
            update_vorschau();
            aktualisiere_fendtertitel();
        }
    }else
    {
        QString msg;
        msg = "Es ist kein Bauteil ausgewählt!";
        QMessageBox mb;
        mb.setText(msg);
        mb.setWindowTitle("Bearbeitung einfügen");
        mb.exec();
    }
}
void MainWindow::on_actionEntfernen_triggered()
{
    if((ui->listWidget_bearb->currentIndex().isValid())  &&  \
        (ui->listWidget_bearb->currentItem()->isSelected())    )
    {
        int index_wst = ui->listWidget_dateien->currentRow();
        //Prüfen ob Fräsbahnen durch das entfernen geteilt werden:
        bool gesund = true;
        //--Prüfen ob eine Fräsbahn nach der Auswahl weiter geht:
        if(auswahl_letzter() < ui->listWidget_bearb->count())
        {
            int zeile_dannach = auswahl_letzter();//index von QListwidget
            text_zw bearb;
            bearb.set_text(Wste.wst(index_wst)->bearb_ptr()->at(zeile_dannach),TRENNZ_BEARB_PARAM);
            if(bearb.at(0) == BEARBART_FRAESERGERADE  ||  bearb.at(0) == BEARBART_FRAESERBOGEN)
            {
                gesund = false;
            }
        }
        //---Prüfen ob eine Fräsbahn vor der Auswahl beginnt:
        if(auswahl_erster() >= 2)
        {
            int zeile_davor = auswahl_erster()-1;//index von QListwidget
            text_zw bearb;
            bearb.set_text(Wste.wst(index_wst)->bearb_ptr()->at(zeile_davor),TRENNZ_BEARB_PARAM);
            text_zw bearb_erster;
            bearb_erster.set_text(Wste.wst(index_wst)->bearb_ptr()->at(auswahl_erster()-1),TRENNZ_BEARB_PARAM);
            if(bearb_erster.at(0) != BEARBART_FRAESERAUFRUF)
            {
                if(bearb.at(0) == BEARBART_FRAESERAUFRUF  || \
                                                                 bearb.at(0) == BEARBART_FRAESERGERADE  || \
                           bearb.at(0) == BEARBART_FRAESERBOGEN)
                {
                    gesund = false;
                }
            }
        }
        //---
        if(gesund == false)
        {
            QMessageBox mb;
            mb.setWindowTitle("Bearbeitung entfernen");
            mb.setText("Das Entfernen dieser Zeilenauswahl ist nicht möglich weil eine Fräsbahn nur vollständig gelöscht werden kann!");
            mb.exec();
            return;
        }
        //---
        int index_liwid = ui->listWidget_bearb->currentRow();
        Wste.wst(index_wst)->bearb_ptr()->entf(auswahl_erster()-1, auswahl_menge());
        Wste.wst(index_wst)->unredo_neu();
        update_listwidget_bearb(Wste.wst(index_wst));
        ui->listWidget_bearb->setCurrentRow(index_liwid-1);
        wkz_magazin wkz;
        if(ui->comboBox_maschinen->currentIndex() >= 0)
        {
            QString masch_bez = ui->comboBox_maschinen->currentText();
            int index = Maschinen.get_index(masch_bez);
            wkz = Maschinen.masch(index)->wkzmag();
        }
        vorschaufenster.slot_aktualisieren(Wste.wst(index_wst)->geo(wkz), \
                                                                          Wste.wst(index_wst)->geo_aktfkon(wkz), index_liwid-1);
    }else
    {
        QMessageBox mb;
        mb.setWindowTitle("Bearbeitung entfernen");
        mb.setText("Sie haben noch keine Bearbeitung ausgewält die entfernt werden kann!");
        mb.exec();
    }
}
//---
void MainWindow::zeile_aendern(int index_bearb, QString bearb, bool unredor_verwenden)
{
    //index ist der index der Bearbeitung
    //bearb ist eine Zeile der Bearbeitugen
    int index_dat = ui->listWidget_dateien->currentRow();

    text_zw bearbeitungen = Wste.wst(index_dat)->bearb();
    bearbeitungen.edit(index_bearb, bearb);
    Wste.wst(index_dat)->bearb_ptr()->edit(index_bearb, bearbeitungen.at(index_bearb));
    if(unredor_verwenden == true)
    {
        Wste.wst(index_dat)->unredo_neu();
    }
    update_listwidget_bearb(Wste.wst(index_dat));
    update_vorschau();
    aktualisiere_fendtertitel();
}
//CAM:
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
//---
void MainWindow::on_actionNC_Kommentar_triggered()
{
    int index_dat = ui->listWidget_dateien->currentRow();
    if(index_dat >= 0)
    {
        Dialog_kommentar_nc dlg;
        dlg.setModal(true);
        kommentar_nc ko;//Default-Daten
        dlg.set_data(ko.text());
        connect(&dlg, SIGNAL(signal_kom(kommentar_nc)), this, SLOT(slot_make_kom(kommentar_nc)));
        dlg.exec();
    }else
    {
        QMessageBox mb;
        mb.setText("Es ist kein aktives Werkstück vorhanden!");
        mb.setWindowTitle("NC-Kommentar einfügen");
        mb.exec();
    }
}
void MainWindow::on_actionNC_Halt_triggered()
{
    int index_dat = ui->listWidget_dateien->currentRow();
    if(index_dat >= 0)
    {
        Dialog_bearb_halt dlg;
        dlg.setModal(true);
        halt_nc ha;//Default-Daten
        dlg.set_data(ha.text());
        connect(&dlg, SIGNAL(signal_halt(halt_nc)), this, SLOT(slot_make_halt(halt_nc)));
        dlg.exec();
    }else
    {
        QMessageBox mb;
        mb.setText("Es ist kein aktives Werkstück vorhanden!");
        mb.setWindowTitle("NC-Halt einfügen");
        mb.exec();
    }
}
void MainWindow::on_actionGehe_zu_Punkt_triggered()
{
    int index_dat = ui->listWidget_dateien->currentRow();
    if(index_dat >= 0)
    {
        Dialog_bearb_gezupu dlg;
        dlg.setModal(true);
        gehezupunkt gzp;//Default-Daten
        dlg.set_data(gzp.text(), Wste.wst(index_dat));
        connect(&dlg, SIGNAL(signal_gzp(gehezupunkt)), this, SLOT(slot_make_gzp(gehezupunkt)));
        dlg.exec();
    }else
    {
        QMessageBox mb;
        mb.setText("Es ist kein aktives Werkstück vorhanden!");
        mb.setWindowTitle("\"Gehe zu Punkt\" einfügen");
        mb.exec();
    }
}
//---
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
    if(unredor_verwenden == true)
    {
        Wste.wst(index_dat)->unredo_neu();
    }
    update_listwidget_bearb(Wste.wst(index_dat));
    update_vorschau();
    aktualisiere_fendtertitel();
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
void MainWindow::slot_make_kom(kommentar_nc ko)
{
    slot_make(ko.text(), true);
}
void MainWindow::slot_make_halt(halt_nc ha)
{
    slot_make(ha.text(), true);
}
void MainWindow::slot_make_gzp(gehezupunkt gzp)
{
    slot_make(gzp.text(), true);
}
//---
void MainWindow::slot_rta(rechtecktasche rta)
{
    int index_bearb = ui->listWidget_bearb->currentRow()-1;//Index-1 weil 1. Zeile WST-Maße sind
    QString bearb = rta.text();
    ui->listWidget_bearb->item(index_bearb)->setText(rta_zu_prgzei(bearb));
    zeile_aendern(index_bearb, bearb, true);
}
void MainWindow::slot_bo(bohrung bo)
{
    int index_bearb = ui->listWidget_bearb->currentRow()-1;//Index-1 weil 1. Zeile WST-Maße sind
    QString bearb = bo.text();
    ui->listWidget_bearb->item(index_bearb)->setText(bohr_zu_prgzei(bearb));
    zeile_aendern(index_bearb, bearb, true);
}
void MainWindow::slot_nut(nut nu)
{
    int index_bearb = ui->listWidget_bearb->currentRow()-1;//Index-1 weil 1. Zeile WST-Maße sind
    QString bearb = nu.text();
    ui->listWidget_bearb->item(index_bearb)->setText(nut_zu_prgzei(bearb));
    zeile_aendern(index_bearb, bearb, true);
}
void MainWindow::slot_kom(kommentar_nc ko)
{
    int index_bearb = ui->listWidget_bearb->currentRow()-1;//Index-1 weil 1. Zeile WST-Maße sind
    QString bearb = ko.text();
    ui->listWidget_bearb->item(index_bearb)->setText(bohr_zu_prgzei(bearb));
    zeile_aendern(index_bearb, bearb, true);
}
void MainWindow::slot_gzp(gehezupunkt gzp)
{
    int index_bearb = ui->listWidget_bearb->currentRow()-1;//Index-1 weil 1. Zeile WST-Maße sind
    QString bearb = gzp.text();
    ui->listWidget_bearb->item(index_bearb)->setText(bohr_zu_prgzei(bearb));
    zeile_aendern(index_bearb, bearb, true);
}
void MainWindow::slot_halt(halt_nc ha)
{
    int index_bearb = ui->listWidget_bearb->currentRow()-1;//Index-1 weil 1. Zeile WST-Maße sind
    QString bearb = ha.text();
    ui->listWidget_bearb->item(index_bearb)->setText(bohr_zu_prgzei(bearb));
    zeile_aendern(index_bearb, bearb, true);
}
void MainWindow::slot_faufruf(fraeseraufruf fa)
{
    int index_bearb = ui->listWidget_bearb->currentRow()-1;//Index-1 weil 1. Zeile WST-Maße sind
    int index_wst = ui->listWidget_dateien->currentRow();
    QString bearb = fa.text();
    ui->listWidget_bearb->item(index_bearb)->setText(fauf_zu_prgzei(bearb));
    text_zw bearbeitungen = Wste.wst(index_wst)->bearb();

    fraeseraufruf fa_alt;
    fa_alt.set_text(bearbeitungen.at(index_bearb));
    if(fa_alt.text() != fa.text())
    {
        //Bezug mit ändern
        //Frästiefe ändern
        //Z-Pos ändern
        //XY-Pos der direkten Folgezeile mit ändern
        double tiefe_neu = fa.tiefe();
        double pos_z = fa.pos().z();
        for(uint i=index_bearb+1; i<bearbeitungen.count() ;i++)
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
                if(i == index_bearb+1)//direkte folgezeile vom Fräseraufruf
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
                if(i == index_bearb+1)//direkte folgezeile vom Fräseraufruf
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
    zeile_aendern(index_bearb, bearb, true);
}
void MainWindow::slot_fgerade(fraesergerade fg)
{
    int index_bearb = ui->listWidget_bearb->currentRow()-1;//Index-1 weil 1. Zeile WST-Maße sind
    int index_wst = ui->listWidget_dateien->currentRow();
    QString bearb = fg.text();
    ui->listWidget_bearb->item(index_bearb)->setText(fgerade_zu_prgzei(bearb));
    text_zw bearbeitungen = Wste.wst(index_wst)->bearb();
    //Zeile davor mit ändern?:
    if(index_bearb-1 >=0)
    {
        text_zw bearb_vor;
        bearb_vor.set_text(bearbeitungen.at(index_bearb-1),TRENNZ_BEARB_PARAM);
        if(bearb_vor.at(0) == BEARBART_FRAESERAUFRUF)
        {
            fraeseraufruf fa_vor(bearb_vor.text());
            fa_vor.set_pos(fg.sp());
            fa_vor.set_tiefe(fg.tiSta());
            zeile_aendern(index_bearb-1, fa_vor.text(), false);
        }else if(bearb_vor.at(0) == BEARBART_FRAESERGERADE)
        {
            fraesergerade fg_vor(bearb_vor.text());
            fg_vor.set_endpunkt(fg.sp());
            fg_vor.set_tiEnd(fg.tiSta());
            zeile_aendern(index_bearb-1, fg_vor.text(), false);
        }else if(bearb_vor.at(0) == BEARBART_FRAESERBOGEN)
        {
            fraeserbogen fb_vor(bearb_vor.text());
            fb_vor.bog_ptr()->versetze_epu(fg.sp());
            fb_vor.set_tiEnd(fg.tiSta());
            zeile_aendern(index_bearb-1, fb_vor.text(), false);
        }
    }
    //Folgezeile mit ändern?:
    if(index_bearb+1 < bearbeitungen.count())
    {
        text_zw bearb_nach;
        bearb_nach.set_text(bearbeitungen.at(index_bearb+1),TRENNZ_BEARB_PARAM);
        if(bearb_nach.at(0) == BEARBART_FRAESERGERADE)
        {
            fraesergerade fg_nach(bearb_nach.text());
            fg_nach.set_startpunkt(fg.ep());
            fg_nach.set_tiSta(fg.tiEnd());
            zeile_aendern(index_bearb+1, fg_nach.text(), false);
        }else if(bearb_nach.at(0) == BEARBART_FRAESERBOGEN)
        {
            fraeserbogen fb_nach(bearb_nach.text());
            fb_nach.bog_ptr()->versetze_spu(fg.ep());
            fb_nach.set_tiSta(fg.tiEnd());
            zeile_aendern(index_bearb+1, fb_nach.text(), false);
        }
    }
    zeile_aendern(index_bearb, bearb, true);
}
void MainWindow::slot_fbogen(fraeserbogen fb)
{
    int index_bearb = ui->listWidget_bearb->currentRow()-1;//Index-1 weil 1. Zeile WST-Maße sind
    int index_wst = ui->listWidget_dateien->currentRow();

    QString bearb = fb.text();
    ui->listWidget_bearb->item(index_bearb)->setText(fbogen_zu_prgzei(bearb));
    text_zw bearbeitungen = Wste.wst(index_wst)->bearb();
    //Zeile davor mit ändern?:
    if(index_bearb-1 >=0)
    {
        text_zw bearb_vor;
        bearb_vor.set_text(bearbeitungen.at(index_bearb-1),TRENNZ_BEARB_PARAM);
        if(bearb_vor.at(0) == BEARBART_FRAESERAUFRUF)
        {
            fraeseraufruf fa_vor(bearb_vor.text());
            fa_vor.set_pos(fb.sp());
            fa_vor.set_tiefe(fb.tiSta());
            zeile_aendern(index_bearb-1, fa_vor.text(), false);
        }else if(bearb_vor.at(0) == BEARBART_FRAESERGERADE)
        {
            fraesergerade fg_vor(bearb_vor.text());
            fg_vor.set_endpunkt(fb.sp());
            fg_vor.set_tiEnd(fb.tiSta());
            zeile_aendern(index_bearb-1, fg_vor.text(), false);
        }else if(bearb_vor.at(0) == BEARBART_FRAESERBOGEN)
        {
            fraeserbogen fb_vor(bearb_vor.text());
            fb_vor.bog_ptr()->versetze_epu(fb.sp());
            fb_vor.set_tiEnd(fb.tiSta());
            zeile_aendern(index_bearb-1, fb_vor.text(), false);
        }
    }
    //Folgezeile mit ändern?:
    if(index_bearb+1 < bearbeitungen.count())
    {
        text_zw bearb_nach;
        bearb_nach.set_text(bearbeitungen.at(index_bearb+1),TRENNZ_BEARB_PARAM);
        if(bearb_nach.at(0) == BEARBART_FRAESERGERADE)
        {
            fraesergerade fg_nach(bearb_nach.text());
            fg_nach.set_startpunkt(fb.ep());
            fg_nach.set_tiSta(fb.tiEnd());
            zeile_aendern(index_bearb+1, fg_nach.text(), false);
        }else if(bearb_nach.at(0) == BEARBART_FRAESERBOGEN)
        {
            fraeserbogen fb_nach(bearb_nach.text());
            fb_nach.bog_ptr()->versetze_spu(fb.ep());
            fb_nach.set_tiSta(fb.tiEnd());
            zeile_aendern(index_bearb+1, fb_nach.text(), false);
        }
    }
    zeile_aendern(index_bearb, bearb, true);
}
//Manipulation:
int MainWindow::auswahl_erster()
{
    QList<QListWidgetItem*> items = ui->listWidget_bearb->selectedItems();
    int row_erstes = 0;//Nummer des ersten Elementes
    for(int i=0; i<ui->listWidget_bearb->count() ;i++)
    {
        if(ui->listWidget_bearb->item(i)->isSelected())
        {
            row_erstes = i;
            break;
        }
    }
    return row_erstes;
}
int MainWindow::auswahl_letzter()
{
    int erster = auswahl_erster();
    int menge = auswahl_menge();
    return erster+menge-1;
}
int MainWindow::auswahl_menge()
{
    QList<QListWidgetItem*> items = ui->listWidget_bearb->selectedItems();
    return items.count();
}
void MainWindow::get_zeilennummer_bearb(uint nr, bool bearbeiten)
{
    if(nr <= ui->listWidget_bearb->count())
    {
        ui->listWidget_bearb->setCurrentRow(nr);
    }
}
void MainWindow::zeile_bearb_bearbeiten(int zeile_bearb)
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
        int index_masch = Maschinen.get_index(masch_bez);
        wkz = Maschinen.masch(index_masch)->wkzmag();
    }

    if(zeile_bearb == 0)
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
        vorschaufenster.slot_aktualisieren(Wste.wst(index_wst)->geo(wkz), \
                                                                          Wste.wst(index_wst)->geo_aktfkon(wkz), zeile_bearb);
        aktualisiere_fendtertitel();
        return;
    }

    //Zeile Auslesen:
    text_zw bearb;
    bearb.set_text(Wste.wst(index_wst)->bearb_ptr()->at(zeile_bearb-1),TRENNZ_BEARB_PARAM);
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
    }else if(bearb.at(0) == BEARBART_KOMMENTAR)
    {
        Dialog_kommentar_nc dlg;
        dlg.setModal(true);
        connect(&dlg, SIGNAL(signal_kom(kommentar_nc)), this, SLOT(slot_kom(kommentar_nc)));
        dlg.set_data(bearb.text());
        dlg.exec();
    }else if(bearb.at(0) == BEARBART_HALT)
    {
        Dialog_bearb_halt dlg;
        dlg.setModal(true);
        connect(&dlg, SIGNAL(signal_halt(halt_nc)), this, SLOT(slot_halt(halt_nc)));
        dlg.set_data(bearb.text());
        dlg.exec();
    }else if(bearb.at(0) == BEARBART_GEZUPU)
    {
        Dialog_bearb_gezupu dlg;
        dlg.setModal(true);
        connect(&dlg, SIGNAL(signal_gzp(gehezupunkt)), this, SLOT(slot_gzp(gehezupunkt)));
        dlg.set_data(bearb.text(), Wste.wst(index_wst));
        dlg.exec();
    }
}
void MainWindow::on_actionVerschieben_triggered()
{
    if((ui->listWidget_bearb->currentIndex().isValid())  &&  \
        (ui->listWidget_bearb->currentItem()->isSelected())    )
    {
        int index_wst = ui->listWidget_dateien->currentRow();
        //Prüfen ob Fräsbahnen durch das Verschieben geteilt werden:
        bool gesund = true;
        //--Prüfen ob eine Fräsbahn nach der Auswahl weiter geht:
        if(auswahl_letzter() < ui->listWidget_bearb->count())
        {
            int zeile_dannach = auswahl_letzter();//index von QListwidget
            text_zw bearb;
            bearb.set_text(Wste.wst(index_wst)->bearb_ptr()->at(zeile_dannach),TRENNZ_BEARB_PARAM);
            if(bearb.at(0) == BEARBART_FRAESERGERADE  ||  bearb.at(0) == BEARBART_FRAESERBOGEN)
            {
                gesund = false;
            }
        }
        //---Prüfen ob eine Fräsbahn vor der Auswahl beginnt:
        if(auswahl_erster() >= 2)
        {
            int zeile_davor = auswahl_erster()-1;//index von QListwidget
            text_zw bearb;
            bearb.set_text(Wste.wst(index_wst)->bearb_ptr()->at(zeile_davor),TRENNZ_BEARB_PARAM);
            text_zw bearb_erster;
            bearb_erster.set_text(Wste.wst(index_wst)->bearb_ptr()->at(auswahl_erster()-1),TRENNZ_BEARB_PARAM);
            if(bearb_erster.at(0) != BEARBART_FRAESERAUFRUF)
            {
                if(bearb.at(0) == BEARBART_FRAESERAUFRUF  || \
                                                                 bearb.at(0) == BEARBART_FRAESERGERADE  || \
                           bearb.at(0) == BEARBART_FRAESERBOGEN)
                {
                    gesund = false;
                }
            }
        }
        //---
        if(gesund == false)
        {
            QMessageBox mb;
            mb.setWindowTitle("Bearbeitung verschieben");
            mb.setText("Das Verschieben dieser Zeilenauswahl ist nicht möglich weil eine Fräsbahn nur vollständig verschoben werden kann!");
            mb.exec();
            return;
        }
        //---
        Dialog_bearb_verschieben dlg;
        dlg.setModal(true);
        connect(&dlg, SIGNAL(signal_punkt(punkt3d)), this, SLOT(slot_verschieben(punkt3d)));
        dlg.exec();
    }else
    {
        QMessageBox mb;
        mb.setWindowTitle("Bearbeitung verschieben");
        mb.setText("Sie haben noch keine Bearbeitung ausgewält die verschoben werden kann!");
        mb.exec();
    }
}
void MainWindow::slot_verschieben(punkt3d p)
{
    int index_bearb = ui->listWidget_bearb->currentRow()-1;//index der Bearbeitung, nicht vom listwidget
    int index_wst = ui->listWidget_dateien->currentRow();
    //Die index-Prüfung erfolgt bereits in der Funktion on_actionVerschieben_triggered()
    int row_erstes = auswahl_erster()-1;//-1 weil index der Bearbeitung, nicht vom listwidget
    int items_menge = auswahl_menge();
    if(items_menge==1)
    {
        if(index_bearb >= 0  &&  index_bearb+1 < ui->listWidget_bearb->count())
        {
            QString bearb = Wste.wst(index_wst)->bearb_ptr()->at(index_bearb);
            double l = Wste.wst(index_wst)->laenge();
            double b = Wste.wst(index_wst)->breite();
            bearb = verschiebe_bearb_einen(bearb, p.x(), p.y(), p.z(), l, l, b, b);
            zeile_aendern(index_bearb, bearb, true);
        }
    }else
    {
        index_bearb = row_erstes;
        if(index_bearb < 0)
        {
            index_bearb = 0;
            items_menge = items_menge-1;
        }
        if(index_bearb+items_menge >= ui->listWidget_bearb->count())
        {
            items_menge = ui->listWidget_bearb->count()-index_bearb-1;
        }
        for (int i=0 ; i<items_menge ; i++)
        {
            QString bearb = Wste.wst(index_wst)->bearb_ptr()->at(index_bearb+i);
            double l = Wste.wst(index_wst)->laenge();
            double b = Wste.wst(index_wst)->breite();
            bearb = verschiebe_bearb_einen(bearb, p.x(), p.y(), p.z(), l, l, b, b);
            zeile_aendern(index_bearb+i, bearb, false);
        }
        Wste.wst(index_wst)->unredo_neu();
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
    }else if(tz.at(0) == BEARBART_GEZUPU)
    {
        gehezupunkt gzp;
        gzp.set_text(bearb);
        gzp.set_x(gzp.x()+ax);
        gzp.set_y(gzp.y()+ay);
        bearb = gzp.text();
    }
    return bearb;
}
void MainWindow::on_actionSchnellaenderung_triggered()
{
    if((ui->listWidget_bearb->currentIndex().isValid())  &&  \
        (ui->listWidget_bearb->currentItem()->isSelected())    )
    {
        int index_wst = ui->listWidget_dateien->currentRow();

        int auswahl_erst;// = auswahl_erster()-1;
        int auswahl_meng;// = auswahl_menge();
        if(auswahl_erster() == 0)//Programmkopf
        {
            auswahl_erst = 0;//index 0 der Bearbeitung
            auswahl_meng = auswahl_menge()-1;//Zeile mit Programmkopf von der Menge abziehen
        }else
        {
            auswahl_erst = auswahl_erster()-1;//Indexverschiebung listwidget zu bearb
            auswahl_meng = auswahl_menge();
        }
        if(  auswahl_erst >= 0  &&  auswahl_meng > 0  )
        {
            Dialog_schnellaenderung dlg;
            dlg.set_Data(Wste.wst(index_wst), auswahl_erst, auswahl_meng);
            connect(&dlg, SIGNAL(werte_wurden_angepasst()), this, SLOT(update_listwid_bearb()));
            connect(&dlg, SIGNAL(werte_wurden_angepasst()), this, SLOT(update_vorschau()));
            dlg.exec();
        }else
        {
            QMessageBox mb;
            mb.setWindowTitle("Schnelländerung");
            mb.setText("Sie haben noch keine Bearbeitungen ausgewält die geändert werden sollen!");
            mb.exec();
        }
    }else
    {
        QMessageBox mb;
        mb.setWindowTitle("Schnelländerung");
        mb.setText("Sie haben noch keine Bearbeitungen ausgewält die geändert werden sollen!");
        mb.exec();
    }
}
void MainWindow::zeile_bearb_afb_umkehren(int zeile_bearb)
{
    if(ui->listWidget_dateien->currentRow() < 0)
    {
        QMessageBox mb;
        mb.setText("Es ist kein Werkstück ausgewählt!");
        mb.setWindowTitle("Ausführbedingung der Bearbeitung umkehren");
        mb.exec();
        return;
    }
    if(zeile_bearb < 0)
    {
        return;
    }
    int index_wst = ui->listWidget_dateien->currentRow();
    if(Wste.wst(index_wst))
    {
        text_zw *alle_bearb = Wste.wst(index_wst)->bearb_ptr();
        if(alle_bearb)
        {
            QString akt_bearb = alle_bearb->at(zeile_bearb);
            text_zw zeile;
            zeile.set_text(akt_bearb,TRENNZ_BEARB_PARAM);
            if(zeile.at(0) == BEARBART_BOHR)
            {
                bohrung item(zeile.text());
                double afb = ausdruck_auswerten(item.afb()).toDouble();
                if(afb <= 0)
                {
                    item.set_afb("1");
                }else
                {
                    item.set_afb("0");
                }
                alle_bearb->edit(zeile_bearb, item.text());
            }else if(zeile.at(0) == BEARBART_BOHRRASTER)
            {
                bohrraster item(zeile.text());
                double afb = ausdruck_auswerten(item.afb()).toDouble();
                if(afb <= 0)
                {
                    item.set_afb("1");
                }else
                {
                    item.set_afb("0");
                }
                alle_bearb->edit(zeile_bearb, item.text());
            }else if(zeile.at(0) == BEARBART_NUT)
            {
                nut item(zeile.text());
                double afb = ausdruck_auswerten(item.afb()).toDouble();
                if(afb <= 0)
                {
                    item.set_afb("1");
                }else
                {
                    item.set_afb("0");
                }
                alle_bearb->edit(zeile_bearb, item.text());
            }else if(zeile.at(0) == BEARBART_RTA)
            {
                rechtecktasche item(zeile.text());
                double afb = ausdruck_auswerten(item.afb()).toDouble();
                if(afb <= 0)
                {
                    item.set_afb("1");
                }else
                {
                    item.set_afb("0");
                }
                alle_bearb->edit(zeile_bearb, item.text());
            }else if(zeile.at(0) == BEARBART_FRAESERAUFRUF)
            {
                fraeseraufruf item(zeile.text());
                double afb = ausdruck_auswerten(item.afb()).toDouble();
                if(afb <= 0)
                {
                    item.set_afb("1");
                }else
                {
                    item.set_afb("0");
                }
                alle_bearb->edit(zeile_bearb, item.text());
            }else if(zeile.at(0) == BEARBART_FRAESERGERADE)
            {
                fraesergerade item(zeile.text());
                double afb = ausdruck_auswerten(item.afb()).toDouble();
                if(afb <= 0)
                {
                    item.set_afb("1");
                }else
                {
                    item.set_afb("0");
                }
                alle_bearb->edit(zeile_bearb, item.text());
            }else if(zeile.at(0) == BEARBART_FRAESERBOGEN)
            {
                fraeserbogen item(zeile.text());
                double afb = ausdruck_auswerten(item.afb()).toDouble();
                if(afb <= 0)
                {
                    item.set_afb("1");
                }else
                {
                    item.set_afb("0");
                }
                alle_bearb->edit(zeile_bearb, item.text());
            }else if(zeile.at(0) == BEARBART_KOMMENTAR)
            {
                kommentar_nc item(zeile.text());
                double afb = ausdruck_auswerten(item.afb()).toDouble();
                if(afb <= 0)
                {
                    item.set_afb("1");
                }else
                {
                    item.set_afb("0");
                }
                alle_bearb->edit(zeile_bearb, item.text());
            }else if(zeile.at(0) == BEARBART_HALT)
            {
                halt_nc item(zeile.text());
                double afb = ausdruck_auswerten(item.afb()).toDouble();
                if(afb <= 0)
                {
                    item.set_afb("1");
                }else
                {
                    item.set_afb("0");
                }
                alle_bearb->edit(zeile_bearb, item.text());
            }else if(zeile.at(0) == BEARBART_GEZUPU)
            {
                gehezupunkt item(zeile.text());
                double afb = ausdruck_auswerten(item.afb()).toDouble();
                if(afb <= 0)
                {
                    item.set_afb("1");
                }else
                {
                    item.set_afb("0");
                }
                alle_bearb->edit(zeile_bearb, item.text());
            }
            update_listwid_bearb();
            update_vorschau();
            aktualisiere_fendtertitel();
        }
    }
}
//Einstellungen:
void MainWindow::getEinstellung(einstellung e)
{
    Einstellung = e;
    schreibe_ini();
}
void MainWindow::on_actionPfade_triggered()
{
    emit sendEinstellungPfade(Einstellung);
}
void MainWindow::on_actionCNC_Maschinen_triggered()
{
    emit sendMaschinen(Maschinen);
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
void MainWindow::on_actionDXF_Grundeinstellung_triggered()
{
    emit sendEinstellungDxf(Einstellung_dxf);
}
void MainWindow::getEinstellungDxf(einstellung_dxf e)
{
    Einstellung_dxf = e;

    QFile file(PrgPfade.path_ini_dxf());
    if(!file.open(QIODevice::WriteOnly | QIODevice::Text))
    {
        QString tmp = "Fehler beim Dateizugriff!\n";
        tmp += PrgPfade.path_ini_dxf();
        tmp += "\n";
        tmp += "in der Funktion getEinstellungDxf";
        QMessageBox::warning(this,"Fehler",tmp,QMessageBox::Ok);
    }else
    {
        file.write(Einstellung_dxf.text().toLatin1());
    }
    file.close();
}
void MainWindow::on_actionDXF_Klasseneinstellung_triggered()
{
    emit sendEinstellungDxfKlassen(Einstellung_dxf, Einstellung_dxf_klassen);
}
void MainWindow::getEinstellungDxfKlassen(einstellung_dxf_klassen e)
{
    Einstellung_dxf_klassen = e;

    QFile file(PrgPfade.path_ini_dxf_klassen());
    if(!file.open(QIODevice::WriteOnly | QIODevice::Text))
    {
        QString tmp = "Fehler beim Dateizugriff!\n";
        tmp += PrgPfade.path_ini_dxf_klassen();
        tmp += "\n";
        tmp += "in der Funktion getEinstellungDxfKlassen";
        QMessageBox::warning(this,"Fehler",tmp,QMessageBox::Ok);
    }else
    {
        file.write(Einstellung_dxf_klassen.text().toLatin1());
    }
    file.close();
}
//ListWidget Datei:
void MainWindow::on_listWidget_dateien_currentRowChanged(int currentRow)
{
    uint row = currentRow;
    if(Wste.wst(row))
    {
        werkstueck *w = Wste.wst(row);
        update_listwidget_bearb(w);
        update_vorschau();
    }else
    {
        ui->listWidget_bearb->clear();
        set_vorschaufenster_default();
    }
    aktualisiere_fendtertitel();
}
void MainWindow::aktualisiere_listwidget_dateien(int akt_index)
{
    ui->listWidget_dateien->clear();
    for(uint i=0; i<Wste.anzahl();i++)
    {
        ui->listWidget_dateien->addItem(Wste.wst(i)->name());
    }
    ui->listWidget_dateien->setCurrentRow(akt_index);
}
//ListWidget Bearbeitung
void MainWindow::on_listWidget_bearb_currentRowChanged(int currentRow)
{
    vorschaufenster.slot_aktives_Element_einfaerben(currentRow);
}
void MainWindow::on_listWidget_bearb_itemDoubleClicked(QListWidgetItem *item)
{
    int index_bearb = ui->listWidget_bearb->currentRow();
    zeile_bearb_bearbeiten(index_bearb);
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
        farbe.setRgb(255,255,255);//weiß
        int deckkraft = 160;
        QString bezug = zeile.at(1);
        double afb = 1;
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
            bohrung bo(zeile.text());
            afb = ausdruck_auswerten(bo.afb()).toDouble();
        }else if(zeile.at(0) == BEARBART_BOHRRASTER)
        {
            bearb = bohrRaster_zu_prgzei(zeile.text());
            bohrraster bora(zeile.text());
            afb = ausdruck_auswerten(bora.afb()).toDouble();
        }else if(zeile.at(0) == BEARBART_NUT)
        {
            bearb = nut_zu_prgzei(zeile.text());
            farbe.setRgb(145,145,255,deckkraft);//helles lila
            nut nu(zeile.text());
            afb = ausdruck_auswerten(nu.afb()).toDouble();
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
            rechtecktasche rta(zeile.text());
            afb = ausdruck_auswerten(rta.afb()).toDouble();
        }else if(zeile.at(0) == BEARBART_FRAESERAUFRUF)
        {
            bearb = fauf_zu_prgzei(zeile.text());
            farbe.setRgb(255,128,0,deckkraft);//orange
            fraeseraufruf fa(zeile.text());
            afb = ausdruck_auswerten(fa.afb()).toDouble();
        }else if(zeile.at(0) == BEARBART_FRAESERGERADE)
        {
            bearb = fgerade_zu_prgzei(zeile.text());
            farbe.setRgb(255,155,106,deckkraft);//helles orange
            fraesergerade fg(zeile.text());
            afb = ausdruck_auswerten(fg.afb()).toDouble();
        }else if(zeile.at(0) == BEARBART_FRAESERBOGEN)
        {
            bearb = fbogen_zu_prgzei(zeile.text());
            farbe.setRgb(255,155,106,deckkraft);//helles orange
            fraeserbogen fb(zeile.text());
            afb = ausdruck_auswerten(fb.afb()).toDouble();
        }else if(zeile.at(0) == BEARBART_KOMMENTAR)
        {
            bearb = kom_zu_prgzei(zeile.text());
            farbe.setRgb(255,255,255,deckkraft);//weiß
            kommentar_nc ko(zeile.text());
            afb = ausdruck_auswerten(ko.afb()).toDouble();
        }else if(zeile.at(0) == BEARBART_HALT)
        {
            bearb = halt_zu_prgzei(zeile.text());
            farbe.setRgb(255,255,255,deckkraft);//weiß
            halt_nc halt(zeile.text());
            afb = ausdruck_auswerten(halt.afb()).toDouble();
        }else if(zeile.at(0) == BEARBART_GEZUPU)
        {
            bearb = gezupu_zu_prgzei(zeile.text());
            farbe.setRgb(255,255,255,deckkraft);//weiß
            gehezupunkt gzp(zeile.text());
            afb = ausdruck_auswerten(gzp.afb()).toDouble();
        }

        ui->listWidget_bearb->addItem(bearb);
        ui->listWidget_bearb->item(i+1)->setBackground(farbe);

        if(afb <= 0)
        {
            QColor textfarbe;
            textfarbe.setRgb(42,110,186);
            ui->listWidget_bearb->item(i+1)->setForeground(textfarbe);
            QFont font = ui->listWidget_bearb->item(i+1)->font(); // Aktuelle Schriftart abrufen
            font.setItalic(true);                                 // Kursiv-Attribut aktivieren
            ui->listWidget_bearb->item(i+1)->setFont(font);       // Geänderte Schriftart zurückweisen
        }

    }
    ui->listWidget_bearb->addItem("...");
    if(currentRow < ui->listWidget_bearb->count())
    {
        ui->listWidget_bearb->setCurrentRow(currentRow);
    }
}
//------------------------------------------------------














