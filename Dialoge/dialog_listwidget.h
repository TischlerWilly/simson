#ifndef DIALOG_LISTWIDGET_H
#define DIALOG_LISTWIDGET_H

#include <QDialog>
#include "Klassen/geo/geo_text.h"

namespace Ui {
class Dialog_listwidget;
}

class Dialog_listwidget : public QDialog
{
    Q_OBJECT

public:
    explicit Dialog_listwidget(QWidget *parent = nullptr);
    ~Dialog_listwidget();
    void set_geotext(geo_text gt);

signals:
    void signal_index_changed(int index);

private slots:
    void resizeEvent(QResizeEvent *event);
    void on_listWidget_currentRowChanged(int currentRow);

private:
    Ui::Dialog_listwidget *ui;
};

#endif // DIALOG_LISTWIDGET_H
