#include "video_player.h"
#include "video_file_transcoder.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QStackedLayout>
#include <iostream>
#include <QFileDialog>
#include <thread>
#include <chrono>
#include <thread>
#include <QLabel>

#include <fstream>

VideoPlayer::VideoPlayer( QWidget *parent, VideoFileReader *fileReader)
  :QWidget(parent)
  ,_videoFileReader(NULL)
   // ,_videoTrack(NULL)
  ,_renderIntervalMs(40)
  ,_lastTs(0)
  ,_lastFrameNum(0)
  ,_bTakingSnapshoot(false)
  ,_snapShoterCb(0)
  ,_snapShoterCbHandle(0)
  ,_objectId(0)
{
  _videoFileReader = fileReader;
  _snaper = new SnapshotWidget;
  _snaper->imageCallback = this;
  _videoFileReader->addReceiver( this );
  setupUI();
}

VideoPlayer::~VideoPlayer()
{
  
}

void VideoPlayer::setupUI()
{
   QVBoxLayout *mainLayout = new QVBoxLayout;
  setLayout(mainLayout);
  
  _renderLayout  = new QStackedLayout;
  _renderWindow  = new VideoRenderWindow;
  _renderLayout->addWidget( _renderWindow );
  
  _slider = new VideoSlider();
  _slider->setDisabled(true);

  _openFile   = new QToolButton;
  _openFile->setIcon(QIcon(QString::fromLatin1(":/images/open.png")));
  _pauseStart = new QToolButton;
  _pauseStart->setIcon(QIcon(QString::fromLatin1(":/images/pause.png")));
  _accelerate = new QToolButton;
  _accelerate->setIcon(QIcon(QString::fromLatin1(":/images/seek-forward.png")));
  _slowDown   = new QToolButton;
  _slowDown->setIcon(QIcon(QString::fromLatin1(":/images/seek-backward.png")));
  _process   =  new QToolButton;
  _process->setIcon(QIcon(QString::fromLatin1(":/images/process.png")));
  QSpacerItem *space1 = new QSpacerItem(10,
				        0,
					QSizePolicy::Expanding,
					QSizePolicy::Fixed);
  QSpacerItem *space2 = new QSpacerItem(10,
					0,
					QSizePolicy::Expanding,
					QSizePolicy::Fixed);

  _curTime = new QLabel("0");
  _curTime->resize( 10, 100 );
  //_endTime = new QLabel("1:0:0");
  //_endTime->resize( 10, 10 );
  
  QHBoxLayout *sliderLayout = new QHBoxLayout;
  sliderLayout->addWidget(_curTime);
  sliderLayout->addWidget(_slider);
  //sliderLayout->addWidget(_endTime);
  //sliderLayout->setMargin(0);
  
  QHBoxLayout *buttonLayout = new QHBoxLayout;
  buttonLayout->addSpacerItem(space1);
  buttonLayout->addWidget(_openFile);
  buttonLayout->addWidget(_pauseStart);
  buttonLayout->addWidget(_accelerate);
  buttonLayout->addWidget(_slowDown);
  buttonLayout->addWidget(_process);
  buttonLayout->addSpacerItem(space2);

  //strech factor 0 means expand this widget
  mainLayout->addLayout(_renderLayout,0);
  mainLayout->addLayout(sliderLayout);
  mainLayout->addLayout(buttonLayout);

  connect(_openFile, SIGNAL(clicked()), SLOT(openFile()));
  connect(_pauseStart, SIGNAL(clicked()), SLOT(playPause()));
  connect(_slider, SIGNAL(drag(int)), this,  SLOT(seek(int)));
  connect(_process, SIGNAL(clicked()), SLOT(process()));
}


void VideoPlayer::process()
{

  if( !_filename.empty() )
  {
    VideoFileTranscoder *transcoder = &boost::serialization::singleton<VideoFileTranscoder>::get_mutable_instance();
    
    const char *buff = _filename.c_str();
    int len = strlen((const char*)buff);
    int videoNameLen = 0;
    int suffixLen = -1;
    for( int i=len-1; i>=0; i-- )
    {
        if(buff[i]=='.'&&suffixLen==-1)
	  suffixLen = videoNameLen;
        if(buff[i]=='/')
            break;
        videoNameLen++;
    }
    
    std::string targetFile = _filename.substr(0, _filename.size()-suffixLen-1 );
    targetFile.append("_labeled.");
    targetFile.append(_filename.substr( _filename.size()-suffixLen, suffixLen ));
    std::string targetlabel = _filename.substr(0, _filename.size()-videoNameLen );
    targetlabel.append("label.txt");
    
    transcoder->config(_filename, targetFile, targetlabel);
    transcoder->start();
  }
}

void VideoPlayer::updateSlider( double ratio )
{
  _slider->updatePos( ratio*_slider->maximum() );
}

void VideoPlayer::seek(int value)
{
  _videoFileReader->seek( ((double)value)/_slider->maximum());
}

void VideoPlayer::playPause()
{
  if ( _videoFileReader->pause() )
  {
     _pauseStart->setIcon(QIcon(QString::fromLatin1(":/images/play.png")));
  }
  else
  {
     _pauseStart->setIcon(QIcon(QString::fromLatin1(":/images/pause.png")));
  }
}

void VideoPlayer::openFile()
{
    _filename = QFileDialog::getOpenFileName(this, tr("open file"),
					     " ",
					     tr("Allfile(*.*);;mp3file(*.mp3)")).toStdString();
    if(  _videoFileReader->config( _filename ) != 0 )
    {
      std::cout<< "open file error"<<std::endl;
      return;
    }

    _slider->setDisabled(false); 
    std::thread decoderThread( &VideoFileReader::start, _videoFileReader );
    decoderThread.detach();
}

int VideoPlayer::onReceiveFrame(VideoFrame &frame)
{

  _lastFrameNum = frame._frameNum;
  _lastTs = frame._timestamp;
  _curTime->setText( QString(std::to_string( _lastFrameNum ).c_str()) );
  _videoWidth = frame.img->width();
  _videoHeight = frame.img->height();  
  double ratio = 1.0*frame._timestamp/_videoFileReader->getDuration();
  updateSlider(ratio);
  
  _image = frame.img;  
  _renderWindow->receive(_image);
  //std::this_thread::sleep_for(std::chrono::milliseconds(_renderIntervalMs));
  return 0;
}

void VideoPlayer::setObjectInfoList( ObjectInfoList *objectInfoList )
{
  _objList = objectInfoList;
}


void VideoPlayer::takeSnapShot(SnapShoterCallback *cb, void *handle )
{
  if(! _videoFileReader->isStart() || _bTakingSnapshoot )
  {
    return;
  }

  _bTakingSnapshoot = true;
  _snapShoterCb = cb;
  _snapShoterCbHandle = handle;
  
  if( !_videoFileReader->isPause() )
  {
    playPause();
  }

  QImage bg = _renderWindow->getImage();
  _snaper->setimage(bg);
  _renderLayout->removeWidget(_renderWindow);
  _renderLayout->addWidget(_snaper);
}

void VideoPlayer::OnImageSelect(QImage &image, QPoint &startPoint, QPoint &endPoint)
{
  _bTakingSnapshoot = false;
  _renderLayout->removeWidget(_snaper);
  _renderLayout->addWidget(_renderWindow);

    QPoint s,e;
    double widthScale = 1.0*_videoWidth/ _renderWindow->width();
    double heightScale = 1.0*_videoHeight/ _renderWindow->height();
    double scale = widthScale>heightScale?widthScale:heightScale;

    auto getValidRange = [](int min, int max, int value)
    {
        if(value<min)
           return min;
        if(value>max)
           return max;
      return value;
    };
    
    if( widthScale>heightScale )
    {
        int imageHeight = _videoHeight/scale;
        int marginHeight = ( _renderWindow->height()-imageHeight )/2;
        int ystartPosImage = startPoint.ry()-marginHeight;
        int yendPosImage = endPoint.ry()-marginHeight;
        s.setX( getValidRange(0, _videoWidth,(int)(startPoint.rx()*scale)) );
        s.setY( getValidRange(0, _videoHeight,(int)(ystartPosImage*scale)) );
        e.setX( getValidRange(0, _videoWidth,(int)(endPoint.rx()*scale))  );
        e.setY( getValidRange(0, _videoHeight,(int)(yendPosImage*scale))  );
    }
    else if( widthScale<heightScale )
    {
        int imageWidth = _videoWidth/scale;
        int marginWidth = (_renderWindow->width()-imageWidth)/2;
        int xstartPosImage = startPoint.rx()-marginWidth;
        int xendPosImage = endPoint.rx()-marginWidth;
        s.setX( getValidRange(0, _videoWidth,(int)(xstartPosImage*scale))   );
        s.setY( getValidRange(0, _videoHeight,(int)(startPoint.ry()*scale)) );
        e.setX( getValidRange(0, _videoWidth,(int)(xendPosImage*scale))     );
        e.setY( getValidRange(0, _videoHeight,(int)(endPoint.ry()*scale))   );
    }

    SnapShot *snap = new SnapShot;
    snap->leftUp_x = s.rx();
    snap->leftUp_y = s.ry();
    snap->width = e.rx() - s.rx();
    snap->height = e.ry() - s.ry();
    snap->img = image.scaled(100, 100, Qt::KeepAspectRatio);
    snap->frameNum = _lastFrameNum;
    _snapShoterCb->onReceiveSnapShot( snap, _snapShoterCbHandle );  
}
