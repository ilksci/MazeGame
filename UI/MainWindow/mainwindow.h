#pragma once
#include <QMainWindow>
#include <QGraphicsView>
#include <QTimer>
#include "Core/Map/mapmanager.h"
#include "Entities/Character/player.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

protected:
    void keyPressEvent(QKeyEvent *event) override;

private slots:
    void gameLoop();

private:
    Ui::MainWindow *ui;
    QGraphicsScene* m_scene;    // 图形场景
    QGraphicsView* m_view;       // 图形视图
    Player* m_player;           // 玩家对象
    QTimer m_gameTimer;         // 游戏循环计时器

    void initScene();           // 初始化场景
    void centerView();          // 居中视图
};
