#ifndef _VIDEO_FILE_SOURCE_H_
#define _VIDEO_FILE_SOURCE_H_

#include <map>
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
    int seek(double position);
    int seekbyFrameNumber( int frameNum );
    long long getDuration();
    virtual int start();
    virtual int stop();
    virtual int pause();
    bool isStart();
    bool isPause();

  private:
    int init();
    bool fileExist(std::string fileName);
    void release();

    std::string _fileName;
    std::mutex  _fileReaderMtx;
    std::condition_variable _fileReaderCv;
    std::map<long long, int> ts2frameNum;
    std::mutex _seekMtx;
    std::mutex _startMtx;
    std::condition_variable _startCv;

    long long _lastTs;
    long long _lastFrameNum;
    bool _bPause;
    
    AVFormatContext   *_pFormatCtx;
    AVCodecContext    *_pCodecCtxOrig;
    AVCodecContext    *_pCodecCtx;
    AVCodec           *_pCodec;
    AVFrame           *_pFrame;
    AVFrame           *_pFrameRGB;
    uint8_t           *_buffer;
    unsigned int      _duration;
    int               _videoStream;
    bool              _bStart;
    bool              _bSeek;
    double            _seekPosition;
    bool              _bSetStop;
};

#endif
