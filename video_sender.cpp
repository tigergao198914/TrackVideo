#include "video_sender.h"

VideoSender::VideoSender()
{
}

 VideoSender::~VideoSender()
{
}

void VideoSender::addReceiver( VideoReceiver* receiver)
{
  for( auto iter=_receiverList.begin(); iter!=_receiverList.end(); iter++ )
  {
    if(*iter==receiver)
      return;
  }
  _receiverList.push_back(receiver);
}

void VideoSender::removeReceiver(VideoReceiver* receiver)
{
  
  
  for( auto iter=_receiverList.begin(); iter!=_receiverList.end(); iter++ )
  {
    if( *iter==receiver)
    {
      _receiverList.erase(iter);
      *iter = nullptr;
    }
  }
}

void VideoSender::pubFrame( VideoFrame &frame)
{
  for( auto iter=_receiverList.begin(); iter!=_receiverList.end(); iter++ )
  {
    if( *iter == nullptr )
    {
      continue;
    }
    (*iter)->onReceiveFrame(frame);
  }
}
