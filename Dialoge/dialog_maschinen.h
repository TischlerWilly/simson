#ifndef DIALOG_MASCHINEN_H
#define DIALOG_MASCHINEN_H

#include <QDialog>
#include <qinputdialog.h>

#include "Klassen/wkz/maschinen.h"
#include "Funktionen/myfunktion.h"
#include "Klassen/prgpfade.h"
#include "mainwin_wkzmagazin.h"

namespace Ui {
class Dialog_maschinen;
}

class Dialog_maschinen : public QDialog
{
    Q_OBJECT

public:
    explicit Dialog_maschinen(QWidget *parent = nullptr);
    ~Dialog_maschinen();

public slots:
    void slot_maschinen(maschinen m);
    void getDialogDataWKZ(QString fenstertitel, wkz_magazin werkzeugmagazin);
    void getAbbruch();

signals:
    void send_maschinen(maschinen m);

private slots:
    void on_pushButton_abbrechen_clicked();    
    void on_pushButton_ok_clicked();
    void on_listWidget_maschinen_currentRowChanged(int currentRow);
    void on_pushButton_laenge_ok_clicked();
    void on_pushButton_breite_ok_clicked();
    void on_pushButton_neue_maschine_clicked();
    void on_pushButton_wkz_clicked();

private:
    Ui::Dialog_maschinen *ui;
    maschinen Maschinen;

    //Dialoge:
    mainwin_wkzmagazin dlg_wkzmag;

};

#endif // DIALOG_MASCHINEN_H
