#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include "image.h"
#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    QString Path;

private slots:
    void on_actionOpen_triggered();

    void on_btnStartProcessing_clicked();

    void on_actionSave_triggered();

    void on_actionLight_triggered();

    void on_buttonChangeImages_clicked();

    void on_actionDark_triggered();

    void on_comboBox_filters_currentIndexChanged(int index);

private:
    Ui::MainWindow *ui;
    //QImage image;
    Image image;
};
#endif // MAINWINDOW_H
