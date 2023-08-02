#include "draw_error_view.h"
#include "ui_draw_error_view.h"

Draw_error_view::Draw_error_view(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::Draw_error_view)
{
    ui->setupUi(this);
    this->setWindowTitle("KF—GINS组合导航解算程序-误差结果绘制");
    setWindowIcon(QIcon(":/Picture/ICON.ico"));
}

Draw_error_view::~Draw_error_view()
{
    delete ui;
}
