#ifndef CONFIG_VIEW_H
#define CONFIG_VIEW_H

#include <QMainWindow>

namespace Ui {
class Config_View;
}

class Config_View : public QMainWindow
{
    Q_OBJECT

public:
    explicit Config_View(QWidget *parent = 0);
    ~Config_View();

private slots:
    void on_complete_clicked();

    void on_Cancellation_clicked();

private:
    Ui::Config_View *ui;
};

#endif // CONFIG_VIEW_H
