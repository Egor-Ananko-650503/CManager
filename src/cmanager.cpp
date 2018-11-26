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
    diskListener->fExit = true;
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
        while (!diskListener->fExit)
        {
            diskListener->listenDiskMask();
            QThread::sleep(3);
        }
    });
}

void CManager::slotDiskMaskChanged(unsigned long disks)
{
    qDebug() << disks;
    leftPanel->setDisks(disks);
    rightPanel->setDisks(disks);
}
