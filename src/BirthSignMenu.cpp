#include "BirthSignMenu.h"

#include "Papyrus.h"


namespace Scaleform
{
	BirthSignMenu::BirthSignMenu()
	{
		using ScaleModeType = RE::GFxMovieView::ScaleModeType;
		using Context = RE::InputMappingManager::Context;
		using Flag = RE::IMenu::Flag;

		flags = Flag::kTryShowCursor;
		auto loader = RE::BSScaleformMovieLoader::GetSingleton();
		if (!loader->LoadMovie(this, view, SWF_NAME, ScaleModeType::kShowAll, 0.0)) {
			assert(false);
			_FATALERROR("BirthSignMenu did not have a view due to missing dependencies! Aborting process!\n");
			MessageBoxA(NULL, "BirthSignMenu did not have a view due to missing dependencies!\r\nAborting process!", NULL, MB_OK);
			std::abort();
		}

		menuDepth = 5;	// JournalMenu == 5
		flags |= Flag::kPreventGameLoad | Flag::kHideOther | Flag::kPauseGame;
		context = Context::kFavor;

		auto mc = RE::MenuControls::GetSingleton();
		mc->RegisterHandler(this);
		view->SetVisible(false);
	}


	BirthSignMenu::~BirthSignMenu()
	{
		auto mc = RE::MenuControls::GetSingleton();
		mc->RemoveHandler(this);
	}


	auto BirthSignMenu::ProcessMessage(RE::UIMessage* a_message)
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
			return MenuBase::ProcessMessage(a_message);
		}
	}


	bool BirthSignMenu::CanProcess(RE::InputEvent* a_event)
	{
		using EventType = RE::InputEvent::EventType;
		return a_event && a_event->eventType == EventType::kButton;
	}


	bool BirthSignMenu::ProcessButton(RE::ButtonEvent* a_event)
	{
		using DeviceType = RE::DeviceType;

		for (auto button = a_event; button; button = static_cast<RE::ButtonEvent*>(button->next)) {
			switch (button->deviceType) {
			case DeviceType::kKeyboard:
				{
					using Key = RE::BSWin32KeyboardDevice::Key;
					switch (button->keyMask) {
					case Key::kEnter:
						SendAcceptEvent();
						break;
					case Key::kEscape:
						Close();
						break;
					default:
						break;
					}
				}
				break;
			case DeviceType::kGamepad:
				{
					using Key = RE::BSWin32GamepadDevice::Key;
					switch (button->keyMask) {
					case Key::kA:
						SendAcceptEvent();
						break;
					case Key::kBack:
						Close();
						break;
					default:
						break;
					}
				}
				break;
			default:
				break;
			}
		}

		return true;
	}


	void BirthSignMenu::Open()
	{
		using Message = RE::UIMessage::Message;
		auto ui = RE::UIManager::GetSingleton();
		ui->AddMessage(Name(), Message::kOpen, 0);
	}


	void BirthSignMenu::Close()
	{
		using Message = RE::UIMessage::Message;
		auto ui = RE::UIManager::GetSingleton();
		ui->AddMessage(Name(), Message::kClose, 0);
	}


	void BirthSignMenu::Register()
	{
		auto mm = RE::MenuManager::GetSingleton();
		mm->Register(Name(), Create);

		_MESSAGE("Registered %s", Name().data());
	}


	RE::IMenu* BirthSignMenu::Create()
	{
		return new BirthSignMenu();
	}


	void BirthSignMenu::OnMenuOpen()
	{
		AdjustScale();
		SetInputs();
		view->SetVisible(true);
	}


	void BirthSignMenu::OnMenuClose()
	{
		return;
	}


	void BirthSignMenu::AdjustScale(double a_scale)
	{
		auto def = view->GetMovieDef();
		auto width = def->GetWidth();
		auto height = def->GetHeight();

		RE::GFxValue num;
		bool success = false;

		num.SetNumber(width * a_scale);
		success = view->SetVariable("_root._width", num);
		assert(success);

		num.SetNumber(height * a_scale);
		success = view->SetVariable("_root._height", num);
		assert(success);

		num.SetNumber(width * (1.0 - a_scale) / 2);
		success = view->SetVariable("_root._x", num);
		assert(success);

		num.SetNumber(height * (1.0 - a_scale) / 2);
		success = view->SetVariable("_root._y", num);
		assert(success);
	}


	void BirthSignMenu::SendAcceptEvent()
	{
		RE::GFxValue result;
		bool success = view->Invoke("birthSignList.getCurrentSign", &result, 0, 0);
		assert(success);
		auto id = result.GetUInt();
		if (id != 0) {
			auto handler = OnBirthSignAcceptHandler::GetSingleton();
			handler->QueueEvent(id);
			BirthSignMenu::Close();
		}
	}


	void BirthSignMenu::SetInputs()
	{
		enum
		{
			kAcceptText,
			kAcceptIcon,
			kCancelText,
			kCancelIcon,
			kNumArgs
		};

		UInt32 acceptKey;
		UInt32 cancelKey;

		auto input = RE::InputManager::GetSingleton();
		if (input->IsGamepadEnabled()) {
			using Key = RE::BSWin32GamepadDevice::Key;
			acceptKey = GetGamepadIndex(Key::kA);
			cancelKey = GetGamepadIndex(Key::kBack);
		} else {
			acceptKey = kEnter;
			cancelKey = kESC;
		}

		auto gmst = RE::GameSettingCollection::GetSingleton();
		auto sAccept = gmst->GetSetting("sAccept");
		std::string acceptText = sAccept->GetString();
		auto sCancel = gmst->GetSetting("sCancel");
		std::string cancelText = sCancel->GetString();

		RE::GFxValue args[kNumArgs];
		args[kAcceptText].SetString(acceptText.c_str());
		args[kAcceptIcon].SetNumber(acceptKey);
		args[kCancelText].SetString(cancelText.c_str());
		args[kCancelIcon].SetNumber(cancelKey);
		auto success = view->Invoke("birthSignList.setInputs", 0, args, kNumArgs);
		assert(success);
	}


	UInt32 BirthSignMenu::GetGamepadIndex(RE::BSWin32GamepadDevice::Key a_key)
	{
		using Key = RE::BSWin32GamepadDevice::Key;

		UInt32 index;
		switch (a_key) {
		case Key::kUp:
			index = 0;
			break;
		case Key::kDown:
			index = 1;
			break;
		case Key::kLeft:
			index = 2;
			break;
		case Key::kRight:
			index = 3;
			break;
		case Key::kStart:
			index = 4;
			break;
		case Key::kBack:
			index = 5;
			break;
		case Key::kLeftThumb:
			index = 6;
			break;
		case Key::kRightThumb:
			index = 7;
			break;
		case Key::kLeftShoulder:
			index = 8;
			break;
		case Key::kRightShoulder:
			index = 9;
			break;
		case Key::kA:
			index = 10;
			break;
		case Key::kB:
			index = 11;
			break;
		case Key::kX:
			index = 12;
			break;
		case Key::kY:
			index = 13;
			break;
		case Key::kLeftTrigger:
			index = 14;
			break;
		case Key::kRightTrigger:
			index = 15;
			break;
		default:
			index = kInvalidButton;
			break;
		}

		return index != kInvalidButton ? index + kGamepadOffset : kInvalidButton;
	}
}
