#ifndef ADDITEMDIALOG_H
#define ADDITEMDIALOG_H

#include <QDialog>

namespace Ui {
class AddItemDialog;
}

class AddItemDialog : public QDialog {
    Q_OBJECT

public:
    explicit AddItemDialog(QWidget *parent = nullptr);
    ~AddItemDialog();

signals:
    void itemAdded(QString itemName, int quantity, double price);  // Signal to send data

private slots:
    void on_btnSave_clicked();  // Function for save button
    void on_btnCancel_clicked(); // Function for save button

private:
    Ui::AddItemDialog *ui;
};

#endif // ADDITEMDIALOG_H
