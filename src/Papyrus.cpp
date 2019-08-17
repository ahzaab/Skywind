#include "Papyrus.h"


OnBirthSignAcceptHandler* OnBirthSignAcceptHandler::GetSingleton()
{
	static OnBirthSignAcceptHandler singleton;
	return &singleton;
}


OnBirthSignAcceptHandler::OnBirthSignAcceptHandler() :
	Base(EVN_NAME)
{}
