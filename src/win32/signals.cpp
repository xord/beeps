#include "../signals.h"


#include <mfapi.h>
#include <mfidl.h>
#include <mfreadwrite.h>
#include <xot/noncopyable.h>
#include <xot/string.h>
#include <xot/windows.h>
#include "exception.h"


namespace Beeps
{


	template <typename T, void (*Delete) (T*)>
	struct Ptr : public Xot::NonCopyable
	{

		T* ptr;

		Ptr () : ptr(NULL) {}

		~Ptr () {Delete(ptr);}

		T* operator -> () {return ptr;}

		operator bool () const {return ptr;}

		bool operator ! () const {return !operator bool();}

	};// Ptr

	template <typename T>
	static void
	release (T* object)
	{
		if (object) object->Release();
	}

	template <typename T>
	static void
	mem_free (T* object)
	{
		if (object) CoTaskMemFree(object);
	}

	template <typename T>
	using ReleasePtr = Ptr<T, release<T>>;

	template <typename T>
	using MemFreePtr = Ptr<T, mem_free<T>>;


	static bool is_file_exist (const char* path)
	{
		DWORD attribs = GetFileAttributes(path);
		return
			attribs != INVALID_FILE_ATTRIBUTES &&
			!(attribs & FILE_ATTRIBUTE_DIRECTORY);
	}

	static std::wstring
	to_wcs (const char* mbs)
	{
		size_t len = strlen(mbs);
		std::wstring wcs(len, L'#');
		mbstowcs(&wcs[0], mbs, len);
		return wcs;
	}

	static void
	load_bytes (WAVEFORMATEX* format, std::vector<BYTE>* bytes, const char* path)
	{
		std::wstring wpath = to_wcs(path);

		ReleasePtr<IMFMediaType> pcm_media_type;
		check_media_foundation_error(
			MFCreateMediaType(&pcm_media_type.ptr),
			__FILE__, __LINE__);
		pcm_media_type->SetGUID(MF_MT_MAJOR_TYPE, MFMediaType_Audio);
		pcm_media_type->SetGUID(MF_MT_SUBTYPE, MFAudioFormat_PCM);

		ReleasePtr<IMFSourceReader> source_reader;
		check_media_foundation_error(
			MFCreateSourceReaderFromURL(wpath.c_str(), NULL, &source_reader.ptr),
			__FILE__, __LINE__);

		check_media_foundation_error(
			source_reader->SetCurrentMediaType(
				MF_SOURCE_READER_FIRST_AUDIO_STREAM, NULL, pcm_media_type.ptr),
			__FILE__, __LINE__);

		ReleasePtr<IMFMediaType> media_type;
		check_media_foundation_error(
			source_reader->GetCurrentMediaType(
				MF_SOURCE_READER_FIRST_AUDIO_STREAM, &media_type.ptr),
			__FILE__, __LINE__);

		MemFreePtr<WAVEFORMATEX> format_;
		check_media_foundation_error(
			MFCreateWaveFormatExFromMFMediaType(media_type.ptr, &format_.ptr, NULL),
			__FILE__, __LINE__);

		*format = *format_.ptr;
		if (format->wFormatTag != WAVE_FORMAT_PCM)
			beeps_error(__FILE__, __LINE__, "'%s' is not a PCM file.", path);

		while (true)
		{
			ReleasePtr<IMFSample> sample;
			DWORD flags = 0;
			check_media_foundation_error(
				source_reader->ReadSample(
					MF_SOURCE_READER_FIRST_AUDIO_STREAM, 0, NULL, &flags, NULL, &sample.ptr),
				__FILE__, __LINE__);
			if (flags & MF_SOURCE_READERF_ENDOFSTREAM)
				break;

			ReleasePtr<IMFMediaBuffer> buffer;
			check_media_foundation_error(
				sample->ConvertToContiguousBuffer(&buffer.ptr),
				__FILE__, __LINE__);

			BYTE* data = NULL;
			DWORD size = 0;
			check_media_foundation_error(
				buffer->Lock(&data, NULL, &size),
				__FILE__, __LINE__);

			bytes->resize(bytes->size() + size);
			memcpy(&(*bytes)[0] + bytes->size() - size, data, size);

			check_media_foundation_error(
				buffer->Unlock(),
				__FILE__, __LINE__);
		}
	}

	Signals
	Signals_load (const char* path)
	{
		if (!is_file_exist(path))
			beeps_error(__FILE__, __LINE__, "'%s' not found.", path);

		WAVEFORMATEX format = {0};
		std::vector<BYTE> bytes;
		load_bytes(&format, &bytes, path);
		if (bytes.empty())
			beeps_error(__FILE__, __LINE__, "failed to read bytes: '%s'", path);

		uint Bps        = format.wBitsPerSample / 8;
		uint nchannels  = format.nChannels;
		uint nsamples   = bytes.size() / Bps / nchannels;
		Signals signals = Signals_create(nsamples, nchannels, format.nSamplesPerSec);

		for (uint channel = 0; channel < nchannels; ++channel)
		{
			switch (Bps)
			{
				case 1:
				{
					Sample*        to_p = Signals_at(&signals, 0, channel);
					const uchar* from_p = ((uchar*) &bytes[0]) + channel;
					for (uint i = 0; i < nsamples; ++i, to_p += nchannels, from_p += nchannels)
						*to_p = (*to_p - 128) / 128.f;
					break;
				}

				case 2:
				{
					Sample*         to_p = Signals_at(&signals, 0, channel);
					const ushort* from_p = ((ushort*) &bytes[0]) + channel;
					for (uint i = 0; i < nsamples; ++i, to_p += nchannels, from_p += nchannels)
						*to_p = *from_p / 32768.f;
					break;
				}
			}
		}

		Signals_set_nsamples(&signals, nsamples);
		return signals;
	}


}// Beeps
