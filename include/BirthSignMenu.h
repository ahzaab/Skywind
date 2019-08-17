#pragma once

#include <string_view>

#include "Delegates.h"

#include "RE/Skyrim.h"
#include "SKSE/API.h"


class BirthSignMenu :
	public RE::IMenu,
	public RE::MenuEventHandler
{
public:
	using Result = RE::IMenu::Result;
	using GRefCountBaseStatImpl::operator new;
	using GRefCountBaseStatImpl::operator delete;


	BirthSignMenu();
	virtual ~BirthSignMenu();

	// IMenu
	virtual Result ProcessMessage(RE::UIMessage* a_message) override;

	// MenuEventHandler
	virtual bool CanProcess(RE::InputEvent* a_event) override;
	virtual bool ProcessButton(RE::ButtonEvent* a_event) override;

	static constexpr std::string_view Name();
	static void Open();
	static void Close();

private:
	using MenuBase = RE::IMenu;
	using HandlerBase = RE::MenuEventHandler;


	enum : UInt32
	{
		kInvalidButton = RE::InputMappingManager::kInvalid,

		kESC = 1,
		kEnter = 28,

		kKeyboardOffset = 0,
		kMouseOffset = 256,
		kGamepadOffset = 266,
		kPS3Offset = 302
	};


	enum class ID: UInt32
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


	void OnOpen();
	void OnClose();
	void SendAcceptEvent();
	void SetVisible(bool a_visible);
	void SetInputs();
	UInt32 GetGamepadIndex(RE::BSWin32GamepadDevice::Key a_key);

	template <class Delegate, class... Args>
	void Dispatch(Args&&... a_args)
	{
		auto task = SKSE::GetTaskInterface();
		task->AddUITask(new Delegate(view, std::forward<Args>(a_args)...));
	}


	static constexpr char SWF_NAME[] = "BirthSignMenu";
};


constexpr std::string_view BirthSignMenu::Name()
{
	return "BirthSignMenu";
}
