#pragma once

#include <optional>
#include <string>
#include <string_view>
#include <utility>
#include <vector>

#include "RE/Skyrim.h"

#include "CLIK/DropdownMenu.h"
#include "CLIK/ScrollingList.h"
#include "CLIK/Slider.h"
#include "CLIK/TextField.h"


namespace Scaleform
{
	namespace
	{
		struct TextEffect
		{
			std::string text;
			RE::FormID effectID;
		};


		struct RangeElem
		{
			using Type = RE::MagicItem::TargetType;

			std::string text;
			Type type;
		};


		struct Range
		{
			CLIK::TextField header;
			CLIK::GFx::Controls::DropdownMenu dropdown;
		};


		struct Magnitude
		{
			void UpdateText();

			CLIK::TextField header;
			CLIK::GFx::Controls::Slider slider;
			CLIK::TextField text;
		};
	}


	class Spellmaking : public RE::IMenu
	{
	public:
		using Base = RE::IMenu;
		using Result = Base::Result;


		Spellmaking();
		virtual ~Spellmaking() = default;

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
		static void Log(const RE::FxDelegateArgs& a_params);
		static void OnAvailablePress(const RE::FxDelegateArgs& a_params);
		static void OnSelectedPress(const RE::FxDelegateArgs& a_params);
		static void OnMagnitudeDragBegin(const RE::FxDelegateArgs& a_params);
		static void OnMagnitudeDragEnd(const RE::FxDelegateArgs& a_params);
		static void OnMagnitudeChange(const RE::FxDelegateArgs& a_params);

		void OnMenuOpen();
		void OnMenuClose();

		void InitExtensions();
		void SetAvailable();
		void SetRange();
		void SetMagnitude();

		bool OnAvailablePress(std::size_t a_availIdx);
		bool OnSelectedPress(std::size_t a_selectedIdx, bool a_remove);
		bool OnMagnitudeDragChange(bool a_enable);
		bool OnMagnitudeChange();

		void SanitizeString(std::string& a_str);


		static constexpr char SWF_NAME[] = "Spellmaking";
		static constexpr std::size_t MAX_EFFECTS = 8;

		CLIK::GFx::Controls::ScrollingList _available;
		CLIK::GFx::Controls::ScrollingList _selected;
		Range _range;
		Magnitude _magnitude;
		std::vector<TextEffect> _availableMappings;
		std::vector<TextEffect> _selectedMappings;
		std::vector<RangeElem> _rangeMappings;
		bool _magnitudeDrag;
	};


	constexpr std::string_view Spellmaking::Name()
	{
		return "Spellmaking";
	}
}
