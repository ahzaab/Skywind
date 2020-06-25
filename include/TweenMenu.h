#pragma once
#include "RE/Skyrim.h"

namespace Scaleform
{
	namespace TweenMenu {

		typedef void (*voidFn)();

		// Start the animation
		extern REL::Function<voidFn> StartCloseMenu;

		// Close the Tween Menu, This function needs to be called to allow the animation to finish;
		extern REL::Function<voidFn> CloseMenu;
	}
}