#ifndef DIALOG_BEARB_FBOGEN_H
#define DIALOG_BEARB_FBOGEN_H

#include "Klassen/wst/fraeserbogen.h"
#include "Klassen/formel.h"
#include "Klassen/wst/werkstueck.h"
#include <QDialog>
#include <QTimer>

namespace Ui {
class Dialog_bearb_fbogen;
}

class Dialog_bearb_fbogen : public QDialog
{
    Q_OBJECT

public:
    explicit Dialog_bearb_fbogen(QWidget *parent = nullptr);
    ~Dialog_bearb_fbogen();
    void set_data(QString d, werkstueck *w);

private slots:
    void on_btn_ok_clicked();
    void on_btn_abbrechen_clicked();
    void on_lineEdit_xs_editingFinished();
    void on_lineEdit_ys_editingFinished();
    void on_lineEdit_xe_editingFinished();
    void on_lineEdit_ye_editingFinished();
    void on_comboBox_uzs_currentIndexChanged(int index);
    void on_lineEdit_rad_editingFinished();

signals:
    void signal_fbogen(fraeserbogen fb);

protected:
    void showEvent(QShowEvent *event) override;

private:
    Ui::Dialog_bearb_fbogen *ui;
    werkstueck *Wst;
    bogen Bog;
    QString var_zu_wert(QString term);

    void aktualisiere_infofelder();
};

#endif // DIALOG_BEARB_FBOGEN_H
