#include "keyframeitem.h"
#include <QDebug>
#include <cmath>
#include <QGraphicsScene>
#include <QGraphicsSceneDragDropEvent>

static const char *s_keyFramePixmap = ":/images/keyframe.png";

KeyFrameItem::KeyFrameItem()
{
  setPixmap(QPixmap(s_keyFramePixmap));
  setFlags(ItemIsMovable);
  setCacheMode(DeviceCoordinateCache);
}

void KeyFrameItem::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
  setPos(snap(event->scenePos()));
}

QPointF KeyFrameItem::snap(const QPointF &pos) const
{
  return QPointF(snap(pos.x()), 0);
}

qreal KeyFrameItem::snap(qreal x) const
{
  // Snap to grid
  x -= fmodf(x, static_cast<qreal>(pixmap().width()));

  // Remain in the scene rect
  if(scene())
    return remainInSceneRect(x);

  return x;
}

qreal KeyFrameItem::remainInSceneRect(qreal x) const
{
  if(!scene())
    return x;

  QRectF sceneRect = scene()->sceneRect();
  return qMax(sceneRect.left(), qMin(sceneRect.right()-pixmap().width(), x));
}
