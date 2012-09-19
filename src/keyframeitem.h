#ifndef KEYFRAMEITEM_H
#define KEYFRAMEITEM_H

#include <QGraphicsPixmapItem>

class KeyFrameItem : public QGraphicsPixmapItem
{
public:
  KeyFrameItem();

protected:
  void mouseMoveEvent(QGraphicsSceneMouseEvent *event);

private:
  QPointF snap(const QPointF &pos) const;
  qreal snap(qreal x) const;
  qreal remainInSceneRect(qreal x) const;
};

#endif // KEYFRAMEITEM_H
