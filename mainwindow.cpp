#include <QtWidgets>
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "patterns.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    game(new Game(this)),
    patterns(new Patterns())
{
    ui->setupUi(this);

    int minInterval = 50;
    int maxInterval = 2000;
    int minSize = 10;
    int maxSize = 100;

    for(int i = 0; i < patterns->getPatternsNames().size(); i++)
    {
        ui->comboBox->addItem(patterns->getPatternsNames()[i]);
    }

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

    connect(ui->saveButton, SIGNAL(clicked(bool)), this, SLOT(saveToFile()));
    connect(ui->loadButton, SIGNAL(clicked(bool)), this, SLOT(loadFromFile()));

    ui->mainLayout->setStretchFactor(ui->gridLayout, 8);
    ui->mainLayout->setStretchFactor(ui->settingsLayout, 2);
    ui->gridLayout->addWidget(game);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::saveToFile()
{
    QString filename = QFileDialog::getSaveFileName(this,
                                                    tr("Save game to file"),
                                                    QDir::homePath());
    if(filename.length() < 1)
        return;
    QFile file(filename);
    if(!file.open(QIODevice::WriteOnly | QIODevice::Truncate))
        return;
    QString size = QString::number(game->cellNumber()) + "\n";
    file.write(size.toUtf8());
    file.write(game->state().toUtf8());
    QString interval = QString::number(game->interval()) + "\n";
    file.write(interval.toUtf8());
    file.close();
}

void MainWindow::loadFromFile()
{
    QString filename = QFileDialog::getOpenFileName(this,
                                                    tr("Load game from file"),
                                                    QDir::homePath());
    if(filename.length() < 1)
        return;
    QFile file(filename);
    if(!file.open(QIODevice::ReadOnly))
        return;
    QTextStream in(&file);
    int size;
    in >> size;
    ui->sizeSlider->setValue(size);
    game->setCellNumber(size);
    QString state = "";
    for(int i = 0; i != size; i++)
    {
        QString row;
        in >> row;
        state.append(row + "\n");
    }
    game->setState(state);
    int interval;
    in >> interval;
    ui->intervalSlider->setValue(interval);
    game->setInterval(interval);
}

void MainWindow::loadPattern(QString name)
{
    QFile file(name);
    QDir::setCurrent("/home/miki/Qt/Projects/GameOfLife/GameOfLife/Patterns");
    if(!file.open(QIODevice::ReadOnly))
        return;
    QTextStream in(&file);
    int size;
    in >> size;
    ui->sizeSlider->setValue(size);
    game->setCellNumber(size);
    QString state = "";
    for(int i = 0; i != size; i++)
    {
        QString row;
        in >> row;
        state.append(row + "\n");
    }
    game->setState(state);
    int interval;
    in >> interval;
    ui->intervalSlider->setValue(interval);
    game->setInterval(interval);
}

void MainWindow::on_patternButton_clicked()
{
    this->loadPattern(ui->comboBox->currentText());
}
