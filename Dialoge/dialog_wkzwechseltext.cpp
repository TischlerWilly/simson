#include "dialog_wkzwechseltext.h"
#include "ui_dialog_wkzwechseltext.h"

Dialog_wkzWechselText::Dialog_wkzWechselText(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::Dialog_wkzWechselText)
{
    ui->setupUi(this);
}

Dialog_wkzWechselText::~Dialog_wkzWechselText()
{
    delete ui;
}

void Dialog_wkzWechselText::set_wkzWechselText(QString text)
{
    ui->plainTextEdit->setPlainText(text);
}

void Dialog_wkzWechselText::on_pushButton_ok_clicked()
{
    emit wkzWechselText(ui->plainTextEdit->toPlainText());
    this->close();
}


void Dialog_wkzWechselText::on_pushButton_abbrechen_clicked()
{
    this->close();
}

