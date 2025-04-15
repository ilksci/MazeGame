#pragma once

#include <QObject>

class Player;

class Item : public QObject
{
    Q_OBJECT
public:
    explicit Item(QObject *parent = nullptr):QObject(parent){}
    virtual void onCollide(Player* play)=0;

signals:
};

