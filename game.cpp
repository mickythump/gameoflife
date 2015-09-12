#include <QtWidgets>
#include "game.h"

Game::Game(QWidget *parent) :
    QWidget(parent),
    timer(new QTimer(this)),
    generations(-1)
{
    timer->setInterval(300);
    universe = new bool[(universeSize + 2) * (universeSize + 2)];
    next = new bool[(universeSize + 2) * (universeSize + 2)];
    memset(universe, false, sizeof(bool)*(universeSize + 2) * (universeSize + 2));
    memset(next, false, sizeof(bool)*(universeSize + 2) * (universeSize + 2));
}

Game::~Game()
{

}

void Game::paintEvent(QPaintEvent *)
{
    QPainter p(this);
    paintGrid(p);
    paintUniverse(p);
}

void Game::mousePressEvent(QMouseEvent *e)
{
    double cellWidth = (double) width() / universeSize;
    double cellHeight = (double) height() / universeSize;
    int k = floor(e->y() / cellHeight) + 1;
    int j = floor(e->x() / cellWidth) + 1;
    universe[k * universeSize + j] = !universe[k * universeSize + j];
    update();
}

void Game::startGame(const int &number)
{
    generations = number;
    timer->start();
}

int Game::cellNumber()
{
    return universeSize;
}

void Game::setCellNumber(const int &s)
{
    universeSize = s;
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

QColor Game::masterColor()
{
    return m_masterColor;
}

void Game::paintGrid(QPainter &p)
{
    m_masterColor.red();
    QRect borders(0, 0, width() - 1, height() - 1);
    QColor gridColor = m_masterColor;
    gridColor.setAlpha(10);
    p.setPen(gridColor);
    double cellWidth = (double)width() / universeSize;
    for(double k = cellWidth; k <= width(); k += cellWidth)
        p.drawLine(k, 0, k, height());
    double cellHeight = (double)height() / universeSize;
    for(double k = cellHeight; k <= height(); k += cellHeight)
        p.drawLine(0, k, width(), k);
    p.drawRect(borders);
}

void Game::paintUniverse(QPainter &p)
{
    m_masterColor.red();
    double cellWidth = (double)width() / universeSize;
    double cellHeight = (double)height() / universeSize;
    for(int k = 1; k <= universeSize; k++)
    {
        for(int j = 1; j <= universeSize; j++)
        {
            if(universe[k*universeSize + j] == true)
            {
                qreal left = (qreal)(j*cellWidth - cellWidth);
                qreal top = (qreal)(k*cellHeight - cellHeight);
                QRectF r(left, top, (qreal) cellWidth, (qreal) cellHeight);
                p.fillRect(r, QBrush(m_masterColor));
            }
        }
    }
}

void Game::resetUniverse()
{
    delete [] universe;
    delete [] next;
    universe = new bool[(universeSize + 2) * (universeSize + 2)];
    next = new bool[(universeSize + 2) * (universeSize + 2)];
    memset(universe, false, sizeof(bool)*(universeSize + 2) * (universeSize + 2));
    memset(next, false, sizeof(bool)*(universeSize + 2) * (universeSize + 2));
}

