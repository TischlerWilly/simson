#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
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

    bogen b;
    b.set_mipu(0,0,0);
    b.set_rad(50);
    b.set_swi(degToRad(90-45));
    b.set_ewi(degToRad(270+45));
    gt.add_bogen(b);
    gt.zeilenvorschub();

    rechteck r;
    r.set_mipu(0,0,0);
    r.set_laenge(500);
    r.set_breite(300);
    r.set_drewi(degToRad(5));
    gt.add_rechteck(r);
    gt.zeilenvorschub();

    r.set_drewi(degToRad(5));
    r.set_mipu(600,400,0);
    gt.add_rechteck(r);
    gt.zeilenvorschub();

    r.set_laenge(500-100);
    r.set_breite(300-100);
    gt.add_rechteck(r);
    gt.zeilenvorschub();

    r.set_laenge(500);
    r.set_breite(300);
    r.set_rad(50);
    gt.add_rechteck(r);
    gt.zeilenvorschub();

    vorschaufenster.slot_aktualisieren(gt, 0);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::resizeEvent(QResizeEvent *event)
{
    //---Vorschaufenster:
    vorschaufenster.setParent(this);
    vorschaufenster.move(5,5);
    vorschaufenster.setFixedWidth(this->width()-10);
    vorschaufenster.setFixedHeight(this->height()-10);
    vorschaufenster.slot_aktualisieren();
}
