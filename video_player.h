#ifndef __VIDEO_PLAYER_H__
#define __VIDEO_PLAYER_H__

#include <QWidget>
#include <QSlider>
#include <QToolButton>
#include <QImage>
#include "video_receiver.h"
#include "video_render_window.h"

namespace Ui{
  class VideoPlayer;
}

class VideoPlayer : public QWidget, public VideoReceiver
{
     Q_OBJECT
 public:
     explicit VideoPlayer( QWidget *parent = 0 );
     ~VideoPlayer();
 private:
     void setupUI();
     virtual int onReceiveFrame(VideoFrame &videoFrame);
     VideoRenderWindow *_renderWindow;
     QSlider *_slider;
     QToolButton *_openFile;
     QToolButton *_pauseStart;
     QToolButton *_accelerate;
     QToolButton *_showDown;

     QImage  *_image;
};

#endif
