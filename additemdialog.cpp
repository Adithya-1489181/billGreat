#include "additemdialog.h"
#include "ui_additemdialog.h"
#include <QMessageBox>

AddItemDialog::AddItemDialog(QWidget *parent, int rowIndex, QString itemName, int quantity, double price)
    : QDialog(parent), ui(new Ui::AddItemDialog), editRowIndex(rowIndex) {
    ui->setupUi(this);

    // Set values if editing an existing row
    ui->lineEditItemName->setText(itemName);
    ui->spinBoxQuantity->setValue(quantity);
    ui->doubleSpinBoxPrice->setValue(price);
}

AddItemDialog::~AddItemDialog() {
    delete ui;
}

void AddItemDialog::on_btnSave_clicked() {
    QString itemName = ui->lineEditItemName->text();
    int quantity = ui->spinBoxQuantity->value();
    double price = ui->doubleSpinBoxPrice->value();

    if (itemName.isEmpty() || !quantity || !price) {
        QMessageBox::warning(this, "Warning", "Please enter all details.");
        return;
    }

    if (editRowIndex != -1) {
        emit itemEdited(editRowIndex, itemName, quantity, price);  // Emit edit signal with row index
    } else {
        emit itemAdded(itemName, quantity, price);  // Emit add signal
    }

    accept();
}

void AddItemDialog::on_btnCancel_clicked() {
    reject();
}
