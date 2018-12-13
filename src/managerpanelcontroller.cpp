#include "managerpanelcontroller.h"
#include "ui_managerpanel.h"

ManagerPanelController::ManagerPanelController(Ui::ManagerPanel *_ui, FileModel *_content,
                                               PFileOperation _fileOperation, QObject *parent) :
    QObject(parent)
{
    ui = _ui;
    content = _content;
    fileOperation = _fileOperation;
    fileOperationManager = new FileOperationManager;

    connectSignals();
    initShortcuts();
// initView();
// initContexMenu();

// setDisks(GetLogicalDrives());
// if (!diskButtons.empty())
    // emit diskButtons.first()->resendingSignal();

    qRegisterMetaType<error_code>("error_code");
}

ManagerPanelController::~ManagerPanelController()
{
    foreach (auto &elem, diskButtons)
        delete elem;
    diskButtons.clear();
    foreach (auto &elem, shortcutHash)
        delete elem;
    shortcutHash.clear();
    fileOperationManager->waitAll();
    delete fileOperationManager;
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
        directory_iterator dIterator(dirPath, ec);
        if (ec.value() == errc::success) { // TODO Error dialog
            for (directory_entry &dEntry
                 : dIterator)
                if (!(FILE_ATTRIBUTE_HIDDEN & GetFileAttributesW(dEntry.path().c_str()))
                    || showHiddenFiles)
                    newFiles.push_back(dEntry.path());

            std::sort(newFiles.begin(), newFiles.end());
        } // TODO Error dialog
    } // TODO Error dialog

    return newFiles;
}

void ManagerPanelController::initView()
{
}

void ManagerPanelController::initContexMenu()
{
}

void ManagerPanelController::initShortcuts()
{
    QTableView *tableView = ui->fileTableView;

    QShortcut *scCopy = new QShortcut(QKeySequence::Copy, tableView, nullptr,
                                      nullptr, Qt::WidgetShortcut);
    connect(scCopy, &QShortcut::activated,
            this, &ManagerPanelController::slotCopy);

    QShortcut *scCut = new QShortcut(QKeySequence::Cut, tableView, nullptr,
                                     nullptr, Qt::WidgetShortcut);
    connect(scCut, &QShortcut::activated,
            this, &ManagerPanelController::slotCut);

    QShortcut *scPaste = new QShortcut(QKeySequence::Paste, tableView, nullptr,
                                       nullptr, Qt::WidgetShortcut);
    connect(scPaste, &QShortcut::activated,
            this, &ManagerPanelController::slotPaste);

    QShortcut *scDelete = new QShortcut(QKeySequence::Delete, tableView, nullptr,
                                        nullptr, Qt::WidgetShortcut);
    connect(scDelete, &QShortcut::activated,
            this, &ManagerPanelController::slotDelete);

    QShortcut *scRename = new QShortcut(QKeySequence(Qt::Key_F2), tableView, nullptr,
                                        nullptr, Qt::WidgetShortcut);
    connect(scRename, &QShortcut::activated,
            this, &ManagerPanelController::slotRename);

    QShortcut *scEncrypt = new QShortcut(QKeySequence("Shift+Ctrl+E"), tableView, nullptr,
                                         nullptr, Qt::WidgetShortcut);
    connect(scEncrypt, &QShortcut::activated,
            this, &ManagerPanelController::slotEncrypt);

    QShortcut *scDecrypt = new QShortcut(QKeySequence("Shift+Ctrl+D"), tableView, nullptr,
                                         nullptr, Qt::WidgetShortcut);
    connect(scDecrypt, &QShortcut::activated,
            this, &ManagerPanelController::slotDecrypt);

    shortcutHash.insert(QKeySequence::Copy, scCopy);
    shortcutHash.insert(QKeySequence::Cut, scCut);
    shortcutHash.insert(QKeySequence::Paste, scPaste);
    shortcutHash.insert(QKeySequence(Qt::Key_F2), scRename);
    shortcutHash.insert(QKeySequence("Shift+Ctrl+E"), scEncrypt);
    shortcutHash.insert(QKeySequence("Shift+Ctrl+D"), scDecrypt);
}

void ManagerPanelController::processPath(path &_path)
{
    error_code ec;

    // =============== REGULAR FILE ===============
    if (is_regular_file(_path, ec) && ec.value() == errc::success) {
        ShellExecuteW(nullptr, L"open", _path.wstring().c_str(),
                      nullptr, nullptr, SW_SHOWNORMAL);
        return;
    } // TODO Error dialog

    // =============== DIRECTORY JUNCTION ===============
    if (IsDirectoryJunction(_path.wstring().c_str())) {
        wchar_t dest[MAX_PATH + 1];
        if (GetJunctionDestination(_path.wstring().c_str(), dest)) {
            _path = path(dest);
            processPath(_path);
        }
        return;
    }

    // =============== SYMLINK ===============
// if (is_symlink(_path, ec) && ec.value() == errc::success) {
// }

    // =============== DIRECTORY ===============
    if (is_directory(_path, ec) && ec.value() == errc::success) {
        if (_path.filename_is_dot_dot()) _path = _path.parent_path().parent_path();
        showDirectory(_path, ec);
        return;
    } // TODO Error dialog
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

    connect(fileOperationManager, &FileOperationManager::copySuccess,
            this, &ManagerPanelController::slotCopySuccess);
    connect(fileOperationManager, &FileOperationManager::copyFail,
            this, &ManagerPanelController::slotCopyFail);
    connect(fileOperationManager, &FileOperationManager::cutSuccess,
            this, &ManagerPanelController::slotCutSuccess);
    connect(fileOperationManager, &FileOperationManager::cutFail,
            this, &ManagerPanelController::slotCutFail);
    connect(fileOperationManager, &FileOperationManager::deleteSuccess,
            this, &ManagerPanelController::slotDeleteSuccess);
    connect(fileOperationManager, &FileOperationManager::deleteFail,
            this, &ManagerPanelController::slotDeleteFail);
    connect(fileOperationManager, &FileOperationManager::encryptSuccess,
            this, &ManagerPanelController::slotEncryptSuccess);
    connect(fileOperationManager, &FileOperationManager::encryptFail,
            this, &ManagerPanelController::slotEncryptFail);
    connect(fileOperationManager, &FileOperationManager::decryptSuccess,
            this, &ManagerPanelController::slotDecryptSuccess);
    connect(fileOperationManager, &FileOperationManager::decryptFail,
            this, &ManagerPanelController::slotDecryptFail);
}

void ManagerPanelController::changeCurrentPath(const path &newPath)
{
    currentPath = newPath;
    ui->currentPathLabel->setText(
        QString::fromStdWString(newPath.wstring()));
    emit currentPathChanged(newPath);
}

QString ManagerPanelController::formatSizeByThousands(QString original)
{
    int countMissing = original.count() % 3;
    return original.left(countMissing)
           + " "
           + original
           .right(original.count() - countMissing)
           .replace(QRegularExpression("(.{3})"), "\\1 ");
}

void ManagerPanelController::showSuccessfullMessage(QString message)
{
    QMessageBox::information(nullptr, "Success", message);
}

void ManagerPanelController::showFailMessage(QString message)
{
    QMessageBox::critical(nullptr, "Fail", message);
}

// =============== SLOTS ===============

void ManagerPanelController::slotDiskButtonClicked(const path &diskPath, const QString &diskLabel)
{
    qDebug() << "Disk button clicked on " << diskPath.wstring(); // DEBUG

    error_code ec;

    if (currentPath.compare(diskPath)) {
        if (currentPath.root_path().compare(diskPath.root_path())) {
            for (auto &&disk
                 : diskButtons) {
                if (!disk->diskPath.root_path()
                    .compare(currentPath.root_path())
                    ) {
                    disk->diskPath = currentPath;
                    break;
                }
            }
        }

        error_code _ec;
        if (!exists(diskPath, _ec) || _ec.value() != errc::success) {
            qDebug() << "Message: "
                     << QString::fromLocal8Bit(_ec.message().c_str())
                     << " Code: "
                     << _ec.value();
            return;
        } // TODO Error dialog

        showDirectory(diskPath, _ec);

        ui->diskLabel->setText(diskLabel);

        ULARGE_INTEGER totalBytes,
                       totalFreeBytes;
        GetDiskFreeSpaceEx(diskPath.wstring().c_str(),
                           nullptr,
                           &totalBytes,
                           &totalFreeBytes);
        ui->diskFreeSpaceButton->setText(
            formatSizeByThousands(QString::number(totalFreeBytes.QuadPart / 1024))
            +" КиБ из "
            +formatSizeByThousands(QString::number(totalBytes.QuadPart / 1024))
            +" КиБ свободно");
    } else {
        slotRootPathButtonClicked();
    }
}

void ManagerPanelController::slotTableViewDBClick(const QModelIndex &index)
{
    if (!index.isValid()
        || index.row() > content->contentCount())
        return;

    path newPath = content->getContent(index.row());

    processPath(newPath);
}

void ManagerPanelController::slotRootPathButtonClicked()
{
    currentPath = currentPath.root_path();
    error_code ec;
    showDirectory(currentPath, ec);
    changeCurrentPath(currentPath);
}

void ManagerPanelController::slotCopy()
{
    try {
        qDebug() << "Ctrl+C";
        QTableView *tableView = ui->fileTableView;
        QModelIndex index = tableView->selectionModel()->currentIndex();
        if (index.isValid()
            && index.row() < content->contentCount()
            && index.row() >= 0
            && !content->getContent(index.row()).filename_is_dot_dot()
            ) {
            fileOperation->sourcePath = content->getContent(index.row());
            fileOperation->operation = COPY;
        }
        qDebug() << fileOperation->operation << " " << fileOperation->sourcePath.wstring();
    } catch (...) {
        qDebug() << "Exception Ctrl+C";
    }
}

void ManagerPanelController::slotCut()
{
    try {
        qDebug() << "Ctrl+X";
        QTableView *tableView = ui->fileTableView;
        QModelIndex index = tableView->selectionModel()->currentIndex();
        if (index.isValid()
            && index.row() < content->contentCount()
            && index.row() >= 0
            && !content->getContent(index.row()).filename_is_dot_dot()
            ) {
            fileOperation->sourcePath = content->getContent(index.row());
            fileOperation->operation = CUT;
        }
        qDebug() << fileOperation->operation << " " << fileOperation->sourcePath.wstring();
    } catch (...) {
        qDebug() << "Exception Ctrl+X";
    }
}

void ManagerPanelController::slotPaste()
{
    try {
        qDebug() << "Ctrl+V";
        if (fileOperation->isValid()) {
            if (fileOperation->operation == COPY)
                fileOperationManager->copy(fileOperation->sourcePath, currentPath);
            if (fileOperation->operation == CUT)
                fileOperationManager->cut(fileOperation->sourcePath, currentPath);
        }
    } catch (...) {
        qDebug() << "Exception Ctrl+V";
    }
}

void ManagerPanelController::slotRename()
{
    qDebug() << "F2";
}

void ManagerPanelController::slotDelete()
{
    try {
        qDebug() << "Delete";
        QTableView *tableView = ui->fileTableView;
        QModelIndex index = tableView->selectionModel()->currentIndex();
        if (index.isValid()
            && index.row() < content->contentCount()
            && index.row() >= 0
            && !content->getContent(index.row()).filename_is_dot_dot()
            )
            fileOperationManager->remove(content->getContent(index.row()));
    } catch (...) {
        qDebug() << "Exception Delete";
    }
}

void ManagerPanelController::slotEncrypt()
{
    QTableView *tableView = ui->fileTableView;
    QModelIndex index = tableView->selectionModel()->currentIndex();
    if (index.isValid()
        && index.row() < content->contentCount()
        && index.row() >= 0
        && !content->getContent(index.row()).filename_is_dot_dot()
        )
        fileOperationManager->encrypt(content->getContent(index.row()));
}

void ManagerPanelController::slotDecrypt()
{
    QTableView *tableView = ui->fileTableView;
    QModelIndex index = tableView->selectionModel()->currentIndex();
    if (index.isValid()
        && index.row() < content->contentCount()
        && index.row() >= 0
        && !content->getContent(index.row()).filename_is_dot_dot()
        )
        fileOperationManager->decrypt(content->getContent(index.row()));
}

void ManagerPanelController::slotDeleteSuccess(QString message)
{
// qDebug() << this << " >> Delete success";
// qDebug() << "Message: " << message << endl;
    showSuccessfullMessage(message);
    error_code ec;
    showDirectory(currentPath, ec);
}

void ManagerPanelController::slotDeleteFail(error_code ec, QString message)
{
// qDebug() << this << " >> Delete fail";
    QString info("Value: ");
    info += QString::number(ec.value());
    info += " Message: ";
    info += QString::fromLocal8Bit(
        ec.message().c_str());

// qDebug() << "Info: " << info;
// qDebug() << "Message: " << message << endl;
    showFailMessage(info + '\n' + message);
}

void ManagerPanelController::slotCopySuccess(QString message)
{
// qDebug() << this << " >> Copy success";
// qDebug() << "Message: " << message << endl;
    showSuccessfullMessage(message);
    error_code ec;
    showDirectory(currentPath, ec);
}

void ManagerPanelController::slotCopyFail(error_code ec, QString message)
{
// qDebug() << this << " >> Copy fail";
    QString info("Value: ");
    info += QString::number(ec.value());
    info += " Message: ";
    info += QString::fromLocal8Bit(
        ec.message().c_str());

// qDebug() << "Info: " << info;
// qDebug() << "Message: " << message << endl;
    showFailMessage(info + '\n' + message);
}

void ManagerPanelController::slotCutSuccess(QString message)
{
// qDebug() << this << "Cut success";
// qDebug() << "Message: " << message << endl;
    showSuccessfullMessage(message);
    error_code ec;
    showDirectory(currentPath, ec);
}

void ManagerPanelController::slotCutFail(error_code ec, QString message)
{
// qDebug() << this << " >> Cut fail";
    QString info("Value: ");
    info += QString::number(ec.value());
    info += " Message: ";
    info += QString::fromLocal8Bit(
        ec.message().c_str());

// qDebug() << "Info: " << info;
// qDebug() << "Message: " << message << endl;
    showFailMessage(info + '\n' + message);
}

void ManagerPanelController::slotEncryptSuccess(QString message)
{
// qDebug() << this << " >> Encrypt (success\\fail)";
// qDebug() << message << endl;
    showSuccessfullMessage(message);
}

void ManagerPanelController::slotEncryptFail(QString message)
{
    showFailMessage(message);
}

void ManagerPanelController::slotDecryptSuccess(QString message)
{
// qDebug() << this << " >> Decrypt (success\\fail)";
// qDebug() << message << endl;
    showSuccessfullMessage(message);
}

void ManagerPanelController::slotDecryptFail(QString message)
{
    showFailMessage(message);
}
