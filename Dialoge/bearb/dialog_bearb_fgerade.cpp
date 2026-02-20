#include "dialog_bearb_fgerade.h"
#include "ui_dialog_bearb_fgerade.h"

Dialog_bearb_fgerade::Dialog_bearb_fgerade(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialog_bearb_fgerade)
{
    ui->setupUi(this);
    Wst = nullptr;
    this->setWindowTitle("Gerade Fräsen");
    ui->lineEdit_zs->setEnabled(false);
    ui->lineEdit_ze->setEnabled(false);
    ui->comboBox_bezug->addItem("Oberseite");   //0
    ui->comboBox_bezug->addItem("Unterseite");  //1
    ui->btn_ok->setFocus();
}

Dialog_bearb_fgerade::~Dialog_bearb_fgerade()
{
    delete ui;
}

void Dialog_bearb_fgerade::set_data(QString d, werkstueck *w)
{
    Wst = w;
    fraesergerade fg;
    fg.set_text(d);
    ui->lineEdit_xs->setText(fg.xs_qstring());
    ui->lineEdit_ys->setText(fg.ys_qstring());
    ui->lineEdit_zs->setText(fg.zs_qstring());
    ui->lineEdit_xe->setText(fg.xe_qstring());
    ui->lineEdit_ye->setText(fg.ye_qstring());
    ui->lineEdit_ze->setText(fg.ze_qstring());
    ui->lineEdit_tiSta->setText(fg.tiSta_qstring());
    ui->lineEdit_tiEnd->setText(fg.tiEnd_qstring());
    //---------
    //Bezug:
    if(fg.bezug() == WST_BEZUG_OBSEI)
    {
        ui->comboBox_bezug->setCurrentIndex(0);
    }else if(fg.bezug() == WST_BEZUG_UNSEI)
    {
        ui->comboBox_bezug->setCurrentIndex(1);
    }
    //---------
    ui->lineEdit_afb->setText(fg.afb());
}

QString Dialog_bearb_fgerade::var_zu_wert(QString term)
{
    if(Wst != nullptr)
    {
        term = term.toUpper();
        term.replace("L", Wst->laenge_qstring());
        term.replace("B", Wst->breite_qstring());
        term.replace("D", Wst->dicke_qstring());
        term = berechnen(term);
    }else
    {
        berechnen(term);
    }
    return term;
}

void Dialog_bearb_fgerade::on_btn_ok_clicked()
{
    fraesergerade fg;
    fg.set_xs(var_zu_wert(ui->lineEdit_xs->text()));
    fg.set_ys(var_zu_wert(ui->lineEdit_ys->text()));
    fg.set_zs(var_zu_wert(ui->lineEdit_zs->text()));
    fg.set_xe(var_zu_wert(ui->lineEdit_xe->text()));
    fg.set_ye(var_zu_wert(ui->lineEdit_ye->text()));
    fg.set_ze(var_zu_wert(ui->lineEdit_ze->text()));
    fg.set_tiSta(var_zu_wert(ui->lineEdit_tiSta->text()));
    fg.set_tiEnd(var_zu_wert(ui->lineEdit_tiEnd->text()));
    QString bezug = ui->comboBox_bezug->currentText();
    if(bezug == "Oberseite")
    {
        fg.set_bezug(WST_BEZUG_OBSEI);
    }else if(bezug == "Unterseite")
    {
        fg.set_bezug(WST_BEZUG_UNSEI);
    }
    fg.set_afb(ui->lineEdit_afb->text());

    emit signal_fgerade(fg);
    this->close();
}

void Dialog_bearb_fgerade::on_btn_abbrechen_clicked()
{
    this->close();
}

void Dialog_bearb_fgerade::showEvent(QShowEvent *event)
{
    QDialog::showEvent(event); // Basisklasse aufrufen nicht vergessen!

    // Startet die Funktion 'aktualisiere_infofelder' nach 50ms (1000ms == 1 Sekunden)
    QTimer::singleShot(50, this, &Dialog_bearb_fgerade::aktualisiere_infofelder);
}
void Dialog_bearb_fgerade::aktualisiere_infofelder()
{
    if(this->isVisible())
    {
        punkt3d sp, ep;
        sp.set_x(var_zu_wert(ui->lineEdit_xs->text()));
        sp.set_y(var_zu_wert(ui->lineEdit_ys->text()));
        ep.set_x(var_zu_wert(ui->lineEdit_xe->text()));
        ep.set_y(var_zu_wert(ui->lineEdit_ye->text()));
        strecke s;
        s.set_stapu(sp);
        s.set_endpu(ep);

        ui->lineEdit_laenge->setText(s.laenge2d_QString());
    }
}

void Dialog_bearb_fgerade::on_lineEdit_xs_editingFinished()
{
    aktualisiere_infofelder();
}
void Dialog_bearb_fgerade::on_lineEdit_ys_editingFinished()
{
    aktualisiere_infofelder();
}
void Dialog_bearb_fgerade::on_lineEdit_xe_editingFinished()
{
    aktualisiere_infofelder();
}
void Dialog_bearb_fgerade::on_lineEdit_ye_editingFinished()
{
    aktualisiere_infofelder();
}

