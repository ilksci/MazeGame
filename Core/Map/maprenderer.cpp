#include "maprenderer.h"
#include "mapmanager.h"
#include <QPainter>

MapRenderer::MapRenderer(QWidget *parent)
    : QWidget{parent},m_cellSize(50)
{
    QSize gridSize = MapManager::instance().gridSize();
    setFixedSize(gridSize.width() * m_cellSize,
        gridSize.height() * m_cellSize);
}

void MapRenderer::paintEvent(QPaintEvent*)
{
    QPainter painter(this);
    painter.setCompositionMode(QPainter::CompositionMode_Source);
    painter.fillRect(rect(), Qt::transparent); // 清除残留

    painter.setCompositionMode(QPainter::CompositionMode_SourceOver);
    auto& map=MapManager::instance();
    //绘制所有网格
    for(int y=0;y<map.gridSize().height();y++)
    {
        for(int x=0;x<map.gridSize().width();x++)
        {
            QRect cell(x*m_cellSize,y*m_cellSize,m_cellSize,m_cellSize);
            QColor color = map.isWalkable(x, y) ? Qt::white : Qt::darkGray;
            painter.fillRect(cell, color);
        }
    }

    // 绘制地图边界框（调试用）
    painter.setPen(Qt::red);
    painter.drawRect(rect()); // 应完全覆盖父窗口的白色区域
}
