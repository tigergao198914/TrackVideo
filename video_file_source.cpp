#include "video_file_source.h"
#include <fstream>
#include <thread>
#include <string>
#include <iostream>

VideoFileReader::VideoFileReader()
  :_pFormatCtx(NULL)
   ,_pCodecCtxOrig(NULL)
   ,_pCodecCtx(NULL)
   ,_pCodec(NULL)
   ,_pFrame(NULL)
   ,_pFrameRGB(NULL)
   ,_buffer(0)
{
}

VideoFileReader::~VideoFileReader()
{
  release();
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

  release();
  struct SwsContext *sws_ctx = NULL;
#if 1
  if( avformat_open_input(&_pFormatCtx, _fileName.c_str(), NULL, NULL)!=0)
    return -1;

  if( avformat_find_stream_info(_pFormatCtx, NULL)<0 )
    return -1;

  int videoStream = -1;
  int i;
  for(i=0; i<_pFormatCtx->nb_streams; i++)
  {
    if(_pFormatCtx->streams[i]->codec->codec_type==AVMEDIA_TYPE_VIDEO) 
    {
      videoStream=i;
      break;
    }
  }

  if( videoStream==-1 )
    return -1;

  _pCodecCtxOrig = _pFormatCtx->streams[videoStream]->codec;

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

  // Read frames and save first five frames to disk
  i=0;
  while(av_read_frame(_pFormatCtx, &packet)>=0)
  {
    // Is this a packet from the video stream?
    if(packet.stream_index==videoStream)
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
	  vf.setParams(VideoFormat_RGB, _pFrame->width,
		       _pFrame->height,_pFrameRGB->linesize[0],
		       _pFrameRGB->data[0]);
	  
          //diliver frame to receivers 
          pubFrame( vf  ); 
        }
    }
    
    // Free the packet that was allocated by av_read_frame
    av_free_packet(&packet);
  }
  #if 0
  // Free the RGB image
  av_free(_buffer);
  av_frame_free(&_pFrameRGB);
  
  // Free the YUV frame
  av_frame_free(&_pFrame);
  
  // Close the codecs
  avcodec_close(_pCodecCtx);
  avcodec_close(_pCodecCtxOrig);

  // Close the video file
  avformat_close_input(&_pFormatCtx);
  #endif
#endif
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
}

int VideoFileReader::pause()
{
}

int VideoFileReader::seekbyFrameNumber( int frameNum )
{

}

bool VideoFileReader::fileExist(std::string fileName)
{
  std::ifstream f(fileName.c_str());
  return f.good();
}

