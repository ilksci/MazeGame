#include "mapmanager.h"
#include "Entities/Items/coinitem.h"
#include <QFile>
#include <QTextStream>
#include <QDebug>
#include <QPoint>
#include <QList>
#include <QRandomGenerator>
#include <algorithm>

MapManager::MapManager(QObject *parent)
    : QObject{parent}
{

}

//单例访问接口
MapManager& MapManager::instance()
{
    static MapManager instance;
    return instance;
}

bool MapManager::loadMap(const QString& path)
{
    qDebug() << "正在加载地图文件:" << path;
    QFile file(path);
    if(!file.open(QIODevice::ReadOnly))
    {
        qWarning() << "Failed to open map file:" << path;
        return false;
    }

    QTextStream ts(&file);

    //解析第一行尺寸
    QString sizeLine=ts.readLine();
    QStringList sizeParts=sizeLine.split('x');
    if(sizeParts.size()!=2)
    {
        qWarning() << "Invalid map size format:" << sizeLine;
        return false;
    }

    m_width=sizeParts[0].toInt();
    m_height=sizeParts[1].toInt();
    m_grid.clear();

    //逐行读取地图数据
    int y=0;
    while (!ts.atEnd() && m_grid.size() < m_height) {
        QString line = ts.readLine().trimmed().left(m_width);
        if (line.length() != m_width) {
            qWarning() << "Map line length mismatch at line" << m_grid.size() + 1;
            return false;
        }

        const int y = m_grid.size(); // 当前行号
        QVector<TileType> row;

        // 使用索引遍历字符以获取x坐标
        for (int x = 0; x < line.length(); ++x) {
            QChar ch = line[x];
            switch (ch.unicode()) {
            case 'W':
                row.append(TileType::Wall);
                break;
            case 'P':
                row.append(TileType::Path);
                break;
            case 'S':
                row.append(TileType::Spawn);
                m_spawnPoints.append(QPoint(x, y)); // 正确记录坐标
                break;
            default:
                row.append(TileType::Wall);
                break;
            }
        }
        m_grid.append(row);
    }
    return true;
}

bool MapManager::isWalkable(int gridX, int gridY) const
{
    // 检查坐标是否越界
    if (gridX < 0 || gridX >= m_width || gridY < 0 || gridY >= m_height)
        return false;
    return m_grid[gridY][gridX];
}

void MapManager::generateCoins(int density)
{
    if(!m_scene)
    {
        qWarning()<<"Cannot generate coins:Scene not set";
        return;
    }
    QVector<QPoint> pathTiles;
    for(int y=0;y<m_height;y++)
    {
        for(int x=0;x<m_width;x++)
        {
            if(m_grid[y][x]==TileType::Path)
            {
                pathTiles.append(QPoint(x,y));
            }
        }
    }
    int count=qMin(pathTiles.size()*density/100,pathTiles.size());
    auto& generator=*QRandomGenerator::global();
    std::shuffle(pathTiles.begin(),pathTiles.end(),generator);

    //我的金币去哪里了！！！
    qDebug() << "== 开始生成金币 ==";
    qDebug() << "可行走网格数:" << pathTiles.size();
    qDebug() << "目标生成数量:" << count;

    for(int i=0;i<count;i++)
    {
        QPoint pos=pathTiles[i];
        CoinItem* coin=new CoinItem();

        qDebug() << "生成金币于网格:" << pos
                 << "实际坐标:" << pos * GRID_SIZE;//金币回家...

        coin->setPos(pos.x()*GRID_SIZE+(GRID_SIZE- coin->boundingRect().width())/2,
                     pos.y()*GRID_SIZE+(GRID_SIZE- coin->boundingRect().height())/2);//用GRID_SIZE计算实际坐标
        m_scene ->addItem(coin);//添加到场景
    }
}

void MapManager::setScene(QGraphicsScene* scene)
{
    m_scene=scene;
}
