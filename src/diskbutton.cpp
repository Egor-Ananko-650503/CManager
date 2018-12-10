#include "diskbutton.h"

DiskButton::DiskButton(const path &path, QWidget *parent) :
    QPushButton(parent),
    diskPath(path)
{
    connect(this, SIGNAL(clicked()),
            this, SLOT(resendingSignal()));
}

DiskButton::DiskButton(const path &path, const QString &text, QWidget *parent) :
    QPushButton(text, parent),
    diskPath(path)
{
    connect(this, SIGNAL(clicked()),
            this, SLOT(resendingSignal()));
}

DiskButton::DiskButton(const path &path, const QIcon &icon, const QString &text, QWidget *parent) :
    QPushButton(icon, text, parent),
    diskPath(path)
{
    connect(this, SIGNAL(clicked()),
            this, SLOT(resendingSignal()));
}

DiskButton::DiskButton(const path &path, QPushButtonPrivate &dd, QWidget *parent) :
    QPushButton(dd, parent),
    diskPath(path)
{
    connect(this, SIGNAL(clicked()),
            this, SLOT(resendingSignal()));
}

DiskButton::~DiskButton()
{
}

void DiskButton::resendingSignal()
{
    emit diskClicked(diskPath, diskLabel);
}
