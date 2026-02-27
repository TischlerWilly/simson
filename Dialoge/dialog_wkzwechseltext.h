#ifndef DIALOG_WKZWECHSELTEXT_H
#define DIALOG_WKZWECHSELTEXT_H

#include <QDialog>

namespace Ui {
class Dialog_wkzWechselText;
}

class Dialog_wkzWechselText : public QDialog
{
    Q_OBJECT

public:
    explicit Dialog_wkzWechselText(QWidget *parent = nullptr);
    ~Dialog_wkzWechselText();
    void set_wkzWechselText(QString text);

signals:
    void wkzWechselText(QString text);

private slots:
    void on_pushButton_ok_clicked();
    void on_pushButton_abbrechen_clicked();

private:
    Ui::Dialog_wkzWechselText *ui;
};

#endif // DIALOG_WKZWECHSELTEXT_H
