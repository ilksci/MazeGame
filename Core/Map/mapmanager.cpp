#include "mapmanager.h"
#include <QFile>
#include <QTextStream>
#include <QDebug>

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
    while (!ts.atEnd() && m_grid.size() < m_height)
    {
        QString line = ts.readLine().trimmed().left(m_width);
        if (line.length() != m_width)
        {
            qWarning() << "Map line length mismatch at line" << m_grid.size()+1;
            return false;
        }

        QVector<bool> row;
        for (QChar ch : line)
        {
            row.append(ch == ' ');  // 空格代表可行走区域
        }
        m_grid.append(row);
    }

    if (m_grid.size() != m_height)
    {
        qWarning() << "Map height mismatch, expected:" << m_height
                   << "actual:" << m_grid.size();
        return false;
    }
    qDebug() << "地图加载成功，尺寸:" << m_width << "x" << m_height;

    if (!m_grid.isEmpty()) {
        QString debugLine;
        for (bool val : m_grid.first()) {
            debugLine.append(val ? "◻" : "◼"); // ◻:可行走, ◼:不可行走
        }
        qDebug() << "首行地图数据:" << debugLine;

        debugLine.clear();
        for (bool val : m_grid.last()) {
            debugLine.append(val ? "◻" : "◼");
        }
        qDebug() << "末行地图数据:" << debugLine;
    }

    return true;
}

bool MapManager::isWalkable(int gridX, int gridY) const {
    // 检查坐标是否越界
    if (gridX < 0 || gridX >= m_width || gridY < 0 || gridY >= m_height)
        return false;
    return m_grid[gridY][gridX];
}
