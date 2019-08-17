#pragma once

#include "RE/Skyrim.h"


namespace Events
{
	class HitHandler : public RE::BSTEventSink<RE::TESHitEvent>
	{
	public:
		using EventResult = RE::EventResult;


		static HitHandler* GetSingleton();
		static void Sink();

		virtual	EventResult	ReceiveEvent(RE::TESHitEvent* a_event, RE::BSTEventSource<RE::TESHitEvent>* a_eventSource) override;

	private:
		HitHandler() = default;
		HitHandler(const HitHandler&) = delete;
		HitHandler(HitHandler&&) = delete;
		virtual ~HitHandler() = default;

		HitHandler& operator=(const HitHandler&) = delete;
		HitHandler& operator=(HitHandler&&) = delete;
	};


	void Install();
}
