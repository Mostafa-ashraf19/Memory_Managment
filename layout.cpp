#include "layout.h"
#include "ui_layout.h"

Layout::Layout(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::Layout)
{
    ui->setupUi(this);
    holedialog = new HolesDialog(this);
    processdialog = new ProcessDialog(this);
}

Layout::~Layout()
{
    delete ui;
}


void Layout::on_Holesbutton_clicked()
{
    holedialog->exec();
}

void Layout::on_Processbutton_clicked()
{
    processdialog->exec();
}
