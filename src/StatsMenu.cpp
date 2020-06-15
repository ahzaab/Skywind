#include "PCH.h"
#include "statsmenu.h"
#include "statsmenuex.h"

namespace Scaleform
{
	namespace StatsMenu {
		constexpr  REL::ID Vtbl(static_cast<std::uint64_t>(269955));
	}
		//static RE::GPtr<RE::StatsMenu> s_statsMenu;

		//REL::Function<ProcessMessage_t> ProcessMessage(REL::ID(51638));

		//REL::Function<Accept_t> Accept(REL::ID(51640));

		//8C0840    51640
	 
	    // 8C0900 MoveCamera, 51641
		// 8C0930 SetFadedIn, 51642


		void StatsMenuHook::Hook_Accept(RE::FxDelegateHandler::CallbackProcessor* a_cbReg)
		{
			if (StatsMenuEx::GetActive())
			{
				fxDelegate->callbacks.Clear();
				return;
			}
			

			//if (!s_statsMenu || !s_statsMenu.get())
			//	return;

			_Accept(this, a_cbReg);

		}

		RE::UI_MESSAGE_RESULTS StatsMenuHook::Hook_ProcessMessage(RE::UIMessage& a_message)
		{
			if (StatsMenuEx::GetActive())
			{
				return RE::UI_MESSAGE_RESULTS::kHandled;
			}
				
			return _ProcessMessage(this, a_message);
		}

		void StatsMenuHook::InstallHooks()
		{

			// Override to prevent the Native StatsMenu from doing anything.
			REL::Offset<std::uintptr_t> vTable(Scaleform::StatsMenu::Vtbl);
			_Accept = vTable.write_vfunc(0x1, &StatsMenuHook::Hook_Accept);
			_ProcessMessage = vTable.write_vfunc(0x4, &StatsMenuHook::Hook_ProcessMessage);
			_MESSAGE("Installed hooks");
		}
}

