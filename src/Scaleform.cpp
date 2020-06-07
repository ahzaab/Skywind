#include "SkywindPCH.h"

#include "Scaleform.h"

#include "BirthSignMenu.h"
#include "LevelUpMenu.h"
#include "MeterMenu.h"
#include "SpellmakingMenu.h"
#include "StatsMenuEx.h"

#include "SKSE/API.h"


namespace Scaleform
{
	void RegisterCallbacks()
	{
		_MESSAGE("Registered all scaleform callbacks");
	}


	void RegisterCreators()
	{
		BirthSignMenu::Register();
		LevelUpMenu::Register();
		MeterMenu::Register();
		SpellmakingMenu::Register();
		StatsMenuEx::Register();

		_MESSAGE("Registered all scaleform creators");
	}
}
