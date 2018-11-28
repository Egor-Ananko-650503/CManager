#include "managerpanel.h"
#include "ui_managerpanel.h"

ManagerPanel::ManagerPanel(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ManagerPanel)
{
    ui->setupUi(this);
    configureTable();

    ui->rootPathButton->setMaximumSize(QSize(30, 30));
    ui->historyComdoBox->setMaximumSize(QSize(30, 30));

    controller = new ManagerPanelController(
        ui,
        content);

    connect(controller, &ManagerPanelController::currentPathChanged,
            this, &ManagerPanel::currentPathChanged);
}

ManagerPanel::~ManagerPanel()
{
    delete ui;
    delete content;
    delete controller;
}

void ManagerPanel::setDisks(unsigned long disks)
{
    controller->setDisks(disks);
}

void ManagerPanel::configureTable()
{
    QTableView *tableView = ui->fileTableView;
    tableView->setModel(content = new FileModel());
    tableView->setContextMenuPolicy(Qt::ActionsContextMenu);
    tableView->horizontalHeader()->setSectionResizeMode(0, QHeaderView::Stretch);
    tableView->verticalHeader()->setSectionResizeMode(QHeaderView::Fixed);
    tableView->verticalHeader()->setDefaultSectionSize(20);
}

void ManagerPanel::showHiddenFiles(bool checked)
{
    controller->setShowHiddenFiles(checked);
}
