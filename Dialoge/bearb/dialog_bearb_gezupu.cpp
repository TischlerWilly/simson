#include "dialog_bearb_gezupu.h"
#include "ui_dialog_bearb_gezupu.h"

Dialog_bearb_gezupu::Dialog_bearb_gezupu(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::Dialog_bearb_gezupu)
{
    ui->setupUi(this);
    Wst = nullptr;
    this->setWindowTitle("Gehe zu Punkt");
}

Dialog_bearb_gezupu::~Dialog_bearb_gezupu()
{
    delete ui;
}

void Dialog_bearb_gezupu::set_data(QString d, werkstueck *w)
{
    Wst = w;
    gehezupunkt gzp;
    gzp.set_text(d);
    ui->lineEdit_x->setText(gzp.x_qstring());
    ui->lineEdit_y->setText(gzp.y_qstring());
    ui->lineEdit_z->setText(gzp.z_qstring());
    ui->lineEdit_afb->setText(gzp.afb());
}

QString Dialog_bearb_gezupu::var_zu_wert(QString term)
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

void Dialog_bearb_gezupu::on_btn_ok_clicked()
{
    gehezupunkt gzp;
    gzp.set_x(var_zu_wert(ui->lineEdit_x->text()));
    gzp.set_y(var_zu_wert(ui->lineEdit_y->text()));
    gzp.set_z(var_zu_wert(ui->lineEdit_z->text()));
    gzp.set_afb(ui->lineEdit_afb->text());

    emit signal_gzp(gzp);
    this->close();
}

void Dialog_bearb_gezupu::on_btn_abbrechen_clicked()
{
    this->close();
}

