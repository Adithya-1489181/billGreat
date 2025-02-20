#include "customerdetaildialogue.h"
#include "ui_customerdetaildialogue.h"


CustomerDetailDialogue::CustomerDetailDialogue(QWidget *parent)
    : QDialog(parent), ui(new Ui::CustomerDetailDialogue)
{
    ui->setupUi(this);

    // Connect OK button to accept() method
    connect(ui->btnOK, &QPushButton::clicked, this, &QDialog::accept);
    // Connect Cancel button to reject() method
    connect(ui->btnCancel, &QPushButton::clicked, this, &QDialog::reject);
}

CustomerDetailDialogue::~CustomerDetailDialogue()
{
    delete ui;
}

QString CustomerDetailDialogue::getCustomerName() const {
    return ui->lineEditCustomerName->text();
}

QString CustomerDetailDialogue::getCustomerPhone() const {
    return ui->lineEditCustomerPhone->text();
}
