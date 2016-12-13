#ifndef __VIDEO_FILTER_H__
#define __VIDEO_FILTER_H__

#include "video_receiver.h"
#include "video_sender.h"

class VideoFilter : public VideoReceiver, public VideoSender
{
 public:
  VideoFilter();
  ~VideoFilter();
 private:
  virtual int onReceiveFrame( VideoFrame &frame);
  void filter();
  void registerFilterInfo();
};


#endif
