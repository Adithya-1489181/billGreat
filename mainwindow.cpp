#include "mainwindow.h"
#include <QHeaderView>
#include "./ui_mainwindow.h"
#include <QGuiApplication>
#include <QScreen>
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "additemdialog.h"

void MainWindow::openForm() {
    AddItemDialog *dialog = new AddItemDialog(this);

    // Connect the signal from dialog to the slot in MainWindow
    connect(dialog, &AddItemDialog::itemAdded, this, &MainWindow::addItemToTable);

    dialog->exec();
}

void MainWindow::addItemToTable(QString itemName, int quantity, double price) {
    // Example: Add data to a QTableWidget in the main window
    int row = ui->tableWidget->rowCount();
    ui->tableWidget->insertRow(row);

    ui->tableWidget->setItem(row, 0, new QTableWidgetItem(itemName));
    ui->tableWidget->setItem(row, 1, new QTableWidgetItem(QString::number(quantity)));
    ui->tableWidget->setItem(row, 2, new QTableWidgetItem(QString::number(price)));
    ui->tableWidget->setItem(row, 3, new QTableWidgetItem(QString::number(quantity * price,'f',2)));
    ui->tableWidget->setItem(row, 4, new QTableWidgetItem("Edit | Delete"));
    double itemTotal = quantity * price;  // Calculate amount for this item

    // Update total amount
    totalAmount += itemTotal;
    updateTotalLabel();  // Call function to update the label
}

void MainWindow::updateTotalLabel() {
    ui->lblTotal->setText(QString("Total: ₹%1").arg(totalAmount, 0, 'f', 2));
}

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    // Set column count
    ui->tableWidget->setColumnCount(5);

    // Set column headers
    QStringList headers = {"Descr.", "Qty", "Rate", "Amount", "Actions"};
    ui->tableWidget->setHorizontalHeaderLabels(headers);

    // Set column widths (optional for better UI)
    ui->tableWidget->setColumnWidth(1, 3000); // Description
    ui->tableWidget->setColumnWidth(2, 60);  // Qty
    ui->tableWidget->setColumnWidth(3, 80);  // Rate
    ui->tableWidget->setColumnWidth(4, 100); // Amount
    ui->tableWidget->setColumnWidth(5, 150); // Actions

    // Set resizing mode for each column
    ui->tableWidget->horizontalHeader()->setSectionResizeMode(0, QHeaderView::Stretch); // Description (Expands)
    ui->tableWidget->horizontalHeader()->setSectionResizeMode(1, QHeaderView::ResizeToContents); // Qty
    ui->tableWidget->horizontalHeader()->setSectionResizeMode(2, QHeaderView::ResizeToContents); // Rate
    ui->tableWidget->horizontalHeader()->setSectionResizeMode(3, QHeaderView::ResizeToContents); // Amount
    ui->tableWidget->horizontalHeader()->setSectionResizeMode(4, QHeaderView::Fixed); // Actions (Expands)

    // Enable word wrapping for Description column
    ui->tableWidget->setWordWrap(true);

    // Get screen size and adjust UI scaling
    QScreen *screen = QGuiApplication::primaryScreen();
    qreal dpi = screen->logicalDotsPerInch();
    qreal scaleFactor = dpi / 96.0;  // 96 DPI is standard desktop

    // Apply scaling to the entire UI
    QFont defaultFont = this->font();
    defaultFont.setPointSizeF(defaultFont.pointSizeF() * scaleFactor);
    this->setFont(defaultFont);

    // Increase row height for touch
    ui->tableWidget->verticalHeader()->setDefaultSectionSize(50);  // Adjust size for fingers

    // Enable touch scrolling
    ui->tableWidget->setVerticalScrollMode(QAbstractItemView::ScrollPerPixel);
    connect(ui->btnAdd, &QPushButton::clicked, this, &MainWindow::openForm);
}

MainWindow::~MainWindow()
{
    delete ui;
}
