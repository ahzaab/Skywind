#pragma once

#include "RE/Skyrim.h"

#include "CLIK/UIComponent.h"


namespace Scaleform
{
	namespace
	{
		class Meter : public CLIK::GFx::Core::UIComponent
		{
		public:
			using super = CLIK::GFx::Core::UIComponent;


			Meter();
			Meter(const Meter& a_rhs);
			Meter(Meter&& a_rhs);
			Meter(const CLIK::GFx::Core::UIComponent& a_rhs);
			Meter(CLIK::GFx::Core::UIComponent&& a_rhs);
			explicit Meter(const RE::GFxValue& a_val);
			explicit Meter(RE::GFxValue&& a_val);
			~Meter();

			Meter& operator=(const Meter& a_rhs);
			Meter& operator=(Meter&& a_rhs);
			Meter& operator=(const CLIK::GFx::Core::UIComponent& a_rhs);
			Meter& operator=(CLIK::GFx::Core::UIComponent&& a_rhs);
			Meter& operator=(const RE::GFxValue& a_rhs);
			Meter& operator=(RE::GFxValue&& a_rhs);


			static constexpr std::string_view FILL_DIRECTION_LEFT = "left";
			static constexpr std::string_view FILL_DIRECTION_RIGHT = "right";
			static constexpr std::string_view FILL_DIRECTION_BOTH = "both";


			double FillDelta() const;
			void FillDelta(double a_fillDelta);

			double EmptyDelta() const;
			void EmptyDelta(double a_emptyDelta);

			double Color() const;
			void Color(double a_primaryColor);

			double FlashColor() const;
			void FlashColor(double a_flashColor);

			std::string_view FillDirection() const;
			void FillDirection(std::string_view a_fillDirection);

			double Percent() const;
			void Percent(double a_percent);

			void SetColors(double a_primaryColor, double a_secondaryColor, double a_flashColor);
			void SetFillDirection(std::string_view a_fillDirection, bool a_restorePercent);
			void SetPercent(double a_percent, bool a_force);

			void StartFlash(bool a_force);
		};
	}


	class MeterMenu : public RE::IMenu
	{
	public:
		using Base = RE::IMenu;
		using Result = RE::IMenu::Result;

		MeterMenu();
		virtual ~MeterMenu() = default;

		// IMenu
		virtual void Accept(RE::FxDelegateHandler::CallbackProcessor* a_cbReg) override;
		virtual Result ProcessMessage(RE::UIMessage* a_message) override;
		virtual void NextFrame(float a_arg1, UInt32 a_currentTime) override;

		static void Open();
		static void Close();

		static constexpr std::string_view Name();

		static void Register();
		static RE::IMenu* Create();

	private:
		static void Log(const RE::FxDelegateArgs& a_params);

		void OnMenuOpen();
		void OnMenuClose();

		void InitExtensions();

		static constexpr char SWF_NAME[] = "Meter";

		Meter _meter;
	};


	constexpr std::string_view MeterMenu::Name()
	{
		return "Meter";
	}
}
