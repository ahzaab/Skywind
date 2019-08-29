#pragma once

#include "RE/Skyrim.h"


namespace Scaleform
{
	template <class T>
	class Logger : public RE::GFxLog
	{
	public:
		virtual void LogMessageVarg(LogMessageType a_messageType, const char* a_fmt, va_list a_argList) override
		{
			va_list args;
			va_copy(args, a_argList);
			std::vector<char> buf(std::vsnprintf(0, 0, a_fmt, a_argList) + 1);
			std::vsnprintf(buf.data(), buf.size(), a_fmt, args);
			va_end(args);
			_MESSAGE("%", T::Name().data(), buf.data());
		}
	};


	void RegisterCallbacks();
	void RegisterCreators();
}
