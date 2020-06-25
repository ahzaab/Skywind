#include "RE\BSScript\ErrorLogger.h"
#include "RE\BSScript\IObjectProcessor\LinkerProcessor.h"
#include "RE\BSCore\BSCoreTypes.h"

namespace HudMenuExt
{
	using VM = RE::BSScript::IVirtualMachine;
	using StackID = RE::VMStackID;
	using Serverity = RE::BSScript::ErrorLogger::Severity;

	void SendNotification(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, RE::BSFixedString a_message);
	bool RegisterFuncs(VM* a_vm);
}
