#include "additemdialog.h"
#include "ui_additemdialog.h"
#include <QMessageBox>
#include <QGuiApplication>
#include <QScreen>

AddItemDialog::AddItemDialog(QWidget *parent)
    : QDialog(parent), ui(new Ui::AddItemDialog) {
    ui->setupUi(this);
}

AddItemDialog::~AddItemDialog() {
    delete ui;
}

// Function to handle save button click
void AddItemDialog::on_btnSave_clicked() {
    QString itemName = ui->lineEditItemName->text();
    int quantity = ui->spinBoxQuantity->value();
    double price = ui->doubleSpinBoxPrice->value();

    if (itemName.isEmpty()||!quantity||!price) {
        QMessageBox::warning(this, "Warning", "You Missed Something !! Enter all the details :)");
        return;
    }

    // Do something with the input (e.g., send to MainWindow)
    emit itemAdded(itemName, quantity, price);
    accept();  // Close the dialog
}

void AddItemDialog::on_btnCancel_clicked() {
    accept();  // Close the dialog
}
