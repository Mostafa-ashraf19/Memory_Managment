#include "layout.h"
#include "ui_layout.h"
#include<QStringList>
#include<QDebug>
#include <QInputDialog>
#include <QMessageBox>
#include <fstream>
#include <sstream>
#include <QFileDialog>
using   std::fstream;

Layout::Layout(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::Layout)
{
    ui->setupUi(this);
    holedialog = new HolesDialog(this);
    processdialog = new ProcessDialog(this);

    ui->HolesTableViewTBlwdgt->setColumnCount(2);
    ui->HolesTableViewTBlwdgt->setHorizontalHeaderLabels({"Starting Address","Hole Size"});

    ui->tabWidget->setTabText(0,"Main");
    ui->tabWidget->setTabText(1,"Processes");

    ui->CBAlgorithmType->addItem("First-Fit");
    ui->CBAlgorithmType->addItem("Best-Fit");

    ui->HolesTableViewTBlwdgt->setEditTriggers(QAbstractItemView::NoEditTriggers);

    ui->ProcessTree->setColumnCount(3);
    ui->ProcessTree->setHeaderLabels({"Process name","Segment name","Segment Size"});

    ui->AllocationInfoTree->setColumnCount(4);
    ui->AllocationInfoTree->setHeaderLabels({"Process name","Segment Name","Starting Address","Segment Size"});

     ui->DealocateButton->setEnabled(false);
    ui->ProcessDealocate->setEnabled(false);
    ui->Holesbutton->setEnabled(false);
    ui->Processbutton->setEnabled(false);
    ui->SimulateButton->setEnabled(false);

}

Layout::~Layout()
{
    delete ui;
}

void Layout::on_Holesbutton_clicked()
{
 int ret = holedialog->exec();
 auto result = holedialog->HoleData();
    if(ret == QDialog::Accepted)
    {
        int rowcount{ui->HolesTableViewTBlwdgt->rowCount()+1};
        ui->HolesTableViewTBlwdgt->setRowCount(rowcount);

        ui->HolesTableViewTBlwdgt->setItem(rowcount-1,0,new QTableWidgetItem(result.first));
        ui->HolesTableViewTBlwdgt->setItem(rowcount-1,1,new QTableWidgetItem(result.second));

        auto HoleAdd = result.first.toInt();
        auto HoleSize = result.second.toInt();
        MemoryOperations.set_free_space(HoleAdd,HoleSize);
        graph.AddHoles(HoleAdd,HoleSize);
        ui->verticalLayout_5->addWidget(graph.ViewChart());
    }
    else
    {
        return;
    }
}

void Layout::on_Processbutton_clicked()
{
    int retval = processdialog->exec();

    if(retval == QDialog::Accepted)
    {
        auto Result= processdialog->ProcessInfo();
//        qDebug() << Result.second.size();
        AddProccesRoot(Result.first,Result.second);
//        ui->ProcessDealocate->addItem(QString::fromStdString(Result.first));

         vector<segment> v;
         for(auto& a:Result.second)
                {
                    v.push_back({Result.first,a.first, atoi(a.second.c_str())});
                }
         if(ui->CBAlgorithmType->currentIndex() == FirstFit)
           MemoryOperations.create_new_process_first_fit(v);
         else
             MemoryOperations.create_new_process_best_fit(v);
    }
    else {
        return;
    }
}

void Layout::on_DrawingView_clicked()
{
    ui->stackedWidget->setCurrentIndex(0);
}

void Layout::on_TablesView_clicked()
{
    ui->stackedWidget->setCurrentIndex(1);
}

void Layout::on_Memorysizebutton_clicked()
{
    if(ui->MemorySizeInput->text().toStdString() == "")
    {
        QMessageBox::information(this,"Error","No Memory Size");
        return;
    }
    auto Memsize = ui->MemorySizeInput->text().toUInt();
    MemoryOperations.set_size_of_memory(Memsize);
    graph.BuildMemory(Memsize);
    ui->verticalLayout_5->addWidget(graph.ViewChart());
    ui->Memorysizebutton->setEnabled(false);
    ui->Holesbutton->setEnabled(true);
    ui->Processbutton->setEnabled(true);
    ui->SimulateButton->setEnabled(true);
}

void Layout::on_SimulateButton_clicked()
{

   auto Info =  MemoryOperations.return_process_information();
   AllocatioRoot(Info);
   auto ToGraph = MemoryOperations.return_memory_section();
   auto procNames = MemoryOperations.ProcessNames();
   for(auto Nm: procNames)
       ui->ProcessDealocate->addItem(QString::fromStdString(Nm));
   graph.Draw(ToGraph,procNames);
   ui->verticalLayout_5->addWidget(graph.ViewChart());

   if(MemoryOperations.return_waiting_queue().size())
       QMessageBox::information(this,"Un Allocated Process","There are "+QString::number(MemoryOperations.return_waiting_queue().size())+" unallocated process Look for the allocation table");
   ui->Holesbutton->setEnabled(false);
   ui->Processbutton->setEnabled(false);
   ui->SimulateButton->setEnabled(false);
   ui->DealocateButton->setEnabled(true);
   ui->ProcessDealocate->setEnabled(true);
   ui->ProcessDealocate->addItem("allocated space");
}
void Layout::AddProccesRoot(std::string Processname, vector<std::pair<std::string,std::string>>Processchild)
{
    QTreeWidgetItem* Process =  new QTreeWidgetItem(ui->ProcessTree);
    Process->setText(0,QString::fromStdString(Processname));
    ui->ProcessTree->addTopLevelItem(Process);
    ProcessChildrens(Process,Processchild);
}

void Layout::ProcessChildrens(QTreeWidgetItem *parent, vector<std::pair<std::string,std::string>> Processchild)
{
    for(auto& childs: Processchild)
    {
        QTreeWidgetItem *Childs =  new QTreeWidgetItem();
        Childs->setText(1,QString::fromStdString(childs.first));
        Childs->setText(2,QString::fromStdString(childs.second));
        parent->addChild(Childs);
    }
}

void Layout::AllocatioRoot(vector<vector<allocated_item_information> > AllocatedINfo)
{
    for(auto& EachProcinfo: AllocatedINfo)
    {
        QTreeWidgetItem* ProcessN= new QTreeWidgetItem(ui->AllocationInfoTree);
        ProcessN->setText(0,QString::fromStdString(EachProcinfo[0].process_name));
        ui->AllocationInfoTree->addTopLevelItem(ProcessN);
        AllocationChild(ProcessN,EachProcinfo);
    }
}

void Layout::AllocationChild(QTreeWidgetItem *parent,vector<allocated_item_information>  ProcessInfo)
{
    for(auto& ProcChild: ProcessInfo)
    {
        QTreeWidgetItem* Childs = new QTreeWidgetItem();
        Childs->setText(1,QString::fromStdString(ProcChild.segment_name));
        Childs->setText(2,QString::number(ProcChild.base_address));
        Childs->setText(3,QString::number(ProcChild.limit));
        parent->addChild(Childs);
    }
}



void Layout::on_actionRun_triggered()
{
    if(ui->MemorySizeInput->text().toStdString() == "")
    {
        QMessageBox::information(this,"Error","No Memory Size");
        return;
    }
    auto Info =  MemoryOperations.return_process_information();
    AllocatioRoot(Info);
    auto ToGraph = MemoryOperations.return_memory_section();
    auto procNames = MemoryOperations.ProcessNames();
    for(auto Nm: procNames)
        ui->ProcessDealocate->addItem(QString::fromStdString(Nm));
    graph.Draw(ToGraph,procNames);
    ui->verticalLayout_5->addWidget(graph.ViewChart());

    if(MemoryOperations.return_waiting_queue().size())
        QMessageBox::information(this,"Un Allocated Process","There are "+QString::number(MemoryOperations.return_waiting_queue().size())+" unallocated process Look for the allocation table");
    ui->Holesbutton->setEnabled(false);
    ui->Processbutton->setEnabled(false);
    ui->SimulateButton->setEnabled(false);
    ui->DealocateButton->setEnabled(true);
    ui->ProcessDealocate->setEnabled(true);
    ui->ProcessDealocate->addItem("allocated space");
}

void Layout::on_actionClear_triggered()
{
    ui->AllocationInfoTree->clear();
    ui->ProcessTree->clear();

    while(ui->HolesTableViewTBlwdgt->rowCount())
        ui->HolesTableViewTBlwdgt->removeRow(0);
    ui->MemorySizeInput->clear();

    MemoryOperations.ClearAllData();
    graph.CLeaRData();
    ui->ProcessDealocate->clear();
    ui->Memorysizebutton->setEnabled(true);
    ui->Holesbutton->setEnabled(false);
    ui->Processbutton->setEnabled(false);
    ui->SimulateButton->setEnabled(true);
    ui->DealocateButton->setEnabled(false);
    ui->ProcessDealocate->setEnabled(false);

}


void Layout::on_DealocateButton_clicked()
{
   auto name =  ui->ProcessDealocate->currentText().toStdString();
   if(name == "allocated space")
      MemoryOperations.deallocation_old_process(QInputDialog::getInt(this,"Dealocate old space","Set Start address"));

  MemoryOperations.deallocated(ui->ProcessDealocate->currentText().toStdString());
  ui->ProcessDealocate->removeItem(ui->ProcessDealocate->currentIndex());
   ui->AllocationInfoTree->setStyleSheet("QTreeWidget::branch{ border-bottom: 1px solid blue;}");

  auto Info =  MemoryOperations.return_process_information();
  AllocatioRoot(Info);

  auto ToGraph = MemoryOperations.return_memory_section();
 // qDebug() << "Size is" << ToGraph.size();
  graph.Draw(ToGraph,MemoryOperations.ProcessNames());

  ui->verticalLayout_5->addWidget(graph.ViewChart());
}

void Layout::on_actionOpen_triggered()
{
    auto FileName = QFileDialog::getOpenFileName(this,"open File","E:",tr("Text File (*.txt)")).toStdString();
    if(!FileName.length())
        return;
    ifstream File (FileName);

    std::string line,val,segname;
    int startadd,size;
    if(File.is_open())
    {
    while(std::getline(File,line))
    {
         std::istringstream Mystream(line);
       // qDebug() << QString::fromStdString(line);
        Mystream >> val;
        if(val == "H")
        {
            Mystream >> startadd >> size;
//            qDebug() <<  QString::number(startadd) << QString::number(size);
        }
        else if (val[0] >= '0' && val[0] <= '9')
        {
//             qDebug() <<  QString::fromStdString(val);
        }
        else
        {
//            Mystream >> segname >> size;
//           qDebug() << QString::fromStdString(val) <<  QString::fromStdString(segname) << QString::number(size);
        }
    }
    }
   else
        QMessageBox::warning(this,"open File","File not Found");
    return;
}

void Layout::on_actionAbout_Project_triggered()
{

}

void Layout::on_actionSaveAs_triggered()
{
//    std::ifstream FileWrite("File.txt");
}
