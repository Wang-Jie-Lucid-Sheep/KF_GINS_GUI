#ifndef DRAW_ERROR_VIEW_H
#define DRAW_ERROR_VIEW_H

#include <QMainWindow>

namespace Ui {
class Draw_error_view;
}

class Draw_error_view : public QMainWindow
{
    Q_OBJECT

public:
    explicit Draw_error_view(QWidget *parent = 0);
    ~Draw_error_view();

private:
    Ui::Draw_error_view *ui;
};

#endif // DRAW_ERROR_VIEW_H
