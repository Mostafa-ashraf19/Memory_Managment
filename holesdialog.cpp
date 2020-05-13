#include "holesdialog.h"
#include "ui_holesdialog.h"

HolesDialog::HolesDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::HolesDialog)
{
    ui->setupUi(this);
}

HolesDialog::~HolesDialog()
{
    delete ui;
}
