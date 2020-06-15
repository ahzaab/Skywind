#pragma once
#include "RE/Skyrim.h"

namespace Scaleform
{
	class StatsMenuHook : public RE::StatsMenu {
		//using ProcessMessage_t = decltype(&RE::StatsMenu::ProcessMessage);
		//extern REL::Function<ProcessMessage_t> ProcessMessage;

		//using Accept_t = decltype(&RE::StatsMenu::Accept);
		//extern REL::Function<Accept_t> Accept;
		//extern RE::GPtr<RE::StatsMenu> s_statsMenu;
	public:
		static void InstallHooks();

	private:
		void Hook_Accept(RE::FxDelegateHandler::CallbackProcessor* a_cbReg);
		RE::UI_MESSAGE_RESULTS Hook_ProcessMessage(RE::UIMessage& a_message);

		using Accept_t = decltype(&RE::StatsMenu::Accept);
		using ProcessMessage_t = decltype(&RE::StatsMenu::ProcessMessage);

		static inline REL::Function<Accept_t> _Accept;
		static inline REL::Function<ProcessMessage_t> _ProcessMessage;

	};
}