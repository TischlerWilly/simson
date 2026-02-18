#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QFileDialog>
#include <QInputDialog>
#include <QDirIterator>
#include <QKeyEvent>

#include "Klassen/einstellung.h"
#include "Klassen/prgpfade.h"
#include "Klassen/geo/vorschau.h"
#include "Klassen/wst/werkstuecke.h"
#include "Funktionen/funktionen_prgtext.h"
#include "Dialoge/dialog_einstellung_pfade.h"
#include "Dialoge/dialog_einstellung_dxf.h"
#include "Dialoge/dialog_einstellung_dxf_klassen.h"
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
#include "Dialoge/bearb/dialog_schnellaenderung.h"
#include "Funktionen/import/ewx.h"
#include "Funktionen/import/dxf.h"
#include "Dialoge/dialog_gcode.h"


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
    void zeile_bearb_afb_umkehren(int zeile_bearb);
    QString verschiebe_bearb_einen(QString bearb, double ax, double ay, double az,\
                                   double wst_l_alt, double wst_l_neu,\
                                   double wst_b_alt, double wst_b_ne  );
    int auswahl_erster();
    int auswahl_letzter();
    int auswahl_menge();

    //Variablen:
    Ui::MainWindow *ui;
    QString Programmversion_simson;
    vorschau vorschaufenster;
    werkstuecke Wste;
    maschinen Maschinen;
    einstellung Einstellung;
    einstellung_dxf Einstellung_dxf;
    einstellung_dxf_klassen Einstellung_dxf_klassen;
    prgpfade PrgPfade;
    QString KopierterEintrag;
    QString Pfad_letzte_geoeffnete_datei;

    //Dialoge:
    Dialog_Einstellung_pfade dlg_Einstellung_pfade;
    Dialog_maschinen dlg_Einstellung_maschinen;
    Dialog_einstellung_dxf dlg_einstellung_dxf;
    Dialog_einstellung_dxf_klassen dlg_einstellung_dxf_klassen;

private slots:
    //Grafik und UI:
    void resizeEvent(QResizeEvent *event);
    void closeEvent(QCloseEvent *event);
    void set_vorschaufenster_default();
    void getMausPosXY(punkt3d p);
    void get_zeilennummer_bearb(uint nr, bool bearbeiten);
    void aktualisiere_fendtertitel();

    //Einstellungen:
    void on_actionPfade_triggered();
    void on_actionCNC_Maschinen_triggered();

    //Dateien/Werkstücke/Bearbeitungen:
    void on_actionNeu_triggered();
    void on_btn_quick_import_clicked();
    void on_action_quick_import_triggered();
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
    void process_undo_redo(bool isUndo);
    void on_actionKopieren_triggered();
    void on_actionEinfuegen_triggered();
    void on_action_schliessen_triggered();       
    void on_action_import_dxf_triggered();    
    void on_actionDXF_Grundeinstellung_triggered();
    void on_actionDXF_Klasseneinstellung_triggered();
    void on_actionEntfernen_triggered();    
    void on_actionSchnellaenderung_triggered();
    void on_actionSpeichern_triggered();
    void on_actionSpeichern_unter_triggered();
    bool speichern(QString dateipfad, werkstueck *wst);
    QString validiere_dateipfad(QString pfad);
    void aktualisiere_listwidget_dateien(int akt_index);    
    void on_actionGCode_Export_triggered();

protected:
    bool eventFilter(QObject *obj, QEvent *event) override;

public slots:
    void getEinstellung(einstellung e);
    void getEinstellungDxf(einstellung_dxf e);
    void getEinstellungDxfKlassen(einstellung_dxf_klassen e);
    void getMaschinen(maschinen m);
    void update_vorschau();
    void update_listwid_bearb();
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
    void sendEinstellungDxf(einstellung_dxf e);
    void sendEinstellungDxfKlassen(einstellung_dxf eg, einstellung_dxf_klassen ek);
    void sendMaschinen(maschinen m);
};
#endif // MAINWINDOW_H
