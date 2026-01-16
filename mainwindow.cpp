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
    s.set_stapu(0,-50,0);
    s.set_endpu(0,50,0);
    gt.add_strecke(s);
    gt.zeilenvorschub();

    punkt3d p(500,500,0);
    gt.add_punkt(p);

    vorschaufenster.slot_aktualisieren(gt, 0);

    connect(&vorschaufenster, SIGNAL(sende_maus_pos(QPoint)),\
            this, SLOT(getMausPosXY(QPoint)));
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

