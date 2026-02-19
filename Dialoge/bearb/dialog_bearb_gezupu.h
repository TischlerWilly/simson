#ifndef DIALOG_BEARB_GEZUPU_H
#define DIALOG_BEARB_GEZUPU_H

#include <QDialog>

#include "Klassen/wst/gehezupunkt.h"
#include "Klassen/wenndannsonst.h"
#include "Klassen/wst/werkstueck.h"

namespace Ui {
class Dialog_bearb_gezupu;
}

class Dialog_bearb_gezupu : public QDialog
{
    Q_OBJECT

public:
    explicit Dialog_bearb_gezupu(QWidget *parent = nullptr);
    ~Dialog_bearb_gezupu();
    void set_data(QString d, werkstueck *w);

private slots:
    void on_btn_ok_clicked();
    void on_btn_abbrechen_clicked();

signals:
    void signal_gzp(gehezupunkt gzp);

private:
    Ui::Dialog_bearb_gezupu *ui;
    werkstueck *Wst;
    QString var_zu_wert(QString term);
};

#endif // DIALOG_BEARB_GEZUPU_H
