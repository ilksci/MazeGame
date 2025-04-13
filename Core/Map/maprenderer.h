#ifndef MAPRENDERER_H
#define MAPRENDERER_H

#include <QWidget>
#pragma once

class MapRenderer : public QWidget
{
    Q_OBJECT
public:
    explicit MapRenderer(QWidget *parent = nullptr);
    void paintEvent(QPaintEvent* event);

private:
    const int m_cellSize=50;

signals:
};

#endif // MAPRENDERER_H
