#ifndef __IMAGE_DRAW_H__
#define __IMAGE_DRAW_H__
#include "basic.h"
#include <QImage>
#ifdef __cplusplus
extern "C"{
#endif
#include <libavcodec/avcodec.h>
#include <libavformat/avformat.h>
#include <libswscale/swscale.h>
#ifdef __cplusplus
}
#endif

struct ImageDraw
{
 public:
  ImageDraw();
  ImageDraw(int width, int height);
  ~ImageDraw(){};
  AVFrame* drawRectangle(AVFrame *in, Position *position);
  AVFrame* drawCircle(AVFrame *in, Position *position );

  void drawCircle( QImage &image, Position *position );
 private:
    int _curFrameNum;
    int _width;
    int _height;
    uint8_t*_rgbBuffer;
    uint8_t*_outBuffer;
    struct SwsContext *_sws_ctx;
    struct SwsContext *_out_sws_ctx;
    AVFrame *_pFrameRGB;
    AVFrame *_pOutFrame;
};

#endif
