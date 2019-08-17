#include "SkyWind.h"

#include <cassert>

#include "LevelUpMenu.h"

#include "SKSE/API.h"


namespace Scaleform
{
	bool SkyWind::Register(RE::GFxMovieView* a_view, RE::GFxValue* a_root)
	{
		GetGlobal::Register(a_view, a_root);
		GetPlayerAV::Register(a_view, a_root);
		GetPlayerLevel::Register(a_view, a_root);
		return true;
	}


	void SkyWind::GetGlobal::Call(Params& a_params)
	{
		assert(a_params.argCount == 2);
		assert(a_params.args[0].IsNumber());
		assert(a_params.args[1].IsString());

		a_params.retVal->SetUndefined();

		auto dataHandler = RE::TESDataHandler::GetSingleton();
		auto mod = dataHandler->LookupModByName(a_params.args[1].GetString());
		if (!mod || mod->modIndex == 0xFF) {
			return;
		}

		auto formID = static_cast<RE::FormID>(std::round(a_params.args[0].GetNumber()));
		formID += mod->modIndex << (8 * 3);
		formID += mod->lightIndex << ((8 * 1) + (8 / 2));

		auto global = RE::TESForm::LookupByID<RE::TESGlobal>(formID);
		if (global) {
			a_params.retVal->SetNumber(global->value);
		}
	}


	void SkyWind::GetGlobal::Register(RE::GFxMovieView* a_view, RE::GFxValue* a_root)
	{
		RE::GFxValue fn;
		a_view->CreateFunction(&fn, new GetGlobal());
		auto success = a_root->SetMember("getGlobal", fn);
		assert(success);
	}


	void SkyWind::GetPlayerAV::Call(Params& a_params)
	{
		assert(a_params.argCount == 1);
		assert(a_params.args[0].IsNumber());

		auto iAV = static_cast<UInt32>(std::round(a_params.args[0].GetNumber()));
		auto av = static_cast<RE::ActorValue>(iAV);
		auto player = RE::PlayerCharacter::GetSingleton();
		auto result = player->GetActorValueCurrent(av);
		a_params.retVal->SetNumber(result);
	}


	void SkyWind::GetPlayerAV::Register(RE::GFxMovieView* a_view, RE::GFxValue* a_root)
	{
		RE::GFxValue fn;
		a_view->CreateFunction(&fn, new GetPlayerAV());
		auto success = a_root->SetMember("getPlayerAV", fn);
		assert(success);
	}


	void SkyWind::GetPlayerLevel::Call(Params& a_params)
	{
		assert(a_params.argCount == 0);

		auto player = RE::PlayerCharacter::GetSingleton();
		auto result = static_cast<double>(player->GetLevel());
		a_params.retVal->SetNumber(result);
	}


	void SkyWind::GetPlayerLevel::Register(RE::GFxMovieView* a_view, RE::GFxValue* a_root)
	{
		RE::GFxValue fn;
		a_view->CreateFunction(&fn, new GetPlayerLevel());
		auto success = a_root->SetMember("getPlayerLevel", fn);
		assert(success);
	}


	void RegisterCallbacks()
	{
		auto scaleform = SKSE::GetScaleformInterface();
		scaleform->Register(SkyWind::Register, "SkyWind");

		_MESSAGE("Registered all scaleform callbacks");
	}


	void RegisterCreators()
	{
		LevelUpMenu::Register();

		_MESSAGE("Registered all scaleform creators");
	}
}
