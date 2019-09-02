#include "skse64_common/BranchTrampoline.h"
#include "skse64_common/skse_version.h"

#include "Events.h"
#include "Patches.h"
#include "Scaleform.h"
#include "version.h"

// TODO
#include "BirthSignMenu.h"
#include "MeterMenu.h"
#include "SpellmakingMenu.h"
#include "StatsMenuEx.h"
// TODO

#include "RE/Skyrim.h"
#include "SKSE/API.h"


namespace
{
	class InputEventHandler : public RE::BSTEventSink<RE::InputEvent*>
	{
	public:
		using EventResult = RE::EventResult;


		static InputEventHandler* GetSingleton()
		{
			static InputEventHandler singleton;
			return &singleton;
		}


		virtual	EventResult ReceiveEvent(RE::InputEvent** a_event, RE::BSTEventSource<RE::InputEvent*>* a_eventSource) override
		{
			using Key = RE::BSKeyboardDevice::Key;
			using Message = RE::UIMessage::Message;

			if (!a_event) {
				return EventResult::kContinue;
			}

			auto mm = RE::MenuManager::GetSingleton();
			auto uiStr = RE::UIStringHolder::GetSingleton();
			if (mm->IsMenuOpen(uiStr->console)) {
				return EventResult::kContinue;
			}

			for (auto event = *a_event; event; event = event->next) {
				if (event->eventType != RE::InputEvent::EventType::kButton) {
					continue;
				}

				auto button = static_cast<RE::ButtonEvent*>(event);
				if (!button->IsDown() || button->deviceType != RE::DeviceType::kKeyboard) {
					continue;
				}

				switch (button->keyMask) {
				case Key::kNum0:
					Scaleform::StatsMenuEx::Open();
					break;
				case Key::kNum9:
					Scaleform::StatsMenuEx::Close();
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

				auto input = RE::InputManager::GetSingleton();
				input->AddEventSink(InputEventHandler::GetSingleton());

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

		switch (a_skse->RuntimeVersion()) {
		case RUNTIME_VERSION_1_5_73:
		case RUNTIME_VERSION_1_5_80:
			break;
		default:
			_FATALERROR("Unsupported runtime version %08X!\n", a_skse->RuntimeVersion());
			return false;
		}

		return true;
	}


	bool SKSEPlugin_Load(const SKSE::LoadInterface* a_skse)
	{
		_MESSAGE("Skywind loaded");

		if (!SKSE::Init(a_skse)) {
			return false;
		}

		if (!SKSE::AllocBranchTrampoline(1024 * 1)) {
			return false;
		}

		auto messaging = SKSE::GetMessagingInterface();
		if (!messaging->RegisterListener("SKSE", MessageHandler)) {
			return false;
		}

		Patches::Install();
		Scaleform::RegisterCallbacks();

		return true;
	}
};
