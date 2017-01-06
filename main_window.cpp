#include "main_window.h"
#include <QVBoxLayout>
#include <thread>
#include "video_file_transcoder.h"
#include "video_track.h"
#include "image_draw.h"

extern "C" void *getDrawContext()
{
  return (void*)&boost::serialization::singleton<VideoFileTranscoder>::get_mutable_instance();	    
}

extern "C" AVFrame* draw(void *context, AVFrame *in, int width,int height )
{
  VideoFileTranscoder * videoFileTranscoder = (VideoFileTranscoder*)context;
  return videoFileTranscoder->draw( in, width, height );
}

MainWindow::MainWindow(QWidget *parent)
  :QWidget(parent)
  ,_objectId(0)
{
  _videoFileReader = new VideoFileReader;
  _objectMarker    = new ObjectMarker;
  _videoTrack       = new VideoTrack(_objectMarker);
  _videoFileReader->addReceiver(_videoTrack);
  _transcoder = &boost::serialization::singleton<VideoFileTranscoder>::get_mutable_instance();
  _transcoder->setVideoTrack( _videoTrack );
  
  setupUI();
  _objectMarker->registeSnapShoter(_player);
  _objectMarker->addObjectViewer( _objList );
}

MainWindow::~MainWindow()
{
  //delete _videoFileReader;
  //delete _videoFileRecorder;
  //delete _player;
}

void MainWindow::setupUI()
{
  QHBoxLayout *mainLayout = new QHBoxLayout;
  setLayout(mainLayout);
    
  QVBoxLayout *leftLayout = new QVBoxLayout;
  _player = new VideoPlayer(this,_videoFileReader);
  leftLayout->addWidget(_player);

  QVBoxLayout *rightLayout = new QVBoxLayout;
  _objectInfoDetailPanel = new ObjectInfoDetailPanel(_objectMarker);
  _objList = new ObjectInfoList(_objectMarker);
  _objList->setCallback(_objectInfoDetailPanel);

  _player->setObjectInfoList(_objList);
  
  rightLayout->addWidget(_objList,3);
  rightLayout->addWidget(_objectInfoDetailPanel,1);

  mainLayout->addLayout(leftLayout,2);
  mainLayout->addLayout(rightLayout,1);
  setWindowState(Qt::WindowMaximized);
}

