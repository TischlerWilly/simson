#include "dialog_bohrer.h"
#include "ui_dialog_bohrer.h"

Dialog_bohrer::Dialog_bohrer(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialog_bohrer)
{
    ui->setupUi(this);
}

Dialog_bohrer::~Dialog_bohrer()
{
    delete ui;
}

void Dialog_bohrer::set_Data(text_zw msg, bool ist_neues_wkz)
{
    clear();
    Wkz_ist_neu = ist_neues_wkz;
    wkz_bohrer wkz(msg);
    ui->lineEdit_nr->setText(wkz.wkznr());
    ui->lineEdit_dm_real->setText(double_to_qstring(wkz.dmexport()));
    ui->lineEdit_dm_cad->setText(double_to_qstring(wkz.dmimport()));
    ui->lineEdit_nutzlaenge->setText(double_to_qstring(wkz.nutzl()));
    ui->lineEdit_zustmassvert->setText(double_to_qstring(wkz.zustmavert()));
    ui->lineEdit_zustmasshori->setText(double_to_qstring(wkz.zustmahori()));
    ui->checkBox_istdubo->setChecked(wkz.istdubo());
    ui->checkBox_hori->setChecked(wkz.isthori());
    ui->checkBox_verti->setChecked(wkz.istverti());
    ui->checkBox_ist_aktiv->setChecked(wkz.istaktiv());
    ui->lineEdit_vorschub->setText(double_to_qstring(wkz.vorschub()));
    this->show();
}

void Dialog_bohrer::neuerBohrer()
{
    clear();
    Wkz_ist_neu = true;
    this->show();
}

void Dialog_bohrer::clear()
{
    ui->lineEdit_dm_cad->clear();
    ui->lineEdit_dm_real->clear();
    ui->lineEdit_nr->clear();
    ui->lineEdit_nutzlaenge->clear();
    ui->lineEdit_zustmassvert->clear();
    ui->lineEdit_zustmasshori->clear();
    ui->checkBox_istdubo->setChecked(false);
    ui->checkBox_verti->setChecked(false);
    ui->checkBox_hori->setChecked(false);
    ui->checkBox_ist_aktiv->setChecked(true);
    ui->lineEdit_vorschub->clear();
}

void Dialog_bohrer::on_pushButton_abbrechen_clicked()
{
    this->hide();
}

void Dialog_bohrer::on_pushButton_ok_clicked()
{
    if(ui->lineEdit_dm_cad->text().isEmpty())
    {
        QMessageBox mb;
        mb.setText("Bitte zuerst einen Import-Durchmesser eintragen!");
        mb.setWindowTitle("Daten unvollständig");
        mb.exec();
    }else if(ui->lineEdit_dm_real->text().isEmpty())
    {
        QMessageBox mb;
        mb.setText("Bitte zuerst einen Export-Durchmesser eintragen!");
        mb.setWindowTitle("Daten unvollständig");
        mb.exec();
    }else if(ui->lineEdit_nutzlaenge->text().isEmpty())
    {
        QMessageBox mb;
        mb.setText("Bitte zuerst eine Nutzlänge eintragen!");
        mb.setWindowTitle("Daten unvollständig");
        mb.exec();
    }else if(ui->lineEdit_zustmasshori->text().isEmpty())
    {
        QMessageBox mb;
        mb.setText("Bitte zuerst eine horizontales Zustellmaß eintragen!");
        mb.setWindowTitle("Daten unvollständig");
        mb.exec();
    }else if(ui->lineEdit_zustmassvert->text().isEmpty())
    {
        QMessageBox mb;
        mb.setText("Bitte zuerst eine vertikales Zustellmaß eintragen!");
        mb.setWindowTitle("Daten unvollständig");
        mb.exec();
    }else if(ui->lineEdit_vorschub->text().isEmpty())
    {
        QMessageBox mb;
        mb.setText("Bitte zuerst den Vorschub eintragen!");
        mb.setWindowTitle("Daten unvollständig");
        mb.exec();
    }else if(berechnen(ui->lineEdit_vorschub->text()).toDouble() <= 0)
    {
        QMessageBox mb;
        mb.setText("Der Vorschub muss größer als 0 sein");
        mb.setWindowTitle("Daten fehlerhaft");
        mb.exec();
    }else
    {
        this->hide();
        wkz_bohrer bohrer;
        bohrer.set_wkznr(ui->lineEdit_nr->text());
        bohrer.set_istaktiv(ui->checkBox_ist_aktiv->isChecked());
        bohrer.set_dmimport(berechnen(ui->lineEdit_dm_cad->text()).toDouble());
        bohrer.set_dmexport(berechnen(ui->lineEdit_dm_real->text()).toDouble());
        bohrer.set_nutzl(berechnen(ui->lineEdit_nutzlaenge->text()).toDouble());
        bohrer.set_zustmavert(berechnen(ui->lineEdit_zustmassvert->text()).toDouble());
        bohrer.set_zustmahori(berechnen(ui->lineEdit_zustmasshori->text()).toDouble());
        bohrer.set_istdubo(ui->checkBox_istdubo->isChecked());
        bohrer.set_isthori(ui->checkBox_hori->isChecked());
        bohrer.set_istverti(ui->checkBox_verti->isChecked());
        bohrer.set_vorschub(berechnen(ui->lineEdit_vorschub->text()).toDouble());

        emit Data(bohrer.daten(), Wkz_ist_neu);
    }

}
