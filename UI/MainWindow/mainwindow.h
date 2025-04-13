#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#pragma once

#include <QMainWindow>
#include "Entities/Character/player.h"
#include <QWidget>
#include <QTimer>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    void keyPressEvent(QKeyEvent *event);
    ~MainWindow();

private slots:
    void gameLoop();

private:
    Ui::MainWindow *ui;
    Player *m_player;
    QTimer m_gameTimer;
};
#endif // MAINWINDOW_H
