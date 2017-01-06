//
// MATLAB Compiler: 6.1 (R2015b)
// Date: Fri Jan  6 12:32:29 2017
// Arguments: "-B" "macro_default" "-W" "cpplib:libTrack" "-T" "link:lib"
// "markObject.p" "endMarkObject.p" "feedFrame.p" "FH_show2.m" "ccc.m"
// "tracker/fhog.m" "tracker/gaussian_correlation.m"
// "tracker/gaussian_shaped_labels.m" "tracker/get_features.m"
// "tracker/get_subwindow.m" "tracker/show_video.m" "tracker/videofig.m"
// "/usr/local/MATLAB/R2015b/toolbox/signal/signal/hann.m" "-C" 
//

#include <stdio.h>
#define EXPORTING_libTrack 1
#include "libTrack.h"

static HMCRINSTANCE _mcr_inst = NULL;


#ifdef __cplusplus
extern "C" {
#endif

static int mclDefaultPrintHandler(const char *s)
{
  return mclWrite(1 /* stdout */, s, sizeof(char)*strlen(s));
}

#ifdef __cplusplus
} /* End extern "C" block */
#endif

#ifdef __cplusplus
extern "C" {
#endif

static int mclDefaultErrorHandler(const char *s)
{
  int written = 0;
  size_t len = 0;
  len = strlen(s);
  written = mclWrite(2 /* stderr */, s, sizeof(char)*len);
  if (len > 0 && s[ len-1 ] != '\n')
    written += mclWrite(2 /* stderr */, "\n", sizeof(char));
  return written;
}

#ifdef __cplusplus
} /* End extern "C" block */
#endif

/* This symbol is defined in shared libraries. Define it here
 * (to nothing) in case this isn't a shared library. 
 */
#ifndef LIB_libTrack_C_API
#define LIB_libTrack_C_API /* No special import/export declaration */
#endif

LIB_libTrack_C_API 
bool MW_CALL_CONV libTrackInitializeWithHandlers(
    mclOutputHandlerFcn error_handler,
    mclOutputHandlerFcn print_handler)
{
    int bResult = 0;
  if (_mcr_inst != NULL)
    return true;
  if (!mclmcrInitialize())
    return false;
    bResult = mclInitializeComponentInstanceNonEmbeddedStandalone(  &_mcr_inst,
                                                                    NULL,
                                                                    "libTrack",
                                                                    LibTarget,
                                                                    error_handler, 
                                                                    print_handler);
    if (!bResult)
    return false;
  return true;
}

LIB_libTrack_C_API 
bool MW_CALL_CONV libTrackInitialize(void)
{
  return libTrackInitializeWithHandlers(mclDefaultErrorHandler, mclDefaultPrintHandler);
}

LIB_libTrack_C_API 
void MW_CALL_CONV libTrackTerminate(void)
{
  if (_mcr_inst != NULL)
    mclTerminateInstance(&_mcr_inst);
}

LIB_libTrack_C_API 
void MW_CALL_CONV libTrackPrintStackTrace(void) 
{
  char** stackTrace;
  int stackDepth = mclGetStackTrace(&stackTrace);
  int i;
  for(i=0; i<stackDepth; i++)
  {
    mclWrite(2 /* stderr */, stackTrace[i], sizeof(char)*strlen(stackTrace[i]));
    mclWrite(2 /* stderr */, "\n", sizeof(char)*strlen("\n"));
  }
  mclFreeStackTrace(&stackTrace, stackDepth);
}


LIB_libTrack_C_API 
bool MW_CALL_CONV mlxMarkObject(int nlhs, mxArray *plhs[], int nrhs, mxArray *prhs[])
{
  return mclFeval(_mcr_inst, "markObject", nlhs, plhs, nrhs, prhs);
}

LIB_libTrack_C_API 
bool MW_CALL_CONV mlxEndMarkObject(int nlhs, mxArray *plhs[], int nrhs, mxArray *prhs[])
{
  return mclFeval(_mcr_inst, "endMarkObject", nlhs, plhs, nrhs, prhs);
}

LIB_libTrack_C_API 
bool MW_CALL_CONV mlxFeedFrame(int nlhs, mxArray *plhs[], int nrhs, mxArray *prhs[])
{
  return mclFeval(_mcr_inst, "feedFrame", nlhs, plhs, nrhs, prhs);
}

LIB_libTrack_C_API 
bool MW_CALL_CONV mlxFH_show2(int nlhs, mxArray *plhs[], int nrhs, mxArray *prhs[])
{
  return mclFeval(_mcr_inst, "FH_show2", nlhs, plhs, nrhs, prhs);
}

LIB_libTrack_C_API 
bool MW_CALL_CONV mlxCcc(int nlhs, mxArray *plhs[], int nrhs, mxArray *prhs[])
{
  return mclFeval(_mcr_inst, "ccc", nlhs, plhs, nrhs, prhs);
}

LIB_libTrack_C_API 
bool MW_CALL_CONV mlxFhog(int nlhs, mxArray *plhs[], int nrhs, mxArray *prhs[])
{
  return mclFeval(_mcr_inst, "fhog", nlhs, plhs, nrhs, prhs);
}

LIB_libTrack_C_API 
bool MW_CALL_CONV mlxGaussian_correlation(int nlhs, mxArray *plhs[], int nrhs, mxArray 
                                          *prhs[])
{
  return mclFeval(_mcr_inst, "gaussian_correlation", nlhs, plhs, nrhs, prhs);
}

LIB_libTrack_C_API 
bool MW_CALL_CONV mlxGaussian_shaped_labels(int nlhs, mxArray *plhs[], int nrhs, mxArray 
                                            *prhs[])
{
  return mclFeval(_mcr_inst, "gaussian_shaped_labels", nlhs, plhs, nrhs, prhs);
}

LIB_libTrack_C_API 
bool MW_CALL_CONV mlxGet_features(int nlhs, mxArray *plhs[], int nrhs, mxArray *prhs[])
{
  return mclFeval(_mcr_inst, "get_features", nlhs, plhs, nrhs, prhs);
}

LIB_libTrack_C_API 
bool MW_CALL_CONV mlxGet_subwindow(int nlhs, mxArray *plhs[], int nrhs, mxArray *prhs[])
{
  return mclFeval(_mcr_inst, "get_subwindow", nlhs, plhs, nrhs, prhs);
}

LIB_libTrack_C_API 
bool MW_CALL_CONV mlxShow_video(int nlhs, mxArray *plhs[], int nrhs, mxArray *prhs[])
{
  return mclFeval(_mcr_inst, "show_video", nlhs, plhs, nrhs, prhs);
}

LIB_libTrack_C_API 
bool MW_CALL_CONV mlxVideofig(int nlhs, mxArray *plhs[], int nrhs, mxArray *prhs[])
{
  return mclFeval(_mcr_inst, "videofig", nlhs, plhs, nrhs, prhs);
}

LIB_libTrack_C_API 
bool MW_CALL_CONV mlxHann(int nlhs, mxArray *plhs[], int nrhs, mxArray *prhs[])
{
  return mclFeval(_mcr_inst, "hann", nlhs, plhs, nrhs, prhs);
}

LIB_libTrack_CPP_API 
void MW_CALL_CONV markObject(const mwArray& i0, const mwArray& i1, const mwArray& i2, 
                             const mwArray& i3, const mwArray& i4)
{
  mclcppMlfFeval(_mcr_inst, "markObject", 0, 0, 5, &i0, &i1, &i2, &i3, &i4);
}

LIB_libTrack_CPP_API 
void MW_CALL_CONV endMarkObject(const mwArray& i0)
{
  mclcppMlfFeval(_mcr_inst, "endMarkObject", 0, 0, 1, &i0);
}

LIB_libTrack_CPP_API 
void MW_CALL_CONV feedFrame(int nargout, mwArray& o0, const mwArray& i0, const mwArray& 
                            i1)
{
  mclcppMlfFeval(_mcr_inst, "feedFrame", nargout, 1, 2, &o0, &i0, &i1);
}

LIB_libTrack_CPP_API 
void MW_CALL_CONV FH_show2(const mwArray& im, const mwArray& positions)
{
  mclcppMlfFeval(_mcr_inst, "FH_show2", 0, 0, 2, &im, &positions);
}

LIB_libTrack_CPP_API 
void MW_CALL_CONV ccc()
{
  mclcppMlfFeval(_mcr_inst, "ccc", 0, 0, 0);
}

LIB_libTrack_CPP_API 
void MW_CALL_CONV fhog(int nargout, mwArray& H, const mwArray& I, const mwArray& binSize, 
                       const mwArray& nOrients, const mwArray& clip, const mwArray& crop)
{
  mclcppMlfFeval(_mcr_inst, "fhog", nargout, 1, 5, &H, &I, &binSize, &nOrients, &clip, &crop);
}

LIB_libTrack_CPP_API 
void MW_CALL_CONV gaussian_correlation(int nargout, mwArray& kf, const mwArray& xf, const 
                                       mwArray& yf, const mwArray& sigma)
{
  mclcppMlfFeval(_mcr_inst, "gaussian_correlation", nargout, 1, 3, &kf, &xf, &yf, &sigma);
}

LIB_libTrack_CPP_API 
void MW_CALL_CONV gaussian_shaped_labels(int nargout, mwArray& labels, const mwArray& 
                                         sigma, const mwArray& sz)
{
  mclcppMlfFeval(_mcr_inst, "gaussian_shaped_labels", nargout, 1, 2, &labels, &sigma, &sz);
}

LIB_libTrack_CPP_API 
void MW_CALL_CONV get_features(int nargout, mwArray& x, const mwArray& im, const mwArray& 
                               features, const mwArray& cell_size, const mwArray& 
                               cos_window)
{
  mclcppMlfFeval(_mcr_inst, "get_features", nargout, 1, 4, &x, &im, &features, &cell_size, &cos_window);
}

LIB_libTrack_CPP_API 
void MW_CALL_CONV get_subwindow(int nargout, mwArray& out, const mwArray& im, const 
                                mwArray& pos, const mwArray& sz)
{
  mclcppMlfFeval(_mcr_inst, "get_subwindow", nargout, 1, 3, &out, &im, &pos, &sz);
}

LIB_libTrack_CPP_API 
void MW_CALL_CONV show_video(int nargout, mwArray& update_visualization_func, const 
                             mwArray& img_files, const mwArray& video_path, const 
                             mwArray& resize_image)
{
  mclcppMlfFeval(_mcr_inst, "show_video", nargout, 1, 3, &update_visualization_func, &img_files, &video_path, &resize_image);
}

LIB_libTrack_CPP_API 
void MW_CALL_CONV videofig(int nargout, mwArray& fig_handle, mwArray& axes_handle, 
                           mwArray& scroll_bar_handles, mwArray& scroll_func, const 
                           mwArray& num_frames, const mwArray& redraw_func, const 
                           mwArray& play_fps, const mwArray& big_scroll, const mwArray& 
                           key_func, const mwArray& varargin)
{
  mclcppMlfFeval(_mcr_inst, "videofig", nargout, 4, -6, &fig_handle, &axes_handle, &scroll_bar_handles, &scroll_func, &num_frames, &redraw_func, &play_fps, &big_scroll, &key_func, &varargin);
}

LIB_libTrack_CPP_API 
void MW_CALL_CONV hann(int nargout, mwArray& w, const mwArray& varargin)
{
  mclcppMlfFeval(_mcr_inst, "hann", nargout, 1, -1, &w, &varargin);
}

LIB_libTrack_CPP_API 
void MW_CALL_CONV hann(int nargout, mwArray& w)
{
  mclcppMlfFeval(_mcr_inst, "hann", nargout, 1, 0, &w);
}

