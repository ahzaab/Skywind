#pragma once

#include <optional>
#include <string>
#include <string_view>
#include <utility>
#include <vector>

#include "RE/Skyrim.h"

#include "CLIK/Button.h"
#include "CLIK/ScrollingList.h"
#include "CLIK/TextField.h"

namespace Scaleform
{
	namespace
	{
		using ActorValueInfoID = RE::FormID;
		using BGSPerkID = RE::FormID;


		constexpr char DRAGONBORN[] = "Dragonborn";
		constexpr char VAMPIRE[] = "Vampire";
		constexpr char WEREWOLF[] = "Werewolf";
		constexpr char ATTRIBUTES[] = "Attributes";


		enum : ActorValueInfoID
		{
			kAVOneHanded = 0x0000044C,
			kAVTwoHanded = 0x0000044D,
			kAVMarksman = 0x0000044E,
			kAVBlock = 0x0000044F,
			kAVSmithing = 0x00000450,
			kAVHeavyArmor = 0x00000451,
			kAVLightArmor = 0x00000452,
			kAVPickpocket = 0x00000453,
			kAVLockpicking = 0x00000454,
			kAVSneak = 0x00000455,
			kAVAlchemy = 0x00000456,
			kAVSpeechCraft = 0x00000457,
			kAVAlteration = 0x00000458,
			kAVConjuration = 0x00000459,
			kAVDestruction = 0x0000045A,
			kAVMysticism = 0x0000045B,
			kAVRestoration = 0x0000045C,
			kAVEnchanting = 0x0000045D,

			kAVMagickaRateMod = 0x00000646,
			kAVVampire = kAVMagickaRateMod,

			kAVHealRatePowerMod = 0x00000645,
			kAVWerewolf = kAVHealRatePowerMod,

			kAVFavorActive = 0x000005F6,
			kAVFavorsPerDay = 0x000005F7,
			kAVFavorsPerDayTimer = 0x000005F8,
			kAVWaitingForPlayer = 0x00000606,
			kAVLastBribedIntimidated = 0x00000601,
			kAVLastFlattered = 0x00000602,
			kAVFame = 0x000005E3,
			kAVInfamy = 0x000005E4,

			kAVStrength = kAVFavorActive,
			kAVEndurance = kAVFavorsPerDay,
			kAVIntelligence = kAVFavorsPerDayTimer,
			kAVWillpower = kAVWaitingForPlayer,
			kAVAgility = kAVLastBribedIntimidated,
			kAVSpeed = kAVLastFlattered,
			kAVPersonality = kAVFame,
			kAVLuck = kAVInfamy
		};


		struct Root
		{
			struct AVInfo
			{
				ActorValueInfoID id;
				RE::ActorValue av;
			};

			std::string text;
			std::vector<AVInfo> info;
			bool sort;
		};


		class RootMap : public std::vector<Root>
		{
		public:
			using Base = std::vector<Root>;

			RootMap();
		};


		struct HeaderList
		{
			void Visible(bool a_visible);

			CLIK::GFx::Controls::ScrollingList list;
			CLIK::TextField header;
		};


		struct Description
		{
			void Init();
			void Visible(bool a_visible);

			CLIK::TextField header;
			CLIK::TextField text;
			HeaderList requisites;
			HeaderList unlocks;
			CLIK::GFx::Controls::Button unlock;
		};


		struct Stats
		{
		public:
			enum class State
			{
				kDefault,
				kVampire,
				kWerewolf
			};

			Stats();
			~Stats() = default;

			void Update();
			void SetDefault();
			void SetVampire();
			void SetWerewolf();
			bool IsBeastMode() const;
			UInt32 GetPerkPoints() const;
			void DecPerkPoints() const;

			CLIK::TextField perkPoints;
			CLIK::TextField magicka;
			CLIK::TextField health;
			CLIK::TextField stamina;

		private:
			std::string BuildStatString(RE::ActorValue a_av) const;

			State _state;
		};


		struct Tree
		{
			std::string text;
			ActorValueInfoID avInfoID;
		};


		struct Rank
		{
			std::string text;
			BGSPerkID perkID;
		};


		struct TextPerkLevel
		{
			enum : UInt32 { kInvalid = static_cast<UInt32>(-1) };

			std::string text;
			BGSPerkID perkID;
			UInt32 level;
		};
	}


	class StatsMenuEx : public RE::IMenu
	{
	public:
		using Base = RE::IMenu;
		using Result = RE::UI_MESSAGE_RESULTS;


		StatsMenuEx();
		virtual ~StatsMenuEx() = default;

		// IMenu
		virtual void Accept(RE::FxDelegateHandler::CallbackProcessor* a_processor) override;
		virtual Result ProcessMessage(RE::UIMessage& a_message) override;

		static void Open();
		static void Close();

		static constexpr std::string_view Name();

		static void Register();
		static RE::IMenu* Create();

	private:
		static void Log(const RE::FxDelegateArgs& a_params);
		static void OnRootPress(const RE::FxDelegateArgs& a_params);
		static void OnTreePress(const RE::FxDelegateArgs& a_params);
		static void OnPerkPress(const RE::FxDelegateArgs& a_params);
		static void OnRankPress(const RE::FxDelegateArgs& a_params);
		static void OnRequisitePress(const RE::FxDelegateArgs& a_params);
		static void OnUnlockPress(const RE::FxDelegateArgs& a_params);
		static void UnlockPerk(const RE::FxDelegateArgs& a_params);

		void OnMenuOpen();
		void OnMenuClose();

		void InitExtensions();
		void SetRoots();
		void UnlockPerk(std::size_t a_rankIdx, std::size_t a_perkIdx, std::size_t a_treeIdx);

		bool OnRootPress(std::size_t a_classIdx);
		bool OnTreePress(std::size_t a_treeIdx);
		bool OnPerkPress(std::size_t a_perkIdx, std::size_t a_treeIdx);
		bool OnRankPress(std::size_t a_rankIdx, std::size_t a_treeIdx);
		bool OnRequisitePress(std::size_t a_requisiteIdx, std::size_t a_treeIdx);
		bool OnUnlockPress(std::size_t a_unlockIdx, std::size_t a_treeIdx);
		bool OnLeadPress(std::vector<TextPerkLevel>& a_lead, std::size_t a_leadIdx, std::size_t a_treeIdx);

		void UpdateTrees(std::size_t a_classIdx);
		void UpdatePerks(std::size_t a_treeIdx);
		void UpdateRanks(std::size_t a_perkIdx);
		void UpdateLeads(std::size_t a_rankIdx, std::size_t a_treeIdx);
		void UpdateLead(std::vector<TextPerkLevel>& a_lead, RE::BSTArray<RE::BGSSkillPerkTreeNode*>& a_srcArr, HeaderList& a_headerList);

		void InvalidateTrees();
		void InvalidatePerks();
		void InvalidateRanks();
		void InvalidateDesc();

		void BFSOnPerkTree(RE::ActorValueInfo* a_av, std::function<bool(RE::BGSSkillPerkTreeNode*)> a_predicate);
		std::optional<UInt32> GetPerkLvlReq(RE::BGSPerk* a_perk);
		void SanitizeString(std::string& a_str);


		static constexpr char SWF_NAME[] = "StatsMenuEx";

		CLIK::GFx::Controls::ScrollingList _roots;
		CLIK::GFx::Controls::ScrollingList _trees;
		CLIK::GFx::Controls::ScrollingList _perks;
		CLIK::GFx::Controls::ScrollingList _ranks;
		Description _desc;
		Stats _stats;
		RootMap _rootMappings;
		std::vector<Tree> _treeMappings;
		std::vector<TextPerkLevel> _perkMappings;
		std::vector<Rank> _rankMappings;
		std::vector<TextPerkLevel> _requisiteMappings;
		std::vector<TextPerkLevel> _unlockMappings;
	};


	constexpr std::string_view StatsMenuEx::Name()
	{
		return "StatsMenuEx";
	}
}
