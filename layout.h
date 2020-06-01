#ifndef LAYOUT_H
#define LAYOUT_H

#include <QMainWindow>
#include "holesdialog.h"
#include "processdialog.h"
#include<QTreeWidget>
#include"CodeBase/memory_mangement.h"
#include"Memorygraph/Memorygraph.h"

#define FirstFit static_cast<int>(0)
#define BestFit  static_cast<int>(1)

QT_BEGIN_NAMESPACE
namespace Ui { class Layout; }
QT_END_NAMESPACE

class Layout : public QMainWindow
{
    Q_OBJECT

public:
    Layout(QWidget *parent = nullptr);
    ~Layout();
//memory_mangement& RetMem();
private slots:

    void on_Holesbutton_clicked();

    void on_Processbutton_clicked();

    void on_DrawingView_clicked();

    void on_TablesView_clicked();

    void on_Memorysizebutton_clicked();

    void on_SimulateButton_clicked();

    void on_actionRun_triggered();

    void on_actionClear_triggered();

    void on_DealocateButton_clicked();

    void on_actionOpen_triggered();

    void on_actionAbout_Project_triggered();

    void on_actionSaveAs_triggered();

private:
    void AddProccesRoot(std::string Processname,vector<std::pair<std::string,std::string>>);
    void ProcessChildrens(QTreeWidgetItem *parent,vector<std::pair<std::string,std::string>>);
    void AllocatioRoot(vector<vector<allocated_item_information>> AllocatedINfo);
    void AllocationChild(QTreeWidgetItem *parent,vector<allocated_item_information>);
//    static void MyUpdate(int index,QBarSet *barset);

    Ui::Layout *ui;
    HolesDialog * holedialog;
    ProcessDialog * processdialog;
    memory_mangement MemoryOperations;
    Memorygraph graph;
};
#endif // LAYOUT_H
