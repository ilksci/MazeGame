#include "collidercomponent.h"
#include "Entities/Character/player.h"
#include "Entities/Items/coinitem.h"
#include <QGraphicsScene>

ColliderComponent::ColliderComponent(Player* owner)
    : m_targetItem(dynamic_cast<QGraphicsItem*>(owner)) {}

void ColliderComponent::checkCollisions() {
    if(!m_targetItem) return;

    const QList<QGraphicsItem*> colliding = m_targetItem->collidingItems();
    for(QGraphicsItem* item : colliding) {
        if(CoinItem* coin = dynamic_cast<CoinItem*>(item)) {
            coin->onCollide(static_cast<Player*>(m_targetItem));
            if(coin->scene()) {
                coin->scene()->removeItem(coin);
                delete coin;
            }
        }
    }
}
