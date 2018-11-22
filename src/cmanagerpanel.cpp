#include "cmanagerpanel.h"
#include "ui_cmanagerpanel.h"

CManagerPanel::CManagerPanel(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::CManagerPanel)
{
    ui->setupUi(this);
}

CManagerPanel::~CManagerPanel()
{
    delete ui;
}
