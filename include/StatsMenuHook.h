#pragma once
#include "RE/Skyrim.h"
#include "StatsMenuEx.h"
#include <cstdint>
#include "SKSE/API.h"

namespace Scaleform
{
	class StatsMenuHook: public RE::BSTEventSink<RE::MenuOpenCloseEvent>, public RE::BSTEventSink<RE::InputEvent*>, public RE::BSTEventSink<SKSE::CameraEvent>
	{
	public:
		
		static StatsMenuHook* GetSingleton();
		static void InstallHooks();
		static void RegisterEvents();
		virtual	RE::BSEventNotifyControl ProcessEvent(RE::MenuOpenCloseEvent const* a_event, [[maybe_unused]] RE::BSTEventSource<RE::MenuOpenCloseEvent>* a_eventSource) override;
		virtual	RE::BSEventNotifyControl ProcessEvent(RE::InputEvent* const* a_event, [[maybe_unused]] RE::BSTEventSource<RE::InputEvent*>* a_eventSource) override;
		virtual	RE::BSEventNotifyControl ProcessEvent(SKSE::CameraEvent const* a_event, [[maybe_unused]] RE::BSTEventSource<SKSE::CameraEvent>* a_eventSource) override;
		//virtual void Process(const RE::GString& a_methodName, RE::FxDelegateHandler::CallbackFn* a_method);
	private:
		static const REL::ID Vtbl;
		static void Hook_Accept(RE::FxDelegateHandler::CallbackProcessor* a_cbReg);
		static RE::UI_MESSAGE_RESULTS Hook_ProcessMessage(RE::UIMessage& a_message);
	};
}