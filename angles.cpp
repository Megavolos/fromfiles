#include "angles.h"
#include "ui_angles.h"

angles::angles(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::angles)
{
    ui->setupUi(this);
}

angles::~angles()
{
    delete ui;
}
