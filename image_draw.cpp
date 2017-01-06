#include "image_draw.h"
#include <QPainter>
#include <iostream>


ImageDraw::ImageDraw()
  :_curFrameNum(0)
{

}

ImageDraw::ImageDraw( int width, int height )
  :_curFrameNum(0)
{
  _width = width;
  _height = height;

  _pOutFrame = av_frame_alloc(); 
  _pOutFrame->quality = 0;
  _pOutFrame->width = width;
  _pOutFrame->height = height;
  _pOutFrame->format = AV_PIX_FMT_YUV420P;

  // Determine required buffer size and allocate buffer
  int numBytes=avpicture_get_size(AV_PIX_FMT_YUV420P, width, height);
  _outBuffer=(uint8_t *)av_malloc(numBytes*sizeof(uint8_t));
  
  // Assign appropriate parts of buffer to image planes in pFrameRGB
  // Note that pFrameRGB is an AVFrame, but AVFrame is a superset
  // of AVPicture
  avpicture_fill((AVPicture *)_pOutFrame, _outBuffer, AV_PIX_FMT_YUV420P,
		 width, height);
  
  _pFrameRGB=av_frame_alloc();
  if(_pFrameRGB==NULL)
    return ;
   // Determine required buffer size and allocate buffer
   numBytes=avpicture_get_size(AV_PIX_FMT_RGB24, width, height);
  _rgbBuffer=(uint8_t *)av_malloc(numBytes*sizeof(uint8_t));
  
  // Assign appropriate parts of buffer to image planes in pFrameRGB
  // Note that pFrameRGB is an AVFrame, but AVFrame is a superset
  // of AVPicture
  avpicture_fill((AVPicture *)_pFrameRGB, _rgbBuffer, AV_PIX_FMT_RGB24,
	         width, height);
  
  // initialize SWS context for software scaling
  _sws_ctx = sws_getContext( width,
			    height,
			    AV_PIX_FMT_YUV420P,
			    width,
			    height,
			    AV_PIX_FMT_RGB24,
			    SWS_BILINEAR,
			    NULL,
			    NULL,
			    NULL );

  _out_sws_ctx = sws_getContext( width,
			    height,
			    AV_PIX_FMT_RGB24,
			    width,
			    height,
			    AV_PIX_FMT_YUV420P,
			    SWS_BILINEAR,
			    NULL,
			    NULL,
			    NULL );
}

AVFrame *ImageDraw::drawRectangle(AVFrame *in, Position *position)
{

  if( position == NULL )
    return in;
  
 sws_scale(_sws_ctx, (uint8_t const * const *)in->data,
              in->linesize, 0, _height,
            _pFrameRGB->data, _pFrameRGB->linesize);

  	  
  QImage tmpImage =  QImage( (uchar*)_pFrameRGB->data[0], _width,
		       _height, _pFrameRGB->linesize[0],
		       QImage::Format_RGB888);

  QPainter p;
  p.begin(&tmpImage);
  p.drawRect( position->leftUp_x, position->leftUp_y,
	      position->width, position->height);
  p.end();

  // Ensuring memory space for image data 
  int size = avpicture_get_size(AV_PIX_FMT_YUV420P, _width, _height);
  int inLinesize[1] = { 3*_width };
  uint8_t * inData[1] = { tmpImage.bits() }; 
  sws_scale(_out_sws_ctx, (uint8_t const * const *)inData ,
              inLinesize, 0, _height,
              _pOutFrame->data , _pOutFrame->linesize);

  _pOutFrame->pts = in->pts;
  return _pOutFrame;
}

AVFrame* ImageDraw::drawCircle(AVFrame *in, Position *position )
{
   
  if( position == NULL )
    return in;
  
 sws_scale(_sws_ctx, (uint8_t const * const *)in->data,
              in->linesize, 0, _height,
            _pFrameRGB->data, _pFrameRGB->linesize);

  	  
  QImage tmpImage =  QImage( (uchar*)_pFrameRGB->data[0], _width,
		       _height, _pFrameRGB->linesize[0],
		       QImage::Format_RGB888);

  drawCircle(tmpImage, position);

  // Ensuring memory space for image data 
  int size = avpicture_get_size(AV_PIX_FMT_YUV420P, _width, _height);
  int inLinesize[1] = { 3*_width };
  uint8_t * inData[1] = { tmpImage.bits() }; 
  sws_scale(_out_sws_ctx, (uint8_t const * const *)inData ,
              inLinesize, 0, _height,
              _pOutFrame->data , _pOutFrame->linesize);

  _pOutFrame->pts = in->pts;
  return _pOutFrame;
}

void ImageDraw::drawCircle(QImage &image, Position *position)
{
  _curFrameNum++;
  int cycle = 30;
  int i = _curFrameNum%cycle;
  double ratio = 1.0*i/cycle;
  QPainter p;
  p.begin(&image);
  p.setBrush( QColor(255,255,255,55));
  p.setPen( QColor(255,255,255,55) );

  double factor = std::min(image.width(), image.height() )/ 360.0;
  int radius = (20+14*ratio)*factor;
  
  p.drawEllipse(position->leftUp_x+position->width/2-radius/2,
		position->leftUp_y+position->height/2-radius/2,
	        radius, radius);
  
  p.setBrush( QColor(255,255,255,255));
  p.setPen( QColor(255,255,255,255) );
  radius = 20*factor;
  p.drawEllipse(position->leftUp_x+position->width/2-radius/2,
		position->leftUp_y+position->height/2-radius/2,
	        radius, radius);

  radius = 12*factor;
  p.setBrush( QColor(34, 223, 236,255*ratio));
  p.drawEllipse(position->leftUp_x+position->width/2-radius/2,
		position->leftUp_y+position->height/2-radius/2,
	        radius, radius);
  p.end(); 
}

