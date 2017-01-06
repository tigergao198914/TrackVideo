#ifndef _VIDEO_FRAME_H_
#define _VIDEO_FRAME_H_
#include <QImage>

enum VideoFormat
{
  VideoFormat_RGB,
  VideoFormat_YUV
};

class VideoFrame
{
  public:
    VideoFrame(){};
    void setParams(QImage *image, unsigned char *data,
		   long long timestamp,int frameNum, double scale)
    {
      _data = data;
      _timestamp = timestamp;
      _frameNum = frameNum;
      _scale = scale;
      img = image;
    }
    ~VideoFrame(){};
  
    QImage *img;
    long long  _timestamp;
    int _frameNum;
    unsigned char *_data;
    double _scale;
};

#endif
