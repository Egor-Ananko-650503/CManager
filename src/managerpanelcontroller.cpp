#include "managerpanelcontroller.h"
#include "managerpanelcontroller.h"
#include "ui_managerpanel.h"

ManagerPanelController::ManagerPanelController(Ui::ManagerPanel *_ui, FileModel *_content,
                                               QObject *parent) :
    QObject(parent)
{
    ui = _ui;
    content = _content;

    connectSignals();

// setDisks(GetLogicalDrives());
// if (!diskButtons.empty())
// emit diskButtons.first()->resendingSignal();
}

void ManagerPanelController::setDisks(unsigned long disks)
{
    if (diskMask != disks) {
        if (!diskButtons.empty()) {
            for (int i = 0; i < diskButtons.length(); ++i)
                delete diskButtons[i];
            diskButtons.clear();
        }

        for (int i = 0; i < 26; ++i) {
            int bit = (disks >> i) & 1;
            if (bit) {
                QString diskLitera('A' + i);
                path diskPath((diskLitera + ":\\").toStdWString());

                wchar_t diskLabel[MAX_PATH + 1];
                GetVolumeInformation(diskPath.wstring().c_str(),
                                     diskLabel, MAX_PATH + 1,
                                     nullptr, nullptr, nullptr, nullptr, NULL);

                QString diskLabelStr = "["
                                       +QString::fromWCharArray(diskLabel, wcslen(diskLabel))
                                       +"]";
                DiskButton *button = new DiskButton(diskPath, diskLitera);
                button->diskLabel = diskLabelStr;

                connect(button, &DiskButton::diskClicked,
                        this, &ManagerPanelController::slotDiskButtonClicked);

                button->setMaximumSize(QSize(50, 30));
                diskButtons.push_back(button);
            }
        }
        for (int i = 0; i < diskButtons.count(); ++i)
            ui->diskChooserHlayout->insertWidget(i, diskButtons[i]);
    }
}

void ManagerPanelController::setShowHiddenFiles(bool checked)
{
    showHiddenFiles = checked;
    error_code ec;
    showDirectory(currentPath, ec);
}

Files ManagerPanelController::listDirectoryFiles(const path &dirPath, error_code &ec)
{
    Files newFiles;

    if (exists(dirPath, ec)
        && ec.value() == errc::success
        && is_directory(dirPath, ec)
        && ec.value() == errc::success) {
        for (directory_entry &dEntry
             : directory_iterator(dirPath))
            if (!(FILE_ATTRIBUTE_HIDDEN & GetFileAttributesW(dEntry.path().c_str()))
                || showHiddenFiles)
                newFiles.push_back(dEntry.path());

        std::sort(newFiles.begin(), newFiles.end());
    }

    return newFiles;
}

void ManagerPanelController::initView()
{
}

void ManagerPanelController::showDirectory(const path &dirPath, error_code &ec)
{
    Files newFiles = listDirectoryFiles(dirPath, ec);
    if (ec.value() != errc::success) return;

    if (dirPath.compare(dirPath.root_path()))
        newFiles.prepend(dirPath / "..");

    content->resetModel(newFiles);

    changeCurrentPath(dirPath);
}

void ManagerPanelController::connectSignals()
{
    connect(ui->fileTableView, &QTableView::doubleClicked,
            this, &ManagerPanelController::slotTableViewDBClick);
    connect(ui->rootPathButton, &QPushButton::clicked,
            this, &ManagerPanelController::slotRootPathButtonClicked);
}

void ManagerPanelController::changeCurrentPath(const path &newPath)
{
    currentPath = newPath;
    ui->currentPathLabel->setText(
        QString::fromStdWString(newPath.wstring()));
    emit currentPathChanged(newPath);
}

// =============== SLOTS ===============

void ManagerPanelController::slotDiskButtonClicked(const path &diskPath, const QString &diskLabel)
{
    qDebug() << "Disk button clicked on " << diskPath.wstring(); // DEBUG

    error_code ec;

    if (currentPath.compare(diskPath)) {
        error_code ec;
        if (!exists(diskPath, ec) || ec.value() != errc::success) {
            qDebug() << "Message: "
                     << QString::fromLocal8Bit(ec.message().c_str())
                     << " Code: "
                     << ec.value();
            return;
        }

        Files newFiles = listDirectoryFiles(diskPath, ec);

        if (ec.value() != errc::success) return;

        content->resetModel(newFiles);
        content->sort(FILE_NAME);

        changeCurrentPath(diskPath);

        ui->diskLabel->setText(diskLabel);
    }
}

void ManagerPanelController::slotTableViewDBClick(const QModelIndex &index)
{
    if (!index.isValid()
        || index.row() > content->contentCount())
        return;

    error_code ec;
    path newPath = content->getContent(index.row());

    // =============== REGULAR FILE ===============
    if (is_regular_file(newPath)) {
    }

    // =============== DIRECTORY JUNCTION ===============
    if (IsDirectoryJunction(newPath.wstring().c_str())) {
    }

    // =============== DIRECTORY ===============
    if (is_directory(newPath, ec) && ec.value() == errc::success) {
        if (newPath.filename_is_dot_dot()) newPath = newPath.parent_path().parent_path();
        showDirectory(newPath, ec);
    }
}

void ManagerPanelController::slotRootPathButtonClicked()
{
    currentPath = currentPath.root_path();
    error_code ec;
    showDirectory(currentPath, ec);
    changeCurrentPath(currentPath);
}
