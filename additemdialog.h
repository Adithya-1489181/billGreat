#ifndef ADDITEMDIALOG_H
#define ADDITEMDIALOG_H

#include <QDialog>

namespace Ui {
class AddItemDialog;
}

class AddItemDialog : public QDialog {
    Q_OBJECT

public:
    explicit AddItemDialog(QWidget *parent = nullptr, int rowIndex = -1, QString itemName = "", int quantity = 0, double price = 0.0);
    ~AddItemDialog();

signals:
    void itemAdded(QString itemName, int quantity, double price);
    void itemEdited(int rowIndex, QString itemName, int quantity, double price);  // Include row index

private slots:
    void on_btnSave_clicked();
    void on_btnCancel_clicked();

private:
    Ui::AddItemDialog *ui;
    int editRowIndex;  // Store row index for editing
};

#endif // ADDITEMDIALOG_H
