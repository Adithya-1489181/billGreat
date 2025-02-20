#include "mainwindow.h"
#include <QHeaderView>
#include "./ui_mainwindow.h"
#include <QGuiApplication>
#include <QScreen>
#include "additemdialog.h"
#include "customerdetaildialogue.h"
#include <QMessageBox>


void MainWindow::openForm() {
    AddItemDialog *dialog = new AddItemDialog(this);
    connect(dialog, &AddItemDialog::itemAdded, this, &MainWindow::addItemToTable);
    dialog->exec();
}

void MainWindow::openCustomerDetailsDialog() {
    CustomerDetailDialogue customerDialog(this);

    if (customerDialog.exec() == QDialog::Accepted) {
        QString customerName = customerDialog.getCustomerName();
        QString customerPhone = customerDialog.getCustomerPhone();

        if (customerName.isEmpty() || customerPhone.isEmpty()) {
            QMessageBox::warning(this, "Warning", "Customer details are required!");
            return;
        }

        QMessageBox::information(this, "Customer Saved",
                                 QString("Customer Name: %1\nCustomer Phone: %2")
                                     .arg(customerName)
                                     .arg(customerPhone));
    }
}

void MainWindow::addItemToTable(QString itemName, int quantity, double price) {
    int row = ui->tableWidget->rowCount();
    ui->tableWidget->insertRow(row);

    // Correct column assignments
    ui->tableWidget->setItem(row, 0, new QTableWidgetItem(itemName));
    ui->tableWidget->setItem(row, 1, new QTableWidgetItem(QString::number(quantity)));
    ui->tableWidget->setItem(row, 2, new QTableWidgetItem(QString::number(price)));
    ui->tableWidget->setItem(row, 3, new QTableWidgetItem(QString::number(quantity * price, 'f', 2)));
    ui->tableWidget->setItem(row, 4, new QTableWidgetItem("")); // Empty total column (Fix shift)

    // Update total amount
    totalAmount += (quantity * price);
    updateTotalLabel();

    // Create Edit Button
    QPushButton *btnEdit = new QPushButton("Edit");
    connect(btnEdit, &QPushButton::clicked, this, [this, btnEdit]() {
        int row = findRow(btnEdit);
        if (row != -1) editRow(row);
    });

    // Create Delete Button
    QPushButton *btnDelete = new QPushButton("Delete");
    connect(btnDelete, &QPushButton::clicked, this, [this, btnDelete]() {
        int row = findRow(btnDelete);
        if (row != -1) {
            totalAmount -= ui->tableWidget->item(row, 3)->text().toDouble();
            updateTotalLabel();
            ui->tableWidget->removeRow(row);
        }
    });

    // Ensure buttons are placed in the correct column
    QWidget *actionWidget = new QWidget();
    QHBoxLayout *layout = new QHBoxLayout(actionWidget);
    layout->addWidget(btnEdit);
    layout->addWidget(btnDelete);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->setSpacing(5);
    actionWidget->setLayout(layout);

    // Insert action buttons into last column (Column 5)
    ui->tableWidget->setCellWidget(row, 5, actionWidget);
}

void MainWindow::editRow(int row) {
    // Get current values from the table
    QTableWidgetItem *itemNameItem = ui->tableWidget->item(row, 0);
    QTableWidgetItem *quantityItem = ui->tableWidget->item(row, 1);
    QTableWidgetItem *priceItem = ui->tableWidget->item(row, 2);

    if (!itemNameItem || !quantityItem || !priceItem) return;  // Prevent crashes

    QString itemName = itemNameItem->text();
    int quantity = quantityItem->text().toInt();
    double price = priceItem->text().toDouble();

    // Open AddItemDialog in edit mode
    AddItemDialog *dialog = new AddItemDialog(this, row, itemName, quantity, price);

    // Connect signal to update the row after editing
    connect(dialog, &AddItemDialog::itemEdited, this, &MainWindow::updateItemInTable);

    dialog->exec();
}

void MainWindow::updateItemInTable(int row, QString newItemName, int newQuantity, double newPrice) {
    double oldTotal = ui->tableWidget->item(row, 3)->text().toDouble();
    double newTotal = newQuantity * newPrice;

    ui->tableWidget->setItem(row, 0, new QTableWidgetItem(newItemName));
    ui->tableWidget->setItem(row, 1, new QTableWidgetItem(QString::number(newQuantity)));
    ui->tableWidget->setItem(row, 2, new QTableWidgetItem(QString::number(newPrice)));
    ui->tableWidget->setItem(row, 3, new QTableWidgetItem(QString::number(newTotal, 'f', 2)));

    // Update total amount correctly
    totalAmount += (newTotal - oldTotal);
    updateTotalLabel();
}



int MainWindow::findRow(QWidget *widget) {
    for (int row = 0; row < ui->tableWidget->rowCount(); ++row) {
        QWidget *cellWidget = ui->tableWidget->cellWidget(row, 5); // Adjust column index if needed
        if (cellWidget) {
            QList<QPushButton*> buttons = cellWidget->findChildren<QPushButton*>();
            for (QPushButton* btn : buttons) {
                if (btn == widget) {
                    return row;
                }
            }
        }
    }
    return -1; // Return -1 if not found
}

void MainWindow::deleteRow(int row) {
    ui->tableWidget->removeRow(row);
}

void MainWindow::updateTotalLabel() {
    ui->lblTotal->setText(QString("Total: ₹%1").arg(totalAmount, 0, 'f', 2));
}

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    ui->tableWidget->setColumnCount(6);

    QStringList headers = {"Descr.", "Qty", "Rate", "Amount", "Total", "Actions"};
    ui->tableWidget->setHorizontalHeaderLabels(headers);

    ui->tableWidget->setColumnWidth(0, 200);
    ui->tableWidget->setColumnWidth(1, 60);
    ui->tableWidget->setColumnWidth(2, 80);
    ui->tableWidget->setColumnWidth(3, 100);
    ui->tableWidget->setColumnWidth(4, 100);
    ui->tableWidget->setColumnWidth(5, 160);

    ui->tableWidget->horizontalHeader()->setSectionResizeMode(0, QHeaderView::Stretch);
    ui->tableWidget->horizontalHeader()->setSectionResizeMode(1, QHeaderView::ResizeToContents);
    ui->tableWidget->horizontalHeader()->setSectionResizeMode(2, QHeaderView::ResizeToContents);
    ui->tableWidget->horizontalHeader()->setSectionResizeMode(3, QHeaderView::ResizeToContents);
    ui->tableWidget->horizontalHeader()->setSectionResizeMode(4, QHeaderView::ResizeToContents);
    ui->tableWidget->horizontalHeader()->setSectionResizeMode(5, QHeaderView::Fixed);

    ui->tableWidget->setWordWrap(true);
    ui->tableWidget->verticalHeader()->setDefaultSectionSize(50);
    ui->tableWidget->setVerticalScrollMode(QAbstractItemView::ScrollPerPixel);

    connect(ui->btnAdd, &QPushButton::clicked, this, &MainWindow::openForm);
    // Connect the "Customer Details" button to open the popup
    connect(ui->btnCustomerDetails, &QPushButton::clicked, this, &MainWindow::openCustomerDetailsDialog);

}


MainWindow::~MainWindow() {
    delete ui;
}
