#include "video_render_window.h"

#include <QPainter>
#include <QPaintEvent>
#include <QWidget>
#include <QTimer>
#include <QVBoxLayout>
#include <QBrush>

GLWidget::GLWidget(QWidget *parent)
    : QOpenGLWidget(parent)
    , _image(0)
    , _bEmpty(true)
{
  // setAutoFillBackground(false);
}

void GLWidget::receive(QImage *image)
{
    {
       std::unique_lock<std::mutex> lock(_imageMutex);
       if( !_bEmpty )
       {
         //_emptyCon.wait(lock);
	 //_emptyCon.wait_for(lock, );
       }
       _image =  new QImage( *image) ;

       _bEmpty = false;
    }
    update();
}

void GLWidget::paintEvent(QPaintEvent *event)
{
    if( _image )
    {
      QPainter painter;
      painter.begin(this);
      painter.setRenderHint(QPainter::Antialiasing);
      {
	 std::unique_lock<std::mutex>  lock(_imageMutex);
         _tmpImage = _image->scaled(event->rect().width(),
				   event->rect().height(),
				   Qt::KeepAspectRatio,
				   Qt::SmoothTransformation);
	 delete _image;
	 _image = 0;
	 _bEmpty = true;
	 _emptyCon.notify_one();
      }

      _width = event->rect().size().width();
      _height = event->rect().size().height();
      _leftX = (event->rect().width()-_tmpImage.width()) /2;
      _leftY = (event->rect().height()-_tmpImage.height())/2;
      _rightX = _leftX + _tmpImage.width();
      _rightY = _leftY + _tmpImage.height();
      QRect rect = QRect( QPoint(_leftX, _leftY),
		          QPoint(_rightX, _rightY)  );

      painter.drawImage(rect, _tmpImage );
      painter.end();
    }
 }

QImage GLWidget::getImage()
{
  _fullImage = new QImage(_width, _height, QImage::Format_RGB888);
  _fullImage->fill( QColor(0,0,0) );
  for( int x=_leftX; x<_rightX; x++ )
    for( int y=_leftY; y<_rightY; y++ )
      _fullImage->setPixelColor(x, y, _tmpImage.pixelColor(x-_leftX, y-_leftY));
  return *_fullImage;
}

VideoRenderWindow::VideoRenderWindow()
{
    _openGL = new GLWidget( this);
    QVBoxLayout *layout = new QVBoxLayout;
    setLayout(layout);
    layout->addWidget(_openGL);
    layout->setMargin(0);
}

void VideoRenderWindow::receive( QImage *image )
{
    _openGL->receive(image);
}

QImage VideoRenderWindow::getImage( )
{
  return _openGL->getImage();
}
