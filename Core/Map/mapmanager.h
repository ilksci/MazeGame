#ifndef MAPMANAGER_H
#define MAPMANAGER_H

#include <QObject>
#include <QVector>
#include <QSize>
#include <QGraphicsScene>
#pragma once

class MapManager : public QObject
{
    Q_OBJECT
public:
    static constexpr int GRID_SIZE = 25; // 网格尺寸缩小到25x25像素；50x50太大，我把握不住
    explicit MapManager(QObject *parent = nullptr);
    static MapManager& instance();

    bool loadMap(const QString& path);//从文件加载地图，返回是否成功
    bool isWalkable(int x,int y)const;//判断指定坐标能不能走
    QSize gridSize() const { return QSize(m_width, m_height); }//获得地图尺寸

    enum TileType{Wall,Path,Spawn};

    void generateCoins(int density);
    void setScene(QGraphicsScene* scene);
    QGraphicsScene* m_scene=nullptr; //场景指针
    QVector<QPoint> spawnPoints() const { return m_spawnPoints; }

private:
    QVector<QVector<TileType>> m_grid;//地图数据网格
    QVector<QPoint> m_spawnPoints; //存储所有出生点
    int m_width=0,m_height=0;
signals:
};

#endif // MAPMANAGER_H
