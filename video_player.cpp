#include "video_player.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QStackedLayout>
#include <iostream>

VideoPlayer::VideoPlayer( QWidget *parent)
  :QWidget(parent)
{
  setupUI();
}

VideoPlayer::~VideoPlayer()
{
  
}

void VideoPlayer::setupUI()
{
   QVBoxLayout *mainLayout = new QVBoxLayout;
  setLayout(mainLayout);
  _renderWindow  = new VideoRenderWindow;
  _slider = new QSlider(Qt::Horizontal);

  _openFile   = new QToolButton;
  _openFile->setIcon(QIcon(QString::fromLatin1(":/images/open.png")));
  _pauseStart = new QToolButton;
  _pauseStart->setIcon(QIcon(QString::fromLatin1(":/images/pause.png")));
  _accelerate = new QToolButton;
  _accelerate->setIcon(QIcon(QString::fromLatin1(":/images/seek-forward.png")));
  _showDown   = new QToolButton;
  _showDown->setIcon(QIcon(QString::fromLatin1(":/images/seek-backward.png")));
  QSpacerItem *space1 = new QSpacerItem(10,
				        0,
					QSizePolicy::Expanding,
					QSizePolicy::Fixed);
  QSpacerItem *space2 = new QSpacerItem(10,
					0,
					QSizePolicy::Expanding,
					QSizePolicy::Fixed);
   
  QHBoxLayout *buttonLayout = new QHBoxLayout;
  buttonLayout->addSpacerItem(space1);
  buttonLayout->addWidget(_openFile);
  buttonLayout->addWidget(_pauseStart);
  buttonLayout->addWidget(_accelerate);
  buttonLayout->addWidget(_showDown);
  buttonLayout->addSpacerItem(space2);

  //strech factor 0 means expand this widget
  mainLayout->addWidget(_renderWindow,0);
  mainLayout->addWidget(_slider);
  mainLayout->addLayout(buttonLayout);

}

int VideoPlayer::onReceiveFrame(VideoFrame &frame)
{
  // QtAV::VideoFrame a;a.
  //_render->receive(const QtAV::VideoFrame &frame);
  std::cout<< "video player receiver a frame"<<std::endl;
  std::cout<< "width:" << frame._width << "height:"<< frame._height<<std::endl;
  std::cout<< "lineSize:"<< frame._lineSize << std::endl;
  std::cout<< "data:"<< frame._data << std::endl;
  _image = new QImage( (uchar*)frame._data, frame._width,
		       frame._height, frame._lineSize,
		       QImage::Format_RGB888);
  _renderWindow->receive(_image);
  return 0;
}


