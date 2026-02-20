#include "dialog_bearb_halt.h"
#include "ui_dialog_bearb_halt.h"

Dialog_bearb_halt::Dialog_bearb_halt(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::Dialog_bearb_halt)
{
    ui->setupUi(this);
    this->setWindowTitle("NC-Halt");
    ui->btn_ok->setFocus();
}

Dialog_bearb_halt::~Dialog_bearb_halt()
{
    delete ui;
}

void Dialog_bearb_halt::set_data(QString d)
{
    halt_nc ha;
    ha.set_text(d);
    ui->lineEdit_afb->setText(ha.afb());
}

void Dialog_bearb_halt::on_btn_ok_clicked()
{
    halt_nc ha;
    ha.set_afb(ui->lineEdit_afb->text());

    emit signal_halt(ha);
    this->close();
}


void Dialog_bearb_halt::on_btn_abbrechen_clicked()
{
    this->close();
}

