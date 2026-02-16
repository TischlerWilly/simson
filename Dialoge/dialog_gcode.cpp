#include "dialog_gcode.h"
#include "ui_dialog_gcode.h"

Dialog_GCode::Dialog_GCode(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::Dialog_GCode)
{
    ui->setupUi(this);
}

Dialog_GCode::~Dialog_GCode()
{
    delete ui;
}
void Dialog_GCode::set_maschine(maschine *m)
{
    Maschine = m;
    ui->lineEdit_maschine->setText(Maschine->name());
}
void Dialog_GCode::set_wst(werkstueck *w)
{
    Wst = w;
    ui->lineEdit_datei->setText(Wst->dateipfad());
}
