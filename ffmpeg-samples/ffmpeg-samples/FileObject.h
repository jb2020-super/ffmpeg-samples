#pragma once
#include <string>

extern "C" {
#include "libavformat/avformat.h"
#include "libavcodec/avcodec.h"
}
class FileObject
{
public:
	FileObject(const char* url);
	~FileObject();

	void ShowStreamInfo();
	void ReadFrames();
private:
	int open_codec_context(AVCodecContext** dec_ctx, AVMediaType type);
private:
	std::string m_url{};
	AVFormatContext* m_avFormatContext{};
	AVCodecContext* m_video_ctx{};
	AVCodecContext* m_audio_ctx{};
};

