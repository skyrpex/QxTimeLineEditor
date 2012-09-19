#include <QApplication>
#include <QxTimeLineEditor>
#include <QMap>
#include <QContextMenuEvent>
#include <QMouseEvent>
#include <QDebug>

class TimeLineEditor : public QxTimeLineEditor
{
public:
  TimeLineEditor()
  {
    m_timeLine.insert(0, "Initial frame");
    m_timeLine.insert(5, "Keyframe 5");
    m_timeLine.insert(10, "Keyframe 10");
    m_timeLine.insert(88, "Keyframe 88");
    setKeyFrames(m_timeLine.keys());
  }

protected:
  void contextMenuEvent(QContextMenuEvent *event)
  {
    // We want to keep the first frame
    if(frameAt(event->pos()) > 0)
      QxTimeLineEditor::contextMenuEvent(event);
  }

  void mousePressEvent(QMouseEvent *event)
  {
    // We want to keep the first frame
    if(frameAt(event->pos()) > 0)
      QxTimeLineEditor::mousePressEvent(event);
  }

  bool addKeyFrame(int frame)
  {
    m_timeLine.insert(frame, "New keyframe");
    qDebug() << m_timeLine;
    return true;
  }

  bool removeKeyFrame(int frame)
  {
    m_timeLine.remove(frame);
    qDebug() << m_timeLine;
    return true;
  }

  bool replaceKeyFrame(int from, int to)
  {
    m_timeLine.remove(to);
    QString text = m_timeLine.take(from);
    m_timeLine.insert(to, text);
    qDebug() << m_timeLine;
    return true;
  }

private:
  QMap<int, QString> m_timeLine;
};



int main(int argc, char **argv)
{
  QApplication app(argc, argv);
  TimeLineEditor e;

  e.show();
  return app.exec();
}
