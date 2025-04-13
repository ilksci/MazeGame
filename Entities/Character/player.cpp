#include "player.h"
#include "Core/Map/mapmanager.h"
#include <QPainter>
#include <QRegion>

Player::Player(QWidget *parent)
    : QWidget(parent), anim(new QPropertyAnimation(this, "pos"))
{
    setFixedSize(GRID_SIZE, GRID_SIZE); // 保持父控件与网格对齐
    anim->setDuration(120);
    anim->setEasingCurve(QEasingCurve::OutExpo);//添加缓动效果
    //连接动画信号
    connect(anim, &QPropertyAnimation::valueChanged, [this]() {
        QRegion updateRegion(QRect(m_oldPos, size()));
        updateRegion += QRect(pos(), size());//持续刷新旧区域
        parentWidget()->update(updateRegion);
        emit posChanged();
    });
}

bool Player::checkCollision(const QPoint& newPos) const {
    QRect collisionRect(newPos.x(), newPos.y(), COLLISION_SIZE, COLLISION_SIZE);
    int left = std::floor((float)collisionRect.left() / GRID_SIZE);
    int right = std::ceil((float)collisionRect.right()/ GRID_SIZE)-1;
    int top = std::floor((float)collisionRect.top() / GRID_SIZE);
    int bottom = std::ceil((float)collisionRect.bottom() / GRID_SIZE)-1;

    qDebug() << "碰撞检测区域: [" << left << "," << right << "]x["
             << top << "," << bottom << "]";//调试输出

    for (int x = left; x <= right; ++x)
    {
        for (int y = top; y <= bottom; ++y)
        {
            if (x < 0 || y < 0 || x >= MapManager::instance().gridSize().width()
                || y >= MapManager::instance().gridSize().height())
            {
                return false;
            }
            if (!MapManager::instance().isWalkable(x, y))
            {
                qDebug() << "阻挡网格: (" << x << "," << y << ")";
                return false;
            }
        }
    }
    return true;
}

void Player::move(int dx, int dy) {
    m_oldPos = pos();
    QPoint newPos = pos() + QPoint(dx * MOVE_STEP, dy * MOVE_STEP);

    if (checkCollision(newPos)) {
        if (anim->state() == QAbstractAnimation::Running) {
            anim->stop();
        }
        anim->setStartValue(pos());
        anim->setEndValue(newPos);
        anim->start();
    }
}

void Player::paintEvent(QPaintEvent *) {
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);

    // 绘制缩小后的玩家（居中显示）
    painter.setBrush(QColor(0, 0, 255, 200));
    painter.setPen(Qt::NoPen);
    QRect displayRect = rect().adjusted(
        (GRID_SIZE - DISPLAY_SIZE)/2,
        (GRID_SIZE - DISPLAY_SIZE)/2,
        -(GRID_SIZE - DISPLAY_SIZE)/2,
        -(GRID_SIZE - DISPLAY_SIZE)/2
        );
    painter.drawEllipse(displayRect);
}
