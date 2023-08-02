#include "draw_result_view.h"
#include "ui_draw_result_view.h"

Draw_result_view::Draw_result_view(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::Draw_result_view)
{
    ui->setupUi(this);
    this->setWindowTitle("KF—GINS组合导航解算程序-解算结果绘制");
    setWindowIcon(QIcon(":/Picture/ICON.ico"));
}

Draw_result_view::~Draw_result_view()
{
    delete ui;
}
