#ifndef __VIDEO_PLAYER_H__
#define __VIDEO_PLAYER_H__

#include <map>
#include <string>
#include <QWidget>
#include <QSlider>
#include <QToolButton>
#include <QImage>
#include <QLabel>
#include <QStackedLayout>
#include "video_receiver.h"
#include "video_render_window.h"
#include "video_file_source.h"
#include "video_slider.h"
#include "video_track.h"

#include "ui/onimageselectedcallback.h"
#include "ui/snapshotwidget.h"
#include "ui/objectinfodetailpanel.h"
#include "ui/objectinfolist.h"
#include "ui/snapshoter.h"

namespace Ui{
  class VideoPlayer;
}

class VideoPlayer : public QWidget, public VideoReceiver, public SnapShoter, public OnImageSelectCallback
{
     Q_OBJECT
 public:
     explicit VideoPlayer( QWidget *parent = 0, VideoFileReader *fileReader=0 );
     ~VideoPlayer();
     virtual void OnImageSelect(QImage &image, QPoint &startPoint, QPoint &endPoint);
     virtual void takeSnapShot( SnapShoterCallback *cb, void *handle=NULL );
     void setObjectInfoList( ObjectInfoList *objectInfoList );
 private:
     void setupUI();
     virtual int onReceiveFrame(VideoFrame &videoFrame);
     void updateSlider( double ratio );
     QStackedLayout    *_renderLayout;
     VideoRenderWindow *_renderWindow;
     SnapshotWidget *_snaper;
     VideoSlider *_slider;
     QLabel      *_curTime;
     QLabel      *_endTime;
     QToolButton *_openFile;
     QToolButton *_pauseStart;
     QToolButton *_accelerate;
     QToolButton *_slowDown;
     QToolButton *_process;
     QImage  *_image;
     std::string _filename;
     VideoFileReader *_videoFileReader;
     int _renderIntervalMs;
     int _durationSec;
     long long _lastTs;
     int _lastFrameNum;
     std::map<long long, int> ts2frameNum;
     bool _bTakingSnapshoot;
     int _videoWidth;
     int _videoHeight;
     
     ObjectInfoList*  _objList;
     // ObjectInfo *_curObjectInfo;
     SnapShoterCallback *_snapShoterCb;
     void *_snapShoterCbHandle;

     int _objectId;
private slots:
     void openFile();
     void playPause();
     void seek(int value);
     void process();
};

#endif
