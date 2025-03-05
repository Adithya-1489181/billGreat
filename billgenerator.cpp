#include "billgenerator.h"
#include "ui_billgenerator.h"

billgenerator::billgenerator(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::billgenerator)
{
    ui->setupUi(this);
}

billgenerator::~billgenerator()
{
    delete ui;
}
