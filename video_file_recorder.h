#ifndef __VIDEO_FILE_RECORDER_H__
#define __VIDEO_FILE_RECORDER_H__

#include <string>
#include "video_frame.h"
#include "video_receiver.h"
#ifdef __cplusplus
extern "C"{
#endif
#include <libavutil/opt.h>
#include <libavutil/mathematics.h>
#include <libavformat/avformat.h>
#include <libswscale/swscale.h>
#include <libswresample/swresample.h>
#include <libavutil/imgutils.h>
#include <libavcodec/avcodec.h>
#ifdef __cplusplus
};
#endif

class VideoFileRecorder : public VideoReceiver
{
 public:
   VideoFileRecorder();
   ~VideoFileRecorder();
   void config( std::string fileName,
		int width, int height,
		int fps, int gob, int bitPerSecond );
   void stop();
   void start();
 private:
   int onReceiveFrame(VideoFrame &frame);
   std::string _fileName;
   int _width;
   int _height;
   int _fps;
   int _gob;
   int _bitPerSecond;
   AVFrame *_avFrame;
   AVCodecContext *_c;
   AVStream *_videoSt;
   AVOutputFormat *_fmt;
   AVFormatContext *_oc;
   AVCodec *_videoCodec;
   AVPicture _srcPicture, _dstPicture;
   int  _swsFlags;
   int _frameCount;
   double _videoTime;
   void closeCodec();
   int setupCodec();
   bool _bSetup;
};

#endif
