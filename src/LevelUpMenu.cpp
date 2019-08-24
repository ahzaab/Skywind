#include "LevelUpMenu.h"

#include "skse64_common/SafeWrite.h"

#include <cassert>

#include "REL/Relocation.h"


namespace Scaleform
{
	LevelUpMenu::LevelUpMenu()
	{
		using ScaleModeType = RE::GFxMovieView::ScaleModeType;
		using Context = RE::InputMappingManager::Context;
		using Flag = RE::IMenu::Flag;

		flags |= Flag::kTryShowCursor;
		auto loader = RE::BSScaleformMovieLoader::GetSingleton();
		if (loader->LoadMovie(this, view, SWF_NAME, ScaleModeType::kShowAll, 0.0)) {
			flags |= Flag::kPauseGame | Flag::kModal | Flag::kPreventGameLoad;
			context = Context::kMenuMode;
		}

		if (!view) {
			assert(false);
			_FATALERROR("LevelUpMenu did not have a view due to missing dependencies! Aborting process!\n");
			MessageBoxA(NULL, "LevelUpMenu did not have a view due to missing dependencies!\r\nAborting process!", NULL, MB_OK);
			std::abort();
		}

		auto input = RE::InputManager::GetSingleton();
		if (input->IsGamepadEnabled()) {
			RE::GFxValue boolean(true);
			view->Invoke("Selection.captureFocus", 0, &boolean, 1);
		}

		view->SetVisible(false);
	}


	void LevelUpMenu::Accept(RE::FxDelegateHandler::CallbackProcessor* a_cbReg)
	{
		a_cbReg->Process("AdvanceLevel", AdvanceLevel);
		a_cbReg->Process("CloseMenu", CloseMenu);
		a_cbReg->Process("GetGlobal", GetGlobal);
		a_cbReg->Process("GetPlayerAV", GetPlayerAV);
		a_cbReg->Process("GetPlayerLevel", GetPlayerLevel);
		a_cbReg->Process("Log", Log);
		a_cbReg->Process("ModPlayerAV", ModPlayerAV);
		a_cbReg->Process("PlaySound", PlaySound);
	}


	auto LevelUpMenu::ProcessMessage(RE::UIMessage* a_message)
		-> Result
	{
		using Message = RE::UIMessage::Message;

		switch (a_message->message) {
		case Message::kOpen:
			OnMenuOpen();
			return Result::kProcessed;
		case Message::kClose:
			OnMenuClose();
			return Result::kProcessed;
		default:
			return Base::ProcessMessage(a_message);
		}
	}


	void LevelUpMenu::Open()
	{
		using Message = RE::UIMessage::Message;

		auto ui = RE::UIManager::GetSingleton();
		ui->AddMessage(Name(), Message::kOpen, 0);
	}


	void LevelUpMenu::Close()
	{
		using Message = RE::UIMessage::Message;

		auto ui = RE::UIManager::GetSingleton();
		ui->AddMessage(Name(), Message::kClose, 0);
	}


	void LevelUpMenu::Register()
	{
		auto mm = RE::MenuManager::GetSingleton();
		auto it = mm->menuTable.find(Name());
		if (it != mm->menuTable.end()) {
			it->second.menuConstructor = Create;
		} else {
			mm->Register(Name(), Create);
		}

		_MESSAGE("Registered %s", Name().data());
	}


	RE::IMenu* LevelUpMenu::Create()
	{
		return new LevelUpMenu();
	}


	void LevelUpMenu::AdvanceLevel(const RE::FxDelegateArgs& a_params)
	{
		assert(a_params.GetArgCount() == 0);

		auto player = RE::PlayerCharacter::GetSingleton();
		player->skills->AdvanceLevel(false);
	}


	void LevelUpMenu::CloseMenu(const RE::FxDelegateArgs& a_params)
	{
		assert(a_params.GetArgCount() == 0);

		Close();
	}


	void LevelUpMenu::GetGlobal(const RE::FxDelegateArgs& a_params)
	{
		assert(a_params.GetArgCount() == 2);
		assert(a_params[0].IsNumber());
		assert(a_params[1].IsString());

		RE::FxResponseArgs<1> response;
		RE::GFxValue retVal;
		retVal.SetUndefined();

		auto dataHandler = RE::TESDataHandler::GetSingleton();
		auto mod = dataHandler->LookupModByName(a_params[1].GetString());
		if (mod && mod->modIndex != 0xFF) {
			auto formID = static_cast<RE::FormID>(a_params[0].GetUInt());
			formID += mod->modIndex << (8 * 3);
			formID += mod->lightIndex << ((8 * 1) + (8 / 2));

			auto global = RE::TESForm::LookupByID<RE::TESGlobal>(formID);
			if (global) {
				retVal.SetNumber(global->value);
			}
		}

		response.Add(retVal);
		a_params.Respond(response);
	}


	void LevelUpMenu::GetPlayerAV(const RE::FxDelegateArgs& a_params)
	{
		assert(a_params.GetArgCount() == 1);
		assert(a_params[0].IsNumber());

		RE::FxResponseArgs<1> response;
		RE::GFxValue retVal;

		auto av = static_cast<RE::ActorValue>(a_params[0].GetUInt());
		auto player = RE::PlayerCharacter::GetSingleton();
		auto currentAV = player->GetActorValueCurrent(av);
		retVal.SetNumber(currentAV);

		response.Add(retVal);
		a_params.Respond(response);
	}


	void LevelUpMenu::GetPlayerLevel(const RE::FxDelegateArgs& a_params)
	{
		assert(a_params.GetArgCount() == 0);

		RE::FxResponseArgs<1> response;
		RE::GFxValue retVal;

		auto player = RE::PlayerCharacter::GetSingleton();
		auto level = static_cast<double>(player->GetLevel());
		retVal.SetNumber(level);

		response.Add(retVal);
		a_params.Respond(response);
	}


	void LevelUpMenu::Log(const RE::FxDelegateArgs& a_params)
	{
		assert(a_params.GetArgCount() == 1);
		assert(a_params[0].IsString());

		_MESSAGE("%s: %s", Name().data(), a_params[0].GetString());
	}


	void LevelUpMenu::ModPlayerAV(const RE::FxDelegateArgs& a_params)
	{
		assert(a_params.GetArgCount() == 2);
		assert(a_params[0].IsNumber());
		assert(a_params[1].IsNumber());

		auto av = static_cast<RE::ActorValue>(a_params[0].GetUInt());
		auto val = static_cast<float>(a_params[0].GetNumber());
		auto player = RE::PlayerCharacter::GetSingleton();
		player->ModActorValueBase(av, val);
	}


	void LevelUpMenu::PlaySound(const RE::FxDelegateArgs& a_params)
	{
		assert(a_params.GetArgCount() == 1);
		assert(a_params[0].IsString());

		RE::PlaySound(a_params[0].GetString());
	}


	void LevelUpMenu::OnMenuOpen()
	{
		view->SetVisible(true);
	}


	void LevelUpMenu::OnMenuClose()
	{
		RE::GFxValue boolean(false);
		view->Invoke("Selection.captureFocus", 0, &boolean, 1);
	}
}
