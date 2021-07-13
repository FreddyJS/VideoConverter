
//#define inline __inline

#define SUPPORTED_FORMATS_NO 2
extern char* vc_supported_formats[SUPPORTED_FORMATS_NO];

extern "C" {
    #include <libavcodec/avcodec.h>
    #include <libavformat/avformat.h>
    #include <libavutil/avutil.h>
}

namespace VidConv
{
    void showVideoInfo(const char* input);
}