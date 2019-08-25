#include "CLIK/MovieClip.h"


namespace CLIK
{
	MovieClip::MovieClip() :
		super()
	{}


	MovieClip::MovieClip(const MovieClip& a_rhs) :
		super(a_rhs._instance)
	{}


	MovieClip::MovieClip(MovieClip&& a_rhs) :
		super(std::move(a_rhs._instance))
	{}


	MovieClip::MovieClip(const RE::GFxValue& a_val) :
		super(a_val)
	{}


	MovieClip::MovieClip(RE::GFxValue&& a_val) :
		super(std::move(a_val))
	{}


	MovieClip::~MovieClip()
	{}


	MovieClip& MovieClip::operator=(const MovieClip& a_rhs)
	{
		_instance = a_rhs._instance;
		return *this;
	}


	MovieClip& MovieClip::operator=(MovieClip&& a_rhs)
	{
		_instance = std::move(a_rhs._instance);
		return *this;
	}


	MovieClip& MovieClip::operator=(const RE::GFxValue& a_rhs)
	{
		_instance = a_rhs;
		return *this;
	}


	MovieClip& MovieClip::operator=(RE::GFxValue&& a_rhs)
	{
		_instance = std::move(a_rhs);
		return *this;
	}


	double MovieClip::Alpha() const
	{
		return GetNumber("_alpha");
	}


	void MovieClip::Alpha(double a_alpha)
	{
		SetNumber("_alpha", a_alpha);
	}


	Object MovieClip::BlendMode() const
	{
		return GetObject("blendMode");
	}


	void MovieClip::BlendMode(Object& a_blendMode)
	{
		SetObject("blendMode", a_blendMode);
	}
}
