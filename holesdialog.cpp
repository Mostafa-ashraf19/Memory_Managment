#include "holesdialog.h"
#include "ui_holesdialog.h"
#include<QDebug>
#include<string>
#include<QMessageBox>

std::pair<QString, QString> HolesDialog::HoleData() const
{
    return  std::make_pair(this->HoleAddress,this->HoleSize);
}

HolesDialog::HolesDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::HolesDialog)
{
    ui->setupUi(this);
    this->setFixedSize(350,150);
}

HolesDialog::~HolesDialog()
{
    delete ui;
}

void HolesDialog::on_HolesAccept_clicked()
{
    QString Address = ui->HoleStartAddress->text();
    QString size = ui->HoleSize->text();

    if(Address.toStdString().empty()| size.toStdString().empty())
    {
        QMessageBox::warning(this,"Empty Fields","please fill all inputs");
    }
    else
    {
        this->HoleSize=size;
        this->HoleAddress=Address;
        accept();
    }
    ui->HoleStartAddress->clear();
    ui->HoleSize->clear();
}

void HolesDialog::on_CancelButton_clicked()
{
    reject();
}
