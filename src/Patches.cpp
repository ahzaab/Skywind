#include "Patches.h"

#include "RE/Skyrim.h"
#include "REL/Relocation.h"
#include "SKSE/API.h"
#include "SKSE/SafeWrite.h"
#include "SKSE/Trampoline.h"


namespace Patches
{
	namespace
	{
		void DisableLegendaryPrompt()
		{
			{
				constexpr std::size_t RUN_START = 0x163;
				constexpr std::size_t RUN_END = 0x1C9;
				constexpr UInt8 NOP = 0x90;

				REL::Offset<std::uintptr_t> funcBase = REL::ID(51654);
				for (auto i = RUN_START; i < RUN_END; ++i) {
					SKSE::SafeWrite8(funcBase.GetAddress() + i, NOP);
				}
			}

			{
				constexpr std::size_t RUN_START = 0x115;
				constexpr std::size_t RUN_END = 0x35A;
				constexpr UInt8 NOP = 0x90;

				REL::Offset<std::uintptr_t> funcBase = REL::ID(51647);
				for (auto i = RUN_START; i < RUN_END; ++i) {
					SKSE::SafeWrite8(funcBase.GetAddress() + i, NOP);
				}
			}

			_MESSAGE("Disabled legendary prompt");
		}


		const char* Hook_GetTemperString([[maybe_unused]] float a_temperFactor, [[maybe_unused]] bool a_isWeapon)
		{
			return 0;
		}


		void DisableTemperString()
		{
			REL::Offset<std::uintptr_t> target(REL::ID(12633), 0x59);
			auto trampoline = SKSE::GetTrampoline();
			trampoline->Write5Call(target.GetAddress(), &Hook_GetTemperString);

			_MESSAGE("Disabled temper string");
		}


		void PatchEra()
		{
			REL::Offset<std::uintptr_t> target = REL::ID(259069);
			constexpr UInt8 NEW_STR[] = ", 3E %d";
			for (std::size_t i = 0; i < std::extent<decltype(NEW_STR)>::value; ++i) {
				SKSE::SafeWrite8(target.GetAddress() + i, NEW_STR[i]);
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
