#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow), dropWidth(10), dropHeight(30), rainSpeed(5) {
    ui->setupUi(this);

    // Set up the timer for updating raindrops
    timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &MainWindow::updateRaindrops);
    timer->start(50); // Update every 50 ms

    // Populate the combo box for intensity
    ui->comboBoxIntensity->addItem("Light");
    ui->comboBoxIntensity->addItem("Moderate");
    ui->comboBoxIntensity->addItem("Heavy");

    // Connect signals
    connect(ui->comboBoxIntensity, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &MainWindow::on_comboBoxIntensity_currentIndexChanged);
    connect(ui->sliderAngle, &QSlider::valueChanged, this, &MainWindow::on_sliderAngle_valueChanged);
    connect(ui->spinBoxSize, QOverload<int>::of(&QSpinBox::valueChanged), this, &MainWindow::on_spinBoxSize_valueChanged);
    connect(ui->sliderSpeed, &QSlider::valueChanged, this, &MainWindow::on_sliderSpeed_valueChanged); // Connect speed slider

    // Add initial raindrops
    addRaindrops(10); // Start with 10 raindrops
}

MainWindow::~MainWindow() {
    delete ui;
}

void MainWindow::addRaindrops(int count) {
    for (int i = 0; i < count; ++i) {
        float x = static_cast<float>(QRandomGenerator::global()->bounded(width()));
        float angle = ui->sliderAngle->value();
        raindrops.append(Raindrop(x, 0, angle));
    }
}

void MainWindow::paintEvent(QPaintEvent *event) {
    Q_UNUSED(event);
    QPainter painter(this);

    // Set the background color to white
    painter.fillRect(rect(), Qt::white);

    // Set the brush color for raindrops
    painter.setBrush(Qt::blue);

    for (const Raindrop &drop : raindrops) {
        // Calculate the x offset based on the angle
        float angleRad = qDegreesToRadians(drop.angle);
        float xOffset = rainSpeed * qSin(angleRad); // Use rainSpeed for horizontal movement
        float yOffset = rainSpeed; // Constant vertical speed

        // Create a rectangle for the raindrop
        QRectF rect(drop.position.x() - dropWidth / 2.0, drop.position.y(), dropWidth, dropHeight);

        // Save the painter state
        painter .save();

        // Translate the painter to the center of the rectangle
        painter.translate(rect.center());

        // Rotate the painter
        painter.rotate(-drop.angle); // Rotate in the opposite direction

        // Draw the rectangle
        painter.drawRect(-dropWidth / 2.0, -dropHeight, dropWidth, dropHeight);

        // Restore the painter state
        painter.restore();
    }
}

void MainWindow::updateRaindrops() {
    for (Raindrop &drop : raindrops) {
        // Update the position of each raindrop based on its angle
        float angleRad = qDegreesToRadians(drop.angle);
        float xOffset = rainSpeed * qSin(angleRad); // Use rainSpeed for horizontal movement
        float yOffset = rainSpeed; // Constant vertical speed

        drop.position.setX(drop.position.x() + xOffset);
        drop.position.setY(drop.position.y() + yOffset);

        // Reset raindrop if it goes off the bottom of the window
        if (drop.position.y() > height()) {
            drop.position.setY(0); // Reset y position
            drop.position.setX(static_cast<float>(QRandomGenerator::global()->bounded(width()))); // Randomize x position
        }
    }
    update(); // Trigger a repaint
}

void MainWindow::on_comboBoxIntensity_currentIndexChanged(int index) {
    switch (index) {
    case 0: // Light
        addRaindrops(5);
        break;
    case 1: // Moderate
        addRaindrops(10);
        break;
    case 2: // Heavy
        addRaindrops(20);
        break;
    }
}

void MainWindow::on_sliderAngle_valueChanged(int value) {
    for (Raindrop &drop : raindrops) {
        drop.angle = value;
    }
}

void MainWindow::on_spinBoxSize_valueChanged(int value) {
    dropWidth = value;
    dropHeight = value * 3; // Make the height 3 times the width
}

void MainWindow::on_sliderSpeed_valueChanged(int value) {
    rainSpeed = value; // Update rainSpeed based on the slider value
}
