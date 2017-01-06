#ifndef _VIDEO_SENDER_H_
#define _VIDEO_SENDER_H_

#include <memory>
#include <vector> 
#include "video_receiver.h"

class VideoSender
{
  public:
    VideoSender();
    ~VideoSender();
    void addReceiver( VideoReceiver* receiver);
    void removeReceiver( VideoReceiver* receiver);
  protected:
    void pubFrame(VideoFrame &frame);
  private:
    std::vector<VideoReceiver*> _receiverList;
};

#endif
