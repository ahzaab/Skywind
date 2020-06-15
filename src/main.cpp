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
#include "StatsMenu.h"
#include "StatsMenuEx.h"
// TODO

#include "RE/Skyrim.h"
#include "SKSE/API.h"


namespace
{
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

			if (!a_event) {
				return EventResult::kContinue;
			}

			auto ui = RE::UI::GetSingleton();
			auto intfcStr = RE::InterfaceStrings::GetSingleton();
			if (ui->IsMenuOpen(intfcStr->console)) {
				return EventResult::kContinue;
			}

			for (auto event = *a_event; event; event = event->next) {
				if (event->eventType != Input::kButton) {
					continue;
				}

				auto button = static_cast<RE::ButtonEvent*>(event);
				if (!button->IsDown() || button->device != Device::kKeyboard) {
					continue;
				}

				switch (button->idCode) {
				case Key::kF1:
					Scaleform::StatsMenuEx::Open();
					break;
				case Key::kF2:
					Scaleform::StatsMenuEx::Close();
					break;
				case Key::kNum8:
					Scaleform::MeterMenu::TweenTo(80);
					break;
				case Key::kNum7:
					Scaleform::MeterMenu::TweenTo(70);
					break;
				case Key::kNum6:
					Scaleform::MeterMenu::TweenTo(60);
					break;
				case Key::kNum5:
					Scaleform::MeterMenu::TweenTo(50);
					break;
				case Key::kNum4:
					Scaleform::MeterMenu::TweenTo(40);
					break;
				case Key::kNum3:
					Scaleform::MeterMenu::TweenTo(30);
					break;
				case Key::kNum2:
					Scaleform::MeterMenu::TweenTo(20);
					break;
				case Key::kNum1:
					Scaleform::MeterMenu::TweenTo(10);
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

				[[maybe_unused]] auto input = RE::BSInputDeviceManager::GetSingleton();
				//input->AddEventSink(InputEventHandler::GetSingleton());

				Scaleform::RegisterCreators();
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

		//auto v  = REL::IDDatabase::OffsetToID(0x8C0840);

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

		Patches::Install();
		Scaleform::RegisterCallbacks();
		Scaleform::StatsMenuHook::InstallHooks();

		return true;
	}
};
