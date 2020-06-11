#include "PCH.h"
#include <memory>

#include "Events.h"
#include "Patches.h"
#include "Scaleform.h"
#include "version.h"

// TODO
#include "BirthSignMenu.h"
#include "MeterMenu.h"
#include "SpellmakingMenu.h"
#include "StatsMenuEx.h"
#include "Papyrus.h"
// TODO

#include "SKSE/API.h"

namespace
{
	static bool isMenuOpen = false;

	class MenuEventHandler : public RE::BSTEventSink<RE::MenuOpenCloseEvent>
	{
	public:
		using EventResult = RE::BSEventNotifyControl;

		static MenuEventHandler* GetSingleton()
		{
			static MenuEventHandler singleton;
			return std::addressof(singleton);
		}

		virtual	EventResult ProcessEvent(RE::MenuOpenCloseEvent const* a_event, [[maybe_unused]] RE::BSTEventSource<RE::MenuOpenCloseEvent>* a_eventSource) override
		{
			if (!a_event) {
				return EventResult::kContinue;
			}

			
			if (a_event->menuName == RE::StatsMenu::MENU_NAME && a_event->opening)
			{
				auto ui = RE::UI::GetSingleton();
				auto stats = ui->GetMenu<RE::StatsMenu>();

				auto t = REL::IDDatabase::OffsetToID(0x16BC118);

				auto t2 = REL::IDDatabase::IDToOffset(271141);
				//23878952
				auto uiQueue = RE::UIMessageQueue::GetSingleton();
				uiQueue->AddMessage(RE::StatsMenu::MENU_NAME, RE::UI_MESSAGE_TYPE::kHide, 0);
				uiQueue->AddMessage(RE::TweenMenu::MENU_NAME, RE::UI_MESSAGE_TYPE::kHide, 0);
				uiQueue->AddMessage(RE::FaderMenu::MENU_NAME, RE::UI_MESSAGE_TYPE::kHide, 0);
				Scaleform::StatsMenuEx::Open();
				return EventResult::kStop;
			}


			return EventResult::kContinue;


		}

	private:
		MenuEventHandler() = default;
		MenuEventHandler(const MenuEventHandler&) = delete;
		MenuEventHandler(MenuEventHandler&&) = delete;
		virtual ~MenuEventHandler() = default;

		MenuEventHandler& operator=(const MenuEventHandler&) = delete;
		MenuEventHandler& operator=(MenuEventHandler&&) = delete;

	};


	class InputEventHandler : public RE::BSTEventSink<RE::InputEvent*>
	{
	public:
		using EventResult = RE::BSEventNotifyControl;

		

		static InputEventHandler* GetSingleton()
		{
			static InputEventHandler singleton;
			return std::addressof(singleton);
		}


		virtual	EventResult ProcessEvent(RE::InputEvent* const* a_event, [[maybe_unused]] RE::BSTEventSource<RE::InputEvent*>* a_eventSource) override
		{
			using Device = RE::INPUT_DEVICE;
			using Input = RE::INPUT_EVENT_TYPE;
			using Key = RE::BSKeyboardDevice::Key;
			using Message = RE::UI_MESSAGE_TYPE;


			//std::string_view ControlMap::GetUserEventName(UInt32 a_buttonID, INPUT_DEVICE a_device, InputContextID a_context) const


			if (!a_event) {
				return EventResult::kContinue;
			}
			
			auto ui = RE::UI::GetSingleton();
			auto intfcStr = RE::InterfaceStrings::GetSingleton();
			if (ui->IsMenuOpen(intfcStr->console)) {
				return EventResult::kContinue;
			}

			for (auto event = *a_event; event; event = event->next) {
				auto button = static_cast<RE::ButtonEvent*>(event);
				if (event->eventType != Input::kButton) {
					continue;
				}

				if (ui->IsMenuOpen(Scaleform::StatsMenuEx::Name()))
				{
					if (button->IsDown() && button->userEvent.length() && (button->userEvent == "Quick Stats" || button->userEvent == "Cancel"))
					{
						Scaleform::StatsMenuEx::Close();

						if (ui->IsMenuOpen(intfcStr->tweenMenu)) {
							auto uiQueue = RE::UIMessageQueue::GetSingleton();
							uiQueue->AddMessage(RE::TweenMenu::MENU_NAME, RE::UI_MESSAGE_TYPE::kHide, 0);
							uiQueue->AddMessage(RE::FaderMenu::MENU_NAME, RE::UI_MESSAGE_TYPE::kHide, 0);
						}

					}
				}

				//{_data = 0x000001e52d4672d8 "Quick Stats" }
				//auto userEvent = button->userEvent.c_str();

				// contolMap = RE::ControlMap::GetSingleton();
				///contolMap->GetUserEventName(button->idCode, button->device, button->)

				


				
				if (!button->IsDown() || button->device != Device::kKeyboard) {
					continue;
				}

				switch (button->idCode) {
				case Key::kF1:
					if (!isMenuOpen)
					{
						isMenuOpen = true;
						Scaleform::StatsMenuEx::Open();
					}
					else
					{
						isMenuOpen = false;
						Scaleform::StatsMenuEx::Close();
					}
					break;
				default:
					break;
				}
			}

			return EventResult::kContinue;
		}

	private:
		InputEventHandler() = default;
		InputEventHandler(const InputEventHandler&) = delete;
		InputEventHandler(InputEventHandler&&) = delete;
		virtual ~InputEventHandler() = default;

		InputEventHandler& operator=(const InputEventHandler&) = delete;
		InputEventHandler& operator=(InputEventHandler&&) = delete;
	};


	void MessageHandler(SKSE::MessagingInterface::Message* a_msg)
	{
		switch (a_msg->type) {
		case SKSE::MessagingInterface::kDataLoaded:
			{
				Events::Install();

				auto input = RE::BSInputDeviceManager::GetSingleton();
				input->AddEventSink(InputEventHandler::GetSingleton());
				Scaleform::RegisterCreators();

				auto ui = RE::UI::GetSingleton();
				ui->AddEventSink(MenuEventHandler::GetSingleton());

			}
			break;
		}
	}
}


extern "C" {
	bool SKSEPlugin_Query(const SKSE::QueryInterface* a_skse, SKSE::PluginInfo* a_info)
	{
		SKSE::Logger::OpenRelative(FOLDERID_Documents, L"\\My Games\\Skyrim Special Edition\\SKSE\\Skywind.log");
		SKSE::Logger::SetPrintLevel(SKSE::Logger::Level::kDebugMessage);
		SKSE::Logger::SetFlushLevel(SKSE::Logger::Level::kDebugMessage);
		SKSE::Logger::UseLogStamp(true);
		SKSE::Logger::TrackTrampolineStats(true);
		SKSE::Logger::HookPapyrusLog(true);
		SKSE::Logger::SetPapyrusLogFilter("BirthSign");

		_MESSAGE("Skywind v%s", SWND_VERSION_VERSTRING);

		a_info->infoVersion = SKSE::PluginInfo::kVersion;
		a_info->name = "Skywind";
		a_info->version = SWND_VERSION_MAJOR;

		if (a_skse->IsEditor()) {
			_FATALERROR("Loaded in editor, marking as incompatible!\n");
			return false;
		}

		auto ver = a_skse->RuntimeVersion();
		if (ver <= SKSE::RUNTIME_1_5_39) {
			_FATALERROR("Unsupported runtime version %s!", ver.GetString().c_str());
			return false;
		}

		return true;
	}


	bool SKSEPlugin_Load(const SKSE::LoadInterface* a_skse)
	{
		_MESSAGE("Skywind loaded");

		//while (!IsDebuggerPresent())
		//{
		//   Sleep(10);
		//}

		//Sleep(1000 * 2);

		if (!SKSE::Init(a_skse)) {
			return false;
		}

		if (!SKSE::AllocTrampoline(1 << 10)) {
			return false;
		}

		auto messaging = SKSE::GetMessagingInterface();
		if (!messaging->RegisterListener("SKSE", MessageHandler)) {
			return false;
		}


		auto papyrus = SKSE::GetPapyrusInterface();
		if (!papyrus){
			return false;
		}

		Papyrus::Register();

		Patches::Install();

		Scaleform::StatsMenuEx::InstallHooks();

		Scaleform::RegisterCallbacks();

		return true;
	}
};
