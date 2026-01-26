#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QFileDialog>
#include <QInputDialog>
#include <QDirIterator>

#include "Klassen/einstellung.h"
#include "Klassen/prgpfade.h"
#include "Klassen/geo/vorschau.h"
#include "Klassen/wst/werkstuecke.h"
#include "Funktionen/funktionen_prgtext.h"
#include "Dialoge/dialog_einstellung_pfade.h"
#include "Dialoge/dialog_maschinen.h"
#include "Klassen/wkz/maschinen.h"
#include "Dialoge/bearb/dialog_bearb_pkopf.h"
#include "Dialoge/bearb/dialog_bearb_bohrung.h"
#include "Dialoge/bearb/dialog_bearb_rta.h"
#include "Dialoge/bearb/dialog_bearb_nut.h"
#include "Dialoge/bearb/dialog_bearb_faufruf.h"
#include "Dialoge/bearb/dialog_bearb_fgerade.h"
#include "Dialoge/bearb/dialog_bearb_fbogen.h"
#include "Dialoge/bearb/dialog_bearb_verschieben.h"
#include "Funktionen/import/ewx.h"
#include "Funktionen/import/dxf.h"


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
    void zeile_bearb_bearbeiten(int zeile_bearb);
    QString verschiebe_bearb_einen(QString bearb, double ax, double ay, double az,\
                                   double wst_l_alt, double wst_l_neu,\
                                   double wst_b_alt, double wst_b_ne  );
    int auswahl_erster();
    int auswahl_letzter();
    int auswahl_menge();

    //Variablen:
    Ui::MainWindow *ui;    
    vorschau vorschaufenster;
    werkstuecke Wste;
    maschinen Maschinen;
    einstellung Einstellung;
    prgpfade PrgPfade;
    QString KopierterEintrag;
    QString Pfad_letzte_geoeffnete_datei;

    //Dialoge:
    Dialog_Einstellung_pfade dlg_Einstellung_pfade;
    Dialog_maschinen dlg_Einstellung_maschinen;

private slots:
    //Grafik und UI:
    void resizeEvent(QResizeEvent *event);
    void set_vorschaufenster_default();
    void getMausPosXY(QPoint p);
    void get_zeilennummer_bearb(uint nr, bool bearbeiten);

    //Einstellungen:
    void on_actionPfade_triggered();
    void on_actionCNC_Maschinen_triggered();

    //Dateien/Werkstücke/Bearbeitungen:
    void on_actionNeu_triggered();
    void on_btn_import_clicked();
    void on_action_oeffnen_triggered();
    void on_listWidget_dateien_currentRowChanged(int currentRow);
    void on_listWidget_bearb_currentRowChanged(int currentRow);
    void on_actionUmbenennen_triggered();
    void on_listWidget_bearb_itemDoubleClicked(QListWidgetItem *item);
    void on_action_make_bohrung_triggered();
    void on_action_make_rta_triggered();
    void on_action_make_nut_triggered();
    void on_actionVerschieben_triggered();
    void on_actionUndo_triggered();
    void on_actionRedo_triggered();
    void on_actionKopieren_triggered();
    void on_actionEinfuegen_triggered();
    void on_action_schliessen_triggered();
    void on_action_importieren_triggered();    
    void on_action_import_dxf_triggered();

public slots:
    void getEinstellung(einstellung e);
    void getMaschinen(maschinen m);
    //Bearbeiten
    void zeile_aendern(int index_bearb, QString bearb, bool unredor_verwenden);
    void slot_rta(rechtecktasche rta);
    void slot_bo(bohrung bo);
    void slot_nut(nut nu);
    void slot_faufruf(fraeseraufruf fa);
    void slot_fgerade(fraesergerade fg);
    void slot_fbogen(fraeserbogen fb);

    void slot_verschieben(punkt3d p);
    //Erstellen/Make:
    void slot_make(QString bearb, bool unredor_verwenden);
    void slot_make_bo(bohrung bo);
    void slot_make_rta(rechtecktasche rt);
    void slot_make_nut(nut nu);

signals:
    void sendEinstellungPfade(einstellung e);
    void sendMaschinen(maschinen m);
};
#endif // MAINWINDOW_H
