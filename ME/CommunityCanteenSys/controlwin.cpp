#include "controlwin.h"
#include "ui_controlwin.h"

controlwin::controlwin(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::controlwin)
{
    ui->setupUi(this);
}

controlwin::~controlwin()
{
    delete ui;
}

