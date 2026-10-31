#pragma once
#include "sixcore.h"
#include <stdint.h>
#include <ostream>
namespace Six{
	enum class SIXAPI SKeyCodes:int16_t {
		SKEY_UNKNOWN            =-1,
		SKEY_SPACE              =32,
		SKEY_APOSTROPHE         =39,  /* ' */
		SKEY_COMMA              =44,  /* , */
		SKEY_MINUS              =45,  /* - */
		SKEY_PERIOD             =46,  /* . */
		SKEY_SLASH              =47,  /* / */
		SKEY_0                  =48,
		SKEY_1                  =49,
		SKEY_2                  =50,
		SKEY_3                  =51,
		SKEY_4                  =52,
		SKEY_5                  =53,
		SKEY_6                  =54,
		SKEY_7                  =55,
		SKEY_8                  =56,
		SKEY_9                  =57,
		SKEY_SEMICOLON          =59,  /* ; */
		SKEY_EQUAL              =61,  /* = */
		SKEY_A                  =65,
		SKEY_B                  =66,
		SKEY_C                  =67,
		SKEY_D                  =68,
		SKEY_E                  =69,
		SKEY_F                  =70,
		SKEY_G                  =71,
		SKEY_H                  =72,
		SKEY_I                  =73,
		SKEY_J                  =74,
		SKEY_K                  =75,
		SKEY_L                  =76,
		SKEY_M                  =77,
		SKEY_N                  =78,
		SKEY_O                  =79,
		SKEY_P                  =80,
		SKEY_Q                  =81,
		SKEY_R                  =82,
		SKEY_S                  =83,
		SKEY_T                  =84,
		SKEY_U                  =85,
		SKEY_V                  =86,
		SKEY_W                  =87,
		SKEY_X                  =88,
		SKEY_Y                  =89,
		SKEY_Z                  =90,
		SKEY_LEFT_BRACKET       =91,  /* [ */
		SKEY_BACKSLASH          =92,  /* \ */
		SKEY_RIGHT_BRACKET      =93,  /* ] */
		SKEY_GRAVE_ACCENT       =96,  /* ` */
		SKEY_WORLD_1            =161, /* non-US #1 */
		SKEY_WORLD_2            =162, /* non-US #2 */
		SKEY_ESCAPE             =256,
		SKEY_ENTER              =257,
		SKEY_TAB                =258,
		SKEY_BACKSPACE          =259,
		SKEY_INSERT             =260,
		SKEY_DELETE             =261,
		SKEY_RIGHT              =262,
		SKEY_LEFT               =263,
		SKEY_DOWN               =264,
		SKEY_UP                 =265,
		SKEY_PAGE_UP            =266,
		SKEY_PAGE_DOWN          =267,
		SKEY_HOME               =268,
		SKEY_END                =269,
		SKEY_CAPS_LOCK          =280,
		SKEY_SCROLL_LOCK        =281,
		SKEY_NUM_LOCK           =282,
		SKEY_PRINT_SCREEN       =283,
		SKEY_PAUSE              =284,
		SKEY_F1                 =290,
		SKEY_F2                 =291,
		SKEY_F3                 =292,
		SKEY_F4                 =293,
		SKEY_F5                 =294,
		SKEY_F6                 =295,
		SKEY_F7                 =296,
		SKEY_F8                 =297,
		SKEY_F9                 =298,
		SKEY_F10                =299,
		SKEY_F11                =300,
		SKEY_F12                =301,
		SKEY_F13                =302,
		SKEY_F14                =303,
		SKEY_F15                =304,
		SKEY_F16                =305,
		SKEY_F17                =306,
		SKEY_F18                =307,
		SKEY_F19                =308,
		SKEY_F20                =309,
		SKEY_F21                =310,
		SKEY_F22                =311,
		SKEY_F23                =312,
		SKEY_F24                =313,
		SKEY_F25                =314,
		SKEY_KP_0               =320,
		SKEY_KP_1               =321,
		SKEY_KP_2               =322,
		SKEY_KP_3               =323,
		SKEY_KP_4               =324,
		SKEY_KP_5               =325,
		SKEY_KP_6               =326,
		SKEY_KP_7               =327,
		SKEY_KP_8               =328,
		SKEY_KP_9               =329,
		SKEY_KP_DECIMAL         =330,
		SKEY_KP_DIVIDE          =331,
		SKEY_KP_MULTIPLY        =332,
		SKEY_KP_SUBTRACT        =333,
		SKEY_KP_ADD             =334,
		SKEY_KP_ENTER           =335,
		SKEY_KP_EQUAL           =336,
		SKEY_LEFT_SHIFT         =340,
		SKEY_LEFT_CONTROL       =341,
		SKEY_LEFT_ALT           =342,
		SKEY_LEFT_SUPER         =343,
		SKEY_RIGHT_SHIFT        =344,
		SKEY_RIGHT_CONTROL      =345,
		SKEY_RIGHT_ALT          =346,
		SKEY_RIGHT_SUPER        =347,
		SKEY_MENU               =348,
		SKEY_LAST               =SKEY_MENU
	};

	inline SIXAPI std::string S_Key2Str(SKeyCodes k){
		switch (k) {
		case SKeyCodes::SKEY_UNKNOWN:return "F**k You(unknown)";
		case SKeyCodes::SKEY_0:return "0";
		case SKeyCodes::SKEY_1:return "1";
		case SKeyCodes::SKEY_2:return "2";
		case SKeyCodes::SKEY_3:return "3";
		case SKeyCodes::SKEY_4:return "4";
		case SKeyCodes::SKEY_5:return "5";
		case SKeyCodes::SKEY_6:return "6";
		case SKeyCodes::SKEY_7:return "7";
		case SKeyCodes::SKEY_8:return "8";
		case SKeyCodes::SKEY_9:return "9";
		case SKeyCodes::SKEY_A:return "A";
		case SKeyCodes::SKEY_B:return "B";
		case SKeyCodes::SKEY_C:return "C";
		case SKeyCodes::SKEY_D:return "D";
		case SKeyCodes::SKEY_E:return "E";
		case SKeyCodes::SKEY_F:return "F";
		case SKeyCodes::SKEY_G:return "G";
		case SKeyCodes::SKEY_H:return "H";
		case SKeyCodes::SKEY_I:return "I";
		case SKeyCodes::SKEY_J:return "J";
		case SKeyCodes::SKEY_K:return "K";
		case SKeyCodes::SKEY_L:return "L";
		case SKeyCodes::SKEY_M:return "M";
		case SKeyCodes::SKEY_N:return "N";
		case SKeyCodes::SKEY_O:return "O";
		case SKeyCodes::SKEY_P:return "P";
		case SKeyCodes::SKEY_Q:return "Q";
		case SKeyCodes::SKEY_R:return "R";
		case SKeyCodes::SKEY_S:return "S";
		case SKeyCodes::SKEY_T:return "T";
		case SKeyCodes::SKEY_U:return "U";
		case SKeyCodes::SKEY_V:return "V";
		case SKeyCodes::SKEY_W:return "W";
		case SKeyCodes::SKEY_X:return "X";
		case SKeyCodes::SKEY_Y:return "Y";
		case SKeyCodes::SKEY_Z:return "Z";
		case SKeyCodes::SKEY_LEFT:return "LEFT";
		case SKeyCodes::SKEY_RIGHT:return "RIGHT";
		case SKeyCodes::SKEY_UP:return "UP";
		case SKeyCodes::SKEY_DOWN:return "DOWN";
		case SKeyCodes::SKEY_SPACE:return "SPACE";
		case SKeyCodes::SKEY_APOSTROPHE:return "'";
		case SKeyCodes::SKEY_COMMA:return ",";
		case SKeyCodes::SKEY_MINUS:return "-";
		case SKeyCodes::SKEY_PERIOD:return ".";
		case SKeyCodes::SKEY_SLASH:return "/";
		case SKeyCodes::SKEY_SEMICOLON:return ";";
		case SKeyCodes::SKEY_EQUAL:return "=";
		case SKeyCodes::SKEY_LEFT_BRACKET:return "[";
		case SKeyCodes::SKEY_BACKSLASH:return "\\";
		case SKeyCodes::SKEY_RIGHT_BRACKET:return "]";
		case SKeyCodes::SKEY_GRAVE_ACCENT:return "`";
		case SKeyCodes::SKEY_WORLD_1:return "WORLD_1";
		case SKeyCodes::SKEY_WORLD_2:return "WORLD_2";
		case SKeyCodes::SKEY_ESCAPE:return "ESCAPE";
		case SKeyCodes::SKEY_ENTER:return "ENTER";
		case SKeyCodes::SKEY_TAB:return "TAB";
		case SKeyCodes::SKEY_BACKSPACE:return "BACKSPACE";
		case SKeyCodes::SKEY_INSERT:return "INSERT";
		case SKeyCodes::SKEY_DELETE:return "DELETE";
		case SKeyCodes::SKEY_PAGE_UP:return "PAGE_UP";
		case SKeyCodes::SKEY_PAGE_DOWN:return "PAGE_DOWN";
		case SKeyCodes::SKEY_HOME:return "HOME";
		case SKeyCodes::SKEY_END:return "END";
		case SKeyCodes::SKEY_CAPS_LOCK:return "CAPS_LOCK";
		case SKeyCodes::SKEY_SCROLL_LOCK:return "SCROLL_LOCK";
		case SKeyCodes::SKEY_NUM_LOCK:return "NUM_LOCK";
		case SKeyCodes::SKEY_PRINT_SCREEN:return "PRINT_SCREEN";
		case SKeyCodes::SKEY_PAUSE:return "PAUSE";
		case SKeyCodes::SKEY_F1:return "F1";
		case SKeyCodes::SKEY_F2:return "F2";
		case SKeyCodes::SKEY_F3:return "F3";
		case SKeyCodes::SKEY_F4:return "F4";
		case SKeyCodes::SKEY_F5:return "F5";
		case SKeyCodes::SKEY_F6:return "F6";
		case SKeyCodes::SKEY_F7:return "F7";
		case SKeyCodes::SKEY_F8:return "F8";
		case SKeyCodes::SKEY_F9:return "F9";
		case SKeyCodes::SKEY_F10:return "F10";
		case SKeyCodes::SKEY_F11:return "F11";
		case SKeyCodes::SKEY_F12:return "F12";
		case SKeyCodes::SKEY_F13:return "F13";
		case SKeyCodes::SKEY_F14:return "F14";
		case SKeyCodes::SKEY_F15:return "F15";
		case SKeyCodes::SKEY_F16:return "F16";
		case SKeyCodes::SKEY_F17:return "F17";
		case SKeyCodes::SKEY_F18:return "F18";
		case SKeyCodes::SKEY_F19:return "F19";
		case SKeyCodes::SKEY_F20:return "F20";
		case SKeyCodes::SKEY_F21:return "F21";
		case SKeyCodes::SKEY_F22:return "F22";
		case SKeyCodes::SKEY_F23:return "F23";
		case SKeyCodes::SKEY_F24:return "F24";
		case SKeyCodes::SKEY_F25:return "F25";
		case SKeyCodes::SKEY_KP_0:return "KP_0";
		case SKeyCodes::SKEY_KP_1:return "KP_1";
		case SKeyCodes::SKEY_KP_2:return "KP_2";
		case SKeyCodes::SKEY_KP_3:return "KP_3";
		case SKeyCodes::SKEY_KP_4:return "KP_4";
		case SKeyCodes::SKEY_KP_5:return "KP_5";
		case SKeyCodes::SKEY_KP_6:return "KP_6";
		case SKeyCodes::SKEY_KP_7:return "KP_7";
		case SKeyCodes::SKEY_KP_8:return "KP_8";
		case SKeyCodes::SKEY_KP_9:return "KP_9";
		case SKeyCodes::SKEY_KP_DECIMAL:return "KP_DECIMAL";
		case SKeyCodes::SKEY_KP_DIVIDE:return "KP_DIVIDE";
		case SKeyCodes::SKEY_KP_MULTIPLY:return "KP_MULTIPLY";
		case SKeyCodes::SKEY_KP_SUBTRACT:return "KP_SUBTRACT";
		case SKeyCodes::SKEY_KP_ADD:return "KP_ADD";
		case SKeyCodes::SKEY_KP_ENTER:return "KP_ENTER";
		case SKeyCodes::SKEY_KP_EQUAL:return "KP_EQUAL";
		case SKeyCodes::SKEY_LEFT_SHIFT:return "LEFT_SHIFT";
		case SKeyCodes::SKEY_LEFT_CONTROL:return "LEFT_CONTROL";
		case SKeyCodes::SKEY_LEFT_ALT:return "LEFT_ALT";
		case SKeyCodes::SKEY_LEFT_SUPER:return "LEFT_SUPER";
		case SKeyCodes::SKEY_RIGHT_SHIFT:return "RIGHT_SHIFT";
		case SKeyCodes::SKEY_RIGHT_CONTROL:return "RIGHT_CONTROL";
		case SKeyCodes::SKEY_RIGHT_ALT:return "RIGHT_ALT";
		case SKeyCodes::SKEY_RIGHT_SUPER:return "RIGHT_SUPER";
		case SKeyCodes::SKEY_MENU:return "MENU";
		}
		return "F**k You(invalid)";
	}
}