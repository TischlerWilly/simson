#ifndef DIALOG_EINSTELLUNGEN_ALLGEM_H
#define DIALOG_EINSTELLUNGEN_ALLGEM_H

#include <QDialog>
#include "Klassen/einstellung.h"

namespace Ui {
class Dialog_einstellungen_allgem;
}

class Dialog_einstellungen_allgem : public QDialog
{
    Q_OBJECT

public:
    explicit Dialog_einstellungen_allgem(QWidget *parent = nullptr);
    ~Dialog_einstellungen_allgem();
    void set_einstellungen(einstellung e);

public slots:

signals:
    void send_einstellungen(einstellung e);

private slots:
    void on_pushButton_ok_clicked();
    void on_pushButton_abbrechen_clicked();

private:
    Ui::Dialog_einstellungen_allgem *ui;
    einstellung Einstellung;
};

#endif // DIALOG_EINSTELLUNGEN_ALLGEM_H
