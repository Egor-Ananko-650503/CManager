#ifndef FILEOPERATIONMANAGER_H
#define FILEOPERATIONMANAGER_H

#include <QObject>
#include <QtConcurrent/QtConcurrentRun>
#include <QFuture>
#include <QFutureWatcher>
#include <QMessageBox>

#include "boost/filesystem.hpp"

#include "junctionpoint.h"

using namespace boost::filesystem;
using namespace boost::system;
using namespace neosmart;

class FileOperationManager : public QObject
{
    Q_OBJECT

    template<class T>
    void addFuture(QFuture<T> future);

public:
    explicit FileOperationManager(QObject *parent = nullptr);

    QList<QFuture<void> > threads;
    QList<QFutureWatcher<void> *> watchers;

    bool recursiveCopy(const path &source, const path &receiver, error_code &ec);
    void copy(const path &source, const path &receiver);
    bool removeProcessing(const path &source, error_code &ec);
    void remove(const path &source);
    void cut(const path &source, const path &receiver);
    void encrypt(const path &source);
    void decrypt(const path &source);
    void waitAll();

signals:
    void errorOccured(error_code ec);
    void operationSuccess(QString message);
    void copySuccess(QString message);
    void copyFail(error_code ec, QString message);
    void cutSuccess(QString message);
    void cutFail(error_code ec, QString message);
    void deleteSuccess(QString message);
    void deleteFail(error_code ec, QString message);
    void decryptSuccess(QString message);
    void decryptFail(QString message);
    void encryptSuccess(QString message);
    void encryptFail(QString message);

public slots:
    void futureFinished();
};

#endif // FILEOPERATIONMANAGER_H
