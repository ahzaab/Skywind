#pragma once

#include "RE/Skyrim.h"


namespace Scaleform
{
	class SkyWind
	{
	public:
		static bool Register(RE::GFxMovieView* a_view, RE::GFxValue* a_root);

	private:
		class GetGlobal : public RE::GFxFunctionHandler
		{
		public:
			GetGlobal() = default;
			virtual ~GetGlobal() = default;

			virtual void Call(Params& a_params) override;

			static void Register(RE::GFxMovieView* a_view, RE::GFxValue* a_root);
		};


		class GetPlayerAV : public RE::GFxFunctionHandler
		{
		public:
			GetPlayerAV() = default;
			virtual ~GetPlayerAV() = default;

			virtual void Call(Params& a_params) override;

			static void Register(RE::GFxMovieView* a_view, RE::GFxValue* a_root);
		};


		class GetPlayerLevel : public RE::GFxFunctionHandler
		{
		public:
			GetPlayerLevel() = default;
			virtual ~GetPlayerLevel() = default;

			virtual void Call(Params& a_params) override;

			static void Register(RE::GFxMovieView* a_view, RE::GFxValue* a_root);
		};
	};


	void RegisterCallbacks();
	void RegisterCreators();
}
