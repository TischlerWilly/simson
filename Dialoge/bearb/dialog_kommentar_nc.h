#ifndef DIALOG_KOMMENTAR_NC_H
#define DIALOG_KOMMENTAR_NC_H

#include <QDialog>

#include "Klassen/wst/kommentar_nc.h"

namespace Ui {
class Dialog_kommentar_nc;
}

class Dialog_kommentar_nc : public QDialog
{
    Q_OBJECT

public:
    explicit Dialog_kommentar_nc(QWidget *parent = nullptr);
    ~Dialog_kommentar_nc();
    void set_data(QString d);

private slots:
    void on_btn_ok_clicked();
    void on_btn_abbrechen_clicked();

signals:
    void signal_kom(kommentar_nc ko);

private:
    Ui::Dialog_kommentar_nc *ui;


};

#endif // DIALOG_KOMMENTAR_NC_H
