#include "Events.h"


namespace Events
{
	HitHandler* HitHandler::GetSingleton()
	{
		static HitHandler singleton;
		return &singleton;
	}


	void HitHandler::Sink()
	{
		auto source = RE::ScriptEventSourceHolder::GetSingleton();
		source->AddEventSink(GetSingleton());
		_MESSAGE("Sinked HitHandler");
	}


	auto HitHandler::ReceiveEvent(RE::TESHitEvent* a_event, RE::BSTEventSource<RE::TESHitEvent>* a_eventSource)
		-> EventResult
	{
		if (!a_event || !a_event->aggressor || !a_event->aggressor->IsPlayerRef() || !a_event->target || a_event->target->IsNot(RE::FormType::ActorCharacter)) {
			return EventResult::kContinue;
		}

		auto player = static_cast<RE::PlayerCharacter*>(a_event->aggressor.get());
		auto hand = player->GetAttackingWeapon();
		bool useLeftHand = hand == player->GetEquippedEntryData(true);
		if (!hand) {
			return EventResult::kContinue;
		}

		RE::ExtraHealth* xHealth = 0;
		if (hand->extraList) {
			auto& xLists = *hand->extraList;
			for (auto& xList : xLists) {
				xHealth = xList->GetByType<RE::ExtraHealth>();
				if (xHealth) {
					break;
				}
			}
		} else {
			hand->extraList = new RE::BSSimpleList<RE::BaseExtraList*>();
		}

		auto gmst = RE::GameSettingCollection::GetSingleton();
		auto fHealthDataValue6 = gmst->GetSetting("fHealthDataValue6");
		auto healthMin = 1.0;
		auto healthAbsMax = fHealthDataValue6->GetFloat();
		auto healthRelMax = healthAbsMax - healthMin;

		if (!xHealth) {
			auto& xLists = *hand->extraList;
			if (xLists.empty()) {
				auto xList = new RE::BaseExtraList();
				xLists.push_front(xList);
			}

			auto xList = xLists.front();
			xHealth = new RE::ExtraHealth(healthAbsMax);
			xList->Add(xHealth);
		}
		
		if (xHealth->health > healthMin) {
			xHealth->health -= healthRelMax * 0.05;
			if (xHealth->health < healthMin) {
				xHealth->health = healthMin;
			}
		}

		auto healthPct = ((xHealth->health - healthMin) / healthRelMax) * 100.0;
		_DMESSAGE("%s hand health: %%%.2f (%.2f)", (useLeftHand ? "Left" : "Right"), healthPct, xHealth->health);

		return EventResult::kContinue;
	}


	void Install()
	{
		//HitHandler::Sink();
		_MESSAGE("Installed all event sinks");
	}
}
