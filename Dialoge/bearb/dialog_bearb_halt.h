#ifndef DIALOG_BEARB_HALT_H
#define DIALOG_BEARB_HALT_H

#include <QDialog>

#include "Klassen/wst/halt_nc.h"

namespace Ui {
class Dialog_bearb_halt;
}

class Dialog_bearb_halt : public QDialog
{
    Q_OBJECT

public:
    explicit Dialog_bearb_halt(QWidget *parent = nullptr);
    ~Dialog_bearb_halt();
    void set_data(QString d);

private slots:
    void on_btn_ok_clicked();
    void on_btn_abbrechen_clicked();

signals:
    void signal_halt(halt_nc ha);

private:
    Ui::Dialog_bearb_halt *ui;
};

#endif // DIALOG_BEARB_HALT_H
