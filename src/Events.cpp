#include "PCH.h"
#include "Events.h"
#include "StatsMenuEx.h"

#include <memory>

namespace Events
{
	HitHandler* HitHandler::GetSingleton()
	{
		static HitHandler singleton;
		return std::addressof(singleton);
	}


	void HitHandler::Sink()
	{
		auto source = RE::ScriptEventSourceHolder::GetSingleton();
		source->AddEventSink(GetSingleton());
		_MESSAGE("Sinked HitHandler");
	}


	auto HitHandler::ProcessEvent(const RE::TESHitEvent* a_event, [[maybe_unused]] RE::BSTEventSource<RE::TESHitEvent>* a_eventSource)
		-> EventResult
	{
		if (!a_event || !a_event->cause || !a_event->cause->IsPlayerRef() || !a_event->target || a_event->target->IsNot(RE::FormType::ActorCharacter)) {
			return EventResult::kContinue;
		}

		auto player = static_cast<RE::PlayerCharacter*>(a_event->cause.get());
		auto hand = player->GetAttackingWeapon();
		bool useLeftHand = hand == player->GetEquippedEntryData(true);
		if (!hand) {
			return EventResult::kContinue;
		}

		RE::ExtraHealth* xHealth = 0;
		if (hand->extraLists) {
			auto& xLists = *hand->extraLists;
			for (auto& xList : xLists) {
				xHealth = xList->GetByType<RE::ExtraHealth>();
				if (xHealth) {
					break;
				}
			}
		} else {
			hand->extraLists = new RE::BSSimpleList<RE::ExtraDataList*>();
		}

		auto gmst = RE::GameSettingCollection::GetSingleton();
		auto fHealthDataValue6 = gmst->GetSetting("fHealthDataValue6");
		auto healthMin = 1.0;
		auto healthAbsMax = fHealthDataValue6->GetFloat();
		auto healthRelMax = healthAbsMax - healthMin;

		if (!xHealth) {
			auto& xLists = *hand->extraLists;
			if (xLists.empty()) {
				auto xList = new RE::ExtraDataList();
				xLists.push_front(xList);
			}

			auto xList = xLists.front();
			xHealth = new RE::ExtraHealth(healthAbsMax);
			xList->Add(xHealth);
		}

		if (xHealth->health > healthMin) {
			xHealth->health -= healthRelMax * 0.05f;
			if (xHealth->health < static_cast<float>(healthMin)) {
				xHealth->health = static_cast<float>(healthMin);
			}
		}

		auto healthPct = ((xHealth->health - healthMin) / healthRelMax) * 100.0;
		_DMESSAGE("%s hand health: %%%.2f (%.2f)", (useLeftHand ? "Left" : "Right"), healthPct, xHealth->health);

		return EventResult::kContinue;
	}



	MenuAndInputHander* MenuAndInputHander::GetSingleton()
	{
		static MenuAndInputHander singleton;
		return std::addressof(singleton);
	}

	void MenuAndInputHander::Sink()
	{
		auto input = RE::BSInputDeviceManager::GetSingleton();
		input->AddEventSink(static_cast<RE::BSTEventSink<RE::InputEvent*>*>(MenuAndInputHander::GetSingleton()));
		auto ui = RE::UI::GetSingleton();
		ui->AddEventSink(static_cast<RE::BSTEventSink<RE::MenuOpenCloseEvent>*>(MenuAndInputHander::GetSingleton()));
	}

	RE::BSEventNotifyControl MenuAndInputHander::ProcessEvent(RE::MenuOpenCloseEvent const* a_event, [[maybe_unused]] RE::BSTEventSource<RE::MenuOpenCloseEvent>* a_eventSource)
	{
		if (!a_event) {
			return RE::BSEventNotifyControl::kContinue;
		}
		if (a_event->menuName == RE::StatsMenu::MENU_NAME && a_event->opening)
		{
			Scaleform::StatsMenuEx::Open();
		}
		return RE::BSEventNotifyControl::kContinue;
	}

	RE::BSEventNotifyControl MenuAndInputHander::ProcessEvent(RE::InputEvent* const* a_event, [[maybe_unused]] RE::BSTEventSource<RE::InputEvent*>* a_eventSource)
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

			if (button->IsDown() && button->device == Device::kKeyboard && button->idCode == Key::kF1)
			{
				auto active = Scaleform::StatsMenuEx::GetActive();
				Scaleform::StatsMenuEx::SetActive(!active);
			}

			if (button->IsDown() && button->userEvent.length() && (button->userEvent == "Quick Stats" || button->userEvent == "Cancel"))
			{
				if (ui->IsMenuOpen(Scaleform::StatsMenuEx::Name()))
				{
					Scaleform::StatsMenuEx::Close();
				}
			}	
		}
		return RE::BSEventNotifyControl::kContinue;
	}

	void Install()
	{
		//HitHandler::Sink();
		MenuAndInputHander::Sink();
		_MESSAGE("Installed all event sinks");
	}
}
