#ifndef LAYOUT_H
#define LAYOUT_H

#include <QMainWindow>
#include "holesdialog.h"
#include "processdialog.h"

QT_BEGIN_NAMESPACE
namespace Ui { class Layout; }
QT_END_NAMESPACE

class Layout : public QMainWindow
{
    Q_OBJECT

public:
    Layout(QWidget *parent = nullptr);
    ~Layout();

private slots:
    void on_Holesbutton_clicked();

    void on_Processbutton_clicked();

private:
    Ui::Layout *ui;
    HolesDialog * holedialog;
    ProcessDialog * processdialog;
};
#endif // LAYOUT_H
