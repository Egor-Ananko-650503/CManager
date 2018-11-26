#include "cmanager.h"
#include "ui_cmanager.h"
#include "boost/filesystem.hpp"

CManager::CManager(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::CManager)
{
    ui->setupUi(this);
    configurePanel();
    runDiskListener();
}

CManager::~CManager()
{
    diskListener->close();
    delete ui;
    delete leftPanel;
    delete rightPanel;
}

void CManager::configurePanel()
{
    this->leftPanel = new ManagerPanel(this);
    this->rightPanel = new ManagerPanel(this);

    ui->CMPanelHBox->addWidget(this->leftPanel);
    ui->CMPanelHBox->addWidget(this->rightPanel);
}

//
void CManager::runDiskListener()
{
    diskListener = new DiskListener;
    connect(diskListener, &DiskListener::diskMaskChanged,
            this, &CManager::slotDiskMaskChanged);

    QtConcurrent::run([=](){
        diskListener->listenDiskMask();
    });
}

void CManager::slotDiskMaskChanged(unsigned long disks)
{
    qDebug() << disks; // DEBUG
    leftPanel->setDisks(disks);
    rightPanel->setDisks(disks);
}
