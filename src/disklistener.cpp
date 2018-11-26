#include "disklistener.h"

DiskListener::DiskListener(QObject *parent) : QObject(parent)
{
}

void DiskListener::listenDiskMask()
{
    DWORD newMask = GetLogicalDrives();
    if (oldMask != newMask) {
        oldMask = newMask;
        emit diskMaskChanged(newMask);
    }
}
