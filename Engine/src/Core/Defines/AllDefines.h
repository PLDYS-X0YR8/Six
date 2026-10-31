#pragma once
#include "KeyCodes.h"
#include "MouseButtons.h"

namespace Six{
	union SIXAPI SCodeButton {
		SKeyCodes keycode;
		SMouseButton mousebutton;
	};
}