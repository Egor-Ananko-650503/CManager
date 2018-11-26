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

    void configurePanel();
    void runDiskListener();

signals:

private slots:

    void slotDiskMaskChanged(unsigned long);
};

#endif // CMANAGER_H
