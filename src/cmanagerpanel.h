#ifndef CMANAGERPANEL_H
#define CMANAGERPANEL_H

#include <QWidget>

namespace Ui {
class CManagerPanel;
}

class CManagerPanel : public QWidget
{
    Q_OBJECT

public:
    explicit CManagerPanel(QWidget *parent = nullptr);
    ~CManagerPanel();

private:
    Ui::CManagerPanel *ui;
};

#endif // CMANAGERPANEL_H
