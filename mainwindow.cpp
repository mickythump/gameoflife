#include <QtWidgets>
#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    game(new Game(this))
{
    ui->setupUi(this);

    int minInterval = 50;
    int maxInterval = 2000;
    int minSize = 10;
    int maxSize = 100;

    ui->intervalSlider->setMinimum(minInterval);
    ui->intervalSlider->setMaximum(maxInterval);
    ui->sizeSlider->setMinimum(minSize);
    ui->sizeSlider->setMaximum(maxSize);
    ui->intervalSpinBox->setMinimum(minInterval);
    ui->intervalSpinBox->setMaximum(maxInterval);
    ui->sizeSpinBox->setMinimum(minSize);
    ui->sizeSpinBox->setMaximum(maxSize);

    connect(ui->intervalSlider, SIGNAL(valueChanged(int)), game, SLOT(setInterval(int)));
    connect(ui->intervalSlider, SIGNAL(valueChanged(int)), ui->intervalSpinBox, SLOT(setValue(int)));
    connect(ui->intervalSpinBox, SIGNAL(valueChanged(int)), ui->intervalSlider, SLOT(setValue(int)));
    connect(ui->sizeSlider, SIGNAL(valueChanged(int)), game, SLOT(setCellNumber(int)));
    connect(ui->sizeSlider, SIGNAL(valueChanged(int)), ui->sizeSpinBox, SLOT(setValue(int)));
    connect(ui->sizeSpinBox, SIGNAL(valueChanged(int)), ui->sizeSlider, SLOT(setValue(int)));
    connect(ui->intervalSpinBox, SIGNAL(valueChanged(int)), game, SLOT(setInterval(int)));
    connect(ui->sizeSpinBox, SIGNAL(valueChanged(int)), game, SLOT(setCellNumber(int)));

    ui->statusBar->showMessage(game->getIterations());
    connect(game, SIGNAL(newIteration(QString)), ui->statusBar, SLOT(showMessage(QString)));

    ui->intervalSpinBox->setValue(int(ui->intervalSlider->value()));
    ui->sizeSpinBox->setValue(int(ui->sizeSlider->value()));

    connect(ui->startButton, SIGNAL(clicked()), game, SLOT(startGame()));
    connect(ui->stopButton, SIGNAL(clicked()), game, SLOT(stopGame()));
    connect(ui->resetButton, SIGNAL(clicked()), game, SLOT(resetGame()));
    connect(ui->stepButton, SIGNAL(clicked()), game, SLOT(step()));
    connect(game, SIGNAL(environmentChanged(bool)), ui->sizeSlider, SLOT(setDisabled(bool)));
    connect(game, SIGNAL(gameEnds(bool)), ui->sizeSlider, SLOT(setEnabled(bool)));


    ui->mainLayout->setStretchFactor(ui->gridLayout, 8);
    ui->mainLayout->setStretchFactor(ui->settingsLayout, 2);
    ui->gridLayout->addWidget(game);
}

MainWindow::~MainWindow()
{
    delete ui;
}
