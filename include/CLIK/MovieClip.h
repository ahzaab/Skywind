#pragma once

#include "Object.h"


namespace CLIK
{
	class MovieClip : public Object
	{
	public:
		using super = Object;


		MovieClip();
		MovieClip(const MovieClip& a_rhs);
		MovieClip(MovieClip&& a_rhs);
		explicit MovieClip(const RE::GFxValue& a_val);
		explicit MovieClip(RE::GFxValue&& a_val);
		~MovieClip();

		MovieClip& operator=(const MovieClip& a_rhs);
		MovieClip& operator=(MovieClip&& a_rhs);
		MovieClip& operator=(const RE::GFxValue& a_rhs);
		MovieClip& operator=(RE::GFxValue&& a_rhs);

		// properties
		double Alpha() const;
		void Alpha(double a_alpha);

		Object BlendMode() const;
		void BlendMode(Object& a_blendMode);
	};
}
