#ifndef DIALOG_GCODE_H
#define DIALOG_GCODE_H

#include <QDialog>
#include "Klassen/wkz/maschine.h"
#include "Klassen/wst/werkstueck.h"
#include "Klassen/gcode/emc2.h"

namespace Ui {
class Dialog_GCode;
}

class Dialog_GCode : public QDialog
{
    Q_OBJECT

public:
    explicit Dialog_GCode(QWidget *parent = nullptr);
    ~Dialog_GCode();
    void set_maschine(maschine *m);
    void set_wst(werkstueck *w);

private slots:
    void on_pushButton_erstellen_clicked();

    void on_pushButton_speichern_clicked();

private:
    Ui::Dialog_GCode *ui;
    maschine *Maschine;
    werkstueck *Wst;
};

#endif // DIALOG_GCODE_H
