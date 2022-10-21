#include "NightOwl/Input/KeyCode.h"
#include "GLFW/glfw3.h"

namespace NightOwl::Input
{
	int NightOwlKeyCodeToGlfwKeyCode(KeyCode nightOwlKeyCode)
	{
		int glfwKeyCode;

		switch (nightOwlKeyCode)
		{
		case KeyCode::KeyNone:
			glfwKeyCode = GLFW_KEY_UNKNOWN;
			break;

		case KeyCode::KeySpace:
			glfwKeyCode = GLFW_KEY_SPACE;
			break;

		case KeyCode::KeyApostrophe:
			glfwKeyCode = GLFW_KEY_APOSTROPHE;
			break;

		case KeyCode::KeyComma:
		case KeyCode::KeyMinus:
		case KeyCode::KeyPeriod:
		case KeyCode::KeySlash:
		case KeyCode::Key0:
		case KeyCode::Key1:
		case KeyCode::Key2:
		case KeyCode::Key3:
		case KeyCode::Key4:
		case KeyCode::Key5:
		case KeyCode::Key6:
		case KeyCode::Key7:
		case KeyCode::Key8:
		case KeyCode::Key9:
			glfwKeyCode = static_cast<int>(nightOwlKeyCode) + GLFW_KEY_COMMA - static_cast<int>(KeyCode::KeyComma);
			break;

		case KeyCode::KeySemicolon:
			glfwKeyCode = GLFW_KEY_SEMICOLON;
			break;

		case KeyCode::KeyEqual:
			glfwKeyCode = GLFW_KEY_EQUAL;
			break;

		case KeyCode::KeyA:
		case KeyCode::KeyB:
		case KeyCode::KeyC:
		case KeyCode::KeyD:
		case KeyCode::KeyE:
		case KeyCode::KeyF:
		case KeyCode::KeyG:
		case KeyCode::KeyH:
		case KeyCode::KeyI:
		case KeyCode::KeyJ:
		case KeyCode::KeyK:
		case KeyCode::KeyL:
		case KeyCode::KeyM:
		case KeyCode::KeyN:
		case KeyCode::KeyO:
		case KeyCode::KeyP:
		case KeyCode::KeyQ:
		case KeyCode::KeyR:
		case KeyCode::KeyS:
		case KeyCode::KeyT:
		case KeyCode::KeyU:
		case KeyCode::KeyV:
		case KeyCode::KeyW:
		case KeyCode::KeyX:
		case KeyCode::KeyY:
		case KeyCode::KeyZ:
		case KeyCode::KeyLeftBracket:
		case KeyCode::KeyBackslash:
		case KeyCode::KeyRightBracket:
			glfwKeyCode = static_cast<int>(nightOwlKeyCode) + GLFW_KEY_A - static_cast<int>(KeyCode::KeyA);
			break;

		case KeyCode::KeyGraveAccent:
			glfwKeyCode = GLFW_KEY_GRAVE_ACCENT;
			break;

		case KeyCode::KeyEscape:
		case KeyCode::KeyEnter:
		case KeyCode::KeyTab:
		case KeyCode::KeyBackspace:
		case KeyCode::KeyInsert:
		case KeyCode::KeyDelete:
		case KeyCode::KeyRight:
		case KeyCode::KeyLeft:
		case KeyCode::KeyDown:
		case KeyCode::KeyUp:
		case KeyCode::KeyPageUp:
		case KeyCode::KeyPageDown:
		case KeyCode::KeyHome:
		case KeyCode::KeyEnd:
			glfwKeyCode = static_cast<int>(nightOwlKeyCode) + GLFW_KEY_ESCAPE - static_cast<int>(KeyCode::KeyEscape);
			break;

		case KeyCode::KeyCapsLock:
		case KeyCode::KeyScrollLock:
		case KeyCode::KeyNumLock:
		case KeyCode::KeyPrintScreen:
		case KeyCode::KeyPause:
			glfwKeyCode = static_cast<int>(nightOwlKeyCode) + GLFW_KEY_CAPS_LOCK - static_cast<int>(KeyCode::KeyCapsLock);
			break;

		case KeyCode::KeyF1:
		case KeyCode::KeyF2:
		case KeyCode::KeyF3:
		case KeyCode::KeyF4:
		case KeyCode::KeyF5:
		case KeyCode::KeyF6:
		case KeyCode::KeyF7:
		case KeyCode::KeyF8:
		case KeyCode::KeyF9:
		case KeyCode::KeyF10:
		case KeyCode::KeyF11:
		case KeyCode::KeyF12:
		case KeyCode::KeyF13:
		case KeyCode::KeyF14:
		case KeyCode::KeyF15:
		case KeyCode::KeyF16:
		case KeyCode::KeyF17:
		case KeyCode::KeyF18:
		case KeyCode::KeyF19:
		case KeyCode::KeyF20:
		case KeyCode::KeyF21:
		case KeyCode::KeyF22:
		case KeyCode::KeyF23:
		case KeyCode::KeyF24:
		case KeyCode::KeyF25:
			glfwKeyCode = static_cast<int>(nightOwlKeyCode) + GLFW_KEY_F1 - static_cast<int>(KeyCode::KeyF1);
			break;

		case KeyCode::KeyNumPad0:
		case KeyCode::KeyNumPad1:
		case KeyCode::KeyNumPad2:
		case KeyCode::KeyNumPad3:
		case KeyCode::KeyNumPad4:
		case KeyCode::KeyNumPad5:
		case KeyCode::KeyNumPad6:
		case KeyCode::KeyNumPad7:
		case KeyCode::KeyNumPad8:
		case KeyCode::KeyNumPad9:
		case KeyCode::KeyNumPadDecimal:
		case KeyCode::KeyNumPadDivide:
		case KeyCode::KeyNumPadMultiply:
		case KeyCode::KeyNumPadSubtract:
		case KeyCode::KeyNumPadAdd:
		case KeyCode::KeyNumPadEnter:
		case KeyCode::KeyNumPadEqual:
			glfwKeyCode = static_cast<int>(nightOwlKeyCode) + GLFW_KEY_KP_0 - static_cast<int>(KeyCode::KeyNumPad0);
			break;

		case KeyCode::KeyLeftShift:
		case KeyCode::KeyLeftControl:
		case KeyCode::KeyLeftAlt:
		case KeyCode::KeyLeftSuper:
		case KeyCode::KeyRightShift:
		case KeyCode::KeyRightControl:
		case KeyCode::KeyRightAlt:
		case KeyCode::KeyRightSuper:
		case KeyCode::KeyMenu:
			glfwKeyCode = static_cast<int>(nightOwlKeyCode) + GLFW_KEY_LEFT_SHIFT - static_cast<int>(KeyCode::KeyLeftShift);
			break;

		default:
			glfwKeyCode = GLFW_KEY_UNKNOWN;
			break;
		}

		return glfwKeyCode;
	}

	KeyCode GlfwKeyCodeToNightOwlKeyCode(int glfwKeyCode)
	{
		KeyCode  nightOwlKeyCode;

		switch (glfwKeyCode)
		{
		case GLFW_KEY_UNKNOWN:
			nightOwlKeyCode = KeyCode::KeyNone;
			break;

		case GLFW_KEY_SPACE:
			nightOwlKeyCode = KeyCode::KeySpace;
			break;

		case GLFW_KEY_APOSTROPHE:
			nightOwlKeyCode = KeyCode::KeyApostrophe;
			break;

		case GLFW_KEY_COMMA:
		case GLFW_KEY_MINUS:
		case GLFW_KEY_PERIOD:
		case GLFW_KEY_SLASH:
		case GLFW_KEY_0:
		case GLFW_KEY_1:
		case GLFW_KEY_2:
		case GLFW_KEY_3:
		case GLFW_KEY_4:
		case GLFW_KEY_5:
		case GLFW_KEY_6:
		case GLFW_KEY_7:
		case GLFW_KEY_8:
		case GLFW_KEY_9:
			nightOwlKeyCode = static_cast<KeyCode>(static_cast<int>(KeyCode::KeyComma) + glfwKeyCode - GLFW_KEY_COMMA);
			break;

		case GLFW_KEY_SEMICOLON:
			nightOwlKeyCode = KeyCode::KeySemicolon;
			break;

		case GLFW_KEY_EQUAL:
			nightOwlKeyCode = KeyCode::KeyEqual;
			break;

		case GLFW_KEY_A:
		case GLFW_KEY_B:
		case GLFW_KEY_C:
		case GLFW_KEY_D:
		case GLFW_KEY_E:
		case GLFW_KEY_F:
		case GLFW_KEY_G:
		case GLFW_KEY_H:
		case GLFW_KEY_I:
		case GLFW_KEY_J:
		case GLFW_KEY_K:
		case GLFW_KEY_L:
		case GLFW_KEY_M:
		case GLFW_KEY_N:
		case GLFW_KEY_O:
		case GLFW_KEY_P:
		case GLFW_KEY_Q:
		case GLFW_KEY_R:
		case GLFW_KEY_S:
		case GLFW_KEY_T:
		case GLFW_KEY_U:
		case GLFW_KEY_V:
		case GLFW_KEY_W:
		case GLFW_KEY_X:
		case GLFW_KEY_Y:
		case GLFW_KEY_Z:
		case GLFW_KEY_LEFT_BRACKET:
		case GLFW_KEY_BACKSLASH:
		case GLFW_KEY_RIGHT_BRACKET:
			nightOwlKeyCode = static_cast<KeyCode>(static_cast<int>(KeyCode::KeyA) + glfwKeyCode - GLFW_KEY_A);
			break;

		case GLFW_KEY_GRAVE_ACCENT:
			nightOwlKeyCode = KeyCode::KeyGraveAccent;
			break;

		case GLFW_KEY_ESCAPE:
		case GLFW_KEY_ENTER:
		case GLFW_KEY_TAB:
		case GLFW_KEY_BACKSPACE:
		case GLFW_KEY_INSERT:
		case GLFW_KEY_DELETE:
		case GLFW_KEY_RIGHT:
		case GLFW_KEY_LEFT:
		case GLFW_KEY_DOWN:
		case GLFW_KEY_UP:
		case GLFW_KEY_PAGE_UP:
		case GLFW_KEY_PAGE_DOWN:
		case GLFW_KEY_HOME:
		case GLFW_KEY_END:
			nightOwlKeyCode = static_cast<KeyCode>(static_cast<int>(KeyCode::KeyEscape) + glfwKeyCode - GLFW_KEY_ESCAPE);
			break;

		case GLFW_KEY_CAPS_LOCK:
		case GLFW_KEY_SCROLL_LOCK:
		case GLFW_KEY_NUM_LOCK:
		case GLFW_KEY_PRINT_SCREEN:
		case GLFW_KEY_PAUSE:
			nightOwlKeyCode = static_cast<KeyCode>(static_cast<int>(KeyCode::KeyCapsLock) + glfwKeyCode - GLFW_KEY_CAPS_LOCK);
			break;

		case GLFW_KEY_F1:
		case GLFW_KEY_F2:
		case GLFW_KEY_F3:
		case GLFW_KEY_F4:
		case GLFW_KEY_F5:
		case GLFW_KEY_F6:
		case GLFW_KEY_F7:
		case GLFW_KEY_F8:
		case GLFW_KEY_F9:
		case GLFW_KEY_F10:
		case GLFW_KEY_F11:
		case GLFW_KEY_F12:
		case GLFW_KEY_F13:
		case GLFW_KEY_F14:
		case GLFW_KEY_F15:
		case GLFW_KEY_F16:
		case GLFW_KEY_F17:
		case GLFW_KEY_F18:
		case GLFW_KEY_F19:
		case GLFW_KEY_F20:
		case GLFW_KEY_F21:
		case GLFW_KEY_F22:
		case GLFW_KEY_F23:
		case GLFW_KEY_F24:
		case GLFW_KEY_F25:
			nightOwlKeyCode = static_cast<KeyCode>(static_cast<int>(KeyCode::KeyF1) + glfwKeyCode - GLFW_KEY_F1);
			break;

		case GLFW_KEY_KP_0:
		case GLFW_KEY_KP_1:
		case GLFW_KEY_KP_2:
		case GLFW_KEY_KP_3:
		case GLFW_KEY_KP_4:
		case GLFW_KEY_KP_5:
		case GLFW_KEY_KP_6:
		case GLFW_KEY_KP_7:
		case GLFW_KEY_KP_8:
		case GLFW_KEY_KP_9:
		case GLFW_KEY_KP_DECIMAL:
		case GLFW_KEY_KP_DIVIDE:
		case GLFW_KEY_KP_MULTIPLY:
		case GLFW_KEY_KP_SUBTRACT:
		case GLFW_KEY_KP_ADD:
		case GLFW_KEY_KP_ENTER:
		case GLFW_KEY_KP_EQUAL:
			nightOwlKeyCode = static_cast<KeyCode>(static_cast<int>(KeyCode::KeyNumPad0) + glfwKeyCode - GLFW_KEY_KP_0);
			break;

		case GLFW_KEY_LEFT_SHIFT:
		case GLFW_KEY_LEFT_CONTROL:
		case GLFW_KEY_LEFT_ALT:
		case GLFW_KEY_LEFT_SUPER:
		case GLFW_KEY_RIGHT_SHIFT:
		case GLFW_KEY_RIGHT_CONTROL:
		case GLFW_KEY_RIGHT_ALT:
		case GLFW_KEY_RIGHT_SUPER:
		case GLFW_KEY_MENU:
			nightOwlKeyCode = static_cast<KeyCode>(static_cast<int>(KeyCode::KeyLeftShift) + glfwKeyCode - GLFW_KEY_LEFT_SHIFT);
			break;

		default:
			nightOwlKeyCode = KeyCode::KeyNone;
			break;
		}

		return nightOwlKeyCode;
	}
}
