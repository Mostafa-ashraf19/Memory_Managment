#ifndef PROCESSDIALOG_H
#define PROCESSDIALOG_H

#include <QDialog>
#include<unordered_map>
#include<vector>
#include<QString>
#include<string>

using std::unordered_map;
using std::vector;

namespace Ui {
class ProcessDialog;
}

class ProcessDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ProcessDialog(QWidget *parent = nullptr);
    std::pair<std::string,vector<std::pair<std::string,std::string>>> ProcessInfo () ;
    ~ProcessDialog();

private slots:
    void on_AddSegemtnButton_clicked();
    void on_Accept_clicked();

    void on_CancelButton_clicked();

private:
    Ui::ProcessDialog *ui;
    std::string Process_name;
    vector<std::pair<std::string,std::string>> Process_Segments;//segment name, segment size
};

#endif // PROCESSDIALOG_H
