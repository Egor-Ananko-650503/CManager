#include "fileoperationmanager.h"

FileOperationManager::FileOperationManager(QObject *parent) : QObject(parent)
{
}

bool FileOperationManager::recursiveCopy(const path &source, const path &receiver, error_code &ec)
{
    ::copy(source, receiver, ec);
    if (ec.value() != errc::success)
        return false;

    if (!IsDirectoryJunction(source.wstring().c_str())
        && is_directory(source, ec)
        && ec.value() == errc::success
        ) {
        for (directory_entry dEntry
             : directory_iterator(source)
             ) {
            recursiveCopy(dEntry.path(), receiver / dEntry.path().filename(), ec);
            if (ec.value() != errc::success) return false;
        }
    } else {
        return false;
    }

    return true;
}

void FileOperationManager::copy(const path &source, const path &receiver)
{
    qDebug() << source.wstring();
    qDebug() << (receiver / source.filename()).wstring();

    QFuture<void> future = QtConcurrent::run([=]() -> void {
        error_code ec;
        bool directoryJunction = IsDirectoryJunction(source.wstring().c_str());

        if (is_regular_file(source, ec)
            && ec.value() == errc::success) {
            ::copy_file(source, receiver / source.filename(), ec);
        } else {
            if (!directoryJunction
                && is_directory(source, ec)
                && ec.value() == errc::success
                )
                recursiveCopy(source, receiver / source.filename(), ec);
        }
        QString message = "Directory junction: ";
        message += (directoryJunction ? "true" : "false"); // DEBUG
        message += " RUN Value: ";
        message += QString::number(ec.value());
        message += " Message: ";
        message += QString::fromLocal8Bit(
            ec.message().c_str());
        emit operationSuccess(message);
    });
    threads.push_back(future);
}

bool FileOperationManager::removeProcessing(const path &source)
{
    qDebug() << source.wstring();
    error_code ec;

    // ===
    if (IsDirectoryJunction(source.wstring().c_str()))
        return DeleteJunctionPoint(source.wstring().c_str());

    // ===
    bool isRegular = is_regular_file(source, ec);
    if (ec.value() != errc::success) return false;
    bool isSymLink = is_symlink(source, ec);
    if (ec.value() != errc::success) return false;
    if (isRegular || isSymLink)
        return boost::filesystem::remove(source);

    // ===
    bool isDir = is_directory(source, ec);
    if (ec.value() != errc::success) return false;
    if (isDir) {
        remove_all(source, ec);
        return ec.value() == errc::success;
    }

    return false;
}

void FileOperationManager::remove(const path &source)
{
    qDebug() << source.wstring();
    error_code ec;
    QString message("Вы действительно хотите удалить ");
    bool isDir = is_directory(source, ec);
    if (ec.value() != errc::success) {
        emit deleteFail(ec, "On remove.");
        return;
    }
    message.append((isDir && !IsDirectoryJunction(source.wstring().c_str()))
                   ? "директорию "
                   : "файл ");
    message.append(QString::fromStdWString(source.filename().wstring()));
    message.append("?");

    QMessageBox::StandardButton confirm = QMessageBox::question(nullptr, "CManager", message);
    if (confirm == QMessageBox::StandardButton::No) return;

    QFuture<bool> future = QtConcurrent::run([=]() -> bool {
        error_code ec;
        QString name = QString::fromStdWString(source.filename().wstring());
        bool isDirJncn = IsDirectoryJunction(source.wstring().c_str());
        bool isRegular = is_regular_file(source, ec);
        if (ec.value() != errc::success) {
            emit deleteFail(ec, "");
            return false;
        }
        bool isSymLink = is_symlink(source, ec);
        if (ec.value() != errc::success) {
            emit deleteFail(ec, "");
            return false;
        }
        bool isDir = is_directory(source, ec);
        if (ec.value() != errc::success) {
            emit deleteFail(ec, "");
            return false;
        }

        if (removeProcessing(source)) {
            if (isDir && !isDirJncn) {
                emit deleteSuccess("Директория " + name + " удалёна.");
                return true;
            }
            if (isRegular || isSymLink) {
                emit deleteSuccess("Файл " + name + " удалён.");
                return true;
            }
            if (isDirJncn) {
                emit deleteSuccess("Junction point " + name + " удалён.");
                return true;
            }
        }

        return false;
    });
    threads.push_back(future);
}

void FileOperationManager::cut(const path &source, const path &receiver)
{
    qDebug() << source.wstring();
    qDebug() << (receiver / source.filename()).wstring();

    QFuture<void> future = QtConcurrent::run([=]() -> void {
        error_code ec;
        bool directoryJunction = IsDirectoryJunction(source.wstring().c_str());

        if (is_regular_file(source, ec)
            && ec.value() == errc::success) {
            ::copy_file(source, receiver / source.filename(), ec);
        } else {
            if (!directoryJunction
                && is_directory(source, ec)
                && ec.value() == errc::success
                )
                recursiveCopy(source, receiver / source.filename(), ec);
        }

        QString message = "Directory junction: ";
        message += (directoryJunction ? "true" : "false"); // DEBUG
        message += " RUN Value: ";
        message += QString::number(ec.value());
        message += " Message: ";
        message += QString::fromLocal8Bit(
            ec.message().c_str());

        bool remRes = removeProcessing(source);

        emit cutSuccess(message + (remRes ? " true" : " false"));
    });
    threads.push_back(future);
}

void FileOperationManager::encrypt(const path &source)
{
    QFuture<bool> future = QtConcurrent::run([=]() -> bool {
        if (!(GetFileAttributesW(source.wstring().c_str()) & FILE_ATTRIBUTE_ENCRYPTED)) {
            unsigned long need;
            if (EncryptFileW(source.wstring().c_str())) {
                emit encryptSuccess("Ecrypted");
                return true;
            }
            emit encryptFail("Not ecrypted");
        }
        return false;
    });
    threads.push_back(future);
}

void FileOperationManager::decrypt(const path &source)
{
    QFuture<bool> future = QtConcurrent::run([=]() -> bool {
        if (GetFileAttributesW(source.wstring().c_str()) & FILE_ATTRIBUTE_ENCRYPTED) {
            unsigned long need;
            if (DecryptFileW(source.wstring().c_str(), need)) {
                emit decryptSuccess("Decrypted");
                return true;
            }
            emit decryptFail("Not decrypted");
        }
        return false;
    });
    threads.push_back(future);
}

void FileOperationManager::waitAll()
{
    for (auto &thread
         : threads)
        thread.waitForFinished();
}
