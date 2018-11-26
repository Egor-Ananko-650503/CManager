#include "disklistener.h"

DiskListener::DiskListener(QObject *parent) : QObject(parent)
{
}

void DiskListener::listenDiskMask()
{
    while (!fExit) {
        DWORD newMask = GetLogicalDrives();
        if (oldMask != newMask) {
            oldMask = newMask;
            emit diskMaskChanged(newMask);
        }

        QThread::sleep(3);
    }
}

void DiskListener::close()
{
    fExit = true;
}
