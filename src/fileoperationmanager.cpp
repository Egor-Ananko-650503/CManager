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
        bool isComplete;
        bool directoryJunction = IsDirectoryJunction(source.wstring().c_str());

        if (is_regular_file(source, ec)
            && ec.value() == errc::success
            ) {
            ::copy_file(source, receiver / source.filename(), ec);
            if (ec.value() == errc::success) {
                QString message = "Файл "
                                  + QString::fromStdWString(source.wstring())
                                  + " успешно скопирован в "
                                  + QString::fromStdWString(receiver.wstring());
                emit copySuccess(message);
            } else {
                emit copyFail(ec, "");
            }
            return;
        } else {
            if (!directoryJunction
                && is_directory(source, ec)
                && ec.value() == errc::success
                )
                isComplete = recursiveCopy(source, receiver / source.filename(), ec);

            if (isComplete) {
                QString message = "Директория "
                                  + QString::fromStdWString(source.wstring())
                                  + " успешно скопирована в "
                                  + QString::fromStdWString(receiver.wstring()) + '.';
                emit copySuccess(message);
            } else {
                emit copyFail(ec, "");
            }
        }
    });
    addFuture(future);
}

bool FileOperationManager::removeProcessing(const path &source, error_code &ec)
{
    qDebug() << source.wstring();

    bool isRegular = is_regular_file(source, ec);
    bool isSymLink = is_symlink(source, ec);
    bool isDir = is_directory(source, ec);
    if (ec.value() != errc::success) return false;

    // ===
    if (IsDirectoryJunction(source.wstring().c_str()))
        return DeleteJunctionPoint(source.wstring().c_str());

    // ===
    if (isRegular || isSymLink)
        return boost::filesystem::remove(source);

    // ===
    if (isDir) {
        remove_all(source, ec);
        if (ec.value() != errc::success)
            emit deleteFail(ec, "");
        return ec.value() == errc::success;
    }

    return false;
}

void FileOperationManager::remove(const path &source)
{
    qDebug() << source.wstring();
    error_code ec;
    bool isDir = is_directory(source, ec);
    if (ec.value() != errc::success) {
        emit deleteFail(ec, "On remove.");
        return;
    }
    QString message("Вы действительно хотите удалить ");
    message.append((isDir && !IsDirectoryJunction(source.wstring().c_str()))
                   ? "директорию "
                   : "файл ");
    message.append(QString::fromStdWString(source.filename().wstring()));
    message.append("?");

    QMessageBox::StandardButton confirm = QMessageBox::question(nullptr, "CManager", message);
    if (confirm == QMessageBox::StandardButton::No) return;

    QFuture<bool> future = QtConcurrent::run([=]() -> bool {
        error_code _ec;
        QString name = QString::fromStdWString(source.filename().wstring());
        bool isDirJncn = IsDirectoryJunction(source.wstring().c_str());
        bool isRegular = is_regular_file(source, _ec);
        bool isSymLink = is_symlink(source, _ec);
        bool _isDir = is_directory(source, _ec);
        if (_ec.value() != errc::success) {
            emit deleteFail(_ec, "");
            return false;
        }

        if (removeProcessing(source, _ec)) {
            if (_isDir && !isDirJncn) {
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
        } else {
            emit deleteFail(_ec, "");
        }

        return false;
    });
    addFuture(future);
}

void FileOperationManager::cut(const path &source, const path &receiver)
{
    qDebug() << source.wstring();
    qDebug() << (receiver / source.filename()).wstring();

    QFuture<void> future = QtConcurrent::run([=]() -> void {
        error_code ec;
        bool directoryJunction = IsDirectoryJunction(source.wstring().c_str());
        bool isRegular = is_regular_file(source, ec);

        if (isRegular
            && ec.value() == errc::success) {
            ::copy_file(source, receiver / source.filename(), ec);
            if (ec.value()) {
                emit cutFail(ec, "При копировании файла");
                return;
            }
        }

        if (!directoryJunction
            && is_directory(source, ec)
            && ec.value() == errc::success
            ) {
            if (!recursiveCopy(source, receiver / source.filename(), ec)) {
                emit cutFail(ec, "При копировании директории");
                return;
            }
        }

        if (removeProcessing(source, ec)) {
            QString message = (isRegular ? "Файл " : "Директория ")
                              + QString::fromStdWString(source.wstring())
                              + " успешно перемещ"
                              + (isRegular ? "ён." : "ена.");
            emit cutSuccess(message);
        } else {
            emit cutFail(ec, "");
        }
    });
    addFuture(future);
}

void FileOperationManager::encrypt(const path &source)
{
    QFuture<bool> future = QtConcurrent::run([=]() -> bool {
        if (!(GetFileAttributesW(source.wstring().c_str()) & FILE_ATTRIBUTE_ENCRYPTED)) {
            if (EncryptFileW(source.wstring().c_str())) {
                emit encryptSuccess("Успешно зашифровано.");
                return true;
            }
            emit encryptFail("Потрачено. (Шифровка)");
            return false;
        }
        emit encryptFail("Потрачено. (Файл уже зашифрован)");
        return false;
    });
    addFuture(future);
}

void FileOperationManager::decrypt(const path &source)
{
    QFuture<bool> future = QtConcurrent::run([=]() -> bool {
        if (GetFileAttributesW(source.wstring().c_str()) & FILE_ATTRIBUTE_ENCRYPTED) {
            unsigned long need = 0;
            if (DecryptFileW(source.wstring().c_str(), need)) {
                emit decryptSuccess("Успешно дешифровано.");
                return true;
            }
            emit decryptFail("Потрачено. (Дешифровка)");
            return false;
        }
        emit decryptFail("Потрачено. (Нешифрованный файл)");
        return false;
    });
    addFuture(future);
}

template<class T>
void FileOperationManager::addFuture(QFuture<T> future)
{
    auto watcher = new QFutureWatcher<void>;

    connect(watcher, &QFutureWatcher<void>::finished,
            this, &FileOperationManager::futureFinished);
    watcher->setFuture(future);

    threads.push_back(future);
    watchers.push_back(watcher);
}

void FileOperationManager::waitAll()
{
    for (auto &thread
         : threads)
        thread.waitForFinished();
}

void FileOperationManager::futureFinished()
{
    QList<int> forRemove;
    for (int i = 0; i < watchers.size(); ++i)
        if (watchers[i]->isFinished()) forRemove.push_back(i);
    qDebug() << watchers;
    qDebug() << forRemove;
    for (int i = forRemove.size() - 1; i >= 0; --i) {
        delete watchers[i];
        watchers.removeAt(forRemove[i]);
    }
    qDebug() << watchers << endl;
}
