#include "coinitem.h"
#include "Entities/Character/player.h"
#include <QGraphicsScene>

CoinItem::CoinItem(QGraphicsItem *parent)
    : Item(),QGraphicsPixmapItem{parent}
{
    QPixmap original(":/maps/Entities/Items/images/coin.png");
    // 使用高质量平滑缩放,之前太糊了
    setPixmap(original.scaled(32, 32, Qt::KeepAspectRatio, Qt::SmoothTransformation));
    setZValue(100);
}

void CoinItem::onCollide(Player* player)
{
    if(player)
    {
    player->addGold(1);
        this->hide();
    deleteLater();//安全删除对象
    }
}
