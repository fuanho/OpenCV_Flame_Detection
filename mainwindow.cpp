#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "converter.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::setSlider()
{
    ui->horizontalSlider->setMinimum(0);
    ui->horizontalSlider->setMaximum(cap.get(cv::CAP_PROP_FRAME_COUNT));
}

void MainWindow::timer_tick()
{
    cap.read(frameColor);
    if(frameColor.empty()){
        cap.set(cv::CAP_PROP_POS_FRAMES, 0);
        cap.read(frameColor);
    }

    int pos = cap.get(cv::CAP_PROP_POS_FRAMES);
    ui->horizontalSlider->setSliderPosition(pos);

    cv::Mat frameHSI;
    cv::Mat frameThreshold;
    cv::Mat frameSubtractedF;
    cv::Mat frameSubtractedL;
    cv::Mat frameSubtracted;
    cv::Mat res;
    cv::subtract(frameColor, firstFrameColor, frameSubtractedF);
    cv::subtract(frameColor, lastFrameColor, frameSubtractedL);
    cv::subtract(frameSubtractedF, frameSubtractedL, frameSubtracted);
    mycv::Converter::cvtColor(frameSubtracted, frameHSI, mycv::COLOR_BGR2HSI);
    mycv::Converter::flameDetection(frameSubtracted, frameHSI, frameThreshold);
    cv::Mat element = cv::getStructuringElement(cv::MORPH_CROSS, cv::Size(13, 13), cv::Point(6, 6));
    cv::dilate(frameThreshold, frameThreshold, element);
    cv::bitwise_and(frameColor, frameColor, res, frameThreshold);
    lastFrameColor = frameColor.clone();
    cv::Size size = cv::Size(700, 500);
    cv::resize(res, res, size, cv::INTER_AREA);

    QImage image = QImage(res.data, res.cols, res.rows, QImage::Format_BGR888);
    QImage image2 = QImage(frameColor.data, frameColor.cols, frameColor.rows, QImage::Format_BGR888);
    ui->label->setPixmap(QPixmap::fromImage(image));
    ui->label_2->setPixmap(QPixmap::fromImage(image2));
    ui->label_2->setScaledContents(true);
}

void MainWindow::on_playButton_clicked()
{
    if(frameColor.empty())
        return;
    if(timer->isActive())
    {
        timer->stop();
        ui->playButton->setText("Play");
    }
    else
    {
        timer->start(1000/fps);
        ui->playButton->setText("Stop");
    }
}

void MainWindow::on_horizontalSlider_valueChanged(int value)
{
    cap.set(cv::CAP_PROP_POS_FRAMES, value);
    cap.read(frameColor);
}

void MainWindow::on_actionOpenFile_triggered()
{
    filename = QFileDialog::getOpenFileName(this, "Select one file to open", "C:\\", "Video (*.avi)");
    if(filename == "")
        return;
    cap = cv::VideoCapture(filename.toStdString());
    fps = cap.get(cv::CAP_PROP_FPS);
    timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &MainWindow::timer_tick);
    setSlider();
    cap.read(frameColor);
    firstFrameColor = frameColor.clone();
    lastFrameColor = frameColor.clone();
    QImage image2 = QImage(frameColor.data, frameColor.cols, frameColor.rows, QImage::Format_BGR888);
    ui->label_2->setPixmap(QPixmap::fromImage(image2));
    ui->label_2->setScaledContents(true);
}
