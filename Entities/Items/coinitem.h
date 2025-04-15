#pragma once
#ifndef COINITEM_H
#define COINITEM_H

#include "item.h"
#include <QGraphicsPixmapItem>
#include <QObject>

class Player;

class CoinItem : public Item,public QGraphicsPixmapItem
{
    Q_OBJECT
public:
    explicit CoinItem(QGraphicsItem *parent = nullptr);
    void onCollide(Player* player) override;//碰撞回调
private:
    static const int COIN_VALUE=1;

signals:
    void collected();
};

#endif // COINITEM_H
