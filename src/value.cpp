#include "beeps/generator.h"


#include <algorithm>
#include "beeps/exception.h"
#include "processor.h"
#include "signals.h"


namespace Beeps
{


	struct Value::Data
	{

		Type type = TYPE_NONE;

		PointList points;

	};// Value::Data


	Value::Point::Point (float value, float time)
	:	value(value), time(time)
	{
	}


	Value::Value (float value, Type type)
	{
		self->type = type;
		set_value(value);
	}

	Value::~Value ()
	{
	}

	void
	Value::set_type (Type type)
	{
		self->type = type;

		set_updated();
	}

	Value::Type
	Value::type () const
	{
		return self->type;
	}

	void
	Value::set_value (float value)
	{
		self->points.clear();
		self->points.push_back(Point(value, 0));

		set_updated();
	}

	void
	Value::insert (float value, float time)
	{
		if (time < 0)
			argument_error(__FILE__, __LINE__, "time must be >= 0");

		auto& points = self->points;
		auto it      = std::find_if(
			points.begin(), points.end(),
			[&](const auto& p) {return time <= p.time;});
		if (it != points.end() && it->time == time)
		{
			if (it->value == value) return;
			it->value = value;
		}
		else
			points.emplace(it, value, time);

		set_updated();
	}

	void
	Value::clear ()
	{
		self->points.clear();

		set_updated();
	}

	Value::iterator
	Value::begin ()
	{
		return self->points.begin();
	}

	Value::const_iterator
	Value::begin () const
	{
		return self->points.begin();
	}

	Value::iterator
	Value::end ()
	{
		return self->points.end();
	}

	Value::const_iterator
	Value::end () const
	{
		return self->points.end();
	}

	static float
	interpolate_linear (const Value::PointList& points, float time)
	{
		if (points.empty())         return 0;
		if (time <= points[0].time) return points[0].value;

		for (size_t i = 0; i + 1 < points.size(); ++i)
		{
			const auto& p0 = points[i];
			const auto& p1 = points[i + 1];
			if (time < p1.time)
			{
				float t = (time - p0.time) / (p1.time - p0.time);
				return p0.value + (p1.value - p0.value) * t;
			}
		}
		return points.back().value;
	}

	static decltype(&interpolate_linear)
	get_interpolate_func (Value::Type type)
	{
		switch (type)
		{
			case Value::LINEAR: return interpolate_linear;
			default:            return NULL;
		}
	}

	void
	Value::generate (Context* context, Signals* signals, uint* offset)
	{
		Super::generate(context, signals, offset);

		auto& points  = self->points;
		uint nsamples = signals->capacity();

		if (points.size() == 1)
			Signals_fill(signals, nsamples, points[0].value);
		else
		{
			auto interpolate = get_interpolate_func(self->type);
			if (!interpolate)
				invalid_state_error(__FILE__, __LINE__);

			double sample_rate = signals->sample_rate();
			Sample* p          = Signals_at(signals, 0);
			for (uint i = 0; i < nsamples; ++i)
				p[i] = interpolate(points, (*offset + i) / sample_rate);
			Signals_set_nsamples(signals, nsamples);
		}

		*offset += signals->nsamples();
	}

	int
	Value::max_segment_size_for_process (
		double sample_rate, uint nsamples) const
	{
		if (self->points.size() == 1)
			return Super::max_segment_size_for_process(sample_rate, nsamples);
		return sample_rate / 256;
	}


}// Beeps
