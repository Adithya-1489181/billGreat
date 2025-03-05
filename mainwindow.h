#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void openForm();
    void addItemToTable(QString itemName, int quantity, double price);
    void editRow(int row);
    void deleteRow(int row);
    void updateItemInTable(int row, QString newItemName, int newQuantity, double newPrice);
    void openCustomerDetailsDialog();
     void generatePDF();
private:
    Ui::MainWindow *ui;
    double totalAmount = 0.0;  // Store total amount
    void updateTotalLabel();   // Function to update the label
    int findRow(QWidget *widget);  // Declare function
};
#endif // MAINWINDOW_H
