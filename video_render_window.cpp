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
         _emptyCon.wait(lock);
       }
       _image = image;
       _bEmpty = false;
    }
    update();
}

void GLWidget::paintEvent(QPaintEvent *event)
{
    if( _image )
    {
      QPainter painter;
      QImage tmpImage;
      painter.begin(this);
      painter.setRenderHint(QPainter::Antialiasing);

      {
	 std::unique_lock<std::mutex>  lock(_imageMutex);
         tmpImage = _image->scaled(event->rect().width(),
				      event->rect().height(),
				       Qt::KeepAspectRatio);
	 _bEmpty = true;
	 _emptyCon.notify_one();
      }
      
      int leftX = (event->rect().width()-tmpImage.width()) /2;
      int leftY = (event->rect().height()-tmpImage.height())/2;
      int rightX = leftX + tmpImage.width();
      int rightY = leftY + tmpImage.height();
      QRect rect = QRect( QPoint(leftX, leftY),
		          QPoint(rightX, rightY)  );
      painter.drawImage(rect, tmpImage );
      drawTextAndEllipse( &painter, "死劲戳我有惊喜哟!", leftX, leftY, 100, 100 );
      painter.end();
    }
 }

void GLWidget::drawTextAndEllipse(QPainter *painter, std::string text, int leftX, int leftY,
				  int width, int height)
{
   QFont textFont("times", 20, QFont::Bold );
   QPen textPen;
   //circlePen = QPen(Qt::black);
   painter->drawEllipse(QRectF(leftX, leftY, width, height));
   textPen = QPen(Qt::white);
   painter->setPen(textPen);
   painter->setFont(textFont);
   painter->drawText( QRect(leftX, leftY+height/2+50, 500, 100),
		      Qt::AlignCenter,
		      text.c_str() );
}

VideoRenderWindow::VideoRenderWindow()
{
    _openGL = new GLWidget( this);
    QVBoxLayout *layout = new QVBoxLayout;
    setLayout(layout);
    layout->addWidget(_openGL);

    //QTimer *timer = new QTimer(this);
    //connect(timer, &QTimer::timeout, openGL, &GLWidget::animate);
    //timer->start(20);
}

void VideoRenderWindow::receive( QImage *image )
{
    _openGL->receive(image);
}
