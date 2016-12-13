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
    void addReceiver(std::shared_ptr<VideoReceiver> receiver);
    void removeReceiver(std::shared_ptr<VideoReceiver> receiver);
  protected:
    void pubFrame(VideoFrame &frame);
  private:
    std::vector<std::shared_ptr<VideoReceiver>> _receiverList;
};

#endif
