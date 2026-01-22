#include "dialog_fraeserauswahl.h"
#include "ui_dialog_fraeserauswahl.h"

Dialog_fraeserauswahl::Dialog_fraeserauswahl(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialog_fraeserauswahl)
{
    ui->setupUi(this);
    this->setWindowTitle("Fräser auswählen");
    this->resize(750, 500);
    Max_dm      = -1;
    Min_nutzl   = -1;
}

Dialog_fraeserauswahl::~Dialog_fraeserauswahl()
{
    delete ui;
}

void Dialog_fraeserauswahl::on_pushButton_ok_clicked()
{
    int i = ui->tableWidget->currentRow();
    uint index_ret = 1;//Alias
    if(!ui->tableWidget->selectedItems().isEmpty())
    {
        emit send_wkz(ui->tableWidget->item(i, index_ret)->text());
        this->close();
    }else
    {
        QMessageBox mb;
        mb.setText("Bitte zuerst einen Fräser aussuchen.");
        mb.exec();
    }
}
void Dialog_fraeserauswahl::on_pushButton_abbrechen_clicked()
{
    this->close();
}
void Dialog_fraeserauswahl::resizeEvent(QResizeEvent *event)
{
    ui->tableWidget->setFixedWidth(this->width()-15);
    ui->tableWidget->setFixedHeight(this->height()-50);
    int btn_breite = 200;
    ui->pushButton_ok->setFixedWidth(btn_breite);
    ui->pushButton_abbrechen->setFixedWidth(btn_breite);
    int x = ui->tableWidget->width()/2 - ui->pushButton_ok->width() - 5;
    int y = ui->tableWidget->pos().y() + ui->tableWidget->height() + 5;
    ui->pushButton_ok->move(x, y);
    x = x + btn_breite+10;
    ui->pushButton_abbrechen->move(x, y);
}

void Dialog_fraeserauswahl::update_wkztabelle()
{
    for(int i=0; i<ui->tableWidget->rowCount() ;i++)//ist notwendig weil qt das clear() nicht
    {                                               //in jedem Fall ausführt!!
        ui->tableWidget->showRow(i);
    }
    ui->tableWidget->clear();
    int anz_spalten = 4;
    ui->tableWidget->setRowCount(Wkzmag.count());
    ui->tableWidget->setColumnCount(anz_spalten);
    QStringList tabkopf;
    tabkopf << "Werkzeug-Nr." << "Alias" << "DM" << "Nutzlänge";
    ui->tableWidget->setHorizontalHeaderLabels(tabkopf);
    ui->tableWidget->setColumnWidth(0,250);
    ui->tableWidget->setColumnWidth(1,250);
    for(uint i=0; i<Wkzmag.count();i++)
    {
        text_zw zeile(Wkzmag.at(i), WKZ_TRENNZ);
        wkz_fraeser fr(zeile);
        ui->tableWidget->setItem(i,0, new QTableWidgetItem(fr.wkznr()));
        ui->tableWidget->setItem(i,1, new QTableWidgetItem(fr.alias()));
        ui->tableWidget->setItem(i,2, new QTableWidgetItem(double_to_qstring(fr.dm())));
        ui->tableWidget->setItem(i,3, new QTableWidgetItem(double_to_qstring(fr.nutzl())));
        ui->tableWidget->item(i,2)->setTextAlignment(Qt::AlignCenter);
        ui->tableWidget->item(i,3)->setTextAlignment(Qt::AlignCenter);
        if(Max_dm > 0 && fr.dm() > Max_dm)
        {
            ui->tableWidget->hideRow(i);
        }
        if(Min_nutzl > 0 && fr.nutzl() < Min_nutzl)
        {
            ui->tableWidget->hideRow(i);
        }
        if(Wkztyp != "")
        {
            QString akttyp = zeile.at(0);
            if(Wkztyp != akttyp)
            {
                ui->tableWidget->hideRow(i);
            }
        }
    }
    set_fenstertitel();
}
void Dialog_fraeserauswahl::set_wkzmag(text_zw wkzm)
{
    Wkzmag = wkzm;
}
void Dialog_fraeserauswahl::set_max_dm(double dm)
{
    Max_dm = dm;
    set_fenstertitel();
}
void Dialog_fraeserauswahl::set_min_nutz(double nutzl)
{
    Min_nutzl = nutzl;
    set_fenstertitel();
}
void Dialog_fraeserauswahl::set_wkztyp(QString typ)
{
    Wkztyp = typ;
}

void Dialog_fraeserauswahl::set_fenstertitel()
{
    QString fenstertitel = "gefilterte Werkzeugauswahl   { ";
    if(Max_dm > 0)
    {
        fenstertitel +=  "DM < ";
        fenstertitel += double_to_qstring(Max_dm);
        fenstertitel += "mm";
    }
    if(Min_nutzl > 0)
    {
        fenstertitel +=  " | Nutzlänge > ";
        fenstertitel += double_to_qstring(Min_nutzl);
        fenstertitel += "mm";
    }
    if(Wkztyp != "")
    {
        fenstertitel +=  " | Werkzeugtyp: ";
        if(Wkztyp == WKZ_TYP_FRAESER)
        {
            fenstertitel += "Fräser";
        }else if(Wkztyp == WKZ_TYP_SAEGE)
        {
            fenstertitel += "Säge";
        }else if(Wkztyp == WKZ_TYP_BOHRER)
        {
            fenstertitel += "Bohrer";
        }
    }
    fenstertitel += " }";
    this->setWindowTitle(fenstertitel);
}










