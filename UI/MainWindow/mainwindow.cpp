#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "Core/Map/maprenderer.h"
#include "Core/Map/mapmanager.h"
#include <QKeyEvent>
#include <QPoint>
#include <QWidget>
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setFixedSize(1500,800);  // 主窗口大小
    setStyleSheet("background:#333");//深灰色背景

    if(!MapManager::instance().loadMap(":/maps/Core/Map/maps/level1.map"))
    {
        qFatal("Failed to load map");//地图加载失败时终止
        QCoreApplication::exit(1); // 确保程序终止
    }


    //创建地图渲染器并且居中显示
    MapRenderer* renderer=new MapRenderer(this);
    // 计算地图渲染器居中坐标
    int rendererX = (width() - renderer->width()) / 2;
    int rendererY = (height() - renderer->height()) / 2;
    renderer->move(rendererX, rendererY); // 确保完全居中
    // 调试输出
    qDebug() << "地图渲染器尺寸:" << renderer->size();
    qDebug() << "地图渲染器位置:" << renderer->pos();

    setAttribute(Qt::WA_TranslucentBackground);

    m_player=(new Player(renderer));//创建玩家

    // MainWindow构造函数中的玩家位置计算部分
    const QSize mapGridSize = MapManager::instance().gridSize();
    const int cellSize = 50; // 确保与MapRenderer和Player中的值一致
    const QPoint centerGrid(
        mapGridSize.width() / 2,
        mapGridSize.height() / 2
        );
    // 计算玩家在renderer中的相对位置（直接使用网格坐标转换）
    QPoint playerPos(
        centerGrid.x() * cellSize - m_player->width()/2,
        centerGrid.y() * cellSize - m_player->height()/2
        );

    m_player->QWidget::move(playerPos.x(),playerPos.y()); // 直接使用QPoint
    m_player->updateGeometry();

    // 调试输出
    qDebug() << "地图渲染器位置:" << renderer->pos()
             << "玩家初始位置:" << m_player->pos();

    m_player->raise();//强制置顶玩家

    setAutoFillBackground(false);
    connect(m_player, &Player::posChanged, [this](){update();});

    //游戏循环（60FPS）
    m_gameTimer.start(16);
    connect(&m_gameTimer,&QTimer::timeout,this,&MainWindow::gameLoop);
}

void MainWindow::gameLoop()
{
    update();//触发重绘
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::keyPressEvent(QKeyEvent *event)
{
    switch(event->key())
    {
    case Qt::Key_W:m_player->move(0,-1);break;
    case Qt::Key_S:m_player->move(0,1);break;
    case Qt::Key_A:m_player->move(-1,0);break;
    case Qt::Key_D:m_player->move(1,0);break;
    }
}
