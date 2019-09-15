#include "LevelUpMenu.h"

#include <cassert>

#include "Scaleform.h"


namespace Scaleform
{
	LevelUpMenu::LevelUpMenu()
	{
		using Context = RE::InputMappingManager::Context;
		using Flag = RE::IMenu::Flag;

		flags |= Flag::kTryShowCursor;
		auto loader = RE::BSScaleformMovieLoader::GetSingleton();
		auto success = loader->LoadMovieStd(this, SWF_NAME, [this](RE::GFxMovieDef* a_def)
		{
			using StateType = RE::GFxState::StateType;

			fxDelegate.reset(new RE::FxDelegate());
			fxDelegate->RegisterHandler(this);
			a_def->SetState(StateType::kExternalInterface, fxDelegate.get());
			fxDelegate->Release();

			auto logger = new Logger<LevelUpMenu>();
			a_def->SetState(StateType::kLog, logger);
			logger->Release();
		});

		if (!success) {
			assert(false);
			_FATALERROR("LevelUpMenu did not have a view due to missing dependencies! Aborting process!\n");
			MessageBoxA(NULL, "LevelUpMenu did not have a view due to missing dependencies!\r\nAborting process!", NULL, MB_OK);
			std::abort();
		}

		flags |= Flag::kPauseGame | Flag::kModal | Flag::kPreventGameLoad;
		context = Context::kMenuMode;

		InitExtensions();
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
		a_cbReg->Process("ModGlobal", ModGlobal);
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

		auto baseID = a_params[0].GetUInt();
		auto pluginName = a_params[1].GetString();

		auto formID = ResolveFormID(baseID, pluginName);
		auto global = RE::TESForm::LookupByID<RE::TESGlobal>(formID);
		if (global) {
			retVal.SetNumber(global->value);
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


	void LevelUpMenu::ModGlobal(const RE::FxDelegateArgs& a_params)
	{
		assert(a_params.GetArgCount() == 3);
		assert(a_params[0].IsNumber());
		assert(a_params[1].IsNumber());
		assert(a_params[2].IsString());

		auto baseID = a_params[0].GetUInt();
		auto pluginName = a_params[2].GetString();

		auto formID = ResolveFormID(baseID, pluginName);
		auto global = RE::TESForm::LookupByID<RE::TESGlobal>(formID);
		if (global) {
			auto mod = a_params[1].GetNumber();
			global->value += mod;
		}
	}


	void LevelUpMenu::ModPlayerAV(const RE::FxDelegateArgs& a_params)
	{
		assert(a_params.GetArgCount() == 2);
		assert(a_params[0].IsNumber());
		assert(a_params[1].IsNumber());

		auto av = static_cast<RE::ActorValue>(a_params[0].GetUInt());
		auto mod = static_cast<float>(a_params[1].GetNumber());
		auto player = RE::PlayerCharacter::GetSingleton();
		player->ModActorValueBase(av, mod);
	}


	void LevelUpMenu::PlaySound(const RE::FxDelegateArgs& a_params)
	{
		assert(a_params.GetArgCount() == 1);
		assert(a_params[0].IsString());

		RE::PlaySound(a_params[0].GetString());
	}


	RE::FormID LevelUpMenu::ResolveFormID(RE::FormID a_baseID, std::string a_pluginName)
	{
		RE::FormID result;

		auto dataHandler = RE::TESDataHandler::GetSingleton();
		auto mod = dataHandler->LookupModByName(a_pluginName);
		if (mod && mod->modIndex != 0xFF) {
			result = a_baseID;
			result += mod->modIndex << (8 * 3);
			result += mod->lightIndex << ((8 * 1) + (8 / 2));
		} else {
			result = 0;
		}

		return result;
	}


	void LevelUpMenu::OnMenuOpen()
	{
		bool success;
		view->SetVisible(true);

		std::vector<std::pair<RE::GFxValue*, std::string>> toGet;
		toGet.push_back(std::make_pair(&_root, "LevelUpMenu_mc"));
		RE::GFxValue var;
		for (auto& elem : toGet) {
			success = view->GetVariable(&var, elem.second.c_str());
			assert(success);
			*elem.first = var;
		}
	}


	void LevelUpMenu::OnMenuClose()
	{
		RE::GFxValue boolean(false);
		view->Invoke("Selection.captureFocus", 0, &boolean, 1);
	}


	void LevelUpMenu::InitExtensions()
	{
		RE::GFxValue boolean(true);
		bool success;

		auto input = RE::InputManager::GetSingleton();
		if (input->IsGamepadEnabled()) {
			view->Invoke("Selection.captureFocus", 0, &boolean, 1);
		}

		success = view->SetVariable("_global.gfxExtensions", boolean);
		assert(success);
		success = view->SetVariable("_global.noInvisibleAdvance", boolean);
		assert(success);
		success = view->SetVariable("Selection.alwaysEnableArrowKeys", boolean);
		assert(success);
		success = view->SetVariable("Selection.alwaysEnableKeyboardPress", boolean);
		assert(success);
		success = view->SetVariable("Selection.disableFocusAutoRelease", boolean);
		assert(success);
	}
}
