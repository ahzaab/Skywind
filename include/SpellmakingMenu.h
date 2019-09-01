#pragma once

#include <optional>
#include <string>
#include <string_view>
#include <utility>
#include <vector>

#include "RE/Skyrim.h"

#include "CLIK/Button.h"
#include "CLIK/DropdownMenu.h"
#include "CLIK/ScrollingList.h"
#include "CLIK/Slider.h"
#include "CLIK/TextField.h"
#include "CLIK/TextInput.h"


namespace Scaleform
{
	namespace
	{
		struct Available
		{
			std::string text;
			RE::FormID effectID;
		};


		struct Selected
		{
			std::string text;
			RE::FormID effectID;
			UInt32 magnitude;
			UInt32 area;
			UInt32 duration;
		};


		struct Range
		{
			CLIK::TextField header;
			CLIK::GFx::Controls::DropdownMenu dropdown;
		};


		struct RangeElem
		{
			using Type = RE::MagicItem::TargetType;

			std::string text;
			Type type;
		};


		struct Slider
		{
		public:
			Slider();
			~Slider() = default;

			void UpdateText();
			void SetDragging(bool a_isDragging);
			bool IsDragging() const;

			CLIK::TextField header;
			CLIK::GFx::Controls::Slider slider;
			CLIK::TextField text;

		private:
			bool _isDragging;
		};
	}


	class SpellmakingMenu : public RE::IMenu
	{
	public:
		using Base = RE::IMenu;
		using Result = Base::Result;


		SpellmakingMenu();
		virtual ~SpellmakingMenu() = default;

		// IMenu
		virtual void Accept(RE::FxDelegateHandler::CallbackProcessor* a_processor) override;
		virtual Result ProcessMessage(RE::UIMessage* a_message) override;
		virtual void NextFrame(float a_arg1, UInt32 a_currentTime) override;

		static void Open();
		static void Close();

		static constexpr std::string_view Name();

		static void Register();
		static RE::IMenu* Create();

	private:
		enum : std::size_t { kInvalid = static_cast<std::size_t>(-1) };


		static void Log(const RE::FxDelegateArgs& a_params);
		static void OnAvailablePress(const RE::FxDelegateArgs& a_params);
		static void OnSelectedPress(const RE::FxDelegateArgs& a_params);
		static void OnMagnitudeDragBegin(const RE::FxDelegateArgs& a_params);
		static void OnMagnitudeDragEnd(const RE::FxDelegateArgs& a_params);
		static void OnMagnitudeChange(const RE::FxDelegateArgs& a_params);
		static void OnDurationDragBegin(const RE::FxDelegateArgs& a_params);
		static void OnDurationDragEnd(const RE::FxDelegateArgs& a_params);
		static void OnDurationChange(const RE::FxDelegateArgs& a_params);
		static void OnAreaDragBegin(const RE::FxDelegateArgs& a_params);
		static void OnAreaDragEnd(const RE::FxDelegateArgs& a_params);
		static void OnAreaChange(const RE::FxDelegateArgs& a_params);
		static void CraftSpell(const RE::FxDelegateArgs& a_params);

		void OnMenuOpen();
		void OnMenuClose();

		void InitExtensions();
		void InitEffectInfo();
		void SetAvailable();
		void SetEffectInfo();
		void CommitSelection();
		void CraftSpell();

		bool OnAvailablePress(std::size_t a_availIdx);
		bool OnSelectedPress(std::size_t a_selectedIdx, bool a_remove);
		bool OnMagnitudeDragChange(bool a_isDragging);
		bool OnMagnitudeChange();
		bool OnDurationDragChange(bool a_isDragging);
		bool OnDurationChange();
		bool OnAreaDragChange(bool a_isDragging);
		bool OnAreaChange();

		void SanitizeString(std::string& a_str);


		static constexpr char SWF_NAME[] = "SpellmakingMenu";
		static constexpr std::size_t MAX_EFFECTS = 4;

		CLIK::GFx::Controls::ScrollingList _available;
		CLIK::GFx::Controls::ScrollingList _selected;
		Range _range;
		Slider _magnitude;
		Slider _duration;
		Slider _area;
		CLIK::GFx::Controls::TextInput _name;
		CLIK::GFx::Controls::Button _craft;
		std::vector<Available> _availableMappings;
		std::vector<Selected> _selectedMappings;
		std::vector<RangeElem> _rangeMappings;
		std::size_t _selectedIdx;
	};


	constexpr std::string_view SpellmakingMenu::Name()
	{
		return "SpellmakingMenu";
	}
}
