#ifndef CUSTOMERDETAILDIALOGUE_H
#define CUSTOMERDETAILDIALOGUE_H

#include <QDialog>

namespace Ui {
class CustomerDetailDialogue;
}

class CustomerDetailDialogue : public QDialog
{
    Q_OBJECT

public:
    explicit CustomerDetailDialogue(QWidget *parent = nullptr);
    ~CustomerDetailDialogue();

    QString getCustomerName() const;
    QString getCustomerPhone() const;

private:
    Ui::CustomerDetailDialogue *ui;
};

#endif // CUSTOMERDETAILDIALOGUE_H
