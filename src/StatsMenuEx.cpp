#include "StatsMenuEx.h"

#include <queue>
#include <set>
#include <unordered_set>


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


		void ScrollingList::SetVisible(bool a_visible)
		{
			bool success;

			RE::GFxValue boolean(a_visible);
			success = list.SetMember("visible", boolean);
			assert(success);
			success = bar.SetMember("visible", boolean);
			assert(success);

			if (a_visible) {
				RE::GFxValue number(-1.0);
				success = list.SetMember("selectedIndex", number);
				assert(success);
			}
		}


		void ScrollingList::SetDataProvider(const RE::GFxValue& a_data)
		{
			auto success = list.SetMember("dataProvider", a_data);
			assert(success);
		}


		void Button::SetVisible(bool a_visible)
		{
			RE::GFxValue boolean(a_visible);
			auto success = btn.SetMember("visible", boolean);
			assert(success);
		}


		void Button::SetDisabled(bool a_disabled)
		{
			RE::GFxValue boolean(a_disabled);
			auto success = btn.SetMember("disabled", boolean);
			assert(success);
		}


		void Description::Init()
		{
			bool success;

			RE::GFxValue boolean(true);
			success = header.SetMember("wordWrap", boolean);
			assert(success);
			success = text.SetMember("wordWrap", boolean);
			assert(success);
		}


		void Description::SetVisible(bool a_visible)
		{
			bool success;

			RE::GFxValue boolean(a_visible);
			success = header.SetMember("_visible", boolean);
			assert(success);
			success = text.SetMember("_visible", boolean);
			assert(success);
			unlock.SetVisible(a_visible);
		}


		void Description::SetHeader(const std::string_view& a_header)
		{
			RE::GFxValue str(a_header.data());
			auto success = header.SetMember("htmlText", str);
			assert(success);
		}


		void Description::SetText(const std::string_view& a_text)
		{
			RE::GFxValue str(a_text.data());
			auto success = text.SetMember("htmlText", str);
			assert(success);
		}


		void Description::SetUnlockText(const std::string_view& a_text)
		{
			RE::GFxValue str(a_text.data());
			auto success = unlock.btn.SetMember("label", str);
			assert(success);
		}


		void Description::SetUnlockDisabled(bool a_disabled)
		{
			unlock.SetDisabled(a_disabled);
		}
	}


	StatsMenuEx::StatsMenuEx() :
		_classes(),
		_trees(),
		_perks(),
		_ranks(),
		_desc(),
		_classMappings(),
		_treeMappings(),
		_perkMappings(),
		_rankMappings()
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

		view->SetVisible(false);
	}


	void StatsMenuEx::Accept(RE::FxDelegateHandler::CallbackProcessor* a_processor)
	{
		a_processor->Process("Log", Log);
		a_processor->Process("OnClassPress", OnClassPress);
		a_processor->Process("OnTreePress", OnTreePress);
		a_processor->Process("OnPerkPress", OnPerkPress);
		a_processor->Process("OnRankPress", OnRankPress);
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


	void StatsMenuEx::UnlockPerk(const RE::FxDelegateArgs& a_params)
	{
		assert(a_params.GetArgCount() == 1);
		assert(a_params[0].IsNumber());

		auto menu = static_cast<StatsMenuEx*>(a_params.GetHandler());
		auto num = a_params[0].GetUInt();
		menu->UnlockPerk(num);
	}


	void StatsMenuEx::OnMenuOpen()
	{
		bool success;
		view->SetVisible(true);

		std::vector<std::pair<RE::GFxValue*, std::string>> toGet;
		toGet.push_back(std::make_pair(&_classes.list, "classes"));
		toGet.push_back(std::make_pair(&_classes.bar, "classesSB"));
		toGet.push_back(std::make_pair(&_trees.list, "trees"));
		toGet.push_back(std::make_pair(&_trees.bar, "treesSB"));
		toGet.push_back(std::make_pair(&_perks.list, "perks"));
		toGet.push_back(std::make_pair(&_perks.bar, "perksSB"));
		toGet.push_back(std::make_pair(&_ranks.list, "ranks"));
		toGet.push_back(std::make_pair(&_ranks.bar, "ranksSB"));
		toGet.push_back(std::make_pair(&_desc.header, "descHeader"));
		toGet.push_back(std::make_pair(&_desc.text, "descText"));
		toGet.push_back(std::make_pair(&_desc.unlock.btn, "unlock"));
		for (auto& elem : toGet) {
			success = view->GetVariable(elem.first, elem.second.c_str());
			assert(success);
		}

		_trees.SetVisible(false);
		_perks.SetVisible(false);
		_ranks.SetVisible(false);
		_desc.SetVisible(false);

		_desc.Init();

		SetClasses();
	}


	void StatsMenuEx::OnMenuClose()
	{
		return;
	}


	void StatsMenuEx::SetClasses()
	{
		std::vector<std::string> names;
		names.emplace_back(DRAGONBORN);
		names.emplace_back(VAMPIRE);
		names.emplace_back(WEREWOLF);

		RE::GFxValue arr;
		view->CreateArray(&arr);
		assert(arr.IsArray());

		RE::GFxValue str;
		for (auto& name : names) {
			str = name.c_str();
			arr.PushBack(str);
		}

		_classes.SetDataProvider(arr);
	}


	void StatsMenuEx::UnlockPerk(std::size_t a_idx)
	{
		_desc.SetUnlockDisabled(true);

		if (a_idx >= _rankMappings.size()) {
			return;
		}

		auto& rank = _rankMappings[a_idx];
		auto perk = RE::TESForm::LookupByID<RE::BGSPerk>(rank.perkID);
		if (!perk) {
			return;
		}

		auto player = RE::PlayerCharacter::GetSingleton();
		player->AddPerk(perk);
	}


	void StatsMenuEx::OnClassPress(std::size_t a_idx)
	{
		RE::GFxValue arr;
		view->CreateArray(&arr);
		assert(arr.IsArray());

		_treeMappings.clear();

		if (a_idx < _classMappings.size()) {
			auto& elem = _classMappings[a_idx];
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

		if (!_treeMappings.empty()) {
			using value_type = decltype(_treeMappings)::value_type;
			std::sort(_treeMappings.begin(), _treeMappings.end(), [](const value_type& a_lhs, const value_type& a_rhs) -> bool
			{
				return a_lhs.text < a_rhs.text;
			});
			for (auto& tree : _treeMappings) {
				arr.PushBack(tree.text.c_str());
			}
			_trees.SetVisible(true);
			InvalidatePerks();
			_trees.SetDataProvider(arr);
		} else {
			InvalidateTrees();
		}
	}


	void StatsMenuEx::OnTreePress(std::size_t a_idx)
	{
		RE::GFxValue arr;
		view->CreateArray(&arr);
		assert(arr.IsArray());

		_perkMappings.clear();

		if (a_idx < _treeMappings.size()) {
			auto& elem = _treeMappings[a_idx];
			auto av = RE::TESForm::LookupByID<RE::ActorValueInfo>(elem.avInfoID);
			BFSOnPerkTree(av, [&](RE::BGSSkillPerkTreeNode* a_node) -> bool
			{
				auto perk = a_node->perk;
				if (perk && !perk->name.empty()) {
					std::string name(perk->name);
					SanitizeString(name);

					auto level = GetPerkLvlReq(perk).value_or(0);

					name += " (";
					name += std::to_string(level);
					name += ')';

					_perkMappings.push_back({ std::move(name), perk->formID, level });
				}
				return true;
			});
		}

		if (!_perkMappings.empty()) {
			std::stable_sort(_perkMappings.begin(), _perkMappings.end(), [](const Perk& a_lhs, const Perk& a_rhs) -> bool
			{
				return a_lhs.level < a_rhs.level;
			});

			for (auto& perk : _perkMappings) {
				arr.PushBack(perk.text.c_str());
			}

			_perks.SetVisible(true);
			InvalidateRanks();
			_perks.SetDataProvider(arr);
		} else {
			InvalidatePerks();
		}
	}


	void StatsMenuEx::OnPerkPress(std::size_t a_idx)
	{
		RE::GFxValue arr;
		view->CreateArray(&arr);
		assert(arr.IsArray());

		_rankMappings.clear();

		if (a_idx < _perkMappings.size()) {
			auto& elem = _perkMappings[a_idx];
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

		if (!_rankMappings.empty()) {
			for (auto& rank : _rankMappings) {
				arr.PushBack(rank.text.c_str());
			}
			_ranks.SetVisible(true);
			InvalidateDesc();
			_ranks.SetDataProvider(arr);
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

		_desc.SetVisible(true);
		_desc.SetHeader("Description");
		_desc.SetText(desc);
		_desc.SetUnlockText("Unlock");

		auto player = RE::PlayerCharacter::GetSingleton();
		bool disabled = player->HasPerk(perk) || !perk->conditions.Run(player, player);
		
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

		_desc.SetUnlockDisabled(disabled);

#if 0
		auto perkIDToFind = _rankMappings[0].second;
		auto av = RE::TESForm::LookupByID<RE::ActorValueInfo>(_treeMappings[a_treeIdx].second);
		BFSOnPerkTree(av, [&](RE::BGSSkillPerkTreeNode* a_node) -> bool
		{
			if (a_node->perk && a_node->perk->formID == perkIDToFind) {
				for (auto& parent : a_node->parents) {
					if (parent->perk && !parent->perk->name.empty()) {
						// push back required
					}
				}
				for (auto& child : a_node->children) {
					if (child->perk && !child->perk->name.empty()) {
						// push back unlocks
					}
				}
				return false;
			}
			return true;
		});
#endif
	}


	void StatsMenuEx::InvalidateTrees()
	{
		_trees.SetVisible(false);
		InvalidatePerks();
	}


	void StatsMenuEx::InvalidatePerks()
	{
		_perks.SetVisible(false);
		InvalidateRanks();
	}


	void StatsMenuEx::InvalidateRanks()
	{
		_ranks.SetVisible(false);
		InvalidateDesc();
	}


	void StatsMenuEx::InvalidateDesc()
	{
		_desc.SetVisible(false);
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
				if (visited.find(child->index) == visited.end()) {
					visited.insert(child->index);
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
