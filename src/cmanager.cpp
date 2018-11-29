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
    connectSignals();

    fileOperation = new FileOperation();
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
    this->leftPanel = new ManagerPanel(fileOperation, this);
    this->rightPanel = new ManagerPanel(fileOperation, this);

    ui->CMPanelHBox->addWidget(this->leftPanel);
    ui->CMPanelHBox->addWidget(this->rightPanel);
}

void CManager::runDiskListener()
{
    diskListener = new DiskListener;
    connect(diskListener, &DiskListener::diskMaskChanged,
            this, &CManager::slotDiskMaskChanged);

    QtConcurrent::run([this](){
        diskListener->listenDiskMask();
    });
}

void CManager::connectSignals()
{
    connect(ui->actionHidden, &QAction::triggered,
            leftPanel, &ManagerPanel::showHiddenFiles);
    connect(ui->actionHidden, &QAction::triggered,
            rightPanel, &ManagerPanel::showHiddenFiles);

    connect(leftPanel, &ManagerPanel::currentPathChanged,
            this, &CManager::slotChangeCurrentPath);
    connect(rightPanel, &ManagerPanel::currentPathChanged,
            this, &CManager::slotChangeCurrentPath);

    connect(ui->cmdComandLineEdit, &QLineEdit::returnPressed,
            this, &CManager::slotCmdPressed);
}

void CManager::slotDiskMaskChanged(unsigned long disks)
{
    qDebug() << disks; // DEBUG
    leftPanel->setDisks(disks);
    rightPanel->setDisks(disks);
}

void CManager::slotChangeCurrentPath(path newPath)
{
    ui->cmdPathLabel->setText(QString::fromStdWString(newPath.wstring()));
    currentPath = newPath;
}

void CManager::slotCmdPressed()
{
    std::wstring command(("/c \"" + ui->cmdComandLineEdit->text() + "\"").toStdWString());
    ShellExecuteW(nullptr, L"open", L"cmd.exe",
                  command.c_str(), currentPath.wstring().c_str(),
                  SW_SHOWNORMAL);
    qDebug() << command;                                  // DEBUG
}
