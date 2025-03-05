#ifndef BILLGENERATOR_H
#define BILLGENERATOR_H

#include <QWidget>

namespace Ui {
class billgenerator;
}

class billgenerator : public QWidget
{
    Q_OBJECT

public:
    explicit billgenerator(QWidget *parent = nullptr);
    ~billgenerator();

private:
    Ui::billgenerator *ui;
};

#endif // BILLGENERATOR_H
