#include "dialog_einstellungen_allgem.h"
#include "ui_dialog_einstellungen_allgem.h"

Dialog_einstellungen_allgem::Dialog_einstellungen_allgem(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::Dialog_einstellungen_allgem)
{
    ui->setupUi(this);
}

Dialog_einstellungen_allgem::~Dialog_einstellungen_allgem()
{
    delete ui;
}

void Dialog_einstellungen_allgem::set_einstellungen(einstellung e)
{
    Einstellung = e;
    ui->checkBox_entwicklermodus->setChecked(e.entwicklermodus());
    ui->checkBox_zeigeLeitlinienFkon->setChecked(e.zeigeLeitlinieFkon());
}

void Dialog_einstellungen_allgem::on_pushButton_ok_clicked()
{
    Einstellung.set_entwicklermodus(ui->checkBox_entwicklermodus->isChecked());
    Einstellung.set_zeigeLeitlinieFkon(ui->checkBox_zeigeLeitlinienFkon->isChecked());
    this->close();
    emit send_einstellungen(Einstellung);
}

void Dialog_einstellungen_allgem::on_pushButton_abbrechen_clicked()
{
    this->close();
}

