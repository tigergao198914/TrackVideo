#ifndef __VIDEO_SLIDER_H__
#define __VIDEO_SLIDER_H__

#include <QSlider>
#include <QMouseEvent>
#include <mutex>

class VideoSlider : public QSlider
{
  Q_OBJECT
 public:
   explicit VideoSlider(QWidget *parent = 0);
   ~VideoSlider();
   void updatePos(int value);
 signals:
   void drag(int value);
 private:
   int _minValue;
   int _maxValue;
   int _bPress;
   std::mutex _pressMtx;
 protected:
   void mousePressEvent(QMouseEvent *ev);
   void mouseMoveEvent(QMouseEvent *ev);
   void mouseReleaseEvent(QMouseEvent *ev);
};


#endif
