#include "cmanager.h"
#include "ui_cmanager.h"
#include "boost/filesystem.hpp"

CManager::CManager(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::CManager)
{
    ui->setupUi(this);
    configurePanel();
}

CManager::~CManager()
{
    delete ui;
    delete leftPanel;
    delete rightPanel;
}

void CManager::configurePanel()
{
    this->leftPanel = new CManagerPanel(this);
    this->rightPanel = new CManagerPanel(this);

    ui->CMPanelHBox->addWidget(this->leftPanel);
    ui->CMPanelHBox->addWidget(this->rightPanel);
}
