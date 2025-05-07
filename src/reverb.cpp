#include "beeps/filter.h"


#include <FreeVerb.h>
#include "beeps/exception.h"
#include "signals.h"


namespace Beeps
{


	struct Reverb::Data
	{

		stk::FreeVerb freeverb;

		float mix       = 0.75;

		float room_size = 0.75;

		float damping   = 0.25;

	};// Reverb::Data


	Reverb::Reverb (Processor* input)
	:	Super(input)
	{
	}

	Reverb::~Reverb ()
	{
	}

	void
	Reverb::set_mix (float mix)
	{
		if (mix < 0)
			argument_error(__FILE__, __LINE__);
		if (mix > 1)
			argument_error(__FILE__, __LINE__);

		if (mix == self->mix)
			return;

		self->mix = mix;
		set_updated();
	}

	float
	Reverb::mix () const
	{
		return self->mix;
	}

	void
	Reverb::set_room_size (float size)
	{
		if (size < 0)
			argument_error(__FILE__, __LINE__);
		if (size > 1)
			argument_error(__FILE__, __LINE__);

		if (size == self->room_size)
			return;

		self->room_size = size;
		set_updated();
	}

	float
	Reverb::room_size () const
	{
		return self->room_size;
	}

	void
	Reverb::set_damping (float damping)
	{
		if (damping < 0)
			argument_error(__FILE__, __LINE__);
		if (damping > 1)
			argument_error(__FILE__, __LINE__);

		if (damping == self->damping)
			return;

		self->damping = damping;
		set_updated();
	}

	float
	Reverb::damping () const
	{
		return self->damping;
	}

	void
	Reverb::filter (Context* context, Signals* signals, uint* offset)
	{
		Super::filter(context, signals, offset);

		self->freeverb.setEffectMix(self->mix);
		self->freeverb.setRoomSize( self->room_size);
		self->freeverb.setDamping(  self->damping);

		uint nchannels   = signals->nchannels();
		Signals filtered = Signals_create(
			signals->nsamples(), nchannels, signals->sample_rate());

		Signals_tick(
			&filtered, *signals,
			[&](stk::StkFrames* out, const stk::StkFrames& in)
			{
				for (uint ch = 0; ch < nchannels; ++ch)
					self->freeverb.tick(const_cast<stk::StkFrames&>(in), *out, ch, ch);
			});

		*signals = filtered;
	}


}// Beeps
