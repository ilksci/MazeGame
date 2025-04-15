#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QKeyEvent>
#include <QDebug>
#include <QFile>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , m_scene(new QGraphicsScene(this))
    , m_view(new QGraphicsView(m_scene, this))
{
    ui->setupUi(this);
    setWindowTitle("迷宫冒险");
    setFixedSize(1500, 800);

    // 初始化视图
    m_view->setFixedSize(1500, 800);
    m_view->setRenderHint(QPainter::Antialiasing);
    m_view->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    m_view->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    m_view->setStyleSheet("background: #333;");

    // 加载地图
    if(!MapManager::instance().loadMap(":/maps/Core/Map/maps/level1.map")) {
        qDebug() << "错误详情: " << QFile(":/maps/level1.map").errorString();//我不明白，为什么又加载失败了
        qFatal("Failed to load map");
        QCoreApplication::exit(1);
    }

    // 初始化场景内容
    initScene();

    // 配置玩家
    const QSize mapSize = MapManager::instance().gridSize();
    /*m_player->setPos(mapSize.width() * MapManager::GRID_SIZE / 2,
                     mapSize.height() * MapManager::GRID_SIZE / 2);*/
    if (!MapManager::instance().spawnPoints().isEmpty()) {
        QPoint spawn = MapManager::instance().spawnPoints().first();
        m_player->setPos(
            spawn.x() * MapManager::GRID_SIZE,
            spawn.y() * MapManager::GRID_SIZE
            );
    } else {
        // 默认居中（安全回退）
        m_player->setPos(
            mapSize.width() * MapManager::GRID_SIZE / 2,
            mapSize.height() * MapManager::GRID_SIZE / 2
            );
    }
    m_player->setZValue(100); // 确保玩家在最上层

    // 游戏循环
    m_gameTimer.start(16); // ~60FPS
    connect(&m_gameTimer, &QTimer::timeout, this, &MainWindow::gameLoop);
}

    void MainWindow::initScene()
    {
        // 生成地图可视化
        const QSize mapSize = MapManager::instance().gridSize();
        for(int y = 0; y < mapSize.height(); ++y) {
            for(int x = 0; x < mapSize.width(); ++x) {
                QGraphicsRectItem* tile = new QGraphicsRectItem(
                    x * MapManager::GRID_SIZE,
                    y * MapManager::GRID_SIZE,
                    MapManager::GRID_SIZE,
                    MapManager::GRID_SIZE
                    );
                tile->setBrush(MapManager::instance().isWalkable(x, y)
                                   ? Qt::white : Qt::darkGray);
                m_scene->addItem(tile);
            }
        }

        // 设置场景到MapManager并生成金币
        MapManager::instance().setScene(m_scene); // 使用已有的m_scene
        MapManager::instance().generateCoins(50);

        // 创建玩家
        m_player = new Player();
        m_scene->addItem(m_player);

        // 居中视图
        centerView();
    }

void MainWindow::centerView()
{
    const QRectF sceneRect = m_scene->itemsBoundingRect();
    m_view->fitInView(sceneRect, Qt::KeepAspectRatio);
    m_view->centerOn(sceneRect.center());
}

void MainWindow::gameLoop()
{
    // 处理动画和碰撞检测
    m_view->viewport()->update();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::keyPressEvent(QKeyEvent *event)
{
    switch(event->key()) {
    case Qt::Key_W: m_player->move(0, -1); break;
    case Qt::Key_S: m_player->move(0, 1); break;
    case Qt::Key_A: m_player->move(-1, 0); break;
    case Qt::Key_D: m_player->move(1, 0); break;
    }
    QMainWindow::keyPressEvent(event);
}
