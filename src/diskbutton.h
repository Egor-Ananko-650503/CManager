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
    DiskButton(const path &path, QWidget *parent = nullptr);
    DiskButton(const path &path, const QString &text, QWidget *parent = nullptr);
    DiskButton(const path &path, const QIcon &icon, const QString &text, QWidget *parent = nullptr);
    DiskButton(const path &path, QPushButtonPrivate &dd, QWidget *parent = nullptr);
    ~DiskButton();

    path diskPath;
    QString diskLabel = nullptr;

signals:
    void diskClicked(const path &path, const QString &label);

public slots:
    void resendingSignal();
};
