#include "dialog_bearb_pkopf.h"
#include "ui_dialog_bearb_pkopf.h"

Dialog_bearb_pkopf::Dialog_bearb_pkopf(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialog_bearb_pkopf)
{
    ui->setupUi(this);
    this->setWindowTitle("Programmkopf");
    Wst = nullptr;
    ui->pushButton_ok->setFocus();
}

Dialog_bearb_pkopf::~Dialog_bearb_pkopf()
{
    delete ui;
}

void Dialog_bearb_pkopf::set_data(werkstueck *w)
{
    Wst = w;
    ui->lineEdit_l->setText(Wst->laenge_qstring());
    ui->lineEdit_b->setText(Wst->breite_qstring());
    ui->lineEdit_d->setText(Wst->dicke_qstring());
    ui->lineEdit_ax->setText(Wst->versatz_x_qstring());
    ui->lineEdit_ay->setText(Wst->versatz_y_qstring());
    ui->lineEdit_az->setText(Wst->versatz_z_qstring());
}

void Dialog_bearb_pkopf::on_pushButton_ok_clicked()
{
    Wst->set_laenge(berechnen(ui->lineEdit_l->text()));
    Wst->set_breite(berechnen(ui->lineEdit_b->text()));
    Wst->set_dicke(berechnen(ui->lineEdit_d->text()));
    Wst->set_versatz_x(ui->lineEdit_ax->text().toDouble());
    Wst->set_versatz_y(ui->lineEdit_ay->text().toDouble());
    Wst->set_versatz_z(ui->lineEdit_az->text().toDouble());
    this->close();
}

void Dialog_bearb_pkopf::on_pushButton_abbrechen_clicked()
{
    this->close();
}
