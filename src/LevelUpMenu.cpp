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
		auto loader = RE::GFxLoader::GetSingleton();
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


	void LevelUpMenu::Accept(RE::FxDelegateHandler::CallbackProcessor* a_cbReg)
	{
		a_cbReg->Process("Close", Close);
		a_cbReg->Process("Log", Log);
		a_cbReg->Process("PlaySound", PlaySound);
		a_cbReg->Process("UpdatePlayerAV", UpdatePlayerAV);
	}


	auto LevelUpMenu::ProcessMessage(RE::UIMessage* a_message)
		-> Result
	{
		using Message = RE::UIMessage::Message;

		switch (a_message->message) {
		case Message::kOpen:
			OnOpen();
			return Result::kProcessed;
		case Message::kClose:
			OnClose();
			return Result::kProcessed;
		default:
			return Base::ProcessMessage(a_message);
		}
	}


	void LevelUpMenu::Close(const RE::FxDelegateArgs& a_params)
	{
		assert(a_params.GetArgCount() == 0);

		auto player = RE::PlayerCharacter::GetSingleton();
		player->skills->AdvanceLevel(false);

		Close();
	}


	void LevelUpMenu::Log(const RE::FxDelegateArgs& a_params)
	{
		assert(a_params.GetArgCount() == 1);
		assert(a_params[0].IsString());

		_MESSAGE("%s: %s", Name().data(), a_params[0].GetString());
	}


	void LevelUpMenu::PlaySound(const RE::FxDelegateArgs& a_params)
	{
		assert(a_params.GetArgCount() == 1);
		assert(a_params[0].IsString());

		RE::PlaySound(a_params[0].GetString());
	}


	void LevelUpMenu::UpdatePlayerAV(const RE::FxDelegateArgs& a_params)
	{
		assert(a_params.GetArgCount() == 2);
		assert(a_params[0].IsNumber());
		assert(a_params[1].IsNumber());

		auto av = static_cast<RE::ActorValue>(static_cast<UInt32>(std::round(a_params[0].GetNumber())));
		auto val = static_cast<float>(a_params[0].GetNumber());

		auto player = RE::PlayerCharacter::GetSingleton();
		player->ModActorValueBase(av, val);
	}


	void LevelUpMenu::OnOpen()
	{
		view->SetVisible(true);
	}


	void LevelUpMenu::OnClose()
	{
		RE::GFxValue boolean(false);
		view->Invoke("Selection.captureFocus", 0, &boolean, 1);
	}
}
