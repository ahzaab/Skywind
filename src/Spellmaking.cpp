#include "Spellmaking.h"

#include "CLIK/Array.h"
#include "Scaleform.h"


namespace Scaleform
{
	namespace
	{
		void Magnitude::UpdateText()
		{
			auto num = static_cast<std::ptrdiff_t>(slider.Value());
			auto val = std::to_string(num);
			text.HTMLText(val);
		}
	}


	Spellmaking::Spellmaking() :
		_available(),
		_selected(),
		_range(),
		_magnitude(),
		_availableMappings(),
		_selectedMappings(),
		_rangeMappings(),
		_magnitudeDrag(false)
	{
		using ScaleModeType = RE::GFxMovieView::ScaleModeType;
		using Context = RE::InputMappingManager::Context;
		using Flag = RE::IMenu::Flag;

		flags = Flag::kTryShowCursor;
		auto loader = RE::BSScaleformMovieLoader::GetSingleton();
		if (loader->LoadMovie(this, view, SWF_NAME, ScaleModeType::kShowAll, 0.0)) {
			menuDepth = 5;	// JournalMenu == 5
			flags |= Flag::kPreventGameLoad | Flag::kHideOther | Flag::kPauseGame;
			context = Context::kFavor;
		}

		if (!view) {
			assert(false);
			_FATALERROR("Spellmaking did not have a view due to missing dependencies! Aborting process!\n");
			MessageBoxA(NULL, "Spellmaking did not have a view due to missing dependencies!\r\nAborting process!", NULL, MB_OK);
			std::abort();
		}

		InitExtensions();
		view->SetVisible(false);
	}


	void Spellmaking::Accept(RE::FxDelegateHandler::CallbackProcessor* a_processor)
	{
		a_processor->Process("Log", Log);
		a_processor->Process("OnAvailablePress", OnAvailablePress);
		a_processor->Process("OnSelectedPress", OnSelectedPress);
		a_processor->Process("OnMagnitudeDragBegin", OnMagnitudeDragBegin);
		a_processor->Process("OnMagnitudeDragEnd", OnMagnitudeDragEnd);
		a_processor->Process("OnMagnitudeChange", OnMagnitudeChange);
	}


	auto Spellmaking::ProcessMessage(RE::UIMessage* a_message)
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


	void Spellmaking::NextFrame(float a_arg1, UInt32 a_currentTime)
	{
		if (_magnitudeDrag) {
			_magnitude.UpdateText();
		}

		Base::NextFrame(a_arg1, a_currentTime);
	}


	void Spellmaking::Open()
	{
		using Message = RE::UIMessage::Message;

		auto ui = RE::UIManager::GetSingleton();
		ui->AddMessage(Name(), Message::kOpen, 0);
	}


	void Spellmaking::Close()
	{
		using Message = RE::UIMessage::Message;

		auto ui = RE::UIManager::GetSingleton();
		ui->AddMessage(Name(), Message::kClose, 0);
	}


	void Spellmaking::Register()
	{
		auto mm = RE::MenuManager::GetSingleton();
		mm->Register(Name(), Create);

		_MESSAGE("Registered %s", Name().data());
	}


	RE::IMenu* Spellmaking::Create()
	{
		return new Spellmaking();
	}


	void Spellmaking::Log(const RE::FxDelegateArgs& a_params)
	{
		assert(a_params.GetArgCount() == 1);
		assert(a_params[0].IsString());

		_MESSAGE("%s: %s", Name().data(), a_params[0].GetString());
	}


	void Spellmaking::OnAvailablePress(const RE::FxDelegateArgs& a_params)
	{
		assert(a_params.GetArgCount() == 1);
		assert(a_params[0].IsNumber());

		auto menu = static_cast<Spellmaking*>(a_params.GetHandler());
		auto num = a_params[0].GetUInt();
		menu->OnAvailablePress(num);
	}


	void Spellmaking::OnSelectedPress(const RE::FxDelegateArgs& a_params)
	{
		assert(a_params.GetArgCount() == 2);
		assert(a_params[0].IsNumber());
		assert(a_params[1].IsBool());

		auto menu = static_cast<Spellmaking*>(a_params.GetHandler());
		auto index = a_params[0].GetUInt();
		auto remove = a_params[1].GetBool();
		menu->OnSelectedPress(index, remove);
	}


	void Spellmaking::OnMagnitudeDragBegin(const RE::FxDelegateArgs& a_params)
	{
		assert(a_params.GetArgCount() == 0);

		auto menu = static_cast<Spellmaking*>(a_params.GetHandler());
		menu->OnMagnitudeDragChange(true);
	}


	void Spellmaking::OnMagnitudeDragEnd(const RE::FxDelegateArgs& a_params)
	{
		assert(a_params.GetArgCount() == 0);

		auto menu = static_cast<Spellmaking*>(a_params.GetHandler());
		menu->OnMagnitudeDragChange(false);
	}


	void Spellmaking::OnMagnitudeChange(const RE::FxDelegateArgs& a_params)
	{
		assert(a_params.GetArgCount() == 0);

		auto menu = static_cast<Spellmaking*>(a_params.GetHandler());
		menu->OnMagnitudeChange();
	}


	void Spellmaking::OnMenuOpen()
	{
		bool success;
		view->SetVisible(true);

		std::vector<std::pair<CLIK::Object*, std::string>> toGet;
		toGet.push_back(std::make_pair(&_available, "available"));
		toGet.push_back(std::make_pair(&_selected, "selected"));
		toGet.push_back(std::make_pair(&_range.header, "rangeHeader"));
		toGet.push_back(std::make_pair(&_range.dropdown, "range"));
		toGet.push_back(std::make_pair(&_magnitude.header, "magnitudeHeader"));
		toGet.push_back(std::make_pair(&_magnitude.slider, "magnitude"));
		toGet.push_back(std::make_pair(&_magnitude.text, "magnitudeText"));
		RE::GFxValue var;
		for (auto& elem : toGet) {
			success = view->GetVariable(&var, elem.second.c_str());
			assert(success);
			*elem.first = var;
		}

		CLIK::Object obj("ScrollBar");
		_available.ScrollBar(obj);
		_range.dropdown.ScrollBar(obj);

		SetAvailable();
	}


	void Spellmaking::OnMenuClose()
	{
		return;
	}


	void Spellmaking::InitExtensions()
	{
		RE::GFxValue boolean(true);
		bool success;

		success = view->SetVariable("_global.gfxExtensions", boolean);
		assert(success);
		success = view->SetVariable("_global.noInvisibleAdvance", boolean);
		assert(success);

		using StateType = RE::GFxState::StateType;
		auto logger = new Logger<Spellmaking>();
		view->SetState(StateType::kLog, logger);
		logger->Release();
	}


	void Spellmaking::SetAvailable()
	{
		using value_type = decltype(_availableMappings)::value_type;

		auto dataHandler = RE::TESDataHandler::GetSingleton();
		for (auto& mgef : dataHandler->GetFormArray<RE::EffectSetting>()) {
			if (!mgef->name.empty()) {
				std::string name(mgef->name);
				SanitizeString(name);
				_availableMappings.push_back({ std::move(name), mgef->formID });
			}
		}

		std::sort(_availableMappings.begin(), _availableMappings.end(), [](const value_type& a_lhs, const value_type& a_rhs)
		{
			return a_lhs.text < a_rhs.text;
		});

		CLIK::Array arr(view);
		CLIK::Object elem;
		for (auto& avail : _availableMappings) {
			elem = avail.text;
			arr.Push(elem);
		}

		_available.DataProvider(arr);
	}


	void Spellmaking::SetRange()
	{
		using Type = decltype(_rangeMappings)::value_type::Type;

		_range.header.HTMLText("Range");

		_rangeMappings.clear();
		_rangeMappings.push_back({ "Self", Type::kSelf });
		_rangeMappings.push_back({ "Touch", Type::kTouch });
		_rangeMappings.push_back({ "Aimed", Type::kAimed });

		CLIK::Array arr(view);
		CLIK::Object elem;
		for (auto& range : _rangeMappings) {
			elem = range.text;
			arr.Push(elem);
		}

		_range.dropdown.DataProvider(arr);
	}


	void Spellmaking::SetMagnitude()
	{
		_magnitude.header.HTMLText("Magnitude");
		_magnitude.text.HTMLText("1");
	}


	bool Spellmaking::OnAvailablePress(std::size_t a_availIdx)
	{
		using value_type = decltype(_selectedMappings)::value_type;

		if (a_availIdx > _availableMappings.size() || _selectedMappings.size() >= MAX_EFFECTS) {
			return false;
		}

		auto& avail = _availableMappings[a_availIdx];
		auto it = std::find_if(_selectedMappings.begin(), _selectedMappings.end(), [&](const value_type& a_val) -> bool
		{
			return a_val.effectID == avail.effectID;
		});
		if (it != _selectedMappings.end()) {
			return false;
		}

		_selectedMappings.push_back(avail);
		std::stable_sort(_selectedMappings.begin(), _selectedMappings.end(), [](const value_type& a_lhs, const value_type& a_rhs) -> bool
		{
			return a_lhs.text < a_rhs.text;
		});

		CLIK::Array arr(view);
		CLIK::Object elem;
		for (auto& selected : _selectedMappings) {
			elem = selected.text;
			arr.Push(elem);
		}

		_selected.DataProvider(arr);

		return true;
	}


	bool Spellmaking::OnSelectedPress(std::size_t a_selectedIdx, bool a_remove)
	{
		if (a_selectedIdx > _selectedMappings.size()) {
			return false;
		}

		if (a_remove) {
			_selectedMappings.erase(_selectedMappings.begin() + a_selectedIdx);

			CLIK::Array arr(view);
			CLIK::Object elem;
			for (auto& selected : _selectedMappings) {
				elem = selected.text;
				arr.Push(elem);
			}

			_selected.DataProvider(arr);
		} else {
			SetRange();
			SetMagnitude();
		}

		return true;
	}


	bool Spellmaking::OnMagnitudeDragChange(bool a_enable)
	{
		_magnitudeDrag = a_enable;
		return true;
	}


	bool Spellmaking::OnMagnitudeChange()
	{
		_magnitude.UpdateText();
		return true;
	}


	void Spellmaking::SanitizeString(std::string& a_str)
	{
		while (!a_str.empty() && std::isspace(a_str.back())) {
			a_str.pop_back();
		}
		while (!a_str.empty() && std::isspace(a_str.front())) {
			a_str.assign(a_str, 1);
		}
	}
}
