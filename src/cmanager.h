#ifndef CMANAGER_H
#define CMANAGER_H

#include <QMainWindow>
#include <QDebug>
#include <QtConcurrent/QtConcurrentRun>
#include <disklistener.h>
#include "managerpanel.h"

namespace Ui {
class CManager;
}

class CManager : public QMainWindow
{
    Q_OBJECT

public:

    explicit CManager(QWidget *parent = nullptr);
    ~CManager();

private:

    Ui::CManager *ui;
    ManagerPanel *leftPanel;
    ManagerPanel *rightPanel;
    DiskListener *diskListener;
    path currentPath;
    PFileOperation fileOperation;

    void configurePanel();
    void runDiskListener();
    void connectSignals();

signals:
    void showHiddenFilesMode(bool checked = false);

private slots:
    void slotDiskMaskChanged(unsigned long);
    void slotChangeCurrentPath(path newPath);
    void slotCmdPressed();
};

#endif // CMANAGER_H
