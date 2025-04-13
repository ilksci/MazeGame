#ifndef MAPMANAGER_H
#define MAPMANAGER_H

#include <QObject>
#include <QVector>
#include <QSize>
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
private:
    QVector<QVector<bool>> m_grid;//地图数据网格
    int m_width=0,m_height=0;
signals:
};

#endif // MAPMANAGER_H
