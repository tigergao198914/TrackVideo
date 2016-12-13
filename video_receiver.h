#ifndef _VIDEO_RECEIVER_H_
#define _VIDEO_RECEIVER_H_

#include "video_frame.h"

class VideoReceiver
{
  public:
    virtual int onReceiveFrame(VideoFrame &frame) = 0;
};

#endif
