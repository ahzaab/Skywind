#pragma once

#include <string>
#include <vector>

#include "RE/Skyrim.h"


namespace Scaleform
{
	template <class T>
	class Logger : public RE::GFxLog
	{
	public:
		virtual void LogMessageVarg(LogMessageType a_messageType, const char* a_fmt, va_list a_argList) override
		{
			std::string fmt(a_fmt ? a_fmt : "");
			while (!fmt.empty() && fmt.back() == '\n') {
				fmt.pop_back();
			}

			va_list args;
			va_copy(args, a_argList);
			std::vector<char> buf(std::vsnprintf(0, 0, fmt.c_str(), a_argList) + 1);
			std::vsnprintf(buf.data(), buf.size(), fmt.c_str(), args);
			va_end(args);

			_MESSAGE("%s: %s", T::Name().data(), buf.data());
		}
	};


	void RegisterCallbacks();
	void RegisterCreators();
}
