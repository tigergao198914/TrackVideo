#ifndef __VIDEO_FILE_TRANSCODER_H__
#define __VIDEO_FILE_TRANSCODER_H__

#include <libavformat/avformat.h>
#include <libavfilter/avfiltergraph.h>
#include <libavfilter/buffersink.h>
#include <libavfilter/buffersrc.h>
#include <libavutil/opt.h>
#include <libavutil/pixdesc.h>
#include <string>
#include <boost/serialization/singleton.hpp>
#include "basic.h"
#include "video_sender.h"
#include "video_track.h"
#include "image_draw.h"
#include <vector>

class VideoFileTranscoder : public VideoSender
{
 public:
  VideoFileTranscoder();
  ~VideoFileTranscoder();
  void start();
  void config( std::string input, std::string output, std::string label);
  void setVideoTrack( VideoTrack *videoTrack );
  AVFrame* draw(AVFrame* in, int width, int height);
 private:
  std::string _input;
  std::string _output;
  std::string _label;
  VideoTrack *_videoTrack;
  ImageDraw  *_imageDraw;
  int _frameCount;
};

#endif
