#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTimer>
#include <QPainter>
#include <QRandomGenerator>
#include <QVector>
#include <QPointF>

struct Raindrop {
    QPointF position;
    float angle; // Angle of the drop's tilt

    Raindrop(float x, float y, float a) : position(x, y), angle(a) {}
};

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

protected:
    void paintEvent(QPaintEvent *event) override;

private slots:
    void updateRaindrops();
    void on_comboBoxIntensity_currentIndexChanged(int index);
    void on_sliderAngle_valueChanged(int value);
    void on_spinBoxSize_valueChanged(int value);
    void on_sliderSpeed_valueChanged(int value); // New slot for speed control

private:
    void addRaindrops(int count);

    Ui::MainWindow *ui;
    QVector<Raindrop> raindrops;
    QTimer *timer;
    int dropWidth;
    int dropHeight;
    int rainSpeed; // New variable for controlling speed
};

#endif // MAINWINDOW_H
