#ifndef MANAGERPANEL_H
#define MANAGERPANEL_H

#include <QWidget>
#include <QDebug>

#include <windows.h>

#include "diskbutton.h"
#include "filemodel.h"

namespace Ui {
class ManagerPanel;
}

class ManagerPanel : public QWidget
{
    Q_OBJECT

public:
    explicit ManagerPanel(QWidget *parent = nullptr);
    ~ManagerPanel();

    void setDisks(DWORD disks);

private:
    Ui::ManagerPanel *ui;
    FileModel *content;
    QList<DiskButton *> diskButtons;
    unsigned long diskMask = 0;

    void configureTable();

private slots:

    void slotDiskButtonClicked(const path &path);
};

#endif // MANAGERPANEL_H
