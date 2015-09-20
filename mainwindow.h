#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QColor>
#include <game.h>
#include "patterns.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

public slots:
    void saveToFile();
    void loadFromFile();
    void loadPattern(QString name);

private slots:
    void on_patternButton_clicked();

    void on_loadButton_clicked();

private:
    Ui::MainWindow *ui;
    Game *game;
    Patterns *patterns;
};

#endif // MAINWINDOW_H
