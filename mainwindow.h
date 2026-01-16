#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QFileDialog>
#include "Klassen/geo/vorschau.h"
#include "Klassen/wst/werkstuecke.h"

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
    Ui::MainWindow *ui;

    //Variablen:
    vorschau vorschaufenster;
    werkstuecke Wste;

private slots:
    void resizeEvent(QResizeEvent *event);
    void getMausPosXY(QPoint p);
    void on_btn_import_clicked();
    void on_action_oeffnen_triggered();
};
#endif // MAINWINDOW_H
