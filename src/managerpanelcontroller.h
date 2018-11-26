#ifndef MANAGERPANELCONTROLLER_H
#define MANAGERPANELCONTROLLER_H

#include <QObject>

#include "filemodel.h"
#include "diskbutton.h"

namespace Ui {
class ManagerPanel;
}
class ManagerPanelController : public QObject
{
    Q_OBJECT
public:
    explicit ManagerPanelController(Ui::ManagerPanel *_ui, FileModel *_content,
                                    QObject *parent = nullptr);

    void setDisks(unsigned long disks);

private:
    Ui::ManagerPanel *ui;
    FileModel *content;
    QList<DiskButton *> diskButtons;
    unsigned long diskMask = 0;

signals:

private slots:
    void slotDiskButtonClicked(const path &diskPath);
};

#endif // MANAGERPANELCONTROLLER_H
