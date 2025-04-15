#pragma once
#include <QGraphicsObject>
#include <QPropertyAnimation>
//#include "Core/Map/mapmanager.h"
#include "Entities/Components/collidercomponent.h"

class Player : public QGraphicsObject {
    Q_OBJECT
public:
    explicit Player(QGraphicsItem *parent = nullptr);
    ~Player();

    //动画控制参数
    static constexpr int ANIM_DURATION=120;
    static constexpr qreal ANIM_STEP=0.016;
    Q_PROPERTY(qreal progress READ progress WRITE steProgress) //不要乱删代码，会前功尽弃；捡回来了动画代码
    qreal progress() const {return m_progress;}
    void setProgress(qreal p);
    //图形项接口
    QRectF boundingRect() const override;
    void paint(QPainter *painter,
               const QStyleOptionGraphicsItem *option,
               QWidget *widget) override;

    //移动控制
    void move(int dx, int dy);
    void addGold(int amount);
    void updateCollision();
    bool checkCollision(const QPoint& newPos) const;

signals:
    void goldChanged(int newAmount);

private:
    QPropertyAnimation* m_anim;    //移动动画
    qreal m_progress=0.0;
    ColliderComponent* m_collider; //碰撞组件
    int m_gold = 0;               //金币数量
    bool m_isMoving=false;
    static constexpr int GRID_SIZE = 25; //网格尺寸

    QPointF m_oldPos;
};
