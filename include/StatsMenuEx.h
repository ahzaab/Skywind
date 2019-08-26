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


		enum : ActorValueInfoID
		{
			kAVOneHanded = 0x0000044C,	// Dragonborn
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

			kAVMagickaRateMod = 0x00000646,	// Vampire

			kAVHealRatePowerMod = 0x00000645,	// Werewolf
		};


		class ClassMap : public std::vector<std::pair<std::string, std::vector<std::pair<ActorValueInfoID, RE::ActorValue>>>>
		{
		public:
			using Base = std::vector<std::pair<std::string, std::vector<std::pair<ActorValueInfoID, RE::ActorValue>>>>;

			ClassMap();
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
			void Update();

			CLIK::TextField perkPoints;
			CLIK::TextField magicka;
			CLIK::TextField health;
			CLIK::TextField stamina;

		private:
			std::string BuildStatString(RE::ActorValue a_av);
		};


		struct Tree
		{
			std::string text;
			ActorValueInfoID avInfoID;
		};


		struct Perk
		{
			enum : UInt32 { kInvalid = static_cast<UInt32>(-1) };

			std::string text;
			BGSPerkID perkID;
			UInt32 level;
		};


		struct TextPerk
		{
			std::string text;
			BGSPerkID perkID;
		};
	}


	class StatsMenuEx : public RE::IMenu
	{
	public:
		using Base = RE::IMenu;
		using Result = Base::Result;


		StatsMenuEx();
		virtual ~StatsMenuEx() = default;

		// IMenu
		virtual void Accept(RE::FxDelegateHandler::CallbackProcessor* a_processor) override;
		virtual Result ProcessMessage(RE::UIMessage* a_message) override;

		static void Open();
		static void Close();

		static constexpr std::string_view Name();

		static void Register();
		static RE::IMenu* Create();

	private:
		static void Log(const RE::FxDelegateArgs& a_params);
		static void OnClassPress(const RE::FxDelegateArgs& a_params);
		static void OnTreePress(const RE::FxDelegateArgs& a_params);
		static void OnPerkPress(const RE::FxDelegateArgs& a_params);
		static void OnRankPress(const RE::FxDelegateArgs& a_params);
		static void OnRequisitePress(const RE::FxDelegateArgs& a_params);
		static void OnUnlockPress(const RE::FxDelegateArgs& a_params);
		static void UnlockPerk(const RE::FxDelegateArgs& a_params);

		void OnMenuOpen();
		void OnMenuClose();

		void InitExtensions();
		void SetClasses();
		void UnlockPerk(std::size_t a_rankIdx, std::size_t a_perkIdx, std::size_t a_treeIdx);

		void OnClassPress(std::size_t a_classIdx);
		void OnTreePress(std::size_t a_treeIdx);
		void OnPerkPress(std::size_t a_perkIdx);
		void OnRankPress(std::size_t a_rankIdx, std::size_t a_treeIdx);
		void OnRequisitePress(std::size_t a_requisiteIdx, std::size_t a_treeIdx);
		void OnUnlockPress(std::size_t a_unlockIdx, std::size_t a_treeIdx);
		void OnLeadPress(std::vector<TextPerk>& a_lead, std::size_t a_leadIdx, std::size_t a_treeIdx);

		void UpdateTrees(std::size_t a_classIdx);
		void UpdatePerks(std::size_t a_treeIdx);
		void UpdateRanks(std::size_t a_perkIdx);
		void UpdateLeads(std::size_t a_rankIdx, std::size_t a_treeIdx);

		void InvalidateTrees();
		void InvalidatePerks();
		void InvalidateRanks();
		void InvalidateDesc();

		void BFSOnPerkTree(RE::ActorValueInfo* a_av, llvm::function_ref<bool(RE::BGSSkillPerkTreeNode*)> a_predicate);
		std::optional<UInt32> GetPerkLvlReq(RE::BGSPerk* a_perk);
		void SanitizeString(std::string& a_str);


		static constexpr char SWF_NAME[] = "StatsMenuEx";

		CLIK::GFx::Controls::ScrollingList _classes;
		CLIK::GFx::Controls::ScrollingList _trees;
		CLIK::GFx::Controls::ScrollingList _perks;
		CLIK::GFx::Controls::ScrollingList _ranks;
		Description _desc;
		Stats _stats;
		ClassMap _classMappings;
		std::vector<Tree> _treeMappings;
		std::vector<Perk> _perkMappings;
		std::vector<TextPerk> _rankMappings;
		std::vector<TextPerk> _requisiteMappings;
		std::vector<TextPerk> _unlockMappings;
	};


	constexpr std::string_view StatsMenuEx::Name()
	{
		return "StatsMenuEx";
	}
}
