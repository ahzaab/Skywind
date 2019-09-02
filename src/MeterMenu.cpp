#include "MeterMenu.h"

#include <cassert>

#include "Scaleform.h"


namespace Scaleform
{
	namespace
	{
		Meter::Meter() :
			super()
		{}


		Meter::Meter(const Meter& a_rhs) :
			super(a_rhs)
		{}


		Meter::Meter(Meter&& a_rhs) :
			super(std::move(a_rhs))
		{}


		Meter::Meter(const CLIK::GFx::Core::UIComponent& a_rhs) :
			super(a_rhs)
		{}


		Meter::Meter(CLIK::GFx::Core::UIComponent&& a_rhs) :
			super(std::move(a_rhs))
		{}


		Meter::Meter(const RE::GFxValue& a_val) :
			super(a_val)
		{
			assert(_instance.IsObject());
		}


		Meter::Meter(RE::GFxValue&& a_val) :
			super(std::move(a_val))
		{
			assert(_instance.IsObject());
		}


		Meter::~Meter()
		{}


		Meter& Meter::operator=(const Meter& a_rhs)
		{
			super::operator=(a_rhs);
			return *this;
		}


		Meter& Meter::operator=(Meter&& a_rhs)
		{
			super::operator=(std::move(a_rhs));
			return *this;
		}


		Meter& Meter::operator=(const CLIK::GFx::Core::UIComponent& a_rhs)
		{
			super::operator=(a_rhs);
			return *this;
		}


		Meter& Meter::operator=(CLIK::GFx::Core::UIComponent&& a_rhs)
		{
			super::operator=(std::move(a_rhs));
			return *this;
		}


		Meter& Meter::operator=(const RE::GFxValue& a_rhs)
		{
			_instance = a_rhs;
			assert(_instance.IsObject());
			return *this;
		}


		Meter& Meter::operator=(RE::GFxValue&& a_rhs)
		{
			_instance = std::move(a_rhs);
			assert(_instance.IsObject());
			return *this;
		}


		double Meter::FillDelta() const
		{
			return GetNumber("fillDelta");
		}


		void Meter::FillDelta(double a_fillDelta)
		{
			SetNumber("fillDelta", a_fillDelta);
		}


		double Meter::EmptyDelta() const
		{
			return GetNumber("emptyDelta");
		}


		void Meter::EmptyDelta(double a_emptyDelta)
		{
			SetNumber("emptyDelta", a_emptyDelta);
		}


		double Meter::Color() const
		{
			return GetNumber("color");
		}


		void Meter::Color(double a_primaryColor)
		{
			SetNumber("color", a_primaryColor);
		}


		double Meter::FlashColor() const
		{
			return GetNumber("flashColor");
		}


		void Meter::FlashColor(double a_flashColor)
		{
			SetNumber("flashColor", a_flashColor);
		}


		std::string_view Meter::FillDirection() const
		{
			return GetString("fillDirection");
		}


		void Meter::FillDirection(std::string_view a_fillDirection)
		{
			SetString("fillDirection", a_fillDirection);
		}


		double Meter::Percent() const
		{
			return GetNumber("percent");
		}


		void Meter::Percent(double a_percent)
		{
			SetNumber("percent", a_percent);
		}


		void Meter::SetFillDirection(std::string_view a_fillDirection, bool a_restorePercent)
		{
			enum
			{
				kFillDirection,
				kRestorePercent,
				kNumArgs
			};

			RE::GFxValue args[kNumArgs];

			args[kFillDirection] = a_fillDirection;
			assert(args[kFillDirection].IsString());

			args[kRestorePercent] = a_restorePercent;
			assert(args[kRestorePercent].IsBool());

			auto success = _instance.Invoke("setFillDirection", 0, args, kNumArgs);
			assert(success);
		}


		void Meter::SetColors(double a_primaryColor, double a_secondaryColor, double a_flashColor)
		{
			enum
			{
				kPrimaryColor,
				kSecondaryColor,
				kFlashColor,
				kNumArgs
			};

			RE::GFxValue args[kNumArgs];

			args[kPrimaryColor] = a_primaryColor;
			assert(args[kPrimaryColor].IsNumber());

			args[kSecondaryColor] = a_secondaryColor;
			assert(args[kSecondaryColor].IsNumber());

			args[kFlashColor] = a_flashColor;
			assert(args[kFlashColor].IsNumber());

			auto success = _instance.Invoke("setColors", 0, args, kNumArgs);
			assert(success);
		}


		void Meter::SetPercent(double a_percent, bool a_force)
		{
			enum
			{
				kPercent,
				kForce,
				kNumArgs
			};

			RE::GFxValue args[kNumArgs];

			args[kPercent] = a_percent;
			assert(args[kPercent].IsNumber());

			args[kForce] = a_force;
			assert(args[kForce].IsBool());

			auto success = _instance.Invoke("setPercent", 0, args, kNumArgs);
			assert(success);
		}


		void Meter::StartFlash(bool a_force)
		{
			enum
			{
				kForce,
				kNumArgs
			};

			RE::GFxValue args[kNumArgs];

			args[kForce] = a_force;
			assert(args[kForce].IsBool());

			auto success = _instance.Invoke("startFlash", 0, args, kNumArgs);
			assert(success);
		}
	}


	MeterMenu::MeterMenu()
	{
		using ScaleModeType = RE::GFxMovieView::ScaleModeType;
		using Context = RE::InputMappingManager::Context;
		using Flag = RE::IMenu::Flag;

		auto loader = RE::BSScaleformMovieLoader::GetSingleton();
		if (!loader->LoadMovie(this, view, SWF_NAME, ScaleModeType::kShowAll, 0.0)) {
			assert(false);
			_FATALERROR("MeterMenu did not have a view due to missing dependencies! Aborting process!\n");
			MessageBoxA(NULL, "MeterMenu did not have a view due to missing dependencies!\r\nAborting process!", NULL, MB_OK);
			std::abort();
		}

		flags |= Flag::kModal;
		context = Context::kGameplay;

		InitExtensions();
		view->SetVisible(false);
	}


	void MeterMenu::Accept(RE::FxDelegateHandler::CallbackProcessor* a_cbReg)
	{
		a_cbReg->Process("Log", Log);
	}


	auto MeterMenu::ProcessMessage(RE::UIMessage* a_message)
		-> Result
	{
		using Message = RE::UIMessage::Message;

		switch (a_message->message) {
		case Message::kOpen:
			OnMenuOpen();
			return Result::kProcessed;
		case Message::kClose:
			OnMenuClose();
			return Result::kProcessed;
		default:
			return Base::ProcessMessage(a_message);
		}
	}


	void MeterMenu::NextFrame(float a_arg1, UInt32 a_currentTime)
	{
		_meter.SetPercent(100, false);
		Base::NextFrame(a_arg1, a_currentTime);
	}


	void MeterMenu::Open()
	{
		using Message = RE::UIMessage::Message;

		auto ui = RE::UIManager::GetSingleton();
		ui->AddMessage(Name(), Message::kOpen, 0);
	}


	void MeterMenu::Close()
	{
		using Message = RE::UIMessage::Message;

		auto ui = RE::UIManager::GetSingleton();
		ui->AddMessage(Name(), Message::kClose, 0);
	}


	void MeterMenu::Register()
	{
		auto mm = RE::MenuManager::GetSingleton();
		mm->Register(Name(), Create);

		_MESSAGE("Registered %s", Name().data());
	}


	RE::IMenu* MeterMenu::Create()
	{
		return new MeterMenu();
	}


	void MeterMenu::Log(const RE::FxDelegateArgs& a_params)
	{
		assert(a_params.GetArgCount() == 1);
		assert(a_params[0].IsString());

		_MESSAGE("%s: %s", Name().data(), a_params[0].GetString());
	}


	void MeterMenu::OnMenuOpen()
	{
		bool success;
		view->SetVisible(true);

		std::vector<std::pair<CLIK::Object*, std::string>> toGet;
		toGet.push_back(std::make_pair(&_meter, "meter"));
		RE::GFxValue var;
		for (auto& elem : toGet) {
			success = view->GetVariable(&var, elem.second.c_str());
			assert(success);
			*elem.first = var;
		}

		_meter.FillDirection(Meter::FILL_DIRECTION_RIGHT);
		_meter.Color(0xFF0000);
		_meter.FillDelta(0.5);
	}


	void MeterMenu::OnMenuClose()
	{
		RE::GFxValue boolean(false);
		view->Invoke("Selection.captureFocus", 0, &boolean, 1);
	}


	void MeterMenu::InitExtensions()
	{
		RE::GFxValue boolean(true);
		bool success;

		success = view->SetVariable("_global.gfxExtensions", boolean);
		assert(success);
		success = view->SetVariable("_global.noInvisibleAdvance", boolean);
		assert(success);

		using StateType = RE::GFxState::StateType;
		auto logger = new Logger<MeterMenu>();
		view->SetState(StateType::kLog, logger);
		logger->Release();
	}
}
