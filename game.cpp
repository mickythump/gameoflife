#include <QtWidgets>
#include <QDebug>
#include "game.h"

Game::Game(QWidget *parent) :
    QWidget(parent),
    timer(new QTimer(this)),
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

//Nadpisana funkcja z klasy potomnej QWidget, która jest wywoływana za każdym razem, kiedy wywołana jest funkcja update()
//Jako argument przyjmuje wskaźnik do obiektu typu QPaintEvent wykorzystanego do narysowania siatki, a następnie wypełnienia jej
//w zależności od stanu

void Game::paintEvent(QPaintEvent *)
{
    QPainter p(this);
    paintGrid(p);
    paintUniverse(p);
}

//Nadpisana funkcja z klasy potomnej QWidget, która jest wywoływana podczas kliknięcia myszą
//Odnajduje współrzędne miejsca, które zostało kliknięte na siatce i na tej podstawie zmienia stan komórki

void Game::mousePressEvent(QMouseEvent *event)
{
    double cellWidth = (double) width() / gridSize;
    double cellHeight = (double) height() / gridSize;
    int i = floor(event->y() / cellHeight) + 1;
    int j = floor(event->x() / cellWidth) + 1;
    currentGrid[i * gridSize + j] = !currentGrid[i * gridSize + j];
    update();
}

//Funkcja startująca grę, odpalająca timer

void Game::startGame()
{
    timer->start();
}

//Funkcja zatrzymująca grę poprzez zatrzymanie timera

void Game::stopGame()
{
    timer->stop();
}

//Funkcja resetująca siatkę poprzez ustawienie wszystkich elementów tablicy stanów na false

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

//Funkcja wywoływana podczas kliknięcia przycisku STEP, umożliwiającego generowanie nowych iteracji krok po kroku

void Game::step()
{
    newGeneration();
}

//Funkcja zwracająca aktualny rozmiar siatki

int Game::cellNumber()
{
    return gridSize;
}

//Funkcja ustawiająca nowy rozmiar siatki. Wywołuje update, tak aby narysować siatkę o nowym rozmiarze

void Game::setCellNumber(const int &cells)
{
    gridSize = cells;
    resetUniverse();
    update();
}

//Funkcja zwracająca aktualny interwał timera

int Game::interval()
{
    return timer->interval();
}

//Funkcja ustawiająca aktualny interwał timera

void Game::setInterval(int msec)
{
    timer->setInterval(msec);
}

//Funkcja zwraca łańcuch znaków, będący stanem aktualnej siatki, wykorzystywana przy zapisie stanu siatki do pliku
//Jeśli komórka jest aktywna, do łańcucha znaków dodawany jest symbol '*', jeśli nieaktywna 'o'

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

//Funkcja przyjmująca łańcuch znaków i na jego podstawie ustawiająca stan siatki. Wywoływana przy wczytywaniu stanu siatki z pliku
//Analogicznie do powyższej, jeśli znak z łańcucha to '*', ustawia komórkę aktywną, jeśli 'o', nieaktywną

void Game::setState(const QString &data)
{
    int index = 0;
    for(int i = 1; i <= gridSize; i++)
    {
        for(int j = 1; j <= gridSize; j++)
        {
            currentGrid[i * gridSize + j] = data[index] == '*';
            index ++;
        }
        index ++;
    }
    update();
}

//Funkcja zwraca numer iteracji/kroku aktualnej gry

QString Game::getIterations()
{
    QString text = "Iteration: " + QString::number(iterations);
    newIteration(text);
    return text;
}

//Funkcja rysująca siatkę, bez wypełnienia.

void Game::paintGrid(QPainter &painter)
{
    QRect borders(0, 0, width() - 1, height() - 1);
    painter.setPen(QPen(QColor(200, 200, 255, 125)));
    double cellWidth = (double)width() / gridSize;
    for(double i = cellWidth; i <= width(); i += cellWidth)
        painter.drawLine(i, 0, i, height()); //rysowanie pionowych linii
    double cellHeight = (double)height() / gridSize;
    for(double i = cellHeight; i <= height(); i += cellHeight)
        painter.drawLine(0, i, width(), i); //rysowanie poziomych linii
    painter.drawRect(borders);
}

//Funkcja zamalowująca aktywne komórki

void Game::paintUniverse(QPainter &painter)
{
    QColor gridColor(23, 42, 39);
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

//Funkcja sprawdzająca stan siatki w kolejnej iteracji i na tej podstawie ustala, czy należy narysować kolejny stan, czy przerwać grę

void Game::newGeneration()
{
    iterations ++;
    qDebug() << getIterations();
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
    if(notChanged == gridSize * gridSize)   //sprawdzenie, czy wszystkie pola są takie same, jak w poprzedniej iteracji
    {
        QMessageBox::information(this, tr("Game lost sense"), tr("The End. Game now finishes."), QMessageBox::Ok);
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
}

//Główna mechanika symulacji Game of Life, za wikipedia:
//Martwa komórka, która ma dokładnie 3 żywych sąsiadów, staje się żywa w następnej jednostce czasu (rodzi się)
//Żywa komórka z 2 albo 3 żywymi sąsiadami pozostaje nadal żywa; przy innej liczbie sąsiadów umiera (z "samotności" albo "zatłoczenia").

//Funkcja wywoływana jest dla każdej z komórek przy przejściu do kolejnej iteracji

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

