#ifndef DISKLISTENER_H
#define DISKLISTENER_H

#include <QObject>
#include <QThread>
#include <windows.h>

class DiskListener : public QObject
{
    Q_OBJECT

public:

    explicit DiskListener(QObject *parent = nullptr);

    bool fExit = false;

    void listenDiskMask();
    void close();

private:

    DWORD oldMask = 0;

signals:

    void diskMaskChanged(unsigned long disks);

public slots:
};

#endif // DISKLISTENER_H
