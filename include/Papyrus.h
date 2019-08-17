#pragma once

#include "SKSE/RegistrationSet.h"


class OnBirthSignAcceptHandler : public SKSE::RegistrationSet<UInt32>
{
public:
	static OnBirthSignAcceptHandler* GetSingleton();

private:
	using Base = SKSE::RegistrationSet<UInt32>;


	OnBirthSignAcceptHandler();
	OnBirthSignAcceptHandler(const OnBirthSignAcceptHandler&) = delete;
	OnBirthSignAcceptHandler(OnBirthSignAcceptHandler&&) = delete;
	~OnBirthSignAcceptHandler() = default;

	OnBirthSignAcceptHandler& operator=(const OnBirthSignAcceptHandler&) = delete;
	OnBirthSignAcceptHandler& operator=(OnBirthSignAcceptHandler&&) = delete;


	static constexpr char EVN_NAME[] = "OnBirthSignAccept";
};
