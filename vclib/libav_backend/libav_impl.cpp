#include <libav_impl.hpp>

static void libavlog(const char *fmt, ...)
{
    va_list args;
    fprintf( stderr, "[libav] " );
    va_start( args, fmt );
    vfprintf( stderr, fmt, args );
    va_end( args );
    fprintf( stderr, "\n" );
}

void VidConv::showVideoInfo(const char *input)
{
    libavlog("Registering all codecs");
    // Setup LibAv

    AVFormatContext *pFormatContext = avformat_alloc_context();
    if (!pFormatContext) {
        libavlog("ERROR could not allocate memory for Format Context");
        return;
    }

    libavlog("Opening the input file (%s) and loading format (container) header", input);

    if (avformat_open_input(&pFormatContext, input, NULL, NULL) != 0) {
        libavlog("ERROR could not open the file");
        return;
    }

    libavlog("Format %s, duration %lld us, bit_rate %i", pFormatContext->iformat->name, pFormatContext->duration, pFormatContext->bit_rate);
    libavlog("Finding stream info from format");

    if (avformat_find_stream_info(pFormatContext,  NULL) < 0) {
        libavlog("ERROR could not get the stream info");
        return;
    }

    const AVCodec *pCodec = NULL;
    int video_stream_index = -1;

    // loop though all the streams and print its main information
    for (int i = 0; i < pFormatContext->nb_streams; i++)
    {
        AVCodecParameters *pLocalCodecParameters =  NULL;
        pLocalCodecParameters = pFormatContext->streams[i]->codecpar;

        libavlog("AVStream->time_base before open coded %d/%d", pFormatContext->streams[i]->time_base.num, pFormatContext->streams[i]->time_base.den);
        libavlog("AVStream->r_frame_rate before open coded %d/%d", pFormatContext->streams[i]->avg_frame_rate.num, pFormatContext->streams[i]->avg_frame_rate.den);
        libavlog("AVStream->start_time %lld", pFormatContext->streams[i]->start_time);
        libavlog("AVStream->duration %lld", pFormatContext->streams[i]->duration);

        libavlog("Finding the proper decoder (CODEC)");

        const AVCodec *pLocalCodec = NULL;

        // finds the registered decoder for a codec ID
        pLocalCodec = avcodec_find_decoder(pLocalCodecParameters->codec_id);

        if (pLocalCodec==NULL) {
            libavlog("ERROR unsupported codec!");
            // In this example if the codec is not found we just skip it
            continue;
        }

        // when the stream is a video we store its index, codec parameters and codec
        if (pLocalCodecParameters->codec_type == AVMEDIA_TYPE_VIDEO) {
            if (video_stream_index == -1) {
                video_stream_index = i;
                pCodec = pLocalCodec;
            }

            libavlog("Video Codec: resolution %d x %d", pLocalCodecParameters->width, pLocalCodecParameters->height);
        } else if (pLocalCodecParameters->codec_type == AVMEDIA_TYPE_AUDIO) {
            libavlog("Audio Codec: %d channels, sample rate %d", pLocalCodecParameters->channels, pLocalCodecParameters->sample_rate);
        }

        // print its name, id and bitrate
        libavlog("Codec %s ID %d bit_rate %lld", pLocalCodec->name, pLocalCodec->id, pLocalCodecParameters->bit_rate);
    }

    if (video_stream_index == -1) {
        libavlog("File %s does not contain a video stream!", input);
        return;
    }


    libavlog("Cleaning memory");

    avformat_close_input(&pFormatContext);
    avformat_free_context(pFormatContext);
}
