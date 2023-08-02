#include "config_view.h"
#include "ui_config_view.h"

Config_View::Config_View(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::Config_View)
{
    ui->setupUi(this);
    this->setWindowTitle("KF—GINS组合导航解算程序配置界面");
    setWindowIcon(QIcon(":/Picture/ICON.ico"));
}

Config_View::~Config_View()
{
    delete ui;
}

void Config_View::on_complete_clicked()
{
    this->close();
}

void Config_View::on_Cancellation_clicked()
{
    this->close();
}
