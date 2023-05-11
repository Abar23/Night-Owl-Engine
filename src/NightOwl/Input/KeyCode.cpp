#include <NightOwlPch.h>

#include "NightOwl/Input/KeyCode.h"
#include "GLFW/glfw3.h"

namespace NightOwl::Input
{
	int NightOwlKeyCodeToGlfwKeyCode(KeyCode nightOwlKeyCode)
	{
		int glfwKeyCode;

		switch (nightOwlKeyCode)
		{
		case KeyCode::None:
			glfwKeyCode = GLFW_KEY_UNKNOWN;
			break;

		case KeyCode::Space:
			glfwKeyCode = GLFW_KEY_SPACE;
			break;

		case KeyCode::Apostrophe:
			glfwKeyCode = GLFW_KEY_APOSTROPHE;
			break;

		case KeyCode::Comma:
		case KeyCode::Minus:
		case KeyCode::Period:
		case KeyCode::Slash:
		case KeyCode::Num0:
		case KeyCode::Num1:
		case KeyCode::Num2:
		case KeyCode::Num3:
		case KeyCode::Num4:
		case KeyCode::Num5:
		case KeyCode::Num6:
		case KeyCode::Num7:
		case KeyCode::Num8:
		case KeyCode::Num9:
			glfwKeyCode = static_cast<int>(nightOwlKeyCode) + GLFW_KEY_COMMA - static_cast<int>(KeyCode::Comma);
			break;

		case KeyCode::Semicolon:
			glfwKeyCode = GLFW_KEY_SEMICOLON;
			break;

		case KeyCode::Equal:
			glfwKeyCode = GLFW_KEY_EQUAL;
			break;

		case KeyCode::A:
		case KeyCode::B:
		case KeyCode::C:
		case KeyCode::D:
		case KeyCode::E:
		case KeyCode::F:
		case KeyCode::G:
		case KeyCode::H:
		case KeyCode::I:
		case KeyCode::J:
		case KeyCode::K:
		case KeyCode::L:
		case KeyCode::M:
		case KeyCode::N:
		case KeyCode::O:
		case KeyCode::P:
		case KeyCode::Q:
		case KeyCode::R:
		case KeyCode::S:
		case KeyCode::T:
		case KeyCode::U:
		case KeyCode::V:
		case KeyCode::W:
		case KeyCode::X:
		case KeyCode::Y:
		case KeyCode::Z:
		case KeyCode::LeftBracket:
		case KeyCode::Backslash:
		case KeyCode::RightBracket:
			glfwKeyCode = static_cast<int>(nightOwlKeyCode) + GLFW_KEY_A - static_cast<int>(KeyCode::A);
			break;

		case KeyCode::GraveAccent:
			glfwKeyCode = GLFW_KEY_GRAVE_ACCENT;
			break;

		case KeyCode::Escape:
		case KeyCode::Enter:
		case KeyCode::Tab:
		case KeyCode::Backspace:
		case KeyCode::Insert:
		case KeyCode::Delete:
		case KeyCode::Right:
		case KeyCode::Left:
		case KeyCode::Down:
		case KeyCode::Up:
		case KeyCode::PageUp:
		case KeyCode::PageDown:
		case KeyCode::Home:
		case KeyCode::End:
			glfwKeyCode = static_cast<int>(nightOwlKeyCode) + GLFW_KEY_ESCAPE - static_cast<int>(KeyCode::Escape);
			break;

		case KeyCode::CapsLock:
		case KeyCode::ScrollLock:
		case KeyCode::NumLock:
		case KeyCode::PrintScreen:
		case KeyCode::Pause:
			glfwKeyCode = static_cast<int>(nightOwlKeyCode) + GLFW_KEY_CAPS_LOCK - static_cast<int>(KeyCode::CapsLock);
			break;

		case KeyCode::F1:
		case KeyCode::F2:
		case KeyCode::F3:
		case KeyCode::F4:
		case KeyCode::F5:
		case KeyCode::F6:
		case KeyCode::F7:
		case KeyCode::F8:
		case KeyCode::F9:
		case KeyCode::F10:
		case KeyCode::F11:
		case KeyCode::F12:
		case KeyCode::F13:
		case KeyCode::F14:
		case KeyCode::F15:
		case KeyCode::F16:
		case KeyCode::F17:
		case KeyCode::F18:
		case KeyCode::F19:
		case KeyCode::F20:
		case KeyCode::F21:
		case KeyCode::F22:
		case KeyCode::F23:
		case KeyCode::F24:
		case KeyCode::F25:
			glfwKeyCode = static_cast<int>(nightOwlKeyCode) + GLFW_KEY_F1 - static_cast<int>(KeyCode::F1);
			break;

		case KeyCode::NumPad0:
		case KeyCode::NumPad1:
		case KeyCode::NumPad2:
		case KeyCode::NumPad3:
		case KeyCode::NumPad4:
		case KeyCode::NumPad5:
		case KeyCode::NumPad6:
		case KeyCode::NumPad7:
		case KeyCode::NumPad8:
		case KeyCode::NumPad9:
		case KeyCode::NumPadDecimal:
		case KeyCode::NumPadDivide:
		case KeyCode::NumPadMultiply:
		case KeyCode::NumPadSubtract:
		case KeyCode::NumPadAdd:
		case KeyCode::NumPadEnter:
		case KeyCode::NumPadEqual:
			glfwKeyCode = static_cast<int>(nightOwlKeyCode) + GLFW_KEY_KP_0 - static_cast<int>(KeyCode::NumPad0);
			break;

		case KeyCode::LeftShift:
		case KeyCode::LeftControl:
		case KeyCode::LeftAlt:
		case KeyCode::LeftSuper:
		case KeyCode::RightShift:
		case KeyCode::RightControl:
		case KeyCode::RightAlt:
		case KeyCode::RightSuper:
		case KeyCode::Menu:
			glfwKeyCode = static_cast<int>(nightOwlKeyCode) + GLFW_KEY_LEFT_SHIFT - static_cast<int>(KeyCode::LeftShift);
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
			nightOwlKeyCode = KeyCode::None;
			break;

		case GLFW_KEY_SPACE:
			nightOwlKeyCode = KeyCode::Space;
			break;

		case GLFW_KEY_APOSTROPHE:
			nightOwlKeyCode = KeyCode::Apostrophe;
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
			nightOwlKeyCode = static_cast<KeyCode>(static_cast<int>(KeyCode::Comma) + glfwKeyCode - GLFW_KEY_COMMA);
			break;

		case GLFW_KEY_SEMICOLON:
			nightOwlKeyCode = KeyCode::Semicolon;
			break;

		case GLFW_KEY_EQUAL:
			nightOwlKeyCode = KeyCode::Equal;
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
			nightOwlKeyCode = static_cast<KeyCode>(static_cast<int>(KeyCode::A) + glfwKeyCode - GLFW_KEY_A);
			break;

		case GLFW_KEY_GRAVE_ACCENT:
			nightOwlKeyCode = KeyCode::GraveAccent;
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
			nightOwlKeyCode = static_cast<KeyCode>(static_cast<int>(KeyCode::Escape) + glfwKeyCode - GLFW_KEY_ESCAPE);
			break;

		case GLFW_KEY_CAPS_LOCK:
		case GLFW_KEY_SCROLL_LOCK:
		case GLFW_KEY_NUM_LOCK:
		case GLFW_KEY_PRINT_SCREEN:
		case GLFW_KEY_PAUSE:
			nightOwlKeyCode = static_cast<KeyCode>(static_cast<int>(KeyCode::CapsLock) + glfwKeyCode - GLFW_KEY_CAPS_LOCK);
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
			nightOwlKeyCode = static_cast<KeyCode>(static_cast<int>(KeyCode::F1) + glfwKeyCode - GLFW_KEY_F1);
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
			nightOwlKeyCode = static_cast<KeyCode>(static_cast<int>(KeyCode::NumPad0) + glfwKeyCode - GLFW_KEY_KP_0);
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
			nightOwlKeyCode = static_cast<KeyCode>(static_cast<int>(KeyCode::LeftShift) + glfwKeyCode - GLFW_KEY_LEFT_SHIFT);
			break;

		default:
			nightOwlKeyCode = KeyCode::None;
			break;
		}

		return nightOwlKeyCode;
	}
}
