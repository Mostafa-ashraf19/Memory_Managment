#ifndef HOLESDIALOG_H
#define HOLESDIALOG_H

#include <QDialog>
#include<QString>
#include<string>
namespace Ui {
class HolesDialog;
}

class HolesDialog : public QDialog
{
    Q_OBJECT

public:
    std::pair<QString,QString> HoleData() const;
    explicit HolesDialog(QWidget *parent = nullptr);
    ~HolesDialog();

private slots:
    void on_HolesAccept_clicked();

    void on_CancelButton_clicked();


private:
    QString HoleAddress;
    QString HoleSize;
    Ui::HolesDialog *ui;
};

#endif // HOLESDIALOG_H
