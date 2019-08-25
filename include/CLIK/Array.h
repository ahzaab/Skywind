#pragma once

#include "CLIK/Object.h"


namespace CLIK
{
	class Array : public Object
	{
	public:
		using super = Object;


		Array();
		Array(const Array& a_rhs);
		Array(Array&& a_rhs);
		explicit Array(RE::GPtr<RE::GFxMovieView>& a_view);
		explicit Array(RE::GFxMovieView* a_view);
		explicit Array(const RE::GFxValue& a_val);
		explicit Array(RE::GFxValue&& a_val);
		~Array();

		Array& operator=(const Array& a_rhs);
		Array& operator=(Array&& a_rhs);
		Array& operator=(const RE::GFxValue& a_rhs);
		Array& operator=(RE::GFxValue&& a_rhs);


		// properties
		static constexpr double CASEINSENSITIVE = 1;
		static constexpr double DESCENDING = 2;
		static constexpr double NUMERIC = 16;
		static constexpr double RETURNINDEXEDARRAY = 8;
		static constexpr double UNIQUESORT = 4;


		double Length() const;
		void Length(double a_length);

		// methods
		Array Concat(std::optional<std::reference_wrapper<Object>> a_value);

		std::string Join(std::optional<const char*> a_delimiter);

		Object Pop();

		double Push(Object& a_value);

		void Reverse();

		Object Shift();
	};
}
