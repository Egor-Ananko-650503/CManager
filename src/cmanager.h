#ifndef CMANAGER_H
#define CMANAGER_H

#include <QMainWindow>
#include "cmanagerpanel.h"

namespace Ui {
class CManager;
}

class CManager : public QMainWindow
{
    Q_OBJECT

public:

    explicit CManager(QWidget *parent = nullptr);
    ~CManager();

private:

    Ui::CManager *ui;
    CManagerPanel *leftPanel;
    CManagerPanel *rightPanel;

    void configurePanel();

private slots:
};

#endif // CMANAGER_H
