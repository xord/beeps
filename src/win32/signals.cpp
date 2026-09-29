#include "../signals.h"


#include <stdint.h>
#include <mfapi.h>
#include <mfidl.h>
#include <mfreadwrite.h>
#include <xot/noncopyable.h>
#include <xot/string.h>
#include <xot/windows.h>
#include <mmreg.h>// must be after windows.h
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
		if (!path) return false;

		DWORD attribs = GetFileAttributesW(Xot::String(path).to_wstr().c_str());
		return
			attribs != INVALID_FILE_ATTRIBUTES &&
			!(attribs & FILE_ATTRIBUTE_DIRECTORY);
	}

	static void
	set_decoded_media_type (IMFMediaType* decoded, IMFSourceReader* source_reader)
	{
		ReleasePtr<IMFMediaType> native;
		check_media_foundation_error(
			source_reader->GetNativeMediaType(
				MF_SOURCE_READER_FIRST_AUDIO_STREAM, 0, &native.ptr),
			__FILE__, __LINE__);

		GUID subtype = {};// all zeros, as GUID_NULL, which needs libuuid
		UINT32 bits  = 0;
		native->GetGUID(MF_MT_SUBTYPE, &subtype);
		native->GetUINT32(MF_MT_AUDIO_BITS_PER_SAMPLE, &bits);

		bool is_float = subtype == MFAudioFormat_Float;
		if (is_float)
			bits = 32;
		else if (subtype != MFAudioFormat_PCM || (bits != 8 && bits != 24 && bits != 32))
			bits = 16;

		decoded->SetGUID(MF_MT_MAJOR_TYPE, MFMediaType_Audio);
		decoded->SetGUID(MF_MT_SUBTYPE, is_float ? MFAudioFormat_Float : MFAudioFormat_PCM);
		decoded->SetUINT32(MF_MT_AUDIO_BITS_PER_SAMPLE, bits);
	}

	static bool
	is_float_format (const WAVEFORMATEX& format)
	{
		if (format.wFormatTag == WAVE_FORMAT_IEEE_FLOAT)
			return true;
		if (format.wFormatTag != WAVE_FORMAT_EXTENSIBLE)
			return false;

		const auto& extensible = (const WAVEFORMATEXTENSIBLE&) format;
		return extensible.SubFormat.Data1 == WAVE_FORMAT_IEEE_FLOAT;
	}

	static void
	load_bytes (
		WAVEFORMATEX* format, bool* is_float, std::vector<BYTE>* bytes,
		const char* path)
	{
		std::wstring wpath = Xot::String(path).to_wstr();

		ReleasePtr<IMFSourceReader> source_reader;
		check_media_foundation_error(
			MFCreateSourceReaderFromURL(wpath.c_str(), NULL, &source_reader.ptr),
			__FILE__, __LINE__);

		ReleasePtr<IMFMediaType> decoded_media_type;
		check_media_foundation_error(
			MFCreateMediaType(&decoded_media_type.ptr),
			__FILE__, __LINE__);
		set_decoded_media_type(decoded_media_type.ptr, source_reader.ptr);

		check_media_foundation_error(
			source_reader->SetCurrentMediaType(
				MF_SOURCE_READER_FIRST_AUDIO_STREAM, NULL, decoded_media_type.ptr),
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

		*format   = *format_.ptr;
		*is_float = is_float_format(*format_.ptr);
		if (
			format->wFormatTag != WAVE_FORMAT_PCM &&
			format->wFormatTag != WAVE_FORMAT_IEEE_FLOAT &&
			format->wFormatTag != WAVE_FORMAT_EXTENSIBLE)
		{
			beeps_error(__FILE__, __LINE__, "'%s' is not a PCM file.", path);
		}

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
			memcpy(bytes->data() + bytes->size() - size, data, size);

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
		bool is_float       = false;
		std::vector<BYTE> bytes;
		load_bytes(&format, &is_float, &bytes, path);
		if (bytes.empty())
			beeps_error(__FILE__, __LINE__, "failed to read bytes: '%s'", path);

		uint Bps       = format.wBitsPerSample / 8;
		uint nchannels = format.nChannels;
		if (is_float ? Bps != 4 : (Bps < 1 || 4 < Bps))
		{
			beeps_error(
				__FILE__, __LINE__, "'%s' has samples of %d bits.",
				path, format.wBitsPerSample);
		}
		if (nchannels == 0)
			beeps_error(__FILE__, __LINE__, "'%s' has no channels.", path);

		uint nsamples = bytes.size() / Bps / nchannels;
		Signals signals(nsamples, nchannels, format.nSamplesPerSec);

		uint step = nchannels * Bps;
		for (uint ch = 0; ch < nchannels; ++ch)
		{
			Sample*        to_p = Signals_at(&signals, 0, ch);
			const uchar* from_p = ((uchar*) bytes.data()) + ch * Bps;
			switch (Bps)
			{
				case 1:
				{
					for (uint i = 0; i < nsamples; ++i, to_p += nchannels, from_p += step)
						*to_p = (*from_p - 128) / 128.f;
					break;
				}

				case 2:
				{
					for (uint i = 0; i < nsamples; ++i, to_p += nchannels, from_p += step)
						*to_p = *(const short*) from_p / 32768.f;
					break;
				}

				case 3:
				{
					for (uint i = 0; i < nsamples; ++i, to_p += nchannels, from_p += step)
					{
						int32_t value = from_p[0] | (from_p[1] << 8) | (from_p[2] << 16);
						if (value & 0x800000) value -= 0x1000000;// sign extension
						*to_p = value / 8388608.f;
					}
					break;
				}

				case 4:
				{
					if (is_float)
					{
						for (uint i = 0; i < nsamples; ++i, to_p += nchannels, from_p += step)
							*to_p = *(const float*) from_p;
					}
					else
					{
						for (uint i = 0; i < nsamples; ++i, to_p += nchannels, from_p += step)
							*to_p = *(const int32_t*) from_p / 2147483648.f;
					}
					break;
				}
			}
		}

		Signals_set_nsamples(&signals, nsamples);
		return signals;
	}


}// Beeps
