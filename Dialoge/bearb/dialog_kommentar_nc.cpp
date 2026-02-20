#include "dialog_kommentar_nc.h"
#include "ui_dialog_kommentar_nc.h"

Dialog_kommentar_nc::Dialog_kommentar_nc(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::Dialog_kommentar_nc)
{
    ui->setupUi(this);
    this->setWindowTitle("NC-Kommentar");
    ui->btn_ok->setFocus();
}

Dialog_kommentar_nc::~Dialog_kommentar_nc()
{
    delete ui;
}

void Dialog_kommentar_nc::set_data(QString d)
{
    kommentar_nc ko;
    ko.set_text(d);
    ui->lineEdit_kom->setText(ko.kom());
    ui->lineEdit_afb->setText(ko.afb());
}

void Dialog_kommentar_nc::on_btn_ok_clicked()
{
    kommentar_nc ko;
    ko.set_kom(ui->lineEdit_kom->text());
    ko.set_afb(ui->lineEdit_afb->text());

    emit signal_kom(ko);
    this->close();
}


void Dialog_kommentar_nc::on_btn_abbrechen_clicked()
{
    this->close();
}

