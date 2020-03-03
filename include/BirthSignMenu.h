#pragma once

#include <string_view>

#include "RE/Skyrim.h"
#include "SKSE/API.h"


namespace Scaleform
{
	class BirthSignMenu :
		public RE::IMenu,
		public RE::MenuEventHandler
	{
	public:
		using MenuBase = RE::IMenu;
		using HandlerBase = RE::MenuEventHandler;
		using Result = RE::UI_MESSAGE_RESULTS;
		using GRefCountBaseStatImpl::operator new;
		using GRefCountBaseStatImpl::operator delete;


		BirthSignMenu();
		virtual ~BirthSignMenu();

		// IMenu
		virtual Result ProcessMessage(RE::UIMessage& a_message) override;

		// MenuEventHandler
		virtual bool CanProcess(RE::InputEvent* a_event) override;
		virtual bool ProcessButton(RE::ButtonEvent* a_event) override;

		static void Open();
		static void Close();

		static constexpr std::string_view Name();

		static void Register();
		static RE::IMenu* Create();

	private:
		enum : UInt32
		{
			kInvalidButton = static_cast<UInt32>(-1),

			kESC = 1,
			kEnter = 28,

			kKeyboardOffset = 0,
			kMouseOffset = 256,
			kGamepadOffset = 266,
			kPS3Offset = 302
		};


		enum class ID : UInt32
		{
			kNone = 0,
			kApprentice,
			kAtronach,
			kLady,
			kLord,
			kLover,
			kMage,
			kRitual,
			kSerpent,
			kShadow,
			kSteed,
			kThief,
			kTower,
			kWarrior
		};


		void OnMenuOpen();
		void OnMenuClose();

		void AdjustScale(double a_scale = 0.9);
		void SendAcceptEvent();
		void SetInputs();
		UInt32 GetGamepadIndex(RE::BSWin32GamepadDevice::Key a_key);


		static constexpr char SWF_NAME[] = "BirthSignMenu";

		RE::GFxValue _root;
	};


	constexpr std::string_view BirthSignMenu::Name()
	{
		return "BirthSignMenu";
	}
}
