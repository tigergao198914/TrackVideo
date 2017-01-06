#include "video_file_recorder.h"
#include <iostream>

VideoFileRecorder::VideoFileRecorder()
  :_width(0)
  ,_height(0)
  ,_fps(0)
  ,_gob(0)
  ,_bitPerSecond(0)
  ,_frameCount(0)
  ,_avFrame(NULL)
  ,_c(NULL)
  ,_videoSt(NULL)
  ,_fmt(NULL)
  ,_oc(NULL)
  ,_videoCodec(NULL)
  ,_bSetup(false)
{

}

VideoFileRecorder::~VideoFileRecorder()
{
  closeCodec();
}

void VideoFileRecorder::config( std::string fileName,
				int width, int height,
				int fps, int gob, int bitPerSecond )
{
  _fileName = fileName;
  _width = width;
  _height = height;
  _fps = fps;
  _gob = gob;
  _bitPerSecond = bitPerSecond;
}

void VideoFileRecorder::start()
{
  //setupCodec();
}

void VideoFileRecorder::stop()
{
  closeCodec();
}

void VideoFileRecorder::closeCodec()
{
  
}

int VideoFileRecorder::setupCodec()
{
  #if 0
  AVFormatContext *_pOFormat = NULL;
  AVOutputFormat *_ofmt = NULL;
  if ( avformat_alloc_output_context2(&_pOFormat, NULL, NULL, _fileName.c_str())<0)
  {
    std::cout<< "avformat_alloc_output_context2 failed!"<<std::endl;
    return -1;
  }
  _ofmt = _pOFormat->oformat;
  if ( avio_open( &(_pOFormat->pb), _fileName.c_str(), AVIO_FLAG_READ_WRITE ) < 0 )
   {
     std::cout<< "avio_open failed!"<<std::endl;
     return -1;
    }

  AVCodecContext *_videoEncCtx = NULL;
  AVCodec *_videoEnc = NULL;
  _videoEnc = avcodec_find_encoder(AV_CODEC_ID_H264);
  AVStream *_videoSt = avformat_new_stream(_pOFormat, _videoEnc);
  if( !_videoSt )
    {
      std::cout<< "avformat_new_stream failed!"<<std::endl;
      return -1;
    }
  _videoEncCtx = _videoSt->codec;
  _videoEncCtx->width = _width;
  _videoEncCtx->height = _height;
  _videoEncCtx->pix_fmt = AV_PIX_FMT_YUV420P;
  _videoEncCtx->time_base.num = 1;
  _videoEncCtx->time_base.den = 25;
  _videoEncCtx->bit_rate = _bitPerSecond;
  _videoEncCtx->gop_size = 250;
  _videoEncCtx->max_b_frames = 10;
  _videoEncCtx->qmin = 10;
  _videoEncCtx->qmax = 51;

  if ( avcodec_open2(_videoEncCtx, _videoEnc, NULL) <0 )
    {
      std::cout<< "avcodec_open2 failed!" << std::endl;
      return -1;
    }

  //mp4
  AVFormatContext *_pMp4Format = NULL;
  AVOutputFormat *_pMp4FoFormat = NULL;
  if( avformat_alloc_output_context2(&_pMp4FoFormat, NULL, NULL, _fileName.c_str())<0 )
    {
      return -1;
    }
  #endif
  return 0;
}

int VideoFileRecorder::onReceiveFrame(VideoFrame &frame)
{

  return 0;
}
