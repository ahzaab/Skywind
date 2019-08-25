#include "Patches.h"

#include "skse64_common/BranchTrampoline.h"
#include "skse64_common/SafeWrite.h"

#include "RE/Skyrim.h"
#include "REL/Relocation.h"


namespace Patches
{
	namespace
	{
		void DisableLegendaryPrompt()
		{
			{
				// E8 ? ? ? ? C6 86 21 03 00 00 00
				constexpr std::uintptr_t FUNC_ADDR = 0x008C2BA0;	// 1_5_80
				constexpr std::size_t RUN_START = 0x163;
				constexpr std::size_t RUN_END = 0x1C9;
				constexpr UInt8 NOP = 0x90;

				REL::Offset<std::uintptr_t> funcBase(FUNC_ADDR);
				for (auto i = RUN_START; i < RUN_END; ++i) {
					SafeWrite8(funcBase.GetAddress() + i, NOP);
				}
			}

			{
				// 48 8B C4 55 56 57 41 54 41 55 41 56 41 57 48 8D A8 18 FF FF FF 48 81 EC B0 01 00 00 48 C7 45 30 FE FF FF FF
				constexpr std::uintptr_t FUNC_ADDR = 0x008C0D90;	// 1_5_80
				constexpr std::size_t RUN_START = 0x115;
				constexpr std::size_t RUN_END = 0x35A;
				constexpr UInt8 NOP = 0x90;

				REL::Offset<std::uintptr_t> funcBase(FUNC_ADDR);
				for (auto i = RUN_START; i < RUN_END; ++i) {
					SafeWrite8(funcBase.GetAddress() + i, NOP);
				}
			}

			_MESSAGE("Disabled legendary prompt");
		}


		const char* Hook_GetTemperString(float a_temperFactor, bool a_isWeapon)
		{
			return 0;
		}


		void DisableTemperString()
		{
			// 33 C9 0F 57 C9 0F 2F C1
			constexpr std::uintptr_t FUNC_ADDR = 0x0013CC20;	// 1_5_80
			constexpr std::size_t OFFSET = 0x59;

			REL::Offset<std::uintptr_t> target(FUNC_ADDR + OFFSET);
			g_branchTrampoline.Write5Call(target.GetAddress(), unrestricted_cast<std::uintptr_t>(&Hook_GetTemperString));

			_MESSAGE("Disabled temper string");
		}


		void PatchEra()
		{
			// , 4E %d
			constexpr std::uintptr_t OFFSET = 0x01647510;	// 1_5_80

			REL::Offset<std::uintptr_t> target(OFFSET);
			constexpr UInt8 NEW_STR[] = ", 3E %d";
			for (std::size_t i = 0; i < std::extent<decltype(NEW_STR)>::value; ++i) {
				SafeWrite8(target.GetAddress() + i, NEW_STR[i]);
			}

			_MESSAGE("Patched era");
		}
	}


	void Install()
	{
		DisableTemperString();
		DisableLegendaryPrompt();
		PatchEra();
		_MESSAGE("Installed all patches");
	}
}
