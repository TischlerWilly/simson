#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QFileDialog>
#include "Klassen/geo/vorschau.h"
#include "Klassen/wst/werkstuecke.h"
#include "Funktionen/funktionen_prgtext.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    void update_listwidget_bearb(werkstueck *w);

    Ui::MainWindow *ui;

    //Variablen:
    vorschau vorschaufenster;
    werkstuecke Wste;

private slots:
    void resizeEvent(QResizeEvent *event);
    void getMausPosXY(QPoint p);
    void get_zeilennummer_bearb(uint nr, bool bearbeiten);
    void on_actionNeu_triggered();
    void on_btn_import_clicked();
    void on_action_oeffnen_triggered();

    void on_listWidget_dateien_currentRowChanged(int currentRow);
    void on_listWidget_bearb_currentRowChanged(int currentRow);
};
#endif // MAINWINDOW_H
