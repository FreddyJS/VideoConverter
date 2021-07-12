
//#define inline __inline

extern "C" {
    #include <libavcodec/avcodec.h>
    #include <libavformat/avformat.h>
    #include <libavutil/avutil.h>
}

namespace VidConv
{
    void showVideoInfo(const char* input);
}