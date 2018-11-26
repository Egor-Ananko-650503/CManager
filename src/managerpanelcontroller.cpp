#include "managerpanelcontroller.h"
#include "ui_managerpanel.h"

ManagerPanelController::ManagerPanelController(Ui::ManagerPanel *_ui, FileModel *_content,
                                               QObject *parent) :
    QObject(parent)
{
    ui = _ui;
    content = _content;
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
                path diskPath((diskLitera + ":\\").toStdString());

                DiskButton *button = new DiskButton(diskPath, diskLitera);
                connect(button, &DiskButton::diskClicked,
                        this, &ManagerPanelController::slotDiskButtonClicked);

                diskButtons.push_back(button);
            }
        }
        for (int i = 0; i < diskButtons.count(); ++i)
            ui->diskChooserHlayout->insertWidget(i, diskButtons[i]);
    }
}

// =============== SLOTS ===============

void ManagerPanelController::slotDiskButtonClicked(const path &diskPath)
{
    qDebug() << "Disk button clicked on " << diskPath.string().c_str(); // DEBUG

    error_code ec;
    if (!exists(diskPath, ec) || ec.value() != errc::success) {
        qDebug() << "Message: "
                 << QString::fromLocal8Bit(ec.message().c_str())
                 << " Code: "
                 << ec.value();
        return;
    }

    Files newFiles;
    for (directory_entry dEntry
         : directory_iterator(diskPath)) {
        path filePath = dEntry.path();
        newFiles.push_back(filePath);
    }

    content->reserModel(newFiles);
    content->sort(FILE_NAME);

    ui->currentPathLabel->setText(
        QString::fromStdString(diskPath.string()));
}
