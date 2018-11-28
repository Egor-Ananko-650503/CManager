#ifndef MANAGERPANELCONTROLLER_H
#define MANAGERPANELCONTROLLER_H

#include <QObject>
#include <QShortcut>
#include <QHash>

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
    ~ManagerPanelController();

    void setDisks(unsigned long disks);
    void setShowHiddenFiles(bool checked);

    void processPath();

private:
    Ui::ManagerPanel *ui;
    FileModel *content;
    QList<DiskButton *> diskButtons;
    unsigned long diskMask = 0;
    path currentPath;
    bool showHiddenFiles = false;
    QHash<QKeySequence, QShortcut *> shortcutHash;

    void initView();
    void initContexMenu();
    void initShortcuts();
    void connectSignals();

    Files listDirectoryFiles(const path &dirPath, error_code &ec);
    void processPath(path &_path);
    void showDirectory(const path &dirPath, error_code &ec);
    void changeCurrentPath(const path &diskPath);
    QString formatSizeByThousands(QString original);

signals:
    void currentPathChanged(path newPath);

private slots:
    void slotDiskButtonClicked(const path &diskPath, const QString &diskLabel);
    void slotTableViewDBClick(const QModelIndex &index);
    void slotRootPathButtonClicked();
    void slotCopy();
    void slotCut();
    void slotPaste();
    void slotRename();
    void slotDelete();
    void slotEncrypt();
    void slotDecrypt();
};

#endif // MANAGERPANELCONTROLLER_H
