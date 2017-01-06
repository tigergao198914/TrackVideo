#include "video_slider.h"
#include <iostream>

VideoSlider::VideoSlider(QWidget *parent)
  :QSlider(parent)
  ,_minValue(0)
  ,_maxValue(100000)
  ,_bPress(false)
{
  this->setMaximum(_maxValue);
  this->setMinimum(_minValue);
  this->setValue(0);
  setOrientation(Qt::Horizontal);
}

VideoSlider::~VideoSlider()
{
  
}

void VideoSlider::mouseMoveEvent(QMouseEvent *ev)
{
  // QSlider::mouseMoveEvent(ev);
  // std::cout<<"mouse move event 1"<<std::endl;
  if( this->isEnabled() )
   {
        int dur = this->maximum() - this->minimum();
        int pos = this->minimum() + dur * ((double)ev->x() / this->width());
	//std::cout<<"mouse move event 2"<<std::endl;
	if(pos < this->sliderPosition())
        {
            this->setValue(pos);
	    //std::cout<<"mouse move event"<<std::endl;
	    emit drag(pos); 
        }
   }
}

void VideoSlider::mousePressEvent(QMouseEvent *ev)
{
    {
       std::unique_lock<std::mutex> l(_pressMtx);
       _bPress = true;
    }
    QSlider::mousePressEvent(ev);
    if((ev->button() & Qt::LeftButton) && this->isEnabled())
    {
        int dur = this->maximum() - this->minimum();
        int pos = this->minimum() + dur * ((double)ev->x() / this->width());
	if(pos < this->sliderPosition())
        {
            this->setValue(pos);
	    emit drag(pos);
        }
    }
}

void VideoSlider::updatePos( int value )
{
  std::unique_lock<std::mutex> l(_pressMtx);
  if( _bPress )
    return;
  setValue(value);
}

void VideoSlider::mouseReleaseEvent(QMouseEvent* ev)
{
  //std::cout<<"mouse release event"<<std::endl;
  {
    std::unique_lock<std::mutex> l(_pressMtx);
    _bPress = false;
  }
  QSlider::mouseReleaseEvent(ev); 
}
