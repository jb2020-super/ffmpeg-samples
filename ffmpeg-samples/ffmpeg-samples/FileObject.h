#pragma once
#include <string>

extern "C" {
#include "libavformat/avformat.h"
#include "libavcodec/avcodec.h"
}
class FileObject
{
public:
	FileObject(const char* url, const std::string& type);
	~FileObject();

	void ShowStreamInfo();
	void ReadFrames();
private:
	int open_codec_context(AVCodecContext** dec_ctx, AVMediaType type);

	void print_packet(const AVPacket* pkt, int cnt);
private:
	std::string m_url{};
	std::string m_type{};
	AVFormatContext* m_avFormatContext{};
	AVCodecContext* m_video_ctx{};
	AVCodecContext* m_audio_ctx{};
	int m_vstream_idx{-1};
	int m_astream_idx{-1};
};

