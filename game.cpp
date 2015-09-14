#include <QtWidgets>
#include <QDebug>
#include "game.h"

Game::Game(QWidget *parent) :
    QWidget(parent),
    timer(new QTimer(this)),
    generations(-1),
    gridSize(50)
{
    timer->setInterval(300);
    currentGrid = new bool[(gridSize + 2) * (gridSize + 2)];
    nextGrid = new bool[(gridSize + 2) * (gridSize + 2)];
    connect(timer, SIGNAL(timeout()), this, SLOT(newGeneration()));
    memset(currentGrid, false, sizeof(bool)*(gridSize + 2) * (gridSize + 2));
    memset(nextGrid, false, sizeof(bool)*(gridSize + 2) * (gridSize + 2));
}

Game::~Game()
{
    delete [] currentGrid;
    delete [] nextGrid;
}

void Game::paintEvent(QPaintEvent *)
{
    QPainter p(this);
    paintGrid(p);
    paintUniverse(p);
}

void Game::mousePressEvent(QMouseEvent *event)
{
    double cellWidth = (double) width() / gridSize;
    double cellHeight = (double) height() / gridSize;
    int i = floor(event->y() / cellHeight) + 1;
    int j = floor(event->x() / cellWidth) + 1;
    currentGrid[i * gridSize + j] = !currentGrid[i * gridSize + j];
    update();
}

void Game::startGame(const int &number)
{
    generations = number;
    timer->start();
}

void Game::stopGame()
{
    timer->stop();
}

void Game::resetGame()
{
    for(int i = 1; i <= gridSize; i++)
    {
        for(int j = 1; j <= gridSize; j++)
        {
            currentGrid[i * gridSize + j] = false;
        }
    }
    iterations = 0;
    gameEnds(true);
    update();
}

void Game::step(const int &number)
{
    generations = number;
    newGeneration();
}

int Game::cellNumber()
{
    return gridSize;
}

void Game::setCellNumber(const int &cells)
{
    gridSize = cells;
    resetUniverse();
    update();
}

int Game::interval()
{
    return timer->interval();
}

void Game::setInterval(int msec)
{
    timer->setInterval(msec);
}

QString Game::state()
{
    char temp;
    QString state = "";
    for(int i = 1; i <= gridSize; i++)
    {
        for(int j = 1; j <= gridSize; j++)
        {
            if(currentGrid[i * gridSize + j] == true)
            {
                temp = '*';
            }
            else
            {
                temp = 'o';
            }
            state.append(temp);
        }
        state.append("\n");
    }
    return state;
}

void Game::setState(const QString &data)
{
    int index = 0;
    for(int i = 0; i <= gridSize; i++)
    {
        for(int j = 0; j <= gridSize; j++)
        {
            currentGrid[i * gridSize + j] = data[index] == '*';
            index ++;
        }
        index ++;
    }
    update();
}

QString Game::getIterations()
{
    QString text = "Iteration: " + QString::number(iterations);
    newIteration(text);
    return text;
}

void Game::paintGrid(QPainter &painter)
{
    QRect borders(0, 0, width() - 1, height() - 1);
    QColor gridColor;
    gridColor.red();
    gridColor.setAlpha(10);
    painter.setPen(gridColor);
    double cellWidth = (double)width() / gridSize;
    for(double i = cellWidth; i <= width(); i += cellWidth)
        painter.drawLine(i, 0, i, height());
    double cellHeight = (double)height() / gridSize;
    for(double i = cellHeight; i <= height(); i += cellHeight)
        painter.drawLine(0, i, width(), i);
    painter.drawRect(borders);
}

void Game::paintUniverse(QPainter &painter)
{
    QColor gridColor;
    gridColor.red();
    double cellWidth = (double)width() / gridSize;
    double cellHeight = (double)height() / gridSize;
    for(int i = 1; i <= gridSize; i++)
    {
        for(int j = 1; j <= gridSize; j++)
        {
            if(currentGrid[i * gridSize + j] == true)
            {
                qreal left = (qreal)(j * cellWidth - cellWidth);
                qreal top = (qreal)(i * cellHeight - cellHeight);
                QRectF r(left, top, (qreal) cellWidth, (qreal) cellHeight);
                painter.fillRect(r, QBrush(gridColor));
            }
        }
    }
}

void Game::newGeneration()
{
    iterations ++;
    qDebug() << getIterations();
    if(generations < 0)
        generations ++;
    int notChanged = 0;
    for(int i = 1; i <= gridSize; i++)
    {
        for(int j = 1; j <= gridSize; j++)
        {
            nextGrid[i * gridSize + j] = isAlive(i, j);
            if(nextGrid[i * gridSize + j] == currentGrid[i * gridSize + j])
                notChanged ++;
        }
    }
    if(notChanged == gridSize * gridSize)
    {
        QMessageBox::information(this, tr("Game lost sense"),
                                 tr("The End. Game now finishes."),
                                 QMessageBox::Ok);
        stopGame();
        iterations = 0;
        gameEnds(true);
        return;
    }
    for(int i = 1; i <= gridSize; i++)
    {
        for(int j = 1; j <= gridSize; j++)
            currentGrid[i * gridSize + j] = nextGrid[i * gridSize + j];
    }
    update();
    generations--;
    if(generations == 0)
    {
        stopGame();
        iterations = 0;
        gameEnds(true);
        QMessageBox::information(this, tr("Game finished."),
                                 tr("Iterations finished."),
                                 QMessageBox::Ok, QMessageBox::Cancel);
    }
}

bool Game::isAlive(int i, int j)
{
    int count = 0;
    count += currentGrid[(i + 1) * gridSize + j];
    count += currentGrid[(i - 1) * gridSize + j];
    count += currentGrid[i * gridSize + (j + 1)];
    count += currentGrid[i * gridSize + (j - 1)];
    count += currentGrid[(i + 1) * gridSize + (j - 1)];
    count += currentGrid[(i - 1) * gridSize + (j + 1)];
    count += currentGrid[(i - 1) * gridSize + (j - 1)];
    count += currentGrid[(i + 1) * gridSize + (j + 1)];
    if (((currentGrid[i * gridSize + j] == true) && (count == 2)) || (count == 3))
        return true;
    return false;
}

void Game::resetUniverse()
{
    delete [] currentGrid;
    delete [] nextGrid;
    currentGrid = new bool[(gridSize + 2) * (gridSize + 2)];
    nextGrid = new bool[(gridSize + 2) * (gridSize + 2)];
    memset(currentGrid, false, sizeof(bool)*(gridSize + 2) * (gridSize + 2));
    memset(nextGrid, false, sizeof(bool)*(gridSize + 2) * (gridSize + 2));
}

