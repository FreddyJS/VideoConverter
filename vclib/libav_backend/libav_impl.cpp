#include <libav_impl.hpp>

#include <logs.hpp>

char* vc_supported_formats[SUPPORTED_FORMATS_NO] = {"mp4", "mov"};

void VidConv::showVideoInfo(const char *input)
{
    AVFormatContext *pFormatContext = avformat_alloc_context();
    if (!pFormatContext) {
        vclog(VCLOGLIBAV, "ERROR could not allocate memory for Format Context");
        return;
    }

    vclog(VCLOGLIBAV, "Opening the input file (%s) and loading format (container) header", input);

    if (avformat_open_input(&pFormatContext, input, NULL, NULL) != 0) {
        vclog(VCLOGLIBAV, "ERROR could not open the file");
        return;
    }

    vclog(VCLOGLIBAV, "Format %s, duration %lld us, bit_rate %i", pFormatContext->iformat->name, pFormatContext->duration, pFormatContext->bit_rate);
    vclog(VCLOGLIBAV, "Finding stream info from format");

    if (avformat_find_stream_info(pFormatContext,  NULL) < 0) {
        vclog(VCLOGLIBAV, "ERROR could not get the stream info");
        return;
    }

    const AVCodec *pCodec = NULL;
    int video_stream_index = -1;

    // loop though all the streams and print its main information
    for (int i = 0; i < pFormatContext->nb_streams; i++)
    {
        AVCodecParameters *pLocalCodecParameters =  NULL;
        pLocalCodecParameters = pFormatContext->streams[i]->codecpar;

        vclog(VCLOGLIBAV, "AVStream->time_base before open coded %d/%d", pFormatContext->streams[i]->time_base.num, pFormatContext->streams[i]->time_base.den);
        vclog(VCLOGLIBAV, "AVStream->r_frame_rate before open coded %d/%d", pFormatContext->streams[i]->avg_frame_rate.num, pFormatContext->streams[i]->avg_frame_rate.den);
        vclog(VCLOGLIBAV, "AVStream->start_time %lld", pFormatContext->streams[i]->start_time);
        vclog(VCLOGLIBAV, "AVStream->duration %lld", pFormatContext->streams[i]->duration);

        vclog(VCLOGLIBAV, "Finding the proper decoder (CODEC)");

        const AVCodec *pLocalCodec = NULL;

        // finds the registered decoder for a codec ID
        pLocalCodec = avcodec_find_decoder(pLocalCodecParameters->codec_id);

        if (pLocalCodec==NULL) {
            vclog(VCLOGLIBAV, "ERROR unsupported codec!");
            // In this example if the codec is not found we just skip it
            continue;
        }

        // when the stream is a video we store its index, codec parameters and codec
        if (pLocalCodecParameters->codec_type == AVMEDIA_TYPE_VIDEO) {
            if (video_stream_index == -1) {
                video_stream_index = i;
                pCodec = pLocalCodec;
            }

            vclog(VCLOGLIBAV, "Video Codec: resolution %d x %d", pLocalCodecParameters->width, pLocalCodecParameters->height);
        } else if (pLocalCodecParameters->codec_type == AVMEDIA_TYPE_AUDIO) {
            vclog(VCLOGLIBAV, "Audio Codec: %d channels, sample rate %d", pLocalCodecParameters->channels, pLocalCodecParameters->sample_rate);
        }

        // print its name, id and bitrate
        vclog(VCLOGLIBAV, "Codec %s ID %d bit_rate %lld", pLocalCodec->long_name, pLocalCodec->id, pLocalCodecParameters->bit_rate);
    }

    if (video_stream_index == -1) {
        vclog(VCLOGLIBAV, "File %s does not contain a video stream!", input);
        return;
    }


    vclog(VCLOGLIBAV, "Cleaning memory");

    avformat_close_input(&pFormatContext);
    avformat_free_context(pFormatContext);
}
