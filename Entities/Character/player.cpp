#include "player.h"
#include "Core/Map/mapmanager.h"
#include <QPainter>
#include <QPropertyAnimation>
#include <QParallelAnimationGroup>

Player::Player(QGraphicsItem *parent)
    : QGraphicsObject(parent),
    m_anim(new QPropertyAnimation(this, "pos")), //绑定图形项的位置属性
    m_collider(new ColliderComponent(this))
{
    //初始化
    setFlag(QGraphicsItem::ItemIsFocusable); //允许接收键盘输入
    setCacheMode(DeviceCoordinateCache);     //启用渲染缓存

    //移动动画
    m_anim->setDuration(100);
    m_anim->setEasingCurve(QEasingCurve::Linear); //缓动曲线

    //连接动画信号
    connect(m_anim, &QPropertyAnimation::finished, [this]()
    {
        m_collider->checkCollisions(); //动画结束检查碰撞
    });
}
Player::~Player()
{
    delete m_anim;
}
//图形项边界矩形
QRectF Player::boundingRect() const
{
    return QRectF(0, 0, GRID_SIZE, GRID_SIZE);
}

//图形项绘制逻辑
void Player::paint(QPainter *painter,
                   const QStyleOptionGraphicsItem *option,
                   QWidget *widget)
{
    Q_UNUSED(option);
    Q_UNUSED(widget);

    //玩家圆形
    painter->setRenderHint(QPainter::Antialiasing);
    painter->setBrush(QColor(0, 128, 255, 200)); //美丽的半透明蓝色
    painter->setPen(QPen(Qt::darkBlue, 2));
    painter->drawEllipse(boundingRect().adjusted(2, 2, -2, -2));
}



/*void Player::move(int dx, int dy) {
    // 计算目标位置（像素坐标）
    const QPointF newPos = pos().toPoint() + QPointF(dx * GRID_SIZE, dy * GRID_SIZE);

    // 提前进行碰撞检测
    if (!checkCollision(newPos.toPoint())) {
        qDebug() << "[碰撞] 目标位置不可行走:" << newPos;
        return;
    }

    // 记录旧位置用于回退
    m_oldPos = pos();

    // 停止当前动画
    if (m_anim->state() == QPropertyAnimation::Running) {
        m_anim->stop();
    }

    // 配置动画参数
    m_anim->setStartValue(pos());
    m_anim->setEndValue(newPos);
    m_anim->start();

    // 实时碰撞检测
    connect(m_anim, &QPropertyAnimation::valueChanged, this, [this]() {
        if (!checkCollision(pos().toPoint())) {
            qDebug() << "[碰撞] 移动过程中检测到碰撞，回退位置";
            m_anim->stop();
            setPos(m_oldPos);
            disconnect(m_anim, &QPropertyAnimation::valueChanged, this, nullptr);
        }
    });
}*/

// player.cpp
void Player::move(int dx, int dy) {
    if (m_isMoving) return; // 正在移动时直接返回

    // 计算目标位置（像素坐标）
    const QPointF newPos = pos().toPoint() + QPointF(dx * GRID_SIZE, dy * GRID_SIZE);

    // 提前进行碰撞检测
    if (!checkCollision(newPos.toPoint())) {
        qDebug() << "[碰撞] 目标位置不可行走:" << newPos;
        return;
    }

    // 标记为移动中
    m_isMoving = true;

    // 停止当前动画
    if (m_anim->state() == QPropertyAnimation::Running) {
        m_anim->stop();
    }

    // 配置动画参数
    m_anim->setStartValue(pos());
    m_anim->setEndValue(newPos);
    m_anim->start();

    // 动画结束时重置状态
    connect(m_anim, &QPropertyAnimation::finished, [this]() {
        m_isMoving = false;
        m_collider->checkCollisions();
    });

    // 实时碰撞检测
    connect(m_anim, &QPropertyAnimation::valueChanged, this, [this]() {
        if (!checkCollision(pos().toPoint())) {
            qDebug() << "[碰撞] 移动过程中检测到碰撞，回退位置";
            m_anim->stop();
            setPos(m_oldPos);
            m_isMoving = false; // 立即解除移动锁定
        }
    });
}

void Player::updateCollision()
{
    // 基于浮点坐标的精确检测
    const QRectF collider = boundingRect().translated(pos());
    const int left = qFloor(collider.left() / GRID_SIZE);
    const int right = qCeil(collider.right() / GRID_SIZE) - 1;
    const int top = qFloor(collider.top() / GRID_SIZE);
    const int bottom = qCeil(collider.bottom() / GRID_SIZE) - 1;

    for (int x = left; x <= right; ++x) {
        for (int y = top; y <= bottom; ++y) {
            if (!MapManager::instance().isWalkable(x, y)) {
                // 立即停止非法移动
                if (QAbstractAnimation::Running == m_anim->state()) {
                    m_anim->stop();
                    setPos(m_anim->startValue().toPointF());
                }
                return;
            }
        }
    }
}

/*bool Player::checkCollision(const QPoint& newPos) const {
    const QRect playerRect(newPos.x(), newPos.y(), GRID_SIZE, GRID_SIZE);
    const int left = playerRect.x() / GRID_SIZE;
    const int right = (playerRect.x() + playerRect.width() - 1) / GRID_SIZE;
    const int top = playerRect.y() / GRID_SIZE;
    const int bottom = (playerRect.y() + playerRect.height() - 1) / GRID_SIZE;

    for (int x = left; x <= right; ++x) {
        for (int y = top; y <= bottom; ++y) {
            if (x < 0 || y < 0 || x >= MapManager::instance().gridSize().width() || y >= MapManager::instance().gridSize().height()) {
                return false;
            }
            if (!MapManager::instance().isWalkable(x, y)) {
                return false;
            }
        }
    }
    return true;
}*/

// player.cpp
bool Player::checkCollision(const QPoint& newPos) const {
    // 玩家碰撞框（GRID_SIZE x GRID_SIZE）
    const QRect playerRect(
        newPos.x(),
        newPos.y(),
        GRID_SIZE,
        GRID_SIZE
        );

    // 计算覆盖的网格范围
    const int left = playerRect.x() / GRID_SIZE;
    const int right = (playerRect.x() + playerRect.width() - 1) / GRID_SIZE;
    const int top = playerRect.y() / GRID_SIZE;
    const int bottom = (playerRect.y() + playerRect.height() - 1) / GRID_SIZE;

    for (int x = left; x <= right; ++x) {
        for (int y = top; y <= bottom; ++y) {
            // 边界检查
            if (x < 0 || y < 0 ||
                x >= MapManager::instance().gridSize().width() ||
                y >= MapManager::instance().gridSize().height()) {
                return false;
            }
            // 可通行性检查
            if (!MapManager::instance().isWalkable(x, y)) {
                return false;
            }
        }
    }
    return true;
}

//金币收集逻辑
void Player::addGold(int amount)
{
    m_gold += amount;
    emit goldChanged(m_gold); //触发UI更新

    //调试输出
    qDebug() << "金币收集! 当前总数:" << m_gold;
}

void Player::setProgress(qreal p)
{
    m_progress=p;
    updateCollision();
}
