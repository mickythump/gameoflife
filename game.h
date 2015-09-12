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
    void mousePressEvent(QMouseEvent *e);

signals:

public slots:
    void startGame(const int &number = -1);

    int cellNumber();
    void setCellNumber(const int &s);

    int interval();
    void setInterval(int msec);

    QColor masterColor();

private slots:
    void paintGrid(QPainter &p);
    void paintUniverse(QPainter &p);

private:
    QColor m_masterColor;
    QTimer *timer;
    int generations;
    int universeSize;
    bool *universe;
    bool *next;
    void resetUniverse();
};

#endif // GAME_H
