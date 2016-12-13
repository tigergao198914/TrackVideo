#ifndef _VIDEO_FRAME_H_
#define _VIDEO_FRAME_H_

enum VideoFormat
{
  VideoFormat_RGB,
  VideoFormat_YUV
};

class VideoFrame
{
  public:
  VideoFrame(){};
    void setParams(VideoFormat format,
		   int width, int height,
		   int lineSize, void *data)
    {
      _format = format;
      _width = width;
      _height = height;
      _lineSize = lineSize;
      _data = data;
    }
    ~VideoFrame(){};
  
   VideoFormat _format;
   int _width;
   int _height;
   void *_data;
   int _timestamp;
   int _lineSize;

};

#endif
