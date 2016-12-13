#ifndef _VIDEO_SOURCE_H_
#define _VIDEO_SOURCE_H_

#include "video_frame.h"
#include "video_sender.h"

class VideoSource : public VideoSender
{
  public:
  VideoSource(){};
  ~VideoSource(){};

    virtual int start()=0;
    virtual int stop()=0;
    virtual int pause()=0;
};

#endif
