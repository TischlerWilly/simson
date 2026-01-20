#include "dialog_maschinen.h"
#include "ui_dialog_maschinen.h"

Dialog_maschinen::Dialog_maschinen(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::Dialog_maschinen)
{
    ui->setupUi(this);
}

Dialog_maschinen::~Dialog_maschinen()
{
    delete ui;
}

void Dialog_maschinen::slot_maschinen(maschinen m)
{
    Maschinen = m;
    ui->listWidget_maschinen->clear();
    ui->lineEdit_tischlaenge->clear();
    ui->lineEdit_tischbreite->clear();
    for(uint i=0; i<m.anzahl();i++)
    {
        ui->listWidget_maschinen->addItem(m.masch(i)->name());
    }
    this->show();
}

void Dialog_maschinen::on_pushButton_abbrechen_clicked()
{
    this->close();
}
void Dialog_maschinen::on_pushButton_laenge_ok_clicked()
{
    if(Maschinen.masch(ui->listWidget_maschinen->currentRow()))
    {
        QString laenge = ausdruck_auswerten(ui->lineEdit_tischlaenge->text());
        Maschinen.masch(ui->listWidget_maschinen->currentRow())->set_laenge(laenge);
    }
}
void Dialog_maschinen::on_pushButton_breite_ok_clicked()
{
    if(Maschinen.masch(ui->listWidget_maschinen->currentRow()))
    {
        QString breite = ausdruck_auswerten(ui->lineEdit_tischbreite->text());
        Maschinen.masch(ui->listWidget_maschinen->currentRow())->set_breite(breite);
    }
}
void Dialog_maschinen::on_pushButton_ok_clicked()
{
    this->close();
    emit send_maschinen(Maschinen);
}
void Dialog_maschinen::on_listWidget_maschinen_currentRowChanged(int currentRow)
{
    if(Maschinen.masch(currentRow))
    {
        ui->lineEdit_tischlaenge->setText(Maschinen.masch(currentRow)->laenge_qstring());
        ui->lineEdit_tischbreite->setText(Maschinen.masch(currentRow)->breite_qstring());
        prgpfade pf;
        ui->label_speierort->setText(pf.path_masch_dir(Maschinen.masch(currentRow)->name()));
    }
}
void Dialog_maschinen::on_pushButton_neue_maschine_clicked()
{
    QString name = "neue Maschine";
    bool ok;
    QString fenstertitel = "CNC-Maschine benennen";
    QString hinweistext  = "Name der Maschine:";
    QString vorgebewert  = name;
    QString neuer_name = QInputDialog::getText(this, fenstertitel,
                                               hinweistext, QLineEdit::Normal,
                                               name, &ok);
    if (ok && !neuer_name.isEmpty())
    {
        if(Maschinen.ist_bekannt(neuer_name))
        {
            QString msg;
            msg  = "Die Maschine kann nicht erzeugt werden.";
            msg += "\n";
            msg += "Es gibt bereits eine Maschine mit diesem Namen ";
            msg += neuer_name;
            msg += ".";
            QMessageBox mb;
            mb.setText(msg);
            mb.exec();
        }else
        {
            Maschinen.neu(neuer_name);
            ui->listWidget_maschinen->addItem(neuer_name);
        }
    }
}

