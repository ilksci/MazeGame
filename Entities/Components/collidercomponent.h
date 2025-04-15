#pragma once
#include <QObject>
#include <QGraphicsItem>

class Player; // 前向声明

class ColliderComponent : public QObject {
    Q_OBJECT
public:
    explicit ColliderComponent(Player* owner);
    void checkCollisions();

private:
    QGraphicsItem* m_targetItem;
};
