#include "dialog_bearb_fbogen.h"
#include "ui_dialog_bearb_fbogen.h"

Dialog_bearb_fbogen::Dialog_bearb_fbogen(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialog_bearb_fbogen)
{
    ui->setupUi(this);
    Wst = nullptr;
    this->setWindowTitle("Bogenfräsung");
    ui->comboBox_bezug->addItem("Oberseite");   //0
    ui->comboBox_bezug->addItem("Unterseite");  //1
    //---
    ui->comboBox_uzs->addItem("UZS");   //0
    ui->comboBox_uzs->addItem("GUZS");  //1
    ui->btn_ok->setFocus();
}

Dialog_bearb_fbogen::~Dialog_bearb_fbogen()
{
    delete ui;
}

void Dialog_bearb_fbogen::set_data(QString d, werkstueck *w)
{
    Wst = w;
    fraeserbogen fb;
    fb.set_text(d);
    ui->lineEdit_xs->setText(fb.xs_qstring());
    ui->lineEdit_ys->setText(fb.ys_qstring());
    ui->lineEdit_xe->setText(fb.xe_qstring());
    ui->lineEdit_ye->setText(fb.ye_qstring());
    ui->lineEdit_tiSta->setText(fb.tiSta_qstring());
    ui->lineEdit_tiEnd->setText(fb.tiEnd_qstring());
    ui->lineEdit_rad->setText(fb.rad_qstring());
    //---------
    //UZS:
    if(fb.bog().uzs() == true)
    {
        ui->comboBox_uzs->setCurrentIndex(0);
    }else
    {
        ui->comboBox_uzs->setCurrentIndex(1);
    }
    //---------
    //Bezug:
    if(fb.bezug() == WST_BEZUG_OBSEI)
    {
        ui->comboBox_bezug->setCurrentIndex(0);
    }else if(fb.bezug() == WST_BEZUG_UNSEI)
    {
        ui->comboBox_bezug->setCurrentIndex(1);
    }
    //---------
    ui->lineEdit_afb->setText(fb.afb());
}

QString Dialog_bearb_fbogen::var_zu_wert(QString term)
{
    if(Wst != nullptr)
    {
        term = term.toUpper();
        term.replace("L", Wst->laenge_qstring());
        term.replace("B", Wst->breite_qstring());
        term.replace("D", Wst->dicke_qstring());
        term = berechnen(term);
    }else
    {
        berechnen(term);
    }
    return term;
}

void Dialog_bearb_fbogen::on_btn_ok_clicked()
{
    punkt3d sp, ep;
    sp.set_x(var_zu_wert(ui->lineEdit_xs->text()));
    sp.set_y(var_zu_wert(ui->lineEdit_ys->text()));
    ep.set_x(var_zu_wert(ui->lineEdit_xe->text()));
    ep.set_y(var_zu_wert(ui->lineEdit_ye->text()));
    double rad;
    rad = var_zu_wert(ui->lineEdit_rad->text()).toDouble();

    fraeserbogen fb;
    fb.set_tiSta(var_zu_wert(ui->lineEdit_tiSta->text()).toDouble());
    fb.set_tiEnd(var_zu_wert(ui->lineEdit_tiEnd->text()).toDouble());
    QString uzs = ui->comboBox_uzs->currentText();
    bool uzs_bool;
    if(uzs == "UZS")
    {
        uzs_bool = true;
    }else
    {
        uzs_bool = false;
    }
    fb.bog_ptr()->set_bogen(sp, ep, rad, uzs_bool);

    QString bezug = ui->comboBox_bezug->currentText();
    if(bezug == "Oberseite")
    {
        fb.set_bezug(WST_BEZUG_OBSEI);
    }else if(bezug == "Unterseite")
    {
        fb.set_bezug(WST_BEZUG_UNSEI);
    }
    fb.set_afb(ui->lineEdit_afb->text());

    emit signal_fbogen(fb);
    this->close();
}

void Dialog_bearb_fbogen::on_btn_abbrechen_clicked()
{
    this->close();
}
