#ifndef TIMELINEEDITOR_H
#define TIMELINEEDITOR_H

#include <qxtimelineeditor_global.h>
#include <QGraphicsView>
#include <QList>
#include <QMap>

class QXTIMELINEEDITOR_EXPORT QxTimeLineEditor : public QGraphicsView
{
  Q_OBJECT
public:
  explicit QxTimeLineEditor(QWidget *parent = 0);

  void setKeyFrames(const QList<int> &keyFrames);

  void setMaximumFrameCount(int frameCount);
  int maximumFrameCount() const;

  int frameAt(const QPoint &pos) const;
  virtual QSize sizeHint() const;

protected:
  virtual void drawBackground(QPainter *painter, const QRectF &rect);
  virtual void contextMenuEvent(QContextMenuEvent *event);
  virtual void mousePressEvent(QMouseEvent *event);
  virtual void mouseReleaseEvent(QMouseEvent *event);
  virtual bool addKeyFrame(int frame);
  virtual bool removeKeyFrame(int frame);
  virtual bool replaceKeyFrame(int from, int to);

private:
  void createItem(int frame);
  void deleteItem(int frame);
  void replaceItem(int from, int to);

  QMap<int, QGraphicsItem *> m_keyFrames;
  bool m_isDragging;
  uint m_startFrame;
};

#endif // TIMELINEEDITOR_H
