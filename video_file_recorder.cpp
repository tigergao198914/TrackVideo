#include "video_file_recorder.h"
#include <iostream>

VideoFileRecorder::VideoFileRecorder()
  :_width(0)
  ,_height(0)
  ,_fps(0)
  ,_gob(0)
  ,_bitPerSecond(0)
  ,_frameCount(0)
  ,_avFrame(NULL)
  ,_c(NULL)
  ,_videoSt(NULL)
  ,_fmt(NULL)
  ,_oc(NULL)
  ,_videoCodec(NULL)
  ,_bSetup(false)
{

}

VideoFileRecorder::~VideoFileRecorder()
{
  closeCodec();
}

void VideoFileRecorder::config( std::string fileName,
			   int width, int height,
			   int fps, int gob, int bitPerSecond )
{
  _fileName = fileName;
  _width = width;
  _height = height;
  _fps = fps;
  _gob = gob;
  _bitPerSecond = bitPerSecond;
}

void VideoFileRecorder::start()
{
  //setupCodec();
}

void VideoFileRecorder::stop()
{
  closeCodec();
}

void VideoFileRecorder::closeCodec()
{
  /*
   avcodec_close(st->codec);  
    av_free(picture->data[0]);  
    av_free(picture);  
    if (tmp_picture) {  
        av_free(tmp_picture->data[0]);  
        av_free(tmp_picture);  
    }  
    av_free(video_outbuf); 
  */ 
}

void VideoFileRecorder::setupCodec()
{
         //---------------------
       //The basic flow for opening a video is:
       //---------------------
       //Register FFMPEG -> Allocate Media Context -> Allocate Stream Format Context ->
       //Allocate Codec Context -> Allocate Frames -> Open Media File -> Start Writing Frames!!
 
       //If file is already open then don't allow user to open it again
       //if (_AVIMutex) {return;}
      
       //Some inits
       int ret;
        _swsFlags = SWS_BICUBIC;
       _frameCount=0;
      
       //You must call these subroutines otherwise you get errors!!
       //avcodec_register_all();
       av_register_all();
      
       //allocate the output media context
       avformat_alloc_output_context2(&_oc, NULL, NULL, _fileName.c_str());
       if (!_oc) {
              return;
       }
 
       //Get the format determined by the conatiner
       _fmt = _oc->oformat;
 
       // Add the audio and video streams using the default format codecs
       // and initialize the codecs.
       _videoSt = NULL;
 
       //Setup the codecs
       _fmt->video_codec = AV_CODEC_ID_H264;
       _fmt->audio_codec = AV_CODEC_ID_NONE;
      
 
       // Add an output stream.
       {
              //Init AV Stream
              AVStream *st;
 
              //find the encoder
              _videoCodec = avcodec_find_encoder_by_name("libx264");
              if (_videoCodec==NULL ) {
		std::cout << "cannot find codec libx264" << std::endl;
                return;
              }
 
              //Create new video stream
              st = avformat_new_stream(_oc, _videoCodec     );
              if (!st) {
		std::cout<< " cannot create vidoe stream "<<std::endl;
                return;
              }
              st->id = _oc->nb_streams-1;
 
              //Set codec context
              _c = st->codec;
             
              //Setup fundumental video stream parameters
               _c->codec_id  =  _fmt->video_codec;
               _c->bit_rate = _bitPerSecond ;  //Bits Per Second
               _c->width    = _width ;  //Note Resolution must be a multiple of 2!!
               _c->height   = _height;   //Note Resolution must be a multiple of 2!!
               _c->time_base.den = _fps ;       //Frames per second
               _c->time_base.num = 1;
               _c->gop_size = _gob        ;       // Intra frames per x P frames
               _c->pix_fmt = AV_PIX_FMT_YUV420P;//Do not change this, H264 needs YUV format not RGB
 
 
              //Some formats want stream headers to be separate.
              if ( _oc->oformat->flags & AVFMT_GLOBALHEADER)
                      _c->flags |= CODEC_FLAG_GLOBAL_HEADER;
 
              //Set our video stream pointer
               _videoSt =st;
       }
 
      
       // Now that all the parameters are set, we can open the audio and
       // video codecs and allocate the necessary encode buffers.
       {
              //Allocated Codec Context
              AVCodecContext *c = _videoSt->codec;
 
              //Open the codec
              ret = avcodec_open2(c, _videoCodec, NULL);
              if (ret < 0) {
		std::cout<<"cannot open codec"<<std::endl;
                return;
              }
 
              //allocate and init a re-usable frame
              _avFrame =  new AVFrame[1];
              if (!_avFrame) {
		std::cout<<"cannot new frame"<<std::endl;
		return;
              }
 
              //Allocate the encoded raw picture.
              ret = avpicture_alloc(&_dstPicture, c->pix_fmt, c->width, c->height);
              if (ret < 0) {
		std::cout<< "cannot allocate dst  picture"<<std::endl;
                     return;
              }
 
              //Allocate RGB frame that we can pass to the YUV frame
              ret = avpicture_alloc(&_srcPicture, AV_PIX_FMT_RGB24, c->width, c->height);
              if (ret < 0) {
		std::cout<<" cannot allocate src picture"<<std::endl;
                return;
              }
             
              //Copy data and linesize picture pointers to frame
              *((AVPicture *)_avFrame) = _dstPicture;
       }
 
       //Tell FFMPEG that we are going to write encoded frames to a file
       av_dump_format(_oc, 0, _fileName.c_str(), 1);
 
       //open the output file, if needed
       if (!(_fmt->flags & AVFMT_NOFILE)) {
	 ret = avio_open(&_oc->pb, _fileName.c_str(), AVIO_FLAG_WRITE);
              if (ret < 0) {
                     return;
              }
       }
 
 
       //Write the stream header, if any.
       ret = avformat_write_header(_oc, NULL);
                    
 
       if (ret < 0) {
              return;
       }
      
       //Set frame count to zero
       if ( _avFrame   )
               _avFrame->pts = 0;
      
       //Frame is initalised!
       //m_AVIMutex=true;
 
}

int VideoFileRecorder::onReceiveFrame(VideoFrame &frame)
{
   //If video is not initalised then don't write frame
   // if (!m_AVIMutex) {return;}

   //Data should be in the format RGBRGBRGB...etc and should be Width*Height*3 long

  #if 0
   //Step through height of frame
   for (int y=0;y<_c->height;y++) {
    //Height Loop
    //Step through width of frame
    for (int x=0;x<_c->width;x++) { //Width Loop
    //Save RGB frame to FFMPEG's source frame
      _srcPicture.data[0][y * _srcPicture.linesize[0]+x*3+0]=((uint8_t*)frame._data)[(y*_c->width+x)*3+0];  //Red Channel
      _srcPicture.data[0][y * _srcPicture.linesize[0]+x*3+1]=((uint8_t*)frame._data)[(y*_c->width+x)*3+1];  //Green Channel
      _srcPicture.data[0][y * _srcPicture.linesize[0]+x*3+2]=((uint8_t*)frame._data)[(y*_c->width+x)*3+2];  //Blue Channel
         }
    }
  #endif

   _width = frame._width;
   _height = frame._height;

   if( !_bSetup )
   {
     setupCodec();
     _bSetup = true;
   }
   
   _srcPicture.linesize[0] = frame._lineSize;
   _srcPicture.data[0] = (uint8_t*)frame._data;
  
  
   //Calculate video time    
   _videoTime = _videoSt ? _videoSt->pts.val * av_q2d(_videoSt->time_base) : 0.0;
 
   //Inits
   int ret;
   static struct SwsContext *sws_ctx;
   AVCodecContext *c = _videoSt->codec;
 
   //If we haven't already done so init the context of the frame conversion from RGB to YUV
   if (!sws_ctx) {
          sws_ctx = sws_getContext(c->width, c->height,
				       AV_PIX_FMT_RGB24,
				       c->width, c->height,
				       AV_PIX_FMT_YUV420P,
                                        _swsFlags, NULL, NULL, NULL);
          if (!sws_ctx) {
                 return 0;
          }
   }
 
   //Convert RGB frame (m_src_picture) to and YUV frame (m_dst_picture)
   sws_scale(sws_ctx,
             _srcPicture.data, _srcPicture.linesize,
             0, c->height, _dstPicture.data, _dstPicture.linesize);
      
      
   //Some inits for encoding the frame
   AVPacket pkt = { 0 };
   int got_packet;
   av_init_packet(&pkt);
 
   //Encode the frame
   ret = avcodec_encode_video2(c, &pkt, _avFrame, &got_packet);
   if (ret < 0) {
          return 0;
   }
      
   //If size of encoded frame is zero, it means the image was buffered.
   if (!ret && got_packet && pkt.size) {
          pkt.stream_index = _videoSt->index;
             
         //Write the compressed frame to the media file.
         ret = av_interleaved_write_frame(_oc, &pkt);
 
         //if non-zero then it means that there was something wrong writing the frame to
         //the file
         if (ret != 0) {
                return 0;
          }
  } 
  else
  {
      ret = 0;
  }
      
  //Increment Frame counter
  _frameCount++;
  _avFrame->pts += av_rescale_q(1, _videoSt->codec->time_base, _videoSt->time_base);
  return 0;
}
