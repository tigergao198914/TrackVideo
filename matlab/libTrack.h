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

#ifndef __libTrack_h
#define __libTrack_h 1

#if defined(__cplusplus) && !defined(mclmcrrt_h) && defined(__linux__)
#  pragma implementation "mclmcrrt.h"
#endif
#include "mclmcrrt.h"
#include "mclcppclass.h"
#ifdef __cplusplus
extern "C" {
#endif

#if defined(__SUNPRO_CC)
/* Solaris shared libraries use __global, rather than mapfiles
 * to define the API exported from a shared library. __global is
 * only necessary when building the library -- files including
 * this header file to use the library do not need the __global
 * declaration; hence the EXPORTING_<library> logic.
 */

#ifdef EXPORTING_libTrack
#define PUBLIC_libTrack_C_API __global
#else
#define PUBLIC_libTrack_C_API /* No import statement needed. */
#endif

#define LIB_libTrack_C_API PUBLIC_libTrack_C_API

#elif defined(_HPUX_SOURCE)

#ifdef EXPORTING_libTrack
#define PUBLIC_libTrack_C_API __declspec(dllexport)
#else
#define PUBLIC_libTrack_C_API __declspec(dllimport)
#endif

#define LIB_libTrack_C_API PUBLIC_libTrack_C_API


#else

#define LIB_libTrack_C_API

#endif

/* This symbol is defined in shared libraries. Define it here
 * (to nothing) in case this isn't a shared library. 
 */
#ifndef LIB_libTrack_C_API 
#define LIB_libTrack_C_API /* No special import/export declaration */
#endif

extern LIB_libTrack_C_API 
bool MW_CALL_CONV libTrackInitializeWithHandlers(
       mclOutputHandlerFcn error_handler, 
       mclOutputHandlerFcn print_handler);

extern LIB_libTrack_C_API 
bool MW_CALL_CONV libTrackInitialize(void);

extern LIB_libTrack_C_API 
void MW_CALL_CONV libTrackTerminate(void);



extern LIB_libTrack_C_API 
void MW_CALL_CONV libTrackPrintStackTrace(void);

extern LIB_libTrack_C_API 
bool MW_CALL_CONV mlxMarkObject(int nlhs, mxArray *plhs[], int nrhs, mxArray *prhs[]);

extern LIB_libTrack_C_API 
bool MW_CALL_CONV mlxEndMarkObject(int nlhs, mxArray *plhs[], int nrhs, mxArray *prhs[]);

extern LIB_libTrack_C_API 
bool MW_CALL_CONV mlxFeedFrame(int nlhs, mxArray *plhs[], int nrhs, mxArray *prhs[]);

extern LIB_libTrack_C_API 
bool MW_CALL_CONV mlxFH_show2(int nlhs, mxArray *plhs[], int nrhs, mxArray *prhs[]);

extern LIB_libTrack_C_API 
bool MW_CALL_CONV mlxCcc(int nlhs, mxArray *plhs[], int nrhs, mxArray *prhs[]);

extern LIB_libTrack_C_API 
bool MW_CALL_CONV mlxFhog(int nlhs, mxArray *plhs[], int nrhs, mxArray *prhs[]);

extern LIB_libTrack_C_API 
bool MW_CALL_CONV mlxGaussian_correlation(int nlhs, mxArray *plhs[], int nrhs, mxArray 
                                          *prhs[]);

extern LIB_libTrack_C_API 
bool MW_CALL_CONV mlxGaussian_shaped_labels(int nlhs, mxArray *plhs[], int nrhs, mxArray 
                                            *prhs[]);

extern LIB_libTrack_C_API 
bool MW_CALL_CONV mlxGet_features(int nlhs, mxArray *plhs[], int nrhs, mxArray *prhs[]);

extern LIB_libTrack_C_API 
bool MW_CALL_CONV mlxGet_subwindow(int nlhs, mxArray *plhs[], int nrhs, mxArray *prhs[]);

extern LIB_libTrack_C_API 
bool MW_CALL_CONV mlxShow_video(int nlhs, mxArray *plhs[], int nrhs, mxArray *prhs[]);

extern LIB_libTrack_C_API 
bool MW_CALL_CONV mlxVideofig(int nlhs, mxArray *plhs[], int nrhs, mxArray *prhs[]);

extern LIB_libTrack_C_API 
bool MW_CALL_CONV mlxHann(int nlhs, mxArray *plhs[], int nrhs, mxArray *prhs[]);


#ifdef __cplusplus
}
#endif

#ifdef __cplusplus

/* On Windows, use __declspec to control the exported API */
#if defined(_MSC_VER) || defined(__BORLANDC__)

#ifdef EXPORTING_libTrack
#define PUBLIC_libTrack_CPP_API __declspec(dllexport)
#else
#define PUBLIC_libTrack_CPP_API __declspec(dllimport)
#endif

#define LIB_libTrack_CPP_API PUBLIC_libTrack_CPP_API

#else

#if !defined(LIB_libTrack_CPP_API)
#if defined(LIB_libTrack_C_API)
#define LIB_libTrack_CPP_API LIB_libTrack_C_API
#else
#define LIB_libTrack_CPP_API /* empty! */ 
#endif
#endif

#endif

extern LIB_libTrack_CPP_API void MW_CALL_CONV markObject(const mwArray& i0, const mwArray& i1, const mwArray& i2, const mwArray& i3, const mwArray& i4);

extern LIB_libTrack_CPP_API void MW_CALL_CONV endMarkObject(const mwArray& i0);

extern LIB_libTrack_CPP_API void MW_CALL_CONV feedFrame(int nargout, mwArray& o0, const mwArray& i0, const mwArray& i1);

extern LIB_libTrack_CPP_API void MW_CALL_CONV FH_show2(const mwArray& im, const mwArray& positions);

extern LIB_libTrack_CPP_API void MW_CALL_CONV ccc();

extern LIB_libTrack_CPP_API void MW_CALL_CONV fhog(int nargout, mwArray& H, const mwArray& I, const mwArray& binSize, const mwArray& nOrients, const mwArray& clip, const mwArray& crop);

extern LIB_libTrack_CPP_API void MW_CALL_CONV gaussian_correlation(int nargout, mwArray& kf, const mwArray& xf, const mwArray& yf, const mwArray& sigma);

extern LIB_libTrack_CPP_API void MW_CALL_CONV gaussian_shaped_labels(int nargout, mwArray& labels, const mwArray& sigma, const mwArray& sz);

extern LIB_libTrack_CPP_API void MW_CALL_CONV get_features(int nargout, mwArray& x, const mwArray& im, const mwArray& features, const mwArray& cell_size, const mwArray& cos_window);

extern LIB_libTrack_CPP_API void MW_CALL_CONV get_subwindow(int nargout, mwArray& out, const mwArray& im, const mwArray& pos, const mwArray& sz);

extern LIB_libTrack_CPP_API void MW_CALL_CONV show_video(int nargout, mwArray& update_visualization_func, const mwArray& img_files, const mwArray& video_path, const mwArray& resize_image);

extern LIB_libTrack_CPP_API void MW_CALL_CONV videofig(int nargout, mwArray& fig_handle, mwArray& axes_handle, mwArray& scroll_bar_handles, mwArray& scroll_func, const mwArray& num_frames, const mwArray& redraw_func, const mwArray& play_fps, const mwArray& big_scroll, const mwArray& key_func, const mwArray& varargin);

extern LIB_libTrack_CPP_API void MW_CALL_CONV hann(int nargout, mwArray& w, const mwArray& varargin);

extern LIB_libTrack_CPP_API void MW_CALL_CONV hann(int nargout, mwArray& w);

#endif
#endif
