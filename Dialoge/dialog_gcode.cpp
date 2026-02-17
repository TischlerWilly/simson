#include "dialog_gcode.h"
#include "ui_dialog_gcode.h"

Dialog_GCode::Dialog_GCode(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::Dialog_GCode)
{
    ui->setupUi(this);
}

Dialog_GCode::~Dialog_GCode()
{
    delete ui;
}
void Dialog_GCode::set_maschine(maschine *m)
{
    Maschine = m;
    ui->lineEdit_maschine->setText(Maschine->name());
}
void Dialog_GCode::set_wst(werkstueck *w)
{
    Wst = w;
    ui->lineEdit_datei->setText(Wst->dateipfad());
}
void Dialog_GCode::on_pushButton_erstellen_clicked()
{
    if(Maschine->ausgabeformat() == "emc2")
    {
        emc2 gcode(Maschine, Wst);
        ui->plainTextEdit->setPlainText(gcode.gcode());
    }
}
void Dialog_GCode::on_pushButton_speichern_clicked()
{
    QString dateipfad = Wst->dateipfad();

    if(dateipfad.isEmpty())
    {
        return;
    }

    dateipfad.replace( DATEIENDUNG_EIGENE, ".gc");
    QFile file(dateipfad);

    if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
    {
        QMessageBox::critical(this, tr("Fehler"), tr("Datei konnte nicht zum Schreiben geöffnet werden."));
        return;
    }

    QString dateiInhalt = ui->plainTextEdit->toPlainText();
    if (dateiInhalt.isEmpty())
    {
        on_pushButton_erstellen_clicked();
        dateiInhalt = ui->plainTextEdit->toPlainText();

        if (dateiInhalt.isEmpty())
        {
            QMessageBox::critical(this, tr("Fehler"), tr("Kein GCode vorhanden der gespeichert werden kann."));
            return;
        }
    }

    if (file.write(dateiInhalt.toUtf8()) == -1)
    {
        QMessageBox::critical(this, tr("Fehler"), tr("Inhalt konnte nicht geschrieben werden."));
        file.close();
        return;
    }

    file.close();

    QString msg;
    msg  = "Die Datei \"";
    msg += dateipfad;
    msg += "\" wurde erfolgreich gespeichert.";
    QMessageBox mb;
    mb.setText(msg);
    mb.setWindowTitle("GCode speichern");
    mb.exec();
}

