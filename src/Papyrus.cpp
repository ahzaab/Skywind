#include "PCH.h"
#include "Papyrus.h"
#include "HudmenuExt.h"

namespace Papyrus
{
	void Register()
	{
		auto papyrus = SKSE::GetPapyrusInterface();

		papyrus->Register(HudMenuExt::RegisterFuncs);

	}
}

OnBirthSignAcceptHandler* OnBirthSignAcceptHandler::GetSingleton()
{
	static OnBirthSignAcceptHandler singleton;
	return &singleton;
}


OnBirthSignAcceptHandler::OnBirthSignAcceptHandler() :
	Base(EVN_NAME)
{}
