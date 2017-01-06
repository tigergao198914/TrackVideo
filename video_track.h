#ifndef __VIDEO_TRACK_H__
#define __VIDEO_TRACK_H__

#include <memory>
#include <QImage>
#include <vector>
#include "basic.h"
#include "video_receiver.h"
#include "object_marker.h"
#include "video_track.h"
#include "image_draw.h"

struct TrackRes
{
  int id;
  Position p;
};

struct FrameTrackRes
{
  std::vector< std::shared_ptr<TrackRes> > res ;
  ~FrameTrackRes()
  {
    for( auto iter = res.begin(); iter != res.end();  )
    {
      iter = res.erase(iter);
    }
  }
};

typedef std::vector<std::shared_ptr<FrameTrackRes>> VideoTrackRes;

class VideoTrack : public VideoReceiver
{
 public:
  VideoTrack( ObjectMarker *marker);
  ~VideoTrack();
  virtual int onReceiveFrame(VideoFrame &frame);
  void mark(  int id,  int leftUp_x,  int leftUp_y,  int width,  int height);
  void endMark(  int id);
  void feed(  int id, QImage *img, double scale );
  std::shared_ptr<FrameTrackRes> getFrameTrackRes( int frameNum );
  void save2File(std::string fileName);
 private:
  static bool _bInit;
  int _curObjectCount;
  ObjectMarker *_objMarker;
  VideoTrackRes _videoTrackRes;
  unsigned char *_buf;
  ImageDraw _imgDraw;
  int _width;
  int _height;
};

#endif
