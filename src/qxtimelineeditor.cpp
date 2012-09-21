#include <QxTimeLineEditor>
#include <QRectF>
#include <QContextMenuEvent>
#include <QMenu>
#include <QDebug>
#include "keyframeitem.h"

static const int s_defaultFrameCount = 60;
static const QSize s_frameSize(8, 20);
static const char *s_backgroundPixmapPath = ":/images/background.png";

static const char *text_AddKeyFrame = "Add keyframe";
static const char *text_RemoveKeyFrame = "Remove keyframe";

QxTimeLineEditor::QxTimeLineEditor(QWidget *parent) :
  QGraphicsView(new QGraphicsScene, parent),
  m_isDragging(false)
{
  setBackgroundBrush(QPixmap(s_backgroundPixmapPath));

  setMaximumHeight(s_frameSize.height());
  setMinimumHeight(s_frameSize.height());
  setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);

  setAlignment(Qt::AlignLeft | Qt::AlignTop);

  setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
  setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

  // Remove the border
  setStyleSheet( "QGraphicsView { border-style: none; }" );
}

void QxTimeLineEditor::setKeyFrames(const QList<int> &keyFrames)
{
  // Update the maximum frame count
  int frameCount = maximumFrameCount();
  if(!keyFrames.isEmpty())
    frameCount = qMax(frameCount, keyFrames.last());
  setMaximumFrameCount(frameCount);

  // Delete old data
  qDeleteAll(m_keyFrames.values());
  m_keyFrames.clear();

  // Create the keyFrames
  foreach(uint frame, keyFrames)
    createItem(frame);
}

void QxTimeLineEditor::setMaximumFrameCount(int frameCount)
{
  // Set the maximum widget width
  int width = s_frameSize.width()*(frameCount+1);
  setMaximumWidth(width+2);

  // Set scene rect
  QRectF rect(0, 0, width, s_frameSize.height());
  Q_ASSERT(scene());
  scene()->setSceneRect(rect);

}

int QxTimeLineEditor::maximumFrameCount() const
{
  Q_ASSERT(scene());

  return scene()->sceneRect().width()/s_frameSize.width();
}

QSize QxTimeLineEditor::sizeHint() const
{
  QSize extra(1, 0);

  if(scene())
    return scene()->sceneRect().size().toSize()+extra;

  return sceneRect().size().toSize()+extra;
}

int QxTimeLineEditor::frameAt(const QPoint &pos) const
{
  int frame = mapToScene(pos).toPoint().x()/s_frameSize.width();
  return qBound(0, frame, maximumFrameCount());
}

void QxTimeLineEditor::drawBackground(QPainter *painter, const QRectF &rect)
{
  QGraphicsView::drawBackground(painter, rect.intersected(sceneRect()));
}

void QxTimeLineEditor::contextMenuEvent(QContextMenuEvent *event)
{
  // Create menu and actions
  QMenu menu;
  QAction *addAction = menu.addAction(tr(text_AddKeyFrame));
  QAction *removeAction = menu.addAction(tr(text_RemoveKeyFrame));

  //
  int frame = frameAt(event->pos());
  QGraphicsItem *item = m_keyFrames.value(frame, NULL);
  addAction->setVisible(!item);
  removeAction->setVisible(item && frame > 0);

  // Execute menu
  QAction *selected = menu.exec(event->globalPos());
  if(selected == addAction)
  {
    // Add a new keyframe
    if(addKeyFrame(frame))
      createItem(frame);
  }
  else if(selected == removeAction)
  {
    // Remove the keyframe
    if(removeKeyFrame(frame))
      deleteItem(frame);
  }
}

void QxTimeLineEditor::mousePressEvent(QMouseEvent *event)
{
  QGraphicsView::mousePressEvent(event);

  // Set the dragging flag to true and store the frame
  if(event->isAccepted())
  {
    m_isDragging = true;
    m_startFrame = frameAt(event->pos());
  }
}

void QxTimeLineEditor::mouseReleaseEvent(QMouseEvent *event)
{
  QGraphicsView::mouseReleaseEvent(event);

  if(m_isDragging)
  {
    m_isDragging = false;

    // Move/replace the keyframes
    int destFrame = frameAt(event->pos());
    if(m_startFrame != (uint)destFrame
       && replaceKeyFrame(m_startFrame, destFrame))
      replaceItem(m_startFrame, destFrame);
  }
}

bool QxTimeLineEditor::addKeyFrame(int frame)
{
  return true;
}

bool QxTimeLineEditor::removeKeyFrame(int frame)
{
  return true;
}

bool QxTimeLineEditor::replaceKeyFrame(int from, int to)
{
  return true;
}

void QxTimeLineEditor::createItem(int frame)
{
  // Delete any previous item
  deleteItem(frame);

  // Create the new item
  KeyFrameItem *item = new KeyFrameItem;
  item->setPos(frame*s_frameSize.width(), 0);

  // Add to the scene and the container
  scene()->addItem(item);
  m_keyFrames.insert(frame, item);
}

void QxTimeLineEditor::deleteItem(int frame)
{
  // Skip if there is no item to delete
  QMap<int, QGraphicsItem*>::Iterator it = m_keyFrames.find(frame);
  if(it == m_keyFrames.end())
    return;

  // Delete and remove from the container
  delete it.value();
  m_keyFrames.erase(it);
}

void QxTimeLineEditor::replaceItem(int from, int to)
{
  // It has no sense to replace an item with itself
  if(from == to)
    return;

  // Skip if there is no item to move
  QMap<int, QGraphicsItem*>::Iterator it = m_keyFrames.find(from);
  if(it == m_keyFrames.end())
    return;

  // Delete any previous item
  deleteItem(to);

  // Move from origin to destination
  QGraphicsItem *item = it.value();
  m_keyFrames.erase(it);
  m_keyFrames.insert(to, item);
}
