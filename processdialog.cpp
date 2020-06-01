#include "processdialog.h"
#include "ui_processdialog.h"
#include<QDebug>
#include<QMessageBox>

ProcessDialog::ProcessDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ProcessDialog)
{
    ui->setupUi(this);
    this->setFixedSize(300,198);

}

std::pair<std::string, vector<std::pair<std::string,std::string>>> ProcessDialog::ProcessInfo()
{
    auto Proses = Process_Segments;
    Process_Segments.clear();
    return std::make_pair(Process_name,Proses);
}

ProcessDialog::~ProcessDialog()
{
    delete ui;
}

void ProcessDialog::on_AddSegemtnButton_clicked()
{
    std::string segmentName =ui->SegmentNameLineedit->text().toStdString();
    auto SegmentInfo = ui->SegmentSizelineedit->text().toStdString();
    if(segmentName.empty()|
       SegmentInfo.empty())
       {
        QMessageBox::warning(this,"Empty Fields","please fill all segment information inputs");
         return;
       }

    Process_Segments.push_back(std::make_pair(segmentName,SegmentInfo));

    ui->SegmentNameLineedit->clear();
    ui->SegmentSizelineedit->clear();
}

void ProcessDialog::on_Accept_clicked()
{
    Process_name = ui->ProcessNameLineedit->text().toStdString();
    ui->ProcessNameLineedit->clear();
    if(Process_name.empty()|Process_Segments.empty())
    {
        QMessageBox::warning(this,"Empty Fields","please fill inputs");
        return;
    }
    accept();
}

void ProcessDialog::on_CancelButton_clicked()
{
    ui->ProcessNameLineedit->clear();
    ui->SegmentNameLineedit->clear();
    ui->SegmentSizelineedit->clear();
    reject();
}
