#ifndef __FFMPEG_TOOLS_H__
#define __FFMPEG_TOOLS_H__
#ifdef __cplusplus
extern "C"
{
#endif
#include <libavcodec/avcodec.h>
#include <libavformat/avformat.h>
#include <libavfilter/avfiltergraph.h>
#include <libavfilter/buffersink.h>
#include <libavfilter/buffersrc.h>
#include <libavutil/opt.h>
#include <libavutil/pixdesc.h>
#ifdef __cplusplus
}
#endif

class FfmpegTool
{
 public:
  int open_input_file( const char* filename, AVFormatContext **ifmt_ctx);
  int  open_output_file( const char* filename, AVFormatContext **ofmt_ctx,
			AVFormatContext *ifmt_ctx);
  int close_input_file( AVFormatContext *ifmt_ctx );
  int close_output_file( AVFormatContext *ofmt_ctx );
  int encode_write_frame( AVFrame *frame, unsigned int stream_index,
			  int *get_frame, AVFormatContext *ofmt_ctx);
  int transcode( AVFormatContext *ifmt_ctx, AVFormatContext *ofmt_ctx );
  int flush_encoder(unsigned int stream_index, AVFormatContext *ofmt_ctx);
  FfmpegTool();
  ~FfmpegTool(){}
};

#endif
