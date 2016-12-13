#ifndef _VIDEO_FILE_SOURCE_H_
#define _VIDEO_FILE_SOURCE_H_

#include <mutex>
#include <condition_variable>

#ifdef __cplusplus
extern "C"{
#endif
#include <libavcodec/avcodec.h>
#include <libavformat/avformat.h>
#include <libswscale/swscale.h>
#ifdef __cplusplus
}
#endif

#include "video_source.h"

class VideoFileReader : public VideoSource 
{
  public:
    VideoFileReader();
    ~VideoFileReader();
    int config( std::string fileName );
    int seek(int position);
    int seekbyFrameNumber( int frameNum );
    virtual int start();
    virtual int stop();
    virtual int pause();

  private:
    int init();
    bool fileExist(std::string fileName);
    void release();

    std::string _fileName;
    std::mutex  _fileReaderMtx;
    std::condition_variable _fileReaderCv;

    AVFormatContext   *_pFormatCtx;
    AVCodecContext    *_pCodecCtxOrig;
    AVCodecContext    *_pCodecCtx;
    AVCodec           *_pCodec;
    AVFrame           *_pFrame;
    AVFrame           *_pFrameRGB;
    uint8_t           *_buffer;
};

#endif
