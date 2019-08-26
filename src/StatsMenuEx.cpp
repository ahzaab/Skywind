#include "StatsMenuEx.h"

#include <queue>
#include <unordered_set>

#include "CLIK/Array.h"


namespace Scaleform
{
	namespace
	{
		ClassMap::ClassMap() :
			Base()
		{
			using AV = RE::ActorValue;

			Base::value_type::first_type key;
			Base::value_type::second_type value;

			key = DRAGONBORN;
			value.clear();
			value.push_back(std::make_pair(kAVOneHanded, AV::kOneHanded));
			value.push_back(std::make_pair(kAVTwoHanded, AV::kTwoHanded));
			value.push_back(std::make_pair(kAVMarksman, AV::kArchery));
			value.push_back(std::make_pair(kAVBlock, AV::kBlock));
			value.push_back(std::make_pair(kAVSmithing, AV::kSmithing));
			value.push_back(std::make_pair(kAVHeavyArmor, AV::kHeavyArmor));
			value.push_back(std::make_pair(kAVLightArmor, AV::kLightArmor));
			value.push_back(std::make_pair(kAVPickpocket, AV::kPickpocket));
			value.push_back(std::make_pair(kAVLockpicking, AV::kLockpicking));
			value.push_back(std::make_pair(kAVSneak, AV::kSneak));
			value.push_back(std::make_pair(kAVAlchemy, AV::kAlchemy));
			value.push_back(std::make_pair(kAVSpeechCraft, AV::kSpeech));
			value.push_back(std::make_pair(kAVAlteration, AV::kAlteration));
			value.push_back(std::make_pair(kAVConjuration, AV::kConjuration));
			value.push_back(std::make_pair(kAVDestruction, AV::kDestruction));
			value.push_back(std::make_pair(kAVMysticism, AV::kIllusion));
			value.push_back(std::make_pair(kAVRestoration, AV::kRestoration));
			value.push_back(std::make_pair(kAVEnchanting, AV::kEnchanting));
			push_back(std::make_pair(key, value));

			key = VAMPIRE;
			value.clear();
			value.push_back(std::make_pair(kAVMagickaRateMod, AV::kMagickaRate));
			push_back(std::make_pair(key, value));

			key = WEREWOLF;
			value.clear();
			value.push_back(std::make_pair(kAVHealRatePowerMod, AV::kHealRateMult));
			push_back(std::make_pair(key, value));
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


		void Stats::Update()
		{
			auto player = RE::PlayerCharacter::GetSingleton();
			std::string text;

			text = "Perk Points: ";
			text += std::to_string(player->numPerkPoints);
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


		std::string Stats::BuildStatString(RE::ActorValue a_av)
		{
			auto player = RE::PlayerCharacter::GetSingleton();
			auto fBase = player->GetActorValueBase(a_av);
			auto fCur = player->GetActorValueMaximum(a_av);

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


	StatsMenuEx::StatsMenuEx() :
		_classes(),
		_trees(),
		_perks(),
		_ranks(),
		_desc(),
		_stats(),
		_classMappings(),
		_treeMappings(),
		_perkMappings(),
		_rankMappings(),
		_requisiteMappings(),
		_unlockMappings()
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
			_FATALERROR("StatsMenuEx did not have a view due to missing dependencies! Aborting process!\n");
			MessageBoxA(NULL, "StatsMenuEx did not have a view due to missing dependencies!\r\nAborting process!", NULL, MB_OK);
			std::abort();
		}

		InitExtensions();
		view->SetVisible(false);
	}


	void StatsMenuEx::Accept(RE::FxDelegateHandler::CallbackProcessor* a_processor)
	{
		a_processor->Process("Log", Log);
		a_processor->Process("OnClassPress", OnClassPress);
		a_processor->Process("OnTreePress", OnTreePress);
		a_processor->Process("OnPerkPress", OnPerkPress);
		a_processor->Process("OnRankPress", OnRankPress);
		a_processor->Process("OnRequisitePress", OnRequisitePress);
		a_processor->Process("OnUnlockPress", OnUnlockPress);
		a_processor->Process("UnlockPerk", UnlockPerk);
	}


	auto StatsMenuEx::ProcessMessage(RE::UIMessage* a_message)
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


	void StatsMenuEx::Open()
	{
		using Message = RE::UIMessage::Message;

		auto ui = RE::UIManager::GetSingleton();
		ui->AddMessage(Name(), Message::kOpen, 0);
	}


	void StatsMenuEx::Close()
	{
		using Message = RE::UIMessage::Message;

		auto ui = RE::UIManager::GetSingleton();
		ui->AddMessage(Name(), Message::kClose, 0);
	}


	void StatsMenuEx::Register()
	{
		auto mm = RE::MenuManager::GetSingleton();
		mm->Register(Name(), Create);

		_MESSAGE("Registered %s", Name().data());
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


	void StatsMenuEx::OnClassPress(const RE::FxDelegateArgs& a_params)
	{
		assert(a_params.GetArgCount() == 1);
		assert(a_params[0].IsNumber());

		auto menu = static_cast<StatsMenuEx*>(a_params.GetHandler());
		auto num = a_params[0].GetUInt();
		menu->OnClassPress(num);
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
		assert(a_params.GetArgCount() == 1);
		assert(a_params[0].IsNumber());

		auto menu = static_cast<StatsMenuEx*>(a_params.GetHandler());
		auto num = a_params[0].GetUInt();
		menu->OnPerkPress(num);
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
		bool success;
		view->SetVisible(true);

		std::vector<std::pair<CLIK::Object*, std::string>> toGet;
		toGet.push_back(std::make_pair(&_classes, "classes"));
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

		CLIK::Object obj("scrollBar");
		_classes.ScrollBar(obj);
		_trees.ScrollBar(obj);
		_perks.ScrollBar(obj);
		_ranks.ScrollBar(obj);

		_trees.Visible(false);
		_perks.Visible(false);
		_ranks.Visible(false);
		_desc.Visible(false);

		_desc.Init();
		_stats.Update();

		SetClasses();
	}


	void StatsMenuEx::OnMenuClose()
	{
		return;
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


	void StatsMenuEx::SetClasses()
	{
		std::vector<std::string> names;
		names.emplace_back(DRAGONBORN);
		names.emplace_back(VAMPIRE);
		names.emplace_back(WEREWOLF);

		CLIK::Array arr(view);
		CLIK::Object elem;
		for (auto& name : names) {
			elem = name;
			arr.Push(elem);
		}

		_classes.DataProvider(arr);
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
		player->numPerkPoints -= 1;

		auto idToFind = _perkMappings[a_perkIdx].perkID;
		UpdatePerks(a_treeIdx);

		CLIK::Array arr(view);
		CLIK::Object str;
		for (auto& perk : _perkMappings) {
			str = perk.text;
			arr.Push(str);
		}

		_perks.DataProvider(arr);

		for (std::size_t i = 0; i < _perkMappings.size(); ++i) {
			if (_perkMappings[i].perkID == idToFind) {
				_perks.SelectedIndex(i);
				break;
			}
		}

		_stats.Update();
	}


	void StatsMenuEx::OnClassPress(std::size_t a_classIdx)
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
		} else {
			InvalidateTrees();
		}
	}


	void StatsMenuEx::OnTreePress(std::size_t a_treeIdx)
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
		} else {
			InvalidatePerks();
		}
	}


	void StatsMenuEx::OnPerkPress(std::size_t a_perkIdx)
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
		} else {
			InvalidateRanks();
		}
	}


	void StatsMenuEx::OnRankPress(std::size_t a_rankIdx, std::size_t a_treeIdx)
	{
		if (a_rankIdx >= _rankMappings.size()) {
			InvalidateDesc();
			return;
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
		bool disabled = player->numPerkPoints == 0 || player->HasPerk(perk) || !perk->conditions.Run(player, player);
		
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
	}


	void StatsMenuEx::OnRequisitePress(std::size_t a_requisiteIdx, std::size_t a_treeIdx)
	{
		OnLeadPress(_requisiteMappings, a_requisiteIdx, a_treeIdx);
	}


	void StatsMenuEx::OnUnlockPress(std::size_t a_unlockIdx, std::size_t a_treeIdx)
	{
		OnLeadPress(_unlockMappings, a_unlockIdx, a_treeIdx);
	}


	void StatsMenuEx::OnLeadPress(std::vector<TextPerkLevel>& a_lead, std::size_t a_leadIdx, std::size_t a_treeIdx)
	{
		if (a_leadIdx >= a_lead.size()) {
			return;
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
	}


	void StatsMenuEx::UpdateTrees(std::size_t a_classIdx)
	{
		using value_type = decltype(_treeMappings)::value_type;

		_treeMappings.clear();

		if (a_classIdx < _classMappings.size()) {
			auto& elem = _classMappings[a_classIdx];
			auto player = RE::PlayerCharacter::GetSingleton();
			for (auto& pair : elem.second) {
				auto avInfoID = pair.first;
				auto av = pair.second;
				auto avInfo = RE::TESForm::LookupByID<RE::ActorValueInfo>(avInfoID);
				if (avInfo && !avInfo->name.empty()) {
					std::string name(avInfo->name);
					SanitizeString(name);

					auto baseVal = static_cast<UInt32>(player->GetActorValueBase(av));

					name += " (";
					name += std::to_string(baseVal);
					name += ')';

					_treeMappings.push_back({ std::move(name), avInfoID });
				}
			}
		}

		std::sort(_treeMappings.begin(), _treeMappings.end(), [](const value_type& a_lhs, const value_type& a_rhs) -> bool
		{
			return a_lhs.text < a_rhs.text;
		});
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
				if (perk && !perk->name.empty()) {
					std::string name(perk->name);
					SanitizeString(name);

					UInt32 level = 0;
					bool incomplete = false;
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
				reqRank = GetPerkLvlReq(perk).value_or(reqRank);

				name += " (";
				name += std::to_string(reqRank);
				name += ')';

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
			if (node->perk && !node->perk->name.empty()) {
				disabled = false;
				std::string name(node->perk->name);
				SanitizeString(name);

				UInt32 level = value_type::kInvalid;
				if (!player->HasPerk(node->perk)) {
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


	void StatsMenuEx::BFSOnPerkTree(RE::ActorValueInfo* a_av, llvm::function_ref<bool(RE::BGSSkillPerkTreeNode*)> a_predicate)
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
		using FunctionID = RE::Condition::FunctionID;

		for (auto cond = a_perk->conditions.head; cond; cond = cond->next) {
			if (cond->functionID == FunctionID::kGetBaseActorValue) {
				return std::make_optional(static_cast<UInt32>(cond->comparisonValue));
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
