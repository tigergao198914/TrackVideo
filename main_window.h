#ifndef __MAINWINDOW_H__
#define __MAINWINDOW_H__

#include <memory>
#include <QWidget>
#include "video_player.h"
#include "video_file_source.h"
#include "video_file_recorder.h"
#include "video_track.h"
#include "object_marker.h"
#include "ui/objectinfolist.h"
#include "ui/objectinfodetailpanel.h"
#include "ui/onimageselectedcallback.h"
#include "ui/snapshoter.h"
#include "video_file_transcoder.h"

namespace Ui{
  class MainWindow;
}

class MainWindow:public QWidget
{
    Q_OBJECT
 public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    // void onObjectCapture( ObjectInfoSeg *objectInfoSeg );
 private:
    VideoPlayer *_player;
    VideoFileReader* _videoFileReader;
    VideoTrack* _videoTrack;
    ObjectMarker *_objectMarker;
    VideoFileTranscoder *_transcoder;
    void setupUI();

    int _objectId;
    ObjectInfoDetailPanel *_objectInfoDetailPanel;
    ObjectInfoList *_objList;
};

#endif
