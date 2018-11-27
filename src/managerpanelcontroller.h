#ifndef MANAGERPANELCONTROLLER_H
#define MANAGERPANELCONTROLLER_H

#include <QObject>

#include "boost/filesystem.hpp"
#include "filemodel.h"
#include "diskbutton.h"
#include "junctionpoint.h"

using namespace boost::filesystem;
using namespace neosmart;

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
    void setShowHiddenFiles(bool checked);

private:
    Ui::ManagerPanel *ui;
    FileModel *content;
    QList<DiskButton *> diskButtons;
    unsigned long diskMask = 0;
    path currentPath;
    bool showHiddenFiles = false;

    Files listDirectoryFiles(const path &dirPath, error_code &ec);
    void initView();
    void showDirectory(const path &dirPath, error_code &ec);
    void connectSignals();
    void changeCurrentPath(const path &diskPath);

signals:
    void currentPathChanged(path newPath);

private slots:
    void slotDiskButtonClicked(const path &diskPath, const QString &diskLabel);
    void slotTableViewDBClick(const QModelIndex &index);
    void slotRootPathButtonClicked();
};

#endif // MANAGERPANELCONTROLLER_H
