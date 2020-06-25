#include "PCH.h"

#include "StatsMenuEx.h"

#include <queue>
#include <unordered_set>

#include "CLIK/Array.h"
#include "Scaleform.h"
#include "SKSE/API.h"

namespace Scaleform
{
	
	namespace
	{
		RootMap::RootMap() :
			Base()
		{
			using AV = RE::ActorValue;

			std::decay_t<decltype(Root::text)> text;
			std::decay_t<decltype(Root::info)> info;

			text = DRAGONBORN;
			info.clear();
			info.push_back({ kAVOneHanded, AV::kOneHanded });
			info.push_back({ kAVTwoHanded, AV::kTwoHanded });
			info.push_back({ kAVMarksman, AV::kArchery });
			info.push_back({ kAVBlock, AV::kBlock });
			info.push_back({ kAVSmithing, AV::kSmithing });
			info.push_back({ kAVHeavyArmor, AV::kHeavyArmor });
			info.push_back({ kAVLightArmor, AV::kLightArmor });
			info.push_back({ kAVPickpocket, AV::kPickpocket });
			info.push_back({ kAVLockpicking, AV::kLockpicking });
			info.push_back({ kAVSneak, AV::kSneak });
			info.push_back({ kAVAlchemy, AV::kAlchemy });
			info.push_back({ kAVSpeechCraft, AV::kSpeech });
			info.push_back({ kAVAlteration, AV::kAlteration });
			info.push_back({ kAVConjuration, AV::kConjuration });
			info.push_back({ kAVDestruction, AV::kDestruction });
			info.push_back({ kAVMysticism, AV::kIllusion });
			info.push_back({ kAVRestoration, AV::kRestoration });
			info.push_back({ kAVEnchanting, AV::kEnchanting });
			push_back({ text, info, true });

			text = VAMPIRE;
			info.clear();
			info.push_back({ kAVMagickaRateMod, AV::kNone });
			push_back({ text, info, true });

			text = WEREWOLF;
			info.clear();
			info.push_back({ kAVHealRatePowerMod, AV::kNone });
			push_back({ text, info, true });

			text = ATTRIBUTES;
			info.clear();
			info.push_back({ kAVStrength, AV::kFavorActive });
			info.push_back({ kAVIntelligence, AV::kFavorsPerDayTimer });
			info.push_back({ kAVWillpower, AV::kWaitingForPlayer });
			info.push_back({ kAVAgility, AV::kLastBribedIntimidated });
			info.push_back({ kAVSpeed, AV::kLastFlattered });
			info.push_back({ kAVEndurance, AV::kFavorsPerDay });
			info.push_back({ kAVPersonality, AV::kFame });
			info.push_back({ kAVLuck, AV::kInfamy });
			push_back({ text, info, false });
		}


		void HeaderList::Visible(bool a_visible)
		{
			list.Visible(a_visible);
			header.Visible(a_visible);
		}


		void Description::Init()
		{
			header.WordWrap(true);
			text.WordWrap(true);
			requisites.header.Text("Requires");
			unlocks.header.Text("Unlocks");
		}


		void Description::Visible(bool a_visible)
		{
			header.Visible(a_visible);
			text.Visible(a_visible);
			requisites.Visible(a_visible);
			unlocks.Visible(a_visible);
			unlock.Visible(a_visible);
		}


		Stats::Stats() :
			perkPoints(),
			magicka(),
			health(),
			stamina(),
			_state(State::kDefault)
		{}


		void Stats::Update()
		{
			std::string text;

			text = "Perk Points: ";
			text += std::to_string(GetPerkPoints());
			perkPoints.HTMLText(text);

			text = "Magicka: ";
			text += BuildStatString(RE::ActorValue::kMagicka);
			magicka.HTMLText(text);

			text = "Health: ";
			text += BuildStatString(RE::ActorValue::kHealth);
			health.HTMLText(text);

			text = "Stamina: ";
			text += BuildStatString(RE::ActorValue::kStamina);
			stamina.HTMLText(text);
		}


		void Stats::SetDefault()
		{
			_state = State::kDefault;
		}


		void Stats::SetVampire()
		{
			_state = State::kVampire;
		}


		void Stats::SetWerewolf()
		{
			_state = State::kWerewolf;
		}


		bool Stats::IsBeastMode() const
		{
			switch (_state) {
			case State::kVampire:
			case State::kWerewolf:
				return true;
			case State::kDefault:
			default:
				return false;
			}
		}


		UInt32 Stats::GetPerkPoints() const
		{
			using Object = RE::DEFAULT_OBJECT;

			std::size_t obj;
			switch (_state) {
			case State::kVampire:
				obj = Object::kVampireAvailablePerks;
				break;
			case State::kWerewolf:
				obj = Object::kWerewolfAvailablePerks;
				break;
			case State::kDefault:
			default:
				{
					auto player = RE::PlayerCharacter::GetSingleton();
					return player->perkCount;
				}
				break;
			}

			auto dobj = RE::BGSDefaultObjectManager::GetSingleton();
			auto global = dobj->GetObject<RE::TESGlobal>(obj);
			return global ? static_cast<UInt32>(global->value) : 0;
		}


		void Stats::DecPerkPoints() const
		{
			using Object = RE::DEFAULT_OBJECT;

			std::size_t obj;
			switch (_state) {
			case State::kVampire:
				obj = Object::kVampireAvailablePerks;
				break;
			case State::kWerewolf:
				obj = Object::kWerewolfAvailablePerks;
				break;
			case State::kDefault:
			default:
				{
					auto player = RE::PlayerCharacter::GetSingleton();
					player->perkCount -= 1;
					return;
				}
				break;
			}

			auto dobj = RE::BGSDefaultObjectManager::GetSingleton();
			auto global = dobj->GetObject<RE::TESGlobal>(obj);
			if (global) {
				global->value -= 1;
			}
		}


		std::string Stats::BuildStatString(RE::ActorValue a_av) const
		{
			auto player = RE::PlayerCharacter::GetSingleton();
			auto fBase = player->GetActorValue(a_av);
			auto fCur = player->GetPermanentActorValue(a_av);

			auto base = static_cast<SInt32>(fBase);
			auto mod = static_cast<SInt32>(fCur - fBase);

			std::string text;
			text = std::to_string(base);
			if (mod != 0) {
				text += " (";
				if (mod > 0) {
					text += '+';
				} else {
					text += '-';
				}
				text += std::to_string(std::abs(mod));
				text += ')';
			}

			return text;
		}
	}

	//constexpr  REL::ID StatsMenuEx::Vtbl(static_cast<std::uint64_t>(269955));
	bool StatsMenuEx::s_active = false;

	StatsMenuEx::StatsMenuEx() :
		Base(),
		_roots(),
		_trees(),
		_perks(),
		_ranks(),
		_desc(),
		_stats(),
		_rootMappings(),
		_treeMappings(),
		_perkMappings(),
		_rankMappings(),
		_requisiteMappings(),
		_unlockMappings()
	{
		using Context = RE::UserEvents::INPUT_CONTEXT_ID;
		using Flag = RE::IMenu::Flag;
		//StatsMenuEx::s_active = false;

		flags |= Flag::kUpdateUsesCursor;
		auto loader = RE::BSScaleformManager::GetSingleton();
		auto success = loader->LoadMovieEx(this, SWF_NAME, [this](RE::GFxMovieDef* a_def)
		{
			using StateType = RE::GFxState::StateType;

			fxDelegate.reset(new RE::FxDelegate());
			fxDelegate->RegisterHandler(this);
			a_def->SetState(StateType::kExternalInterface, fxDelegate.get());
			fxDelegate->Release();

			auto logger = new Logger<StatsMenuEx>();
			a_def->SetState(StateType::kLog, logger);
			logger->Release();
		});

		if (!success) {
			assert(false);
			_FATALERROR("StatsMenuEx did not have a view due to missing dependencies! Aborting process!\n");
			MessageBoxA(NULL, "StatsMenuEx did not have a view due to missing dependencies!\r\nAborting process!", NULL, MB_OK);
			std::abort();
		}

		//Matched the flags used by the Native StatsMenu. kCustomRendering is needed to allow the Tween Menu animation
		flags = Flag::kPausesGame | Flag::kUsesCursor | Flag::kUsesMenuContext | Flag::kDisablePauseMenu | Flag::kCustomRendering;
		context = Context::kStats;
		menuDepth = 5;	// JournalMenu == 5

		InitExtensions();
		view->SetVisible(false);
	}


	void StatsMenuEx::Accept(RE::FxDelegateHandler::CallbackProcessor* a_processor)
	{
		a_processor->Process("Log", Log);
		a_processor->Process("OnRootPress", OnRootPress);
		a_processor->Process("OnTreePress", OnTreePress);
		a_processor->Process("OnPerkPress", OnPerkPress);
		a_processor->Process("OnRankPress", OnRankPress);
		a_processor->Process("OnRequisitePress", OnRequisitePress);
		a_processor->Process("OnUnlockPress", OnUnlockPress);
		a_processor->Process("UnlockPerk", UnlockPerk);
	}

	auto StatsMenuEx::ProcessMessage(RE::UIMessage& a_message)
		-> Result
	{
		using Message = RE::UI_MESSAGE_TYPE;

		// Not used right now.  It will be for developerment
		using ProcessMessage_t = decltype(&RE::StatsMenu::ProcessMessage);
		REL::Function<ProcessMessage_t> _ProcessMessage(REL::ID(51638));

		switch (a_message.type) {
		case Message::kShow:
			OnMenuOpen();
			return Result::kHandled;
		case Message::kHide:
			OnMenuClose();
			return Result::kHandled;
		default:
			return Base::ProcessMessage(a_message);
		}
	}


	void StatsMenuEx::Open()
	{
		if (!StatsMenuEx::s_active)
			return;

		auto uiQueue = RE::UIMessageQueue::GetSingleton();
		uiQueue->AddMessage(Name(), RE::UI_MESSAGE_TYPE::kShow, 0);
		uiQueue->AddMessage(RE::FaderMenu::MENU_NAME, RE::UI_MESSAGE_TYPE::kHide, 0);
		uiQueue->AddMessage(RE::StatsMenu::MENU_NAME, RE::UI_MESSAGE_TYPE::kHide, 0);
	}


	void StatsMenuEx::Close()
	{
		if (!StatsMenuEx::s_active)
			return;

		auto uiQueue = RE::UIMessageQueue::GetSingleton();
		uiQueue->AddMessage(Name(), RE::UI_MESSAGE_TYPE::kHide, 0);
	}


	void StatsMenuEx::Register()
	{
		auto ui = RE::UI::GetSingleton();
		ui->Register(Name(), Create);

		_MESSAGE("Registered %s", Name().data());
	}

	bool StatsMenuEx::SetActive(bool active)
	{
		auto ui = RE::UI::GetSingleton();
		if (ui->IsMenuOpen(Scaleform::StatsMenuEx::Name()) || ui->IsMenuOpen(RE::StatsMenu::MENU_NAME))
			return false;

		StatsMenuEx::s_active = active;

		return true;
	}

	bool StatsMenuEx::GetActive()
	{
		return StatsMenuEx::s_active;
	}


	RE::IMenu* StatsMenuEx::Create()
	{
		return new StatsMenuEx();
	}

	void StatsMenuEx::Log(const RE::FxDelegateArgs& a_params)
	{
		assert(a_params.GetArgCount() == 1);
		assert(a_params[0].IsString());

		_MESSAGE("%s: %s", Name().data(), a_params[0].GetString());
	}


	void StatsMenuEx::OnRootPress(const RE::FxDelegateArgs& a_params)
	{
		assert(a_params.GetArgCount() == 1);
		assert(a_params[0].IsNumber());

		auto menu = static_cast<StatsMenuEx*>(a_params.GetHandler());
		auto num = a_params[0].GetUInt();
		menu->OnRootPress(num);
	}


	void StatsMenuEx::OnTreePress(const RE::FxDelegateArgs& a_params)
	{
		assert(a_params.GetArgCount() == 1);
		assert(a_params[0].IsNumber());

		auto menu = static_cast<StatsMenuEx*>(a_params.GetHandler());
		auto num = a_params[0].GetUInt();
		menu->OnTreePress(num);
	}


	void StatsMenuEx::OnPerkPress(const RE::FxDelegateArgs& a_params)
	{
		assert(a_params.GetArgCount() == 2);
		assert(a_params[0].IsNumber());
		assert(a_params[1].IsNumber());

		auto menu = static_cast<StatsMenuEx*>(a_params.GetHandler());
		auto perkIndex = a_params[0].GetUInt();
		auto treeIndex = a_params[1].GetUInt();
		menu->OnPerkPress(perkIndex, treeIndex);
	}


	void StatsMenuEx::OnRankPress(const RE::FxDelegateArgs& a_params)
	{
		assert(a_params.GetArgCount() == 2);
		assert(a_params[0].IsNumber());
		assert(a_params[1].IsNumber());

		auto menu = static_cast<StatsMenuEx*>(a_params.GetHandler());
		auto rankIndex = a_params[0].GetUInt();
		auto treeIndex = a_params[1].GetUInt();
		menu->OnRankPress(rankIndex, treeIndex);
	}


	void StatsMenuEx::OnRequisitePress(const RE::FxDelegateArgs& a_params)
	{
		assert(a_params.GetArgCount() == 2);
		assert(a_params[0].IsNumber());
		assert(a_params[1].IsNumber());

		auto menu = static_cast<StatsMenuEx*>(a_params.GetHandler());
		auto requisiteIndex = a_params[0].GetUInt();
		auto treeIndex = a_params[1].GetUInt();
		menu->OnRequisitePress(requisiteIndex, treeIndex);
	}


	void StatsMenuEx::OnUnlockPress(const RE::FxDelegateArgs& a_params)
	{
		assert(a_params.GetArgCount() == 2);
		assert(a_params[0].IsNumber());
		assert(a_params[1].IsNumber());

		auto menu = static_cast<StatsMenuEx*>(a_params.GetHandler());
		auto unlockIndex = a_params[0].GetUInt();
		auto treeIndex = a_params[1].GetUInt();
		menu->OnUnlockPress(unlockIndex, treeIndex);
	}


	void StatsMenuEx::UnlockPerk(const RE::FxDelegateArgs& a_params)
	{
		assert(a_params.GetArgCount() == 3);
		assert(a_params[0].IsNumber());
		assert(a_params[1].IsNumber());
		assert(a_params[2].IsNumber());

		auto menu = static_cast<StatsMenuEx*>(a_params.GetHandler());
		auto rankIndex = a_params[0].GetUInt();
		auto perkIndex = a_params[1].GetUInt();
		auto treeIndex = a_params[2].GetUInt();
		menu->UnlockPerk(rankIndex, perkIndex, treeIndex);
	}


	void StatsMenuEx::OnMenuOpen()
	{
		auto bm = RE::UIBlurManager::GetSingleton();
		
		// set blur
		bm->IncrementBlurCount();

		auto uiQueue = RE::UIMessageQueue::GetSingleton();
		uiQueue->AddMessage(RE::FaderMenu::MENU_NAME, RE::UI_MESSAGE_TYPE::kHide, 0);

		bool success;
		view->SetVisible(true);
		std::vector<std::pair<CLIK::Object*, std::string>> toGet;
		toGet.push_back(std::make_pair(&_roots, "roots"));
		toGet.push_back(std::make_pair(&_trees, "trees"));
		toGet.push_back(std::make_pair(&_perks, "perks"));
		toGet.push_back(std::make_pair(&_ranks, "ranks"));
		toGet.push_back(std::make_pair(&_desc.header, "descHeader"));
		toGet.push_back(std::make_pair(&_desc.text, "descText"));
		toGet.push_back(std::make_pair(&_desc.requisites.header, "requisitesHeader"));
		toGet.push_back(std::make_pair(&_desc.requisites.list, "requisites"));
		toGet.push_back(std::make_pair(&_desc.unlocks.header, "unlocksHeader"));
		toGet.push_back(std::make_pair(&_desc.unlocks.list, "unlocks"));
		toGet.push_back(std::make_pair(&_desc.unlock, "unlock"));
		toGet.push_back(std::make_pair(&_stats.perkPoints, "perkPoints"));
		toGet.push_back(std::make_pair(&_stats.magicka, "magicka"));
		toGet.push_back(std::make_pair(&_stats.health, "health"));
		toGet.push_back(std::make_pair(&_stats.stamina, "stamina"));
		RE::GFxValue var;
		for (auto& elem : toGet) {
			success = view->GetVariable(&var, elem.second.c_str());
			assert(success);
			*elem.first = var;
		}

		CLIK::Object obj("ScrollBar");
		_roots.ScrollBar(obj);
		_trees.ScrollBar(obj);
		_perks.ScrollBar(obj);
		_ranks.ScrollBar(obj);

		_trees.Visible(false);
		_perks.Visible(false);
		_ranks.Visible(false);
		_desc.Visible(false);

		_desc.Init();
		_stats.Update();

		SetRoots();
	}

	static int count = 0;
	void TaskFunc() {
		auto pc = RE::PlayerCamera::GetSingleton();
		pc->currentState.get()->Update(pc->cameraStates[RE::CameraStates::kTween]);
		Sleep(100);

		if (--count > 0)
		{
			auto task = SKSE::GetTaskInterface();
			task->AddTask(TaskFunc);
		}
	};

	void StatsMenuEx::OnMenuClose()
	{
		auto bm = RE::UIBlurManager::GetSingleton();
		// Remove blur
		bm->DecrementBlurCount();
		
		auto ui = RE::UI::GetSingleton();
		if (ui->IsMenuOpen(RE::TweenMenu::MENU_NAME))
		{
			// Start to close the animation
			TweenMenu::StartCloseMenu();

			// Close the Tween Menu, This function needs to be called to allow the animation to finish;
			TweenMenu::CloseMenu();
		}

		// Make sure we are back on the Hud
		auto uiQueue = RE::UIMessageQueue::GetSingleton();
		uiQueue->AddMessage(RE::FaderMenu::MENU_NAME, RE::UI_MESSAGE_TYPE::kHide, 0);
		uiQueue->AddMessage(RE::CursorMenu::MENU_NAME, RE::UI_MESSAGE_TYPE::kHide, 0);
	}


	void StatsMenuEx::InitExtensions()
	{
		RE::GFxValue boolean(true);
		bool success;

		success = view->SetVariable("_global.gfxExtensions", boolean);
		assert(success);
		success = view->SetVariable("_global.noInvisibleAdvance", boolean);
		assert(success);
	}


	void StatsMenuEx::SetRoots()
	{
		CLIK::Array arr(view);
		CLIK::Object elem;
		for (auto& root : _rootMappings) {
			elem = root.text;
			arr.Push(elem);
		}

		_roots.DataProvider(arr);
	}


	void StatsMenuEx::UnlockPerk(std::size_t a_rankIdx, std::size_t a_perkIdx, std::size_t a_treeIdx)
	{
		assert(a_rankIdx < _rankMappings.size());
		assert(a_perkIdx < _perkMappings.size());
		assert(a_treeIdx < _treeMappings.size());

		_desc.unlock.Disabled(true);

		auto& rank = _rankMappings[a_rankIdx];
		auto perk = RE::TESForm::LookupByID<RE::BGSPerk>(rank.perkID);
		if (!perk) {
			return;
		}

		auto player = RE::PlayerCharacter::GetSingleton();
		player->AddPerk(perk);
		_stats.DecPerkPoints();

		auto idToFind = _perkMappings[a_perkIdx].perkID;
		UpdatePerks(a_treeIdx);

		CLIK::Array arr(view);
		CLIK::Object str;
		for (auto& mappedPerk : _perkMappings) {
			str = mappedPerk.text;
			arr.Push(str);
		}

		_perks.DataProvider(arr);

		for (std::size_t i = 0; i < _perkMappings.size(); ++i) {
			if (_perkMappings[i].perkID == idToFind) {
				_perks.SelectedIndex(static_cast<double>(i));
				break;
			}
		}

		_stats.Update();
	}


	bool StatsMenuEx::OnRootPress(std::size_t a_classIdx)
	{
		CLIK::Array arr(view);
		UpdateTrees(a_classIdx);

		if (!_treeMappings.empty()) {
			CLIK::Object str;
			for (auto& tree : _treeMappings) {
				str = tree.text;
				arr.Push(str);
			}
			_trees.Visible(true);
			InvalidatePerks();
			_trees.DataProvider(arr);
			_trees.SelectedIndex(-1);
			return true;
		} else {
			InvalidateTrees();
			return false;
		}
	}


	bool StatsMenuEx::OnTreePress(std::size_t a_treeIdx)
	{
		CLIK::Array arr(view);
		UpdatePerks(a_treeIdx);

		if (!_perkMappings.empty()) {
			CLIK::Object str;
			for (auto& perk : _perkMappings) {
				str = perk.text;
				arr.Push(str);
			}
			_perks.Visible(true);
			InvalidateRanks();
			_perks.DataProvider(arr);
			_perks.SelectedIndex(-1);
			return true;
		} else {
			InvalidatePerks();
			return false;
		}
	}


	bool StatsMenuEx::OnPerkPress(std::size_t a_perkIdx, std::size_t a_treeIdx)
	{
		CLIK::Array arr(view);
		UpdateRanks(a_perkIdx);

		if (!_rankMappings.empty()) {
			CLIK::Object str;
			for (auto& rank : _rankMappings) {
				str = rank.text;
				arr.Push(str);
			}
			_ranks.Visible(true);
			InvalidateDesc();
			_ranks.DataProvider(arr);
			_ranks.SelectedIndex(-1);
			if (OnRankPress(0, a_treeIdx)) {
				_ranks.SelectedIndex(0);
			}
			return true;
		} else {
			InvalidateRanks();
			return false;
		}
	}


	bool StatsMenuEx::OnRankPress(std::size_t a_rankIdx, std::size_t a_treeIdx)
	{
		if (a_rankIdx >= _rankMappings.size()) {
			InvalidateDesc();
			return false;
		}

		auto& elem = _rankMappings[a_rankIdx];
		auto perk = RE::TESForm::LookupByID<RE::BGSPerk>(elem.perkID);
		RE::BSString desc;
		perk->GetDescription(desc, perk);

		_desc.Visible(true);
		_desc.header.Text("Description");
		_desc.text.Text(desc);
		_desc.unlock.Label("Unlock");

		auto player = RE::PlayerCharacter::GetSingleton();
		bool disabled = _stats.GetPerkPoints() == 0 || player->HasPerk(perk) || !perk->perkConditions.IsTrue(player, player);

#if 0
		// this second check might be unnecessary, the vanilla game seems to base perk eligibility on the previous check
		if (!disabled) {
			if (a_rankIdx > 0) {
				auto prevPerk = RE::TESForm::LookupByID<RE::BGSPerk>(_rankMappings[a_rankIdx - 1].second);
				disabled = !player->HasPerk(prevPerk);
			} else {
				auto perkIDToFind = _rankMappings[0].second;
				auto av = RE::TESForm::LookupByID<RE::ActorValueInfo>(_treeMappings[a_treeIdx].second);
				BFSOnPerkTree(av, [&](RE::BGSSkillPerkTreeNode* a_node) -> bool
				{
					if (a_node->perk && a_node->perk->formID == perkIDToFind) {
						for (auto& parent : a_node->parents) {
							if (parent->perk && !player->HasPerk(parent->perk)) {
								disabled = true;
								break;
							}
						}
						return false;
					}
					return true;
				});
			}
		}
#endif

		_desc.unlock.Disabled(disabled);
		UpdateLeads(a_rankIdx, a_treeIdx);
		return true;
	}


	bool StatsMenuEx::OnRequisitePress(std::size_t a_requisiteIdx, std::size_t a_treeIdx)
	{
		return OnLeadPress(_requisiteMappings, a_requisiteIdx, a_treeIdx);
	}


	bool StatsMenuEx::OnUnlockPress(std::size_t a_unlockIdx, std::size_t a_treeIdx)
	{
		return OnLeadPress(_unlockMappings, a_unlockIdx, a_treeIdx);
	}


	bool StatsMenuEx::OnLeadPress(std::vector<TextPerkLevel>& a_lead, std::size_t a_leadIdx, std::size_t a_treeIdx)
	{
		if (a_leadIdx >= a_lead.size()) {
			return false;
		}

		auto& lead = a_lead[a_leadIdx];
		for (std::size_t i = 0; i < _perkMappings.size(); ++i) {
			if (_perkMappings[i].perkID == lead.perkID) {
				_perks.SelectedIndex(i);
				UpdateRanks(i);
				CLIK::Array arr(view);
				CLIK::Object str;
				for (auto& rank : _rankMappings) {
					str = rank.text;
					arr.Push(str);
				}
				_ranks.DataProvider(arr);
				_ranks.SelectedIndex(0);
				OnRankPress(0, a_treeIdx);
				break;
			}
		}

		return true;
	}


	void StatsMenuEx::UpdateTrees(std::size_t a_classIdx)
	{
		using value_type = decltype(_treeMappings)::value_type;

		_treeMappings.clear();
		_stats.SetDefault();

		if (a_classIdx < _rootMappings.size()) {
			auto& elem = _rootMappings[a_classIdx];
			auto player = RE::PlayerCharacter::GetSingleton();
			for (auto& info : elem.info) {
				switch (info.id) {
				case kAVVampire:
					_stats.SetVampire();
					break;
				case kAVWerewolf:
					_stats.SetWerewolf();
					break;
				}

				auto avInfo = RE::TESForm::LookupByID<RE::ActorValueInfo>(info.id);
				if (avInfo && !avInfo->fullName.empty()) {
					std::string name(avInfo->fullName);
					SanitizeString(name);

					if (info.av != RE::ActorValue::kNone) {
						auto baseVal = static_cast<UInt32>(player->GetActorValue(info.av));
						name += " (";
						name += std::to_string(baseVal);
						name += ')';
					}

					_treeMappings.push_back({ std::move(name), info.id });
				}
			}

			if (elem.sort) {
				std::sort(_treeMappings.begin(), _treeMappings.end(), [](const value_type& a_lhs, const value_type& a_rhs) -> bool
				{
					return a_lhs.text < a_rhs.text;
				});
			}
		}

		_stats.Update();
	}


	void StatsMenuEx::UpdatePerks(std::size_t a_treeIdx)
	{
		using value_type = decltype(_perkMappings)::value_type;

		_perkMappings.clear();

		if (a_treeIdx < _treeMappings.size()) {
			auto& elem = _treeMappings[a_treeIdx];
			auto av = RE::TESForm::LookupByID<RE::ActorValueInfo>(elem.avInfoID);
			auto player = RE::PlayerCharacter::GetSingleton();
			BFSOnPerkTree(av, [&](RE::BGSSkillPerkTreeNode* a_node) -> bool
			{
				auto perk = a_node->perk;
				if (perk && !perk->fullName.empty()) {
					std::string name(perk->fullName);
					SanitizeString(name);

					UInt32 level = 0;
					bool incomplete = false;
					if (!_stats.IsBeastMode()) {
						for (auto rank = perk; rank; rank = rank->nextPerk) {
							level = GetPerkLvlReq(rank).value_or(level);
							if (!player->HasPerk(rank)) {
								incomplete = true;
								name += " (";
								name += std::to_string(level);
								name += ')';
								break;
							}
						}
					}

					if (!incomplete) {
						level = value_type::kInvalid;
					}

					_perkMappings.push_back({ std::move(name), perk->formID, level });
				}
				return true;
			});
		}

		std::stable_sort(_perkMappings.begin(), _perkMappings.end(), [](const value_type& a_lhs, const value_type& a_rhs) -> bool
		{
			return a_lhs.level < a_rhs.level;
		});
	}


	void StatsMenuEx::UpdateRanks(std::size_t a_perkIdx)
	{
		_rankMappings.clear();

		if (a_perkIdx < _perkMappings.size()) {
			auto& elem = _perkMappings[a_perkIdx];
			auto perk = RE::TESForm::LookupByID<RE::BGSPerk>(elem.perkID);
			std::size_t idx = 1;
			std::string name;
			UInt32 reqRank = 0;
			while (perk) {
				name = std::to_string(idx++);

				if (!_stats.IsBeastMode()) {
					reqRank = GetPerkLvlReq(perk).value_or(reqRank);
					name += " (";
					name += std::to_string(reqRank);
					name += ')';
				}

				_rankMappings.push_back({ std::move(name), perk->formID });
				perk = perk->nextPerk;
			}
		}
	}


	void StatsMenuEx::UpdateLeads(std::size_t a_rankIdx, std::size_t a_treeIdx)
	{
		_requisiteMappings.clear();
		_unlockMappings.clear();

		if (a_rankIdx == 0) {
			_desc.requisites.list.SelectedIndex(-1);
			_desc.unlocks.list.SelectedIndex(-1);
			auto perkIDToFind = _rankMappings[0].perkID;
			auto av = RE::TESForm::LookupByID<RE::ActorValueInfo>(_treeMappings[a_treeIdx].avInfoID);
			BFSOnPerkTree(av, [&](RE::BGSSkillPerkTreeNode* a_node) -> bool
			{
				if (a_node->perk && a_node->perk->formID == perkIDToFind) {
					UpdateLead(_requisiteMappings, a_node->parents, _desc.requisites);
					UpdateLead(_unlockMappings, a_node->children, _desc.unlocks);
					return false;
				}
				return true;
			});
		} else {
			CLIK::Array arr(view);
			_desc.requisites.list.DataProvider(arr);
			_desc.requisites.list.Disabled(true);
			_desc.unlocks.list.DataProvider(arr);
			_desc.unlocks.list.Disabled(true);
		}
	}


	void StatsMenuEx::UpdateLead(std::vector<TextPerkLevel>& a_lead, RE::BSTArray<RE::BGSSkillPerkTreeNode*>& a_srcArr, HeaderList& a_headerList)
	{
		using value_type = std::decay_t<decltype(a_lead)>::value_type;

		bool disabled = true;
		auto player = RE::PlayerCharacter::GetSingleton();

		for (auto& node : a_srcArr) {
			if (node->perk && !node->perk->fullName.empty()) {
				disabled = false;
				std::string name(node->perk->fullName);
				SanitizeString(name);

				UInt32 level = value_type::kInvalid;
				if (!_stats.IsBeastMode() && !player->HasPerk(node->perk)) {
					level = GetPerkLvlReq(node->perk).value_or(0);
					name += " (";
					name += std::to_string(level);
					name += ')';
				}

				a_lead.push_back({ std::move(name), node->perk->formID, level });
			}
		}

		std::stable_sort(a_lead.begin(), a_lead.end(), [](const value_type& a_lhs, const value_type& a_rhs) -> bool
		{
			return a_lhs.level < a_rhs.level;
		});

		CLIK::Array arr(view);
		CLIK::Object str;
		for (auto& elem : a_lead) {
			str = elem.text;
			arr.Push(str);
		}

		a_headerList.list.DataProvider(arr);
		a_headerList.list.Disabled(disabled);
	}


	void StatsMenuEx::InvalidateTrees()
	{
		_trees.Visible(false);
		_trees.SelectedIndex(-1);
		InvalidatePerks();
	}


	void StatsMenuEx::InvalidatePerks()
	{
		_perks.Visible(false);
		_perks.SelectedIndex(-1);
		InvalidateRanks();
	}


	void StatsMenuEx::InvalidateRanks()
	{
		_ranks.Visible(false);
		_ranks.SelectedIndex(-1);
		InvalidateDesc();
	}


	void StatsMenuEx::InvalidateDesc()
	{
		_desc.Visible(false);
	}


	void StatsMenuEx::BFSOnPerkTree(RE::ActorValueInfo* a_av, std::function<bool(RE::BGSSkillPerkTreeNode*)> a_predicate)
	{
		if (!a_av || !a_av->perkTree) {
			return;
		}

		std::unordered_set<UInt32> visited;
		std::queue<RE::BGSSkillPerkTreeNode*> q;
		auto node = a_av->perkTree;
		q.push(node);
		visited.insert(node->index);
		do {
			node = q.front();
			q.pop();
			if (!a_predicate(node)) {
				break;
			}
			for (auto& child : node->children) {
				auto result = visited.insert(child->index);
				if (result.second) {
					q.push(child);
				}
			}
		} while (!q.empty());
	}


	std::optional<UInt32> StatsMenuEx::GetPerkLvlReq(RE::BGSPerk* a_perk)
	{
		using FunctionID = RE::FUNCTION_DATA::FunctionID;

		for (auto cond = a_perk->perkConditions.head; cond; cond = cond->next) {
			if (cond->data.functionData.function == FunctionID::kGetBaseActorValue) {
				return std::make_optional(static_cast<UInt32>(cond->data.comparisonValue.f));
			}
		}
		return std::nullopt;
	}


	void StatsMenuEx::SanitizeString(std::string& a_str)
	{
		while (!a_str.empty() && std::isspace(a_str.back())) {
			a_str.pop_back();
		}
		while (!a_str.empty() && std::isspace(a_str.front())) {
			a_str.assign(a_str, 1);
		}
	}


}
