#include "ffmpeg_tools.h"

FfmpegTool:: FfmpegTool(){
  av_register_all();
}

int FfmpegTool::open_input_file(const char *filename, AVFormatContext **ifmt_ctx)
{
  int ret;
  unsigned int i;

  *ifmt_ctx = NULL;
  if ((ret = avformat_open_input(ifmt_ctx, filename, NULL, NULL)) < 0) {
    av_log(NULL, AV_LOG_ERROR, "Cannot open input file\n");
    return ret;
  }

  if ((ret = avformat_find_stream_info(*ifmt_ctx, NULL)) < 0) {
    av_log(NULL, AV_LOG_ERROR, "Cannot find stream information\n");
    return ret;
  }

  for (i = 0; i < (*ifmt_ctx)->nb_streams; i++) {
    AVStream *stream;
    AVCodecContext *codec_ctx;
    stream = (*ifmt_ctx)->streams[i];
    codec_ctx = stream->codec;
    /* Reencode video & audio and remux subtitles etc. */
    if (codec_ctx->codec_type == AVMEDIA_TYPE_VIDEO
	|| codec_ctx->codec_type == AVMEDIA_TYPE_AUDIO) {
      /* Open decoder */
      ret = avcodec_open2(codec_ctx,
			  avcodec_find_decoder(codec_ctx->codec_id), NULL);
      if (ret < 0) {
	av_log(NULL, AV_LOG_ERROR, "Failed to open decoder for stream #%u\n", i);
	return ret;
      }
    }
  }

  av_dump_format(*ifmt_ctx, 0, filename, 0);
  return 0;
}

int FfmpegTool::open_output_file(const char *filename, AVFormatContext **ofmt_ctx, AVFormatContext *ifmt_ctx)
{
  AVStream *out_stream;
  AVStream *in_stream;
  AVCodecContext *dec_ctx, *enc_ctx;
  AVCodec *encoder;
  int ret;
  unsigned int i;

  *ofmt_ctx = NULL;
  avformat_alloc_output_context2(ofmt_ctx, NULL, NULL, filename);
  if (!*ofmt_ctx) {
    av_log(NULL, AV_LOG_ERROR, "Could not create output context\n");
    return AVERROR_UNKNOWN;
  }

  for (i = 0; i < ifmt_ctx->nb_streams; i++) {
    out_stream = avformat_new_stream(*ofmt_ctx, NULL);
    if (!out_stream) {
      av_log(NULL, AV_LOG_ERROR, "Failed allocating output stream\n");
      return AVERROR_UNKNOWN;
    }

    in_stream = ifmt_ctx->streams[i];
    dec_ctx = in_stream->codec;
    enc_ctx = out_stream->codec;
    
    if ((*ofmt_ctx)->oformat->flags & AVFMT_GLOBALHEADER)
      enc_ctx->flags |= AV_CODEC_FLAG_GLOBAL_HEADER;
    
    if (dec_ctx->codec_type == AVMEDIA_TYPE_VIDEO
	|| dec_ctx->codec_type == AVMEDIA_TYPE_AUDIO) {
      /* in this example, we choose transcoding to same codec */
      encoder = avcodec_find_encoder(dec_ctx->codec_id);
      if (!encoder) {
	av_log(NULL, AV_LOG_FATAL, "Necessary encoder not found\n");
	return AVERROR_INVALIDDATA;
      }

      /* In this example, we transcode to same properties (picture size,
       * sample rate etc.). These properties can be changed for output
       * streams easily using filters */
      if (dec_ctx->codec_type == AVMEDIA_TYPE_VIDEO) {
	enc_ctx->height = dec_ctx->height;
	enc_ctx->width = dec_ctx->width;
	enc_ctx->sample_aspect_ratio = dec_ctx->sample_aspect_ratio;
	/* take first format from list of supported formats */
	if (encoder->pix_fmts)
	  enc_ctx->pix_fmt = encoder->pix_fmts[0];
	else
	  enc_ctx->pix_fmt = dec_ctx->pix_fmt;
	/* video time_base can be set to whatever is handy and supported by encoder */
	enc_ctx->time_base = dec_ctx->time_base;
	enc_ctx->me_range = 16; 
	enc_ctx->max_qdiff = 4;
	enc_ctx->qmin = 10; 
	enc_ctx->qmax = 31; 
	enc_ctx->qcompress = 0.6;
      } else {
	enc_ctx->sample_rate = dec_ctx->sample_rate;
	enc_ctx->channel_layout = dec_ctx->channel_layout;
	enc_ctx->channels = av_get_channel_layout_nb_channels(enc_ctx->channel_layout);
	/* take first format from list of supported formats */
	enc_ctx->sample_fmt = encoder->sample_fmts[0];
	enc_ctx->time_base = (AVRational){1, enc_ctx->sample_rate};
      }

      /* Third parameter can be used to pass settings to encoder */
      ret = avcodec_open2(enc_ctx, encoder, NULL);
      if (ret < 0) {
	av_log(NULL, AV_LOG_ERROR, "Cannot open video encoder for stream #%u\n", i);
	return ret;
      }
    } else if (dec_ctx->codec_type == AVMEDIA_TYPE_UNKNOWN) {
      av_log(NULL, AV_LOG_FATAL, "Elementary stream #%d is of unknown type, cannot proceed\n", i);
      return AVERROR_INVALIDDATA;
    } else {
      /* if this stream must be remuxed */
      ret = avcodec_copy_context((*ofmt_ctx)->streams[i]->codec,
				 ifmt_ctx->streams[i]->codec);
      if (ret < 0) {
	av_log(NULL, AV_LOG_ERROR, "Copying stream context failed\n");
	return ret;
      }
    }
  }
  av_dump_format(*ofmt_ctx, 0, filename, 1);

  if (!((*ofmt_ctx)->oformat->flags & AVFMT_NOFILE)) {
    ret = avio_open(&(*ofmt_ctx)->pb, filename, AVIO_FLAG_WRITE);
    if (ret < 0) {
      av_log(NULL, AV_LOG_ERROR, "Could not open output file '%s'", filename);
      return ret;
    }
  }

  /* init muxer, write output file header */
  ret = avformat_write_header(*ofmt_ctx, NULL);
  if (ret < 0) {
    av_log(NULL, AV_LOG_ERROR, "Error occurred when opening output file\n");
    return ret;
  }

  return 0;
}


int FfmpegTool::close_input_file( AVFormatContext *ifmt_ctx )
{
  int i = 0;
  if( !ifmt_ctx )
    {
      return 0;
    }
  for( i = 0; i<ifmt_ctx->nb_streams; i++ )
    {
      avcodec_close(ifmt_ctx->streams[i]->codec);
    }
  avformat_close_input(&ifmt_ctx);
  return 0;
}

int FfmpegTool::close_output_file( AVFormatContext *ofmt_ctx )
{
  int i = 0;
  if( !ofmt_ctx )
    {
      return 0;
    }
  for( i = 0; i<ofmt_ctx->nb_streams; i++ )
    {
      if( ofmt_ctx->streams[i]
	  &&ofmt_ctx->streams[i]->codec)
	{
	  avcodec_close(ofmt_ctx->streams[i]->codec);
	}
    }
  if( !ofmt_ctx->oformat->flags & AVFMT_NOFILE )
    {
      avio_closep(&ofmt_ctx->pb);
    }
  avformat_free_context(ofmt_ctx);
  return 0;
}

int FfmpegTool::flush_encoder(unsigned int stream_index, AVFormatContext *ofmt_ctx)
{
    int ret;
    int got_frame;

    if (!(ofmt_ctx->streams[stream_index]->codec->codec->capabilities &
                AV_CODEC_CAP_DELAY))
        return 0;

    while (1) {
        av_log(NULL, AV_LOG_INFO, "Flushing stream #%u encoder\n", stream_index);
        ret = encode_write_frame(NULL, stream_index, &got_frame, ofmt_ctx);
        if (ret < 0)
            break;
        if (!got_frame)
            return 0;
    }
    return ret;
}

int FfmpegTool::encode_write_frame(AVFrame *frame,
				   unsigned int stream_index,
				   int *got_frame,
				   AVFormatContext *ofmt_ctx) {
  int ret;
  int got_frame_local;
  AVPacket enc_pkt;
  int (*enc_func)(AVCodecContext *, AVPacket *, const AVFrame *, int *) =
    (ofmt_ctx->streams[stream_index]->codec->codec_type ==
     AVMEDIA_TYPE_VIDEO) ? avcodec_encode_video2 : avcodec_encode_audio2;

  if (!got_frame)
    got_frame = &got_frame_local;

  av_log(NULL, AV_LOG_INFO, "Encoding frame\n");
  /* encode filtered frame */
  enc_pkt.data = NULL;
  enc_pkt.size = 0;
  av_init_packet(&enc_pkt);
  ret = enc_func(ofmt_ctx->streams[stream_index]->codec, &enc_pkt,
		 frame, got_frame);
  // av_frame_free(&frame);
  if (ret < 0)
    return ret;
  if (!(*got_frame))
    return 0;

  /* prepare packet for muxing */
  enc_pkt.stream_index = stream_index;
  av_packet_rescale_ts(&enc_pkt,
		       ofmt_ctx->streams[stream_index]->codec->time_base,
		       ofmt_ctx->streams[stream_index]->time_base);

  av_log(NULL, AV_LOG_DEBUG, "Muxing frame\n");
  /* mux encoded frame */
  ret = av_interleaved_write_frame(ofmt_ctx, &enc_pkt);
  return ret;
}



int FfmpegTool::transcode( AVFormatContext *ifmt_ctx, AVFormatContext *ofmt_ctx )
{
  int ret = 0;
  AVPacket packet;
  AVFrame *frame = NULL;
  enum AVMediaType type;
  int stream_index = 0;
  int got_frame = 0;
  int (*dec_func)( AVCodecContext *, AVFrame *, int *, const AVPacket *);
  while(1)
    {
      if( (ret = av_read_frame(ifmt_ctx, &packet)) < 0 )
	{
	  break;
	}

      stream_index = packet.stream_index;
      type = ifmt_ctx->streams[packet.stream_index]->codec->codec_type;
      av_log(NULL, AV_LOG_DEBUG, "Demuxer gave frame of stream_index %u\n",
	     stream_index);

      if (type==AVMEDIA_TYPE_VIDEO)
	{
	  av_log(NULL, AV_LOG_DEBUG, "Going to reencode&filter the frame\n");
	  frame = av_frame_alloc();
	  if (!frame) {
	    ret = AVERROR(ENOMEM);
	    break;
	  }
	  av_packet_rescale_ts(&packet,
			       ifmt_ctx->streams[stream_index]->time_base,
			       ifmt_ctx->streams[stream_index]->codec->time_base);
	  dec_func = (type == AVMEDIA_TYPE_VIDEO) ? avcodec_decode_video2 :
	    avcodec_decode_audio4;
	  ret = dec_func(ifmt_ctx->streams[stream_index]->codec, frame,
			 &got_frame, &packet);
	  if (ret < 0) {
	    av_frame_free(&frame);
	    av_log(NULL, AV_LOG_ERROR, "Decoding failed\n");
	    break;
	  }

	  if (got_frame) {	    
	    frame->pts = av_frame_get_best_effort_timestamp(frame);
	    ret = encode_write_frame(frame, stream_index, NULL, ofmt_ctx);
	    av_frame_free(&frame);
	    if (ret < 0)
	      break;
	  } else {
	    av_frame_free(&frame);
	  }
        }
      #if 0
      else
	{
	  /* remux this frame without reencoding */
	  av_packet_rescale_ts(&packet,
			       ifmt_ctx->streams[stream_index]->time_base,
			       ofmt_ctx->streams[stream_index]->time_base);

	  ret = av_interleaved_write_frame(ofmt_ctx, &packet);
	  if (ret < 0)
	    break;
        }
      #endif
      av_packet_unref(&packet);
    }

  for ( int i = 0; i<ofmt_ctx->nb_streams; i++ )
  {
    /*
      ret = encode_write_frame(NULL, i, NULL, ofmt_ctx);
      if (ret<0)
      {
        return ret;
	}*/
      ret = flush_encoder(i, ofmt_ctx);
      
      if ( ret < 0 )
      {
        return  ret;
      }
  }
  
  av_write_trailer(ofmt_ctx);
  return 0;
}
