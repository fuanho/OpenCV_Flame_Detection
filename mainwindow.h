#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QFileDialog>
#include <QtGui>
#include <QTimer>
#include <QList>

#include <opencv2/opencv.hpp>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_playButton_clicked();
    void timer_tick();
    void on_horizontalSlider_valueChanged(int value);
    void on_actionOpenFile_triggered();

private:
    Ui::MainWindow *ui;
    cv::VideoCapture cap;
    QTimer* timer = nullptr;
    QString filename;
    cv::Mat frameColor;
    cv::Mat lastFrameColor;
    int fps;
    void setSlider();
};
#endif // MAINWINDOW_H
