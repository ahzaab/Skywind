#include "Scaleform.h"

#include "LevelUpMenu.h"

#include "SKSE/API.h"


namespace Scaleform
{
	void RegisterCallbacks()
	{
		_MESSAGE("Registered all scaleform callbacks");
	}


	void RegisterCreators()
	{
		LevelUpMenu::Register();

		_MESSAGE("Registered all scaleform creators");
	}
}
