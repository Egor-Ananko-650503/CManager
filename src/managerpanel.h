#ifndef MANAGERPANEL_H
#define MANAGERPANEL_H

#include <QWidget>
#include <QTableView>
#include <QScrollBar>
#include <QDebug>

#include <windows.h>

#include "diskbutton.h"
#include "filemodel.h"
#include "managerpanelcontroller.h"
#include "operationstructure.h"

namespace Ui {
class ManagerPanel;
}

class ManagerPanel : public QWidget
{
    Q_OBJECT

public:
    explicit ManagerPanel(PFileOperation _fileOperation, QWidget *parent = nullptr);
    ~ManagerPanel();

    void setDisks(unsigned long disks);

private:
    Ui::ManagerPanel *ui;
    FileModel *content;
    ManagerPanelController *controller;
    PFileOperation fileOperation;

    void configureTable();

signals:
    void currentPathChanged(path newPath);

public slots:
    void showHiddenFiles(bool checked = false);
};

#endif // MANAGERPANEL_H
