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

    for(int i = 0; i < patterns->getPatternsNames().size(); i++)
    {
        ui->comboBox->addItem(patterns->getPatternsNames()[i]);
    }

//    Ustawienie ikon dla akcji w menu oraz buttonów

    ui->actionStart->setIcon(QIcon(":/icons/start.png"));
    ui->actionStop->setIcon(QIcon(":/icons/stop.png"));
    ui->actionReset->setIcon(QIcon(":/icons/reset.png"));
    ui->actionStep->setIcon(QIcon(":/icons/step.png"));
    ui->actionLoad->setIcon(QIcon(":/icons/load.png"));
    ui->actionSave->setIcon(QIcon(":/icons/save.png"));
    ui->startButton->setIcon(QIcon(":/icons/start.png"));
    ui->stopButton->setIcon(QIcon(":/icons/stop.png"));
    ui->resetButton->setIcon(QIcon(":/icons/reset.png"));
    ui->stepButton->setIcon(QIcon(":/icons/step.png"));
    ui->loadButton->setIcon(QIcon(":/icons/load.png"));
    ui->saveButton->setIcon(QIcon(":/icons/save.png"));

//    Zmienne określające min/max interwał pomiędzy generacjami oraz min/max rozmiar siatki

    int minInterval = 50;
    int maxInterval = 2000;
    int minSize = 10;
    int maxSize = 100;

//    Ustawienie min/max wartości dla sliderów i spinboxów interwału oraz rozmiaru siatki

    ui->intervalSlider->setMinimum(minInterval);
    ui->intervalSlider->setMaximum(maxInterval);
    ui->sizeSlider->setMinimum(minSize);
    ui->sizeSlider->setMaximum(maxSize);
    ui->intervalSpinBox->setMinimum(minInterval);
    ui->intervalSpinBox->setMaximum(maxInterval);
    ui->sizeSpinBox->setMinimum(minSize);
    ui->sizeSpinBox->setMaximum(maxSize);

//    Powiązanie sliderów i spinboxów ze slotami odpowiadającymi za zmiany rozmiaru siatki oraz interwału

    connect(ui->intervalSlider, SIGNAL(valueChanged(int)), game, SLOT(setInterval(int)));
    connect(ui->intervalSlider, SIGNAL(valueChanged(int)), ui->intervalSpinBox, SLOT(setValue(int)));
    connect(ui->intervalSpinBox, SIGNAL(valueChanged(int)), ui->intervalSlider, SLOT(setValue(int)));
    connect(ui->sizeSlider, SIGNAL(valueChanged(int)), game, SLOT(setCellNumber(int)));
    connect(ui->sizeSlider, SIGNAL(valueChanged(int)), ui->sizeSpinBox, SLOT(setValue(int)));
    connect(ui->sizeSpinBox, SIGNAL(valueChanged(int)), ui->sizeSlider, SLOT(setValue(int)));
    connect(ui->intervalSpinBox, SIGNAL(valueChanged(int)), game, SLOT(setInterval(int)));
    connect(ui->sizeSpinBox, SIGNAL(valueChanged(int)), game, SLOT(setCellNumber(int)));

    ui->intervalSpinBox->setValue(int(ui->intervalSlider->value()));
    ui->sizeSpinBox->setValue(int(ui->sizeSlider->value()));

//    Wypisanie numeru aktualnej iteracji na pasku statusu

    ui->statusBar->showMessage(game->getIterations());
    connect(game, SIGNAL(newIteration(QString)), ui->statusBar, SLOT(showMessage(QString)));

//    Powiązanie akcji wciśnięcia buttonów oraz akcji w menu z odpowiadającymi im slotami

    connect(ui->startButton, SIGNAL(clicked()), game, SLOT(startGame()));
    connect(ui->actionStart, SIGNAL(triggered(bool)), game, SLOT(startGame()));
    connect(ui->stopButton, SIGNAL(clicked()), game, SLOT(stopGame()));
    connect(ui->actionStop, SIGNAL(triggered(bool)), game, SLOT(stopGame()));
    connect(ui->resetButton, SIGNAL(clicked()), game, SLOT(resetGame()));
    connect(ui->actionReset, SIGNAL(triggered(bool)), game, SLOT(resetGame()));
    connect(ui->stepButton, SIGNAL(clicked()), game, SLOT(step()));
    connect(ui->actionStep, SIGNAL(triggered(bool)), game, SLOT(step()));

    connect(ui->saveButton, SIGNAL(clicked(bool)), this, SLOT(saveToFile()));
    connect(ui->actionSave, SIGNAL(triggered(bool)), this, SLOT(saveToFile()));
    connect(ui->loadButton, SIGNAL(clicked(bool)), this, SLOT(loadFromFile()));
    connect(ui->actionLoad, SIGNAL(triggered(bool)), this, SLOT(loadFromFile()));

    ui->mainLayout->setStretchFactor(ui->gridLayout, 8);
    ui->mainLayout->setStretchFactor(ui->settingsLayout, 2);
    ui->gridLayout->addWidget(game);
}

MainWindow::~MainWindow()
{
    delete ui;
}

//Funkcja zapisująca stan siatki do pliku w formacie:
//rozmiar_siatki interwał
//stan siatki

void MainWindow::saveToFile()
{
    QString filename = QFileDialog::getSaveFileName(this, tr("Save game to file"), QDir::homePath());
    if(filename.length() < 1)
        return;
    QFile file(filename);
    if(!file.open(QIODevice::WriteOnly | QIODevice::Truncate)) // jeśli plik istnieje, to skasuj jego zawartość i pisz od nowa
    {
        qDebug() << "Error opening the file";
        return;
    }
    QString size = QString::number(game->cellNumber()) + " ";
    file.write(size.toUtf8());
    QString interval = QString::number(game->interval()) + "\n";
    file.write(interval.toUtf8());
    file.write(game->state().toUtf8());
    file.close();
}

//Funkcja wczytująca stan siatki z pliku

void MainWindow::loadFromFile()
{
    QString filename = QFileDialog::getOpenFileName(this,
                                                    tr("Load game from file"),
                                                    QDir::homePath());
    if(filename.length() < 1)
        return;
    QFile file(filename);
    if(!file.open(QIODevice::ReadOnly))
    {
        qDebug() << "Error opening the file";
        return;
    }
    QTextStream in(&file);
    int size, interval;
    in >> size >> interval;
    ui->sizeSlider->setValue(size);
    game->setCellNumber(size);
    ui->intervalSlider->setValue(interval);
    game->setInterval(interval);
    QString state = "";
    for(int i = 0; i != size; i++)
    {
        QString row;
        in >> row;
        state.append(row + "\n");
    }
    game->setState(state);
}

//Funkcja jako argument przyjmuje nazwę szablonu, który potem wczytuje z folderu, w którym znajdują się gotowe szablony

void MainWindow::loadPattern(QString name)
{
    QFile file(name);
    QDir::setCurrent("/home/miki/Qt/Projects/GameOfLife/GameOfLife/Patterns");
    if(!file.open(QIODevice::ReadOnly))
        return;
    QTextStream in(&file);
    int size, interval;
    in >> size >> interval;
    ui->sizeSlider->setValue(size);
    game->setCellNumber(size);
    ui->intervalSlider->setValue(interval);
    game->setInterval(interval);
    QString state = "";
    for(int i = 0; i != size; i++)
    {
        QString row;
        in >> row;
        state.append(row + "\n");
    }
    game->setState(state);
}

//Funkcja wywołująca powyższą funkcję po kliknięciu na przycisk załadowania szablonu

void MainWindow::on_patternButton_clicked()
{
    this->loadPattern(ui->comboBox->currentText());
}

