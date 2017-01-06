#include "video_file_source.h"
#include <fstream>
#include <thread>
#include <string>
#include <iostream>
#include <QPainter>

VideoFileReader::VideoFileReader()
  :_pFormatCtx(NULL)
   ,_pCodecCtxOrig(NULL)
   ,_pCodecCtx(NULL)
   ,_pCodec(NULL)
   ,_pFrame(NULL)
   ,_pFrameRGB(NULL)
   ,_buffer(0)
   ,_bPause(false)
  ,_bSetStop(false)
   ,_videoStream(-1)
  ,_bStart(false)
  ,_bSeek(false)
  ,_seekPosition(0)
  ,_lastFrameNum(0)
{
}

VideoFileReader::~VideoFileReader()
{
  release();
}

bool VideoFileReader::isStart()
{
  return _bStart;
}

bool VideoFileReader::isPause()
{
  return _bPause;
}

long long VideoFileReader::getDuration()
{
  //std::cout<<_duration<<std::endl;
  //std::cout<<av_q2d(_pFormatCtx->streams[_videoStream]->time_base)<<std::endl;
  return _duration;
}

int VideoFileReader::config( std::string fileName )
{
  /*check if the file exist*/
  if( !fileExist(fileName) )
  {
    return -1;
  }
  
  /* initialize decoder and list */
  if( init()!=0 )
  {
    return -1;
  }

  _fileName = fileName;

  stop();
  release();

  if( avformat_open_input(&_pFormatCtx, _fileName.c_str(), NULL, NULL)!=0)
  {
      std::cout<< "avformat_open_input failed"<<std::endl;
      return -1;
  }

  if( avformat_find_stream_info(_pFormatCtx, NULL)<0 )
  {
    std::cout<<"avformat_find_stream_info"<<std::endl;
    return -1;
  }

  for(int i=0; i<_pFormatCtx->nb_streams; i++)
  {
    if(_pFormatCtx->streams[i]->codec->codec_type==AVMEDIA_TYPE_VIDEO) 
    {
      _videoStream=i;
      break;
    }
  }

  if( _videoStream==-1 )
    return -1;

  _duration = _pFormatCtx->streams[_videoStream]->duration;
  _pCodecCtxOrig = _pFormatCtx->streams[_videoStream]->codec;
  return 0;
}

int VideoFileReader::init()
{
  av_register_all();
  return 0;
}

int VideoFileReader::start()
{
  AVPacket          packet;
  int               frameFinished;
  int               numBytes;

  struct SwsContext *sws_ctx = NULL;

  _pCodec=avcodec_find_decoder(_pCodecCtxOrig->codec_id);
  if(_pCodec==NULL) {
    fprintf(stderr, "Unsupported codec!\n");
    return -1; // Codec not found
  }
  // Copy context
  _pCodecCtx = avcodec_alloc_context3(_pCodec);
  if(avcodec_copy_context(_pCodecCtx, _pCodecCtxOrig) != 0) {
    fprintf(stderr, "Couldn't copy codec context");
    return -1; // Error copying codec context
  }

  // Open codec
  if(avcodec_open2(_pCodecCtx, _pCodec, NULL)<0)
    return -1; // Could not open codec
  
  // Allocate video frame
  _pFrame=av_frame_alloc();
  
  // Allocate an AVFrame structure
  _pFrameRGB=av_frame_alloc();
  if(_pFrameRGB==NULL)
    return -1;

  double scale = 1;
  int targetWidth, targetHeight;
  if( (_pCodecCtx->width==1920 && _pCodecCtx->height == 1080) ||
      (_pCodecCtx->height==1080 && _pCodecCtx->width == 1920) )
  {
      scale = 3;
      targetWidth = _pCodecCtx->width/scale;
      targetHeight = _pCodecCtx->height/scale;
      std::cout<< targetWidth<<","<<targetHeight<<std::endl;
  }
  else
  {
     targetWidth = _pCodecCtx->width;
     targetHeight = _pCodecCtx->height;
  }

  #if 0
  // Determine required buffer size and allocate buffer
  numBytes=avpicture_get_size(AV_PIX_FMT_RGB24, _pCodecCtx->width,
			      _pCodecCtx->height);
  _buffer=(uint8_t *)av_malloc(numBytes*sizeof(uint8_t));
  
  // Assign appropriate parts of buffer to image planes in pFrameRGB
  // Note that pFrameRGB is an AVFrame, but AVFrame is a superset
  // of AVPicture
  avpicture_fill((AVPicture *)_pFrameRGB, _buffer, AV_PIX_FMT_RGB24,
		 _pCodecCtx->width, _pCodecCtx->height);
  
  // initialize SWS context for software scaling
  sws_ctx = sws_getContext(_pCodecCtx->width,
			   _pCodecCtx->height,
			   _pCodecCtx->pix_fmt,
			   _pCodecCtx->width,
			   _pCodecCtx->height,
			   AV_PIX_FMT_RGB24,
			   SWS_BILINEAR,
			   NULL,
			   NULL,
			   NULL
			   );
  #else
   // Determine required buffer size and allocate buffer
  numBytes=avpicture_get_size(AV_PIX_FMT_RGB24, targetWidth ,
			      targetHeight);
  _buffer=(uint8_t *)av_malloc(numBytes*sizeof(uint8_t));
  
  // Assign appropriate parts of buffer to image planes in pFrameRGB
  // Note that pFrameRGB is an AVFrame, but AVFrame is a superset
  // of AVPicture
  avpicture_fill((AVPicture *)_pFrameRGB, _buffer, AV_PIX_FMT_RGB24,
		 targetWidth, targetHeight );
  
  // initialize SWS context for software scaling
  sws_ctx = sws_getContext(_pCodecCtx->width,
			   _pCodecCtx->height,
			   _pCodecCtx->pix_fmt,
			   targetWidth,
			   targetHeight,
			   AV_PIX_FMT_RGB24,
			   SWS_BILINEAR,
			   NULL,
			   NULL,
			   NULL
			   );
#endif
  // Read frames and save first five frames to disk
  _startMtx.lock();
  _bStart = true;
  _startMtx.unlock();
  while(av_read_frame(_pFormatCtx, &packet)>=0&&!_bSetStop)
  {
    // Is this a packet from the video stream?
    if(packet.stream_index==_videoStream)
    {
        // Decode video frame
        avcodec_decode_video2(_pCodecCtx, _pFrame, &frameFinished, &packet);

	//_pFrame->display_picture_number
        // Did we get a video frame?
        if(frameFinished)
        {
          // Convert the image from its native format to RGB
          sws_scale(sws_ctx, (uint8_t const * const *)_pFrame->data,
              _pFrame->linesize, 0, _pCodecCtx->height,
            _pFrameRGB->data, _pFrameRGB->linesize);

	  VideoFrame vf;
	  long long timestamp =  _pFrame->pts;

	  if( _lastTs < timestamp )
          {
            _lastFrameNum += 1;
            ts2frameNum[ timestamp ] = _lastFrameNum;
          }
          else
          {
            _lastFrameNum = ts2frameNum[ timestamp ];
          }

	  if( _lastFrameNum <= 0)
	    {
	      _lastFrameNum = 1;
	    }
	 
          _lastTs = timestamp;
	  
	  QImage tmpImage =  QImage( (uchar*)_pFrameRGB->data[0], targetWidth,
		       targetHeight, _pFrameRGB->linesize[0],
		       QImage::Format_RGB888);
	  vf.setParams( &tmpImage, _pFrameRGB->data[0], timestamp, _lastFrameNum, scale );

 
        
          //diliver frame to receivers 
           pubFrame( vf );


	  if( _bPause )
	  {
	    std::unique_lock<std::mutex> l(_fileReaderMtx);
	    if( _bPause )
	    {
	       _fileReaderCv.wait(l);
	    }
          }
	  
	  if( _bSeek )
	  {
	    std::unique_lock<std::mutex>(_seekMtx);
	    if( _bSeek )
	    {
              av_seek_frame( _pFormatCtx, _videoStream,
		   _seekPosition*_duration, AVSEEK_FLAG_FRAME );
	      _bSeek = false;
	      _seekPosition = 0;
	    }
	  }
	  
        }
    }
    
    // Free the packet that was allocated by av_read_frame
    av_free_packet(&packet);
  }

  {
    std::unique_lock<std::mutex> l(_startMtx);
    _bStart = false;
  }
  _startCv.notify_one();
  return 0;
}

void VideoFileReader::release()
{
  if( _buffer )
    {
      av_free(_buffer);
      _buffer = NULL;
    }

  if( _pFrameRGB )
    {
      av_frame_free( &_pFrameRGB);
      _pFrameRGB = 0;
    }

  if( _pFrame )
    {
      av_frame_free( &_pFrame);
      _pFrame = 0;
    }

  if( _pCodecCtx )
    {
      avcodec_close(_pCodecCtx);
      _pCodecCtx = 0;
    }

  if( _pCodecCtxOrig )
    {
      avcodec_close(_pCodecCtxOrig);
      _pCodecCtx = 0;
    }

  if( _pFormatCtx )
    {
      avformat_close_input(&_pFormatCtx);
      _pFormatCtx = 0;
    }
}

int VideoFileReader::stop()
{
   {
      std::unique_lock<std::mutex> l(_startMtx);
      if( !_bStart )
        return 0;
      
      _bSetStop = true;
     if( _bPause )
       pause();
     _startCv.wait(l);
     _bSetStop = false;
  }
  return 0;
}

int VideoFileReader::pause()
{
  std::cout << "pause"<<std::endl;
  if( !_bStart )
    return 0;
  std::unique_lock<std::mutex> l(_fileReaderMtx);
  _bPause = !_bPause;
  if( !_bPause )
    {
      _fileReaderCv.notify_one();
    }
  return _bPause;
}

int VideoFileReader::seek(double position)
{
  if( !_bStart )
    return 0;
  {
    std::unique_lock<std::mutex>(_seekMtx);
    _bSeek = true;
    _seekPosition = position;

    if( _bPause )
    {
      _fileReaderCv.notify_one();
    }
  }
  return 0;
}

bool VideoFileReader::fileExist(std::string fileName)
{
  std::ifstream f(fileName.c_str());
  return f.good();
}

