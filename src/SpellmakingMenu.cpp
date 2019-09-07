#include "SpellmakingMenu.h"

#include <map>

#include "CLIK/Array.h"
#include "Scaleform.h"


namespace Scaleform
{
	namespace
	{
		Slider::Slider() :
			header(),
			slider(),
			text(),
			_isDragging(false)
		{}


		void Slider::UpdateText()
		{
			auto num = static_cast<std::ptrdiff_t>(slider.Value());
			auto val = std::to_string(num);
			text.HTMLText(val);
		}


		void Slider::SetDragging(bool a_isDragging)
		{
			_isDragging = a_isDragging;
		}


		bool Slider::IsDragging() const
		{
			return _isDragging;
		}
	}


	SpellmakingMenu::SpellmakingMenu() :
		_available(),
		_selected(),
		_range(),
		_magnitude(),
		_duration(),
		_area(),
		_availableMappings(),
		_selectedMappings(),
		_rangeMappings(),
		_selectedIdx(kInvalid)
	{
		using Context = RE::InputMappingManager::Context;
		using Flag = RE::IMenu::Flag;

		flags |= Flag::kTryShowCursor;
		auto loader = RE::BSScaleformMovieLoader::GetSingleton();
		auto success = loader->LoadMovieStd(this, SWF_NAME, [this](RE::GFxMovieDef* a_def)
		{
			using StateType = RE::GFxState::StateType;

			fxDelegate.reset(new RE::FxDelegate());
			fxDelegate->RegisterHandler(this);
			a_def->SetState(StateType::kExternalInterface, fxDelegate.get());
			fxDelegate->Release();

			auto logger = new Logger<SpellmakingMenu>();
			a_def->SetState(StateType::kLog, logger);
			logger->Release();
		});

		if (!success) {
			assert(false);
			_FATALERROR("SpellmakingMenu did not have a view due to missing dependencies! Aborting process!\n");
			MessageBoxA(NULL, "SpellmakingMenu did not have a view due to missing dependencies!\r\nAborting process!", NULL, MB_OK);
			std::abort();
		}

		menuDepth = 5;	// JournalMenu == 5
		flags |= Flag::kPreventGameLoad | Flag::kHideOther | Flag::kPauseGame;
		context = Context::kFavor;

		InitExtensions();
		view->SetVisible(false);
	}


	void SpellmakingMenu::Accept(RE::FxDelegateHandler::CallbackProcessor* a_processor)
	{
		a_processor->Process("Log", Log);
		a_processor->Process("OnAvailablePress", OnAvailablePress);
		a_processor->Process("OnSelectedPress", OnSelectedPress);
		a_processor->Process("OnMagnitudeDragBegin", OnMagnitudeDragBegin);
		a_processor->Process("OnMagnitudeDragEnd", OnMagnitudeDragEnd);
		a_processor->Process("OnMagnitudeChange", OnMagnitudeChange);
		a_processor->Process("OnDurationDragBegin", OnDurationDragBegin);
		a_processor->Process("OnDurationDragEnd", OnDurationDragEnd);
		a_processor->Process("OnDurationChange", OnDurationChange);
		a_processor->Process("OnAreaDragBegin", OnAreaDragBegin);
		a_processor->Process("OnAreaDragEnd", OnAreaDragEnd);
		a_processor->Process("OnAreaChange", OnAreaChange);
		a_processor->Process("CraftSpell", CraftSpell);
	}


	auto SpellmakingMenu::ProcessMessage(RE::UIMessage* a_message)
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


	void SpellmakingMenu::NextFrame(float a_arg1, UInt32 a_currentTime)
	{
		if (_magnitude.IsDragging()) {
			_magnitude.UpdateText();
		}

		if (_duration.IsDragging()) {
			_duration.UpdateText();
		}

		if (_area.IsDragging()) {
			_area.UpdateText();
		}

		Base::NextFrame(a_arg1, a_currentTime);
	}


	void SpellmakingMenu::Open()
	{
		using Message = RE::UIMessage::Message;

		auto ui = RE::UIManager::GetSingleton();
		ui->AddMessage(Name(), Message::kOpen, 0);
	}


	void SpellmakingMenu::Close()
	{
		using Message = RE::UIMessage::Message;

		auto ui = RE::UIManager::GetSingleton();
		ui->AddMessage(Name(), Message::kClose, 0);
	}


	void SpellmakingMenu::Register()
	{
		auto mm = RE::MenuManager::GetSingleton();
		mm->Register(Name(), Create);

		_MESSAGE("Registered %s", Name().data());
	}


	RE::IMenu* SpellmakingMenu::Create()
	{
		return new SpellmakingMenu();
	}


	void SpellmakingMenu::Log(const RE::FxDelegateArgs& a_params)
	{
		assert(a_params.GetArgCount() == 1);
		assert(a_params[0].IsString());

		_MESSAGE("%s: %s", Name().data(), a_params[0].GetString());
	}


	void SpellmakingMenu::OnAvailablePress(const RE::FxDelegateArgs& a_params)
	{
		assert(a_params.GetArgCount() == 1);
		assert(a_params[0].IsNumber());

		auto menu = static_cast<SpellmakingMenu*>(a_params.GetHandler());
		auto num = a_params[0].GetUInt();
		menu->OnAvailablePress(num);
	}


	void SpellmakingMenu::OnSelectedPress(const RE::FxDelegateArgs& a_params)
	{
		assert(a_params.GetArgCount() == 2);
		assert(a_params[0].IsNumber());
		assert(a_params[1].IsBool());

		auto menu = static_cast<SpellmakingMenu*>(a_params.GetHandler());
		auto index = a_params[0].GetUInt();
		auto remove = a_params[1].GetBool();
		menu->OnSelectedPress(index, remove);
	}


	void SpellmakingMenu::OnMagnitudeDragBegin(const RE::FxDelegateArgs& a_params)
	{
		assert(a_params.GetArgCount() == 0);

		auto menu = static_cast<SpellmakingMenu*>(a_params.GetHandler());
		menu->OnMagnitudeDragChange(true);
	}


	void SpellmakingMenu::OnMagnitudeDragEnd(const RE::FxDelegateArgs& a_params)
	{
		assert(a_params.GetArgCount() == 0);

		auto menu = static_cast<SpellmakingMenu*>(a_params.GetHandler());
		menu->OnMagnitudeDragChange(false);
	}


	void SpellmakingMenu::OnMagnitudeChange(const RE::FxDelegateArgs& a_params)
	{
		assert(a_params.GetArgCount() == 0);

		auto menu = static_cast<SpellmakingMenu*>(a_params.GetHandler());
		menu->OnMagnitudeChange();
	}


	void SpellmakingMenu::OnDurationDragBegin(const RE::FxDelegateArgs& a_params)
	{
		assert(a_params.GetArgCount() == 0);

		auto menu = static_cast<SpellmakingMenu*>(a_params.GetHandler());
		menu->OnDurationDragChange(true);
	}


	void SpellmakingMenu::OnDurationDragEnd(const RE::FxDelegateArgs& a_params)
	{
		assert(a_params.GetArgCount() == 0);

		auto menu = static_cast<SpellmakingMenu*>(a_params.GetHandler());
		menu->OnDurationDragChange(false);
	}


	void SpellmakingMenu::OnDurationChange(const RE::FxDelegateArgs& a_params)
	{
		assert(a_params.GetArgCount() == 0);

		auto menu = static_cast<SpellmakingMenu*>(a_params.GetHandler());
		menu->OnDurationChange();
	}


	void SpellmakingMenu::OnAreaDragBegin(const RE::FxDelegateArgs& a_params)
	{
		assert(a_params.GetArgCount() == 0);

		auto menu = static_cast<SpellmakingMenu*>(a_params.GetHandler());
		menu->OnAreaDragChange(true);
	}


	void SpellmakingMenu::OnAreaDragEnd(const RE::FxDelegateArgs& a_params)
	{
		assert(a_params.GetArgCount() == 0);

		auto menu = static_cast<SpellmakingMenu*>(a_params.GetHandler());
		menu->OnAreaDragChange(false);
	}


	void SpellmakingMenu::OnAreaChange(const RE::FxDelegateArgs& a_params)
	{
		assert(a_params.GetArgCount() == 0);

		auto menu = static_cast<SpellmakingMenu*>(a_params.GetHandler());
		menu->OnAreaChange();
	}


	void SpellmakingMenu::CraftSpell(const RE::FxDelegateArgs& a_params)
	{
		assert(a_params.GetArgCount() == 0);

		auto menu = static_cast<SpellmakingMenu*>(a_params.GetHandler());
		menu->CraftSpell();
	}


	void SpellmakingMenu::OnMenuOpen()
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
		toGet.push_back(std::make_pair(&_duration.header, "durationHeader"));
		toGet.push_back(std::make_pair(&_duration.slider, "duration"));
		toGet.push_back(std::make_pair(&_duration.text, "durationText"));
		toGet.push_back(std::make_pair(&_area.header, "areaHeader"));
		toGet.push_back(std::make_pair(&_area.slider, "area"));
		toGet.push_back(std::make_pair(&_area.text, "areaText"));
		toGet.push_back(std::make_pair(&_name, "name"));
		toGet.push_back(std::make_pair(&_craft, "craft"));
		RE::GFxValue var;
		for (auto& elem : toGet) {
			success = view->GetVariable(&var, elem.second.c_str());
			assert(success);
			*elem.first = var;
		}

		CLIK::Object obj("ScrollBar");
		_available.ScrollBar(obj);
		_range.dropdown.ScrollBar(obj);

		_craft.Label("Craft Spell");
		_craft.Disabled(true);

		InitEffectInfo();
		SetAvailable();
		SetEffectInfo();
	}


	void SpellmakingMenu::OnMenuClose()
	{
		return;
	}


	void SpellmakingMenu::InitExtensions()
	{
		RE::GFxValue boolean(true);
		bool success;

		success = view->SetVariable("_global.gfxExtensions", boolean);
		assert(success);
		success = view->SetVariable("_global.noInvisibleAdvance", boolean);
		assert(success);

		using StateType = RE::GFxState::StateType;
		auto logger = new Logger<SpellmakingMenu>();
		view->SetState(StateType::kLog, logger);
		logger->Release();
	}


	void SpellmakingMenu::InitEffectInfo()
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

		_range.dropdown.Disabled(false);
		_range.dropdown.DataProvider(arr);

		_magnitude.header.HTMLText("Magnitude");
		_duration.header.HTMLText("Duration");
		_area.header.HTMLText("Area");
	}


	void SpellmakingMenu::SetAvailable()
	{
		using value_type = decltype(_availableMappings)::value_type;

		std::map<RE::FormID, RE::EffectSetting*> knownEffects;

#if 1
		auto player = RE::PlayerCharacter::GetSingleton();
		for (auto& spell : player->addedSpells) {
			for (auto& effect : spell->effects) {
				auto mgef = effect->baseEffect;
				knownEffects.insert(std::make_pair(mgef->formID, mgef));
				if (!mgef->name.empty()) {
					std::string name(mgef->name);
					SanitizeString(name);
					_availableMappings.push_back({ std::move(name), mgef->formID });
				}
			}
		}

		auto base = player->GetActorBase();
		if (base->actorEffects) {
			auto effects = base->actorEffects;
			for (UInt32 i = 0; i < effects->numSpells; ++i) {
				auto spell = effects->spells[i];
				for (auto& effect : spell->effects) {
					auto mgef = effect->baseEffect;
					knownEffects.insert(std::make_pair(mgef->formID, mgef));
				}
			}
		}
#else
		auto dataHandler = RE::TESDataHandler::GetSingleton();
		for (auto& mgef : dataHandler->GetFormArray<RE::EffectSetting>()) {
			knownEffects.insert(std::make_pair(mgef->formID, mgef));
		}
#endif

		for (auto& effect : knownEffects) {
			auto mgef = effect.second;
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


	void SpellmakingMenu::SetEffectInfo()
	{
		if (_selectedIdx < _selectedMappings.size()) {
			auto& selected = _selectedMappings[_selectedIdx];

			_magnitude.slider.Disabled(false);
			_magnitude.slider.Value(selected.magnitude);
			_magnitude.text.HTMLText(std::to_string(selected.magnitude));

			_duration.slider.Disabled(false);
			_duration.slider.Value(selected.duration);
			_duration.text.HTMLText(std::to_string(selected.duration));

			_area.slider.Disabled(false);
			_area.slider.Value(selected.area);
			_area.text.HTMLText(std::to_string(selected.area));
		} else {
			_magnitude.slider.Disabled(true);
			_magnitude.text.HTMLText("-");

			_duration.slider.Disabled(true);
			_duration.text.HTMLText("-");

			_area.slider.Disabled(true);
			_area.text.HTMLText("-");
		}
	}


	void SpellmakingMenu::CommitSelection()
	{
		if (_selectedIdx < _selectedMappings.size()) {
			auto& selected = _selectedMappings[_selectedIdx];
			selected.magnitude = _magnitude.slider.Value();
			selected.duration = _duration.slider.Value();
			selected.area = _area.slider.Value();
		}
	}


	void SpellmakingMenu::CraftSpell()
	{
		CommitSelection();

		auto factory = RE::IFormFactory::GetConcreteFormFactoryByType<RE::SpellItem>();
		if (!factory) {
			return;
		}

		auto spell = factory->Create();
		if (!spell) {
			return;
		}

		//spell->name = _name.Text();
		spell->name = "Spell of Awesomeness";
		spell->data.type = RE::MagicItem::MagicType::kSpell;
		spell->data.castType = RE::MagicItem::CastType::kConcentration;
		spell->data.targetType = static_cast<RE::MagicItem::TargetType>(static_cast<std::ptrdiff_t>(_area.slider.Value()));

		spell->menuDisplayObject = RE::TESForm::LookupByID<RE::TESObjectSTAT>(0x000A0E7D);	// MAGINVHealSpellArt

		spell->objectBounds.x1 = 0;
		spell->objectBounds.y1 = 0;
		spell->objectBounds.z1 = 0;
		spell->objectBounds.x2 = 0;
		spell->objectBounds.y2 = 0;
		spell->objectBounds.z2 = 0;

		for (auto& selected : _selectedMappings) {
			auto effect = new RE::Effect();
			effect->effectItem.magnitude = selected.magnitude;
			effect->effectItem.area = selected.area;
			effect->effectItem.duration = selected.duration;
			effect->baseEffect = RE::TESForm::LookupByID<RE::EffectSetting>(selected.effectID);
			spell->effects.push_back(effect);
		}

		auto player = RE::PlayerCharacter::GetSingleton();
		auto cost = spell->GetEffectiveMagickaCost(player);
		player->AddSpell(spell);
	}


	bool SpellmakingMenu::OnAvailablePress(std::size_t a_availIdx)
	{
		using value_type = decltype(_selectedMappings)::value_type;

		if (a_availIdx >= _availableMappings.size() || _selectedMappings.size() >= MAX_EFFECTS) {
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

		_selectedMappings.push_back({ avail.text, avail.effectID, 1, 0, 1 });
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
		_craft.Disabled(_selectedMappings.empty());

		return true;
	}


	bool SpellmakingMenu::OnSelectedPress(std::size_t a_selectedIdx, bool a_remove)
	{
		if (a_selectedIdx < _selectedMappings.size() && a_remove) {
			_selectedMappings.erase(_selectedMappings.begin() + a_selectedIdx);

			CLIK::Array arr(view);
			CLIK::Object elem;
			for (auto& selected : _selectedMappings) {
				elem = selected.text;
				arr.Push(elem);
			}

			_selected.DataProvider(arr);
			_selected.SelectedIndex(-1);
			_selectedIdx = kInvalid;
		} else {
			CommitSelection();
			_selectedIdx = a_selectedIdx;
		}

		SetEffectInfo();
		_craft.Disabled(_selectedMappings.empty());

		return true;
	}


	bool SpellmakingMenu::OnMagnitudeDragChange(bool a_isDragging)
	{
		_magnitude.SetDragging(a_isDragging);
		return true;
	}


	bool SpellmakingMenu::OnMagnitudeChange()
	{
		_magnitude.UpdateText();
		return true;
	}


	bool SpellmakingMenu::OnDurationDragChange(bool a_isDragging)
	{
		_duration.SetDragging(a_isDragging);
		return true;
	}


	bool SpellmakingMenu::OnDurationChange()
	{
		_duration.UpdateText();
		return true;
	}


	bool SpellmakingMenu::OnAreaDragChange(bool a_isDragging)
	{
		_area.SetDragging(a_isDragging);
		return true;
	}


	bool SpellmakingMenu::OnAreaChange()
	{
		_area.UpdateText();
		return true;
	}


	void SpellmakingMenu::SanitizeString(std::string& a_str)
	{
		while (!a_str.empty() && std::isspace(a_str.back())) {
			a_str.pop_back();
		}
		while (!a_str.empty() && std::isspace(a_str.front())) {
			a_str.assign(a_str, 1);
		}
	}
}
