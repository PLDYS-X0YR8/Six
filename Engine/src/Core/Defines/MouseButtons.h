#pragma once
#include "spch.h"

namespace Six {
	enum class SIXAPI SMouseButton {
		SMOUSE_BUTTON_1         =0,
		SMOUSE_BUTTON_2         =1,
		SMOUSE_BUTTON_3         =2,
		SMOUSE_BUTTON_4         =3,
		SMOUSE_BUTTON_5         =4,
		SMOUSE_BUTTON_6         =5,
		SMOUSE_BUTTON_7         =6,
		SMOUSE_BUTTON_8         =7,
		SMOUSE_BUTTON_LAST      =SMOUSE_BUTTON_8,
		SMOUSE_BUTTON_LEFT      =SMOUSE_BUTTON_1,
		SMOUSE_BUTTON_RIGHT     =SMOUSE_BUTTON_2,
		SMOUSE_BUTTON_MIDDLE    =SMOUSE_BUTTON_3,
	};

	inline SIXAPI std::string S_MouseButton2Str(SMouseButton b) {
		switch (b) {
		case SMouseButton::SMOUSE_BUTTON_1:return "LEFT";
		case SMouseButton::SMOUSE_BUTTON_2:return "RIGHT";
		case SMouseButton::SMOUSE_BUTTON_3:return "MIDDLE";
		case SMouseButton::SMOUSE_BUTTON_4:return "BUTTON_4";
		case SMouseButton::SMOUSE_BUTTON_5:return "BUTTON_5";
		case SMouseButton::SMOUSE_BUTTON_6:return "BUTTON_6";
		case SMouseButton::SMOUSE_BUTTON_7:return "BUTTON_7";
		case SMouseButton::SMOUSE_BUTTON_8:return "BUTTON_8";
		default:return "F**k You(unknown)";
		}
	}
}