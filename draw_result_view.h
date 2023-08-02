#ifndef DRAW_RESULT_VIEW_H
#define DRAW_RESULT_VIEW_H

#include <QMainWindow>

namespace Ui {
class Draw_result_view;
}

class Draw_result_view : public QMainWindow
{
    Q_OBJECT

public:
    explicit Draw_result_view(QWidget *parent = 0);
    ~Draw_result_view();

private:
    Ui::Draw_result_view *ui;
};

#endif // DRAW_RESULT_VIEW_H
