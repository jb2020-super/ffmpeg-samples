#include "FileObject.h"
#include <exception>

FileObject::FileObject(const char* url, const std::string& type):
	m_url{ url },
	m_type{ type }
{
	m_avFormatContext = avformat_alloc_context();
	if (avformat_open_input(&m_avFormatContext, url, nullptr, nullptr)) {
		throw std::exception("avformat_open_input error");
	}
}

FileObject::~FileObject()
{
	avformat_free_context(m_avFormatContext);
}

void FileObject::ShowStreamInfo()
{
	if (avformat_find_stream_info(m_avFormatContext, nullptr) < 0) {
		throw std::exception("avformat_find_stream_info failed");
	}
	av_dump_format(m_avFormatContext, 0, m_url.c_str(), 0);

	m_vstream_idx = open_codec_context(&m_video_ctx, AVMEDIA_TYPE_VIDEO);
	
	m_astream_idx = open_codec_context(&m_audio_ctx, AVMEDIA_TYPE_AUDIO);
}

void FileObject::ReadFrames()
{
	AVPacket* pkt = av_packet_alloc();
	int vcnt{ 0 }, acnt{ 0 };
	while (av_read_frame(m_avFormatContext, pkt) >= 0) {
		if (pkt->stream_index == m_vstream_idx) {
			if ((m_type == "v" || m_type == "video") 
				|| m_type.empty())
			{
				print_packet(pkt, vcnt++);
			}
		}
		else if (pkt->stream_index == m_astream_idx) {
			if ((m_type == "a" || m_type == "audio") 
				|| m_type.empty())
			{
				print_packet(pkt, acnt++);
			}
		}

		av_packet_unref(pkt);
		if (vcnt >= 1000) {
			break;
		}
	}
	av_packet_free(&pkt);
}

int FileObject::open_codec_context(AVCodecContext** dec_ctx, AVMediaType type)
{
	int stream_no{};
	stream_no = av_find_best_stream(m_avFormatContext, type, -1, -1, nullptr, 0);
	if (stream_no < 0)
	{
		throw std::exception("av_find_best_stream failed");
	}
	AVStream* st = m_avFormatContext->streams[stream_no];
	printf("Stream %s: %d, time base (%d, %d)\n", av_get_media_type_string(type), stream_no, st->time_base.num, st->time_base.den);
	auto dec = avcodec_find_decoder(st->codecpar->codec_id);
	if (!dec) {
		throw std::exception("avcodec_find_decoder failed");
	}
	*dec_ctx = avcodec_alloc_context3(dec);
	if (!*dec_ctx) {
		throw std::exception("avcodec_alloc_context3 failed");
	}
	if (avcodec_open2(*dec_ctx, dec, nullptr) < 0) {
		throw std::exception("avcodec_open2 failed");
	}
	return stream_no;
}

void FileObject::print_packet(const AVPacket* pkt, int cnt)
{
	printf("[%d] [%d] pts: %lld, dts: %lld, duration: %lld, size: %d\n",
		cnt, pkt->stream_index, pkt->pts, pkt->dts, pkt->duration, pkt->size);
}
