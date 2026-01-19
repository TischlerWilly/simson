#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->setWindowTitle("Simson 2026.01.12");

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
}

MainWindow::~MainWindow()
{
    delete ui;
}

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

    ui->label_mauspos->move(x, ui->listWidget_bearb->pos().y() + h + 2);
    ui->label_mauspos->setFixedWidth(b);
    ui->label_mauspos->setFixedHeight(20);
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
void MainWindow::on_actionNeu_triggered()
{
    QString name = "Unbekannt";

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

