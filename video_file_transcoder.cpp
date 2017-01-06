#include "video_file_transcoder.h"
#include <iostream>
extern "C" int ffmpeg(int argc,const char **argv);

VideoFileTranscoder::VideoFileTranscoder()
  :_input("")
  ,_output("")
  ,_videoTrack(NULL)
  ,_imageDraw(NULL)
  ,_frameCount(0)
{
  
}

void VideoFileTranscoder::config(std::string input, std::string output, std::string label)
{
  _input = input;
  _output = output;
  _label = label;
}

VideoFileTranscoder::~VideoFileTranscoder()
{

}

void VideoFileTranscoder::start()
{
  if( _input.empty() || _output.empty() )
  {
      return;
  }
  if( _videoTrack != NULL )
  {
    _videoTrack->save2File(_label.c_str());
  }
  _frameCount = 0;
  
  const char *argv[8] = {(char*)"ffmpeg",(char*)"-i",
			 (char*)_input.c_str(),
	                 (char*)"-acodec",
	       	         (char*)"copy",
			 (char*)"-qscale",
			 (char*)"0",
			 (char*)_output.c_str()};
  ffmpeg( 8, argv);
}

void VideoFileTranscoder::setVideoTrack( VideoTrack *videoTrack )
{
  _videoTrack = videoTrack;
}

AVFrame* VideoFileTranscoder::draw(AVFrame* in, int width, int height)
{
  AVFrame *tmp = in;
  _frameCount++;
  //std::cout<< "frameCount:"<< _frameCount <<std::endl; 
  if( _imageDraw == NULL )
    _imageDraw = new ImageDraw( width, height );

  if( _videoTrack == NULL )
    return in;

   std::shared_ptr<FrameTrackRes> res =_videoTrack->getFrameTrackRes( _frameCount );
   if( res == nullptr){
     return in;
   }
   for( auto iter = res->res.begin(); iter != res->res.end(); iter++ )
   {
     tmp =  _imageDraw->drawCircle( tmp, &(*iter)->p );
   }
   return tmp;
}



