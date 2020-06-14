#include "PCH.h"
#include "StatsMenuHook.h"
#include <CommonLibSSE\include\SKSE\API.h>


namespace Scaleform
{
	//269955
	constexpr  REL::ID StatsMenuHook::Vtbl(static_cast<std::uint64_t>(269955));

	StatsMenuHook* StatsMenuHook::GetSingleton()
	{
		static StatsMenuHook singleton;
		return std::addressof(singleton);
	}

	void StatsMenuHook::RegisterEvents()
	{
		auto input = RE::BSInputDeviceManager::GetSingleton();
		input->AddEventSink(static_cast<RE::BSTEventSink<RE::InputEvent*>*>(StatsMenuHook::GetSingleton()));
		auto ui = RE::UI::GetSingleton();
		ui->AddEventSink(static_cast<RE::BSTEventSink<RE::MenuOpenCloseEvent>*>(StatsMenuHook::GetSingleton()));
		auto cam = SKSE::GetCameraEventSource();
		cam->AddEventSink(static_cast<RE::BSTEventSink<SKSE::CameraEvent>*>(StatsMenuHook::GetSingleton()));

		//RE::FxDelegate::RegisterHandler(StatsMenuHook::GetSingleton());

	}

	void Process(const RE::GString& a_methodName, RE::FxDelegateHandler::CallbackFn* a_method)
	{
		(void)a_methodName;
		(void)a_method;
	}

	RE::BSEventNotifyControl StatsMenuHook::ProcessEvent(RE::MenuOpenCloseEvent const* a_event, [[maybe_unused]] RE::BSTEventSource<RE::MenuOpenCloseEvent>* a_eventSource)
	{
		if (!a_event) {
			return RE::BSEventNotifyControl::kContinue;
		}
		if (a_event->menuName == RE::StatsMenu::MENU_NAME && a_event->opening)
		{
			Scaleform::StatsMenuEx::Open();
			auto uiQueue = RE::UIMessageQueue::GetSingleton();
			uiQueue->AddMessage(RE::FaderMenu::MENU_NAME, RE::UI_MESSAGE_TYPE::kHide, 0);
			uiQueue->AddMessage(RE::StatsMenu::MENU_NAME, RE::UI_MESSAGE_TYPE::kHide, 0);
		}
		return RE::BSEventNotifyControl::kContinue;
	}


	RE::BSEventNotifyControl StatsMenuHook::ProcessEvent(RE::InputEvent* const* a_event, [[maybe_unused]] RE::BSTEventSource<RE::InputEvent*>* a_eventSource)
	{
		using Device = RE::INPUT_DEVICE;
		using Input = RE::INPUT_EVENT_TYPE;
		using Key = RE::BSKeyboardDevice::Key;
		using Message = RE::UI_MESSAGE_TYPE;

		if (!a_event) {
			return RE::BSEventNotifyControl::kContinue;
		}

		auto ui = RE::UI::GetSingleton();
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
				}
			}
		}
		return RE::BSEventNotifyControl::kContinue;
	}

	RE::BSEventNotifyControl StatsMenuHook::ProcessEvent(SKSE::CameraEvent const* a_event, [[maybe_unused]] RE::BSTEventSource<SKSE::CameraEvent>* a_eventSource)
	{
		(void)a_event;
		(void)a_eventSource;
		return RE::BSEventNotifyControl::kContinue;
	}

	void StatsMenuHook::Hook_Accept(RE::FxDelegateHandler::CallbackProcessor* a_cbReg)
	{
		(void)a_cbReg;
	}

	RE::UI_MESSAGE_RESULTS StatsMenuHook::Hook_ProcessMessage(RE::UIMessage& a_message)
	{
		(void)a_message;
		return RE::UI_MESSAGE_RESULTS::kHandled;
	}

	void StatsMenuHook::InstallHooks()
	{
		// Override to prevent the Native StatsMenu from doing anything.
		REL::Offset<std::uintptr_t> vTable(StatsMenuHook::Vtbl);
		vTable.write_vfunc(0x1, &StatsMenuHook::Hook_Accept);
		vTable.write_vfunc(0x4, &StatsMenuHook::Hook_ProcessMessage);
		_MESSAGE("Installed hooks");
	}
}