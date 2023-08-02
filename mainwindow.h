#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QFileDialog>
#include <QFile>
#include <QMessageBox>

#include <config_view.h>
#include <draw_error_view.h>
#include <draw_result_view.h>


namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_GINS_Caculate_clicked();

    void on_GINS_Output_clicked();

    void on_GNSS_Input_clicked();

    void on_INS_Input_clicked();

    void on_GINS_Close_clicked();

    void on_GINS_Config_clicked();

    void on_GINS_Plot_clicked();

    void Input_Config_Yaml();

    void Save_Pos();

    void Information_action();

    void Exit_action();

    void Draw_result();

    void Draw_error();

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
