#ifndef PLAYER_H
#define PLAYER_H

#include <QWidget>
#include <QPropertyAnimation>
#include "Core/Map/mapmanager.h"

class Player : public QWidget {
    Q_OBJECT
public:
    explicit Player(QWidget *parent = nullptr);
    void move(int dx, int dy);
    void paintEvent(QPaintEvent *event);

private:
    bool checkCollision(const QPoint& newPos) const;
    QPropertyAnimation *anim;
    QPoint m_oldPos;

    static constexpr int DISPLAY_SIZE = 30;   // 显示尺寸
    static constexpr int COLLISION_SIZE = 30; // 碰撞体积
    static constexpr int GRID_SIZE = 50;      // 网格尺寸
    static constexpr int MOVE_STEP = MapManager::GRID_SIZE / 2; // 12像素/步

signals:
    void posChanged();
};

#endif // PLAYER_H
