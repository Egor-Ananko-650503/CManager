#pragma once
#include <QPushButton>
#include <QWidget>
#include <QString>
#include <QIcon>
#include <boost/filesystem.hpp>

using namespace boost::filesystem;

class DiskButton : public QPushButton
{
    Q_OBJECT
public:
    path diskPath;

    DiskButton(const path &path, QWidget *parent = nullptr);
    DiskButton(const path &path, const QString &text, QWidget *parent = nullptr);
    DiskButton(const path &path, const QIcon &icon, const QString &text, QWidget *parent = nullptr);
    DiskButton(const path &path, QPushButtonPrivate &dd, QWidget *parent = nullptr);
    ~DiskButton();

    void setPath(const path &path);

signals:
    void diskClicked(const path &path);

public slots:
    void resendingSignal();
};
