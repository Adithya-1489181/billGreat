#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "additemdialog.h"
#include "customerdetaildialogue.h"
#include <QHeaderView>
#include <QGuiApplication>
#include <QScreen>
#include <QMessageBox>
#include <QTextDocument>
#include <QFileDialog>
#include <QPrintDialog>
#include <QDateTime>
#include <QDesktopServices>
#include <QUrl>
#include <QPainter>
#include <QPrinter>




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

void MainWindow::generatePDF() {
    QString fileName = QFileDialog::getSaveFileName(this, "Save PDF", "Bill.pdf", "PDF Files (*.pdf)");
    if (fileName.isEmpty()) return;

    QPrinter printer(QPrinter::PrinterResolution);
    printer.setOutputFormat(QPrinter::PdfFormat);
    printer.setOutputFileName(fileName);
    printer.setPageSize(QPageSize(QPageSize::A4));

    QPainter painter;
    if (!painter.begin(&printer)) {
        QMessageBox::warning(this, "Error", "Failed to create PDF!");
        return;
    }

    // Set font
    QFont font("Arial", 12);
    painter.setFont(font);

    int yPos = 30;  // Initial Y position

    // **Header**
    painter.setFont(QFont("Arial", 10, QFont::Bold));
    painter.drawText(10, yPos, "Prop:Girisha K");
    painter.setFont(QFont("Arial", 10, QFont::Bold));
    painter.drawText(415, yPos, "GSTIN: 29AQFPK6091E1ZO");
    yPos += 20;
    painter.setFont(QFont("Arial", 10, QFont::Bold));
    painter.drawText(10, yPos, "MOB:9901424996");
    yPos += 20;
    painter.setFont(QFont("Arial", 12, QFont::Bold));
    painter.drawText(225, yPos, "COMPOSITION DEALER");
    yPos += 15;
    painter.setFont(QFont("Arial", 10, QFont::Bold));
    painter.drawText(261, yPos, "Bill of Supply");
    yPos += 28;

    painter.setFont(QFont("Times New Roman", 18, QFont::Bold));
    painter.drawText(205, yPos, "ADITHYA SERVICES");
    yPos += 20;

    painter.setFont(QFont("Arial", 10));
    painter.drawText(145, yPos, "D. No. 2-21(E) Soorya, Idkidu Village & Post, Bantwal T. Q. 574220");
    yPos += 20;

    painter.drawLine(10, yPos, 550, yPos);  // Header Bottom Border
    yPos += 30;

    // **Bill Info**
    painter.setFont(QFont("Arial", 12));
    painter.drawText(10, yPos, QString("Bill No: %1").arg(53));  // Replace with actual Bill Number
    painter.drawText(400, yPos, QString("Date: %1").arg(QDate::currentDate().toString("dd-MM-yyyy")));
    yPos += 30;

    painter.drawText(10, yPos, "To: _________________________");
    painter.drawText(400, yPos, "GSTIN: __________________");
    yPos += 40;

    // **Table Headers**
    int xStart = 10;
    int colWidths[] = {40, 200, 100, 50, 80, 80};  // Column width distribution

    painter.setFont(QFont("Arial", 11, QFont::Bold));
    QStringList headers = {"Sl. No.", "Description", "HSN Code", "Qty", "Rate (₹)", "Amount (₹)"};

    int x = xStart;
    for (int i = 0; i < headers.size(); ++i) {
        painter.drawText(x, yPos, headers[i]);
        x += colWidths[i];
    }
    yPos += 20;

    painter.drawLine(10, yPos, 550, yPos);  // Header Bottom Border
    yPos += 10;

    // **Table Content (Sample)**
    painter.setFont(QFont("Arial", 10));

    for (int row = 0; row < ui->tableWidget->rowCount(); ++row) {
        x = xStart;
        QString itemName = ui->tableWidget->item(row, 0)->text();
        QString qty = ui->tableWidget->item(row, 1)->text();
        QString rate = ui->tableWidget->item(row, 2)->text();
        QString amount = ui->tableWidget->item(row, 3)->text();

        QStringList rowData = {QString::number(row + 1), itemName, "", qty, rate, amount};  // HSN left blank

        for (int i = 0; i < rowData.size(); ++i) {
            painter.drawText(x, yPos, rowData[i]);
            x += colWidths[i];
        }
        yPos += 20;
    }

    painter.drawLine(10, yPos, 550, yPos);  // Table Bottom Border
    yPos += 30;

    // **Total Amount**
    painter.setFont(QFont("Arial", 12, QFont::Bold));
    painter.drawText(467, yPos, QString("Total: ₹%1").arg(totalAmount));  // Replace with actual total amount
    yPos += 40;

    // **Signature and Footer**
    painter.setFont(QFont("Arial", 12, QFont::Bold));
    painter.drawText(10, yPos, "Rupees in Words: ___________________________");
    yPos += 40;

    painter.setFont(QFont("Arial", 12, QFont::Bold));
    painter.drawText(430, yPos, "For Adithya Services");
    yPos += 50;

    painter.drawText(460, yPos, "Signature");

    // End PDF creation
    painter.end();

    QMessageBox::information(this, "Success", "Bill saved as PDF!");
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
    connect(ui->btnCustomerDetails, &QPushButton::clicked, this, &MainWindow::openCustomerDetailsDialog);
    connect(ui->btnGeneratePDF, &QPushButton::clicked, this, &MainWindow::generatePDF);
}


MainWindow::~MainWindow() {
    delete ui;
}
