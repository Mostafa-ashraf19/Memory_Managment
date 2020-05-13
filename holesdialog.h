#ifndef HOLESDIALOG_H
#define HOLESDIALOG_H

#include <QDialog>

namespace Ui {
class HolesDialog;
}

class HolesDialog : public QDialog
{
    Q_OBJECT

public:
    explicit HolesDialog(QWidget *parent = nullptr);
    ~HolesDialog();

private:
    Ui::HolesDialog *ui;
};

#endif // HOLESDIALOG_H
