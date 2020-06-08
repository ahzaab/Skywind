#include "PCH.h"

#include "HudMenuExt.h"

#include <cmath>
#include <cstdint>
#include "SKSE\API.h"

namespace HudMenuExt
{
	void SendNotification(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, RE::BSFixedString a_message) {
        auto task = SKSE::GetTaskInterface();
        task->AddUITask([a_vm, a_stackID, a_message]() {
            enum
            {
                QUEST_UPDATE,
                SKILL_LEVEL_UPDATE,
                PLAYER_LEVEL_UPDATE,
                SHOUT_UPDATE
            };

            enum
            {
                kNotificationText,
                kStatus,
                kSoundID,
                kObjectiveCount,
                kNotificationType,
                kLevel,
                kStartPercent,
                kEndPercent,
                kDragonText
            };

            auto ui = RE::UI::GetSingleton();
            auto hud = ui->GetMenu<RE::HUDMenu>();

            if (!hud) {
                a_vm->TraceStack("Hud menu is not open!", a_stackID, Serverity::kWarning);
                return;
            }

            auto view = hud ? hud->view : nullptr;
            auto delegate = hud ? hud->fxDelegate : nullptr;
            if (view && delegate) {
                RE::FxResponseArgsEx<9> args;
                args[kNotificationText] = a_message.c_str();
                args[kStatus] = "";
                args[kSoundID] = "";
                args[kObjectiveCount] = 0;
                args[kNotificationType] = SKILL_LEVEL_UPDATE;
                args[kLevel] = 42;
                args[kStartPercent] = 0.20;
                args[kEndPercent] = 0.80;
                args[kDragonText] = "";

                delegate->Invoke(view.get(), "ShowNotification", args);
            }
            });
	}


	bool RegisterFuncs(VM* a_vm)
	{
        a_vm->RegisterFunction("SendNotification", "Skywind", SendNotification, true);

		return true;
	}
}