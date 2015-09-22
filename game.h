#ifndef GAME_H
#define GAME_H

#include <QWidget>
#include <QColor>

class Game : public QWidget
{
    Q_OBJECT
public:
    explicit Game(QWidget *parent = 0);
    ~Game();

protected:
    void paintEvent(QPaintEvent *);
    void mousePressEvent(QMouseEvent *event);

signals:
    void gameEnds(bool ok);
    void newIteration(QString iter);

public slots:
    void startGame();
    void stopGame();
    void resetGame();
    void step();

    int cellNumber();
    void setCellNumber(const int &cells);

    int interval();
    void setInterval(int msec);

    QString state();
    void setState(const QString &data);

    QString getIterations();

private slots:
    void paintGrid(QPainter &painter);
    void paintUniverse(QPainter &painter);
    void newGeneration();

private:
    QTimer *timer;
    int iterations;
    int gridSize;
    bool *currentGrid;
    bool *nextGrid;
    bool isAlive(int i, int j);
    void resetUniverse();
};

#endif // GAME_H
