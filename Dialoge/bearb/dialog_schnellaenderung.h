#ifndef DIALOG_SCHNELLAENDERUNG_H
#define DIALOG_SCHNELLAENDERUNG_H

#include <QDialog>
#include <QMessageBox>
#include "Klassen/text_zw.h"
#include "Funktionen/umwandeln.h"
#include "Funktionen/text.h"
#include "Klassen/wst/werkstueck.h"

namespace Ui {
class Dialog_schnellaenderung;
}

class Dialog_schnellaenderung : public QDialog
{
    Q_OBJECT

public:
    explicit Dialog_schnellaenderung(QWidget *parent = NULL);
    ~Dialog_schnellaenderung();

private slots:
    void on_comboBox_dlg_currentIndexChanged(int index);
    void on_comboBox_param_currentIndexChanged();
    void on_pushButton_werte_aendern_clicked();
    void on_pushButton_schliessen_clicked();
    void on_comboBox_alt_currentIndexChanged(int index);

public slots:
    void set_Data(werkstueck *w, uint start, uint menge);

signals:
    void werte_wurden_angepasst();


private:
    Ui::Dialog_schnellaenderung *ui;
    werkstueck *Wst;
    uint Startzeile;
    uint Menge;
    text_zw Dialoge;
    text_zw Param_alle;
    text_zw Param_bohr;
    text_zw Param_nut;
    text_zw Param_kta;
    text_zw Param_rta;
    text_zw Param_fauf;

    void werte_ermitteln();
};

#endif // DIALOG_SCHNELLAENDERUNG_H
