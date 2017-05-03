#ifndef ANGLES_H
#define ANGLES_H

#include <QDialog>

namespace Ui {
class angles;
}

class angles : public QDialog
{
    Q_OBJECT

public:
    explicit angles(QWidget *parent = 0);

    ~angles();

private:
    Ui::angles *ui;
};

#endif // ANGLES_H
