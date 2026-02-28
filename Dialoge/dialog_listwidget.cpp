#include "dialog_listwidget.h"
#include "ui_dialog_listwidget.h"

Dialog_listwidget::Dialog_listwidget(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::Dialog_listwidget)
{
    ui->setupUi(this);
}

Dialog_listwidget::~Dialog_listwidget()
{
    delete ui;
}
void Dialog_listwidget::set_geotext(geo_text gt)
{
    ui->listWidget->clear();
    for(uint i=0;i<gt.count();i++)
    {
        text_zw spalten = gt.at(i);
        QString msg = "(";
        msg += int_to_qstring(i);
        msg += ") ";
        msg += spalten.text();
        ui->listWidget->addItem(msg);
    }
}

void Dialog_listwidget::resizeEvent(QResizeEvent *event)
{
    ui->listWidget->move(5,5);
    ui->listWidget->setFixedWidth(this->width()-10);
    ui->listWidget->setFixedHeight(this->height()-10);
}

void Dialog_listwidget::on_listWidget_currentRowChanged(int currentRow)
{
    emit signal_index_changed(currentRow);
}

