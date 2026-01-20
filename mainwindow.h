#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QFileDialog>
#include <QInputDialog>

#include "Klassen/einstellung.h"
#include "Klassen/prgpfade.h"
#include "Klassen/geo/vorschau.h"
#include "Klassen/wst/werkstuecke.h"
#include "Funktionen/funktionen_prgtext.h"
#include "Dialoge/dialog_einstellung_pfade.h"
#include "Dialoge/dialog_maschinen.h"
#include "Klassen/wkz/maschinen.h"

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
    //Funktionen:
    void setup();
    void update_listwidget_bearb(werkstueck *w);
    void schreibe_ini();
    void schreibe_maschinen();
    void maschinen_einlesen();

    //Variablen:
    Ui::MainWindow *ui;    
    vorschau vorschaufenster;
    werkstuecke Wste;
    maschinen Maschinen;
    einstellung Einstellung;
    prgpfade PrgPfade;

    //Dialoge:
    Dialog_Einstellung_pfade dlg_Einstellung_pfade;
    Dialog_maschinen dlg_Einstellung_maschinen;

private slots:
    //Grafik und UI:
    void resizeEvent(QResizeEvent *event);
    void getMausPosXY(QPoint p);
    void get_zeilennummer_bearb(uint nr, bool bearbeiten);

    //Einstellungen:
    void on_actionPfade_triggered();
    void on_actionCNC_Maschinen_triggered();

    //Dateien/Werkstücke:
    void on_actionNeu_triggered();
    void on_btn_import_clicked();
    void on_action_oeffnen_triggered();
    void on_listWidget_dateien_currentRowChanged(int currentRow);
    void on_listWidget_bearb_currentRowChanged(int currentRow);
    void on_actionUmbenennen_triggered();    


public slots:
    void getEinstellung(einstellung e);
    void getMaschinen(maschinen m);

signals:
    void sendEinstellungPfade(einstellung e);
    void sendMaschinen(maschinen m);
};
#endif // MAINWINDOW_H
