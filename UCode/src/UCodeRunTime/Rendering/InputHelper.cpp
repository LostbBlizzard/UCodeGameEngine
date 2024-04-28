#include "InputHelper.hpp"
#include "unordered_map"
RenderAPIStart

std::unordered_map<InputKey, String> KeyToStringMap =
{
	{InputKey::A,"A"},
	{InputKey::B,"B"},
	{InputKey::C,"C"},
	{InputKey::D,"D"},
	{InputKey::E,"E"},
	{InputKey::F,"F"},
	{InputKey::G,"G"},
	{InputKey::H,"H"},
	{InputKey::I,"I"},
	{InputKey::J,"J"},
	{InputKey::K,"K"},
	{InputKey::L,"L"},
	{InputKey::N,"N"},
	{InputKey::M,"M"},
	{InputKey::O,"O"},
	{InputKey::P,"P"},
	{InputKey::Q,"Q"},
	{InputKey::R,"R"},
	{InputKey::S,"S"},
	{InputKey::T,"T"},
	{InputKey::U,"U"},
	{InputKey::V,"V"},
	{InputKey::W,"W"},
	{InputKey::X,"X"},
	{InputKey::W,"W"},
	{InputKey::Y,"Y"},
	{InputKey::Z,"Z"},
	{InputKey::Escape,"Escape"},
	{InputKey::Space,"Space"},
	{InputKey::DownArrow,"DownArrow"},
	{InputKey::UpArrow,"UpArrow"},
	{InputKey::LeftArrow,"LeftArrow"},
	{InputKey::RightArrow,"RightArrow"},
	{InputKey::Null,"null"},
};
String UCodeKeyToString(InputKey key)
{
	return KeyToStringMap.at(key);
}

InputKey ToStringToKey(StringView str)
{
	for (auto& Item : KeyToStringMap)
	{
		if (Item.second == str)
		{
			return Item.first;
		}
	}
	return InputKey::Escape;
}
std::unordered_map<ImGuiKey, InputKey> ImGuikeytoKeyMap =
{
	{ ImGuiKey::ImGuiKey_MouseLeft, InputKey::LeftMouseButtion},
	{ ImGuiKey::ImGuiKey_MouseRight,InputKey::RightMouseButtion },
	{ ImGuiKey::ImGuiKey_MouseMiddle, InputKey::MiddleMouseButtion },

	{ ImGuiKey::ImGuiKey_Space, InputKey::Space },
	{ ImGuiKey::ImGuiKey_Tab, InputKey::Tab },
	{ ImGuiKey::ImGuiKey_Escape, InputKey::Escape},
	{ ImGuiKey::ImGuiKey_Enter, InputKey::Enter },
	{ ImGuiKey::ImGuiKey_Backspace, InputKey::BackSpace },
	{ ImGuiKey::ImGuiKey_Delete, InputKey::Delete },

	{ ImGuiKey::ImGuiKey_LeftArrow, InputKey::LeftArrow },
	{ ImGuiKey::ImGuiKey_RightArrow, InputKey::RightArrow },
	{ ImGuiKey::ImGuiKey_DownArrow, InputKey::DownArrow },
	{ ImGuiKey::ImGuiKey_UpArrow, InputKey::UpArrow },

	{ ImGuiKey::ImGuiKey_0,InputKey::Key_0 },
	{ ImGuiKey::ImGuiKey_1,InputKey::Key_1 },
	{ImGuiKey::ImGuiKey_2,InputKey::Key_2},
	{ ImGuiKey::ImGuiKey_3,InputKey::Key_3},
	{ ImGuiKey::ImGuiKey_4,InputKey::Key_4},
	{ ImGuiKey::ImGuiKey_5,InputKey::Key_5},
	{ ImGuiKey::ImGuiKey_6,InputKey::Key_6},
	{ ImGuiKey::ImGuiKey_7,InputKey::Key_7},
	{ ImGuiKey::ImGuiKey_8,InputKey::Key_8},
	{ ImGuiKey::ImGuiKey_9,InputKey::Key_9},
	//leters

	{ ImGuiKey::ImGuiKey_A,InputKey::A},
	{ ImGuiKey::ImGuiKey_B,InputKey::B},
	{ ImGuiKey::ImGuiKey_C,InputKey::C},
	{ ImGuiKey::ImGuiKey_D,InputKey::D},
	{ ImGuiKey::ImGuiKey_E,InputKey::E},
	{ ImGuiKey::ImGuiKey_F,InputKey::F},
	{ ImGuiKey::ImGuiKey_G,InputKey::G},
	{ ImGuiKey::ImGuiKey_H,InputKey::H},
	{ ImGuiKey::ImGuiKey_I,InputKey::I},
	{ ImGuiKey::ImGuiKey_J,InputKey::J},
	{ ImGuiKey::ImGuiKey_K,InputKey::K},
	{ ImGuiKey::ImGuiKey_L,InputKey::L},
	{ ImGuiKey::ImGuiKey_N,InputKey::N},
	{ ImGuiKey::ImGuiKey_M,InputKey::M},
	{ ImGuiKey::ImGuiKey_O,InputKey::O},
	{ ImGuiKey::ImGuiKey_P,InputKey::P},
	{ ImGuiKey::ImGuiKey_Q,InputKey::Q},
	{ ImGuiKey::ImGuiKey_R,InputKey::R},
	{ ImGuiKey::ImGuiKey_S,InputKey::S},
	{ ImGuiKey::ImGuiKey_T,InputKey::T},
	{ ImGuiKey::ImGuiKey_U,InputKey::U},
	{ ImGuiKey::ImGuiKey_V,InputKey::V},
	{ ImGuiKey::ImGuiKey_W,InputKey::W},
	{ ImGuiKey::ImGuiKey_X,InputKey::X},
	{ ImGuiKey::ImGuiKey_Y,InputKey::Y},
	{ ImGuiKey::ImGuiKey_Z,InputKey::Z},
	{ ImGuiKey::ImGuiKey_Z,InputKey::Z},
};
InputKey ImguiKeyToUCodeKey(ImGuiKey key)
{
	if (!ImGuikeytoKeyMap.count(key))
	{
		return InputKey::Null;
	}
	return ImGuikeytoKeyMap.at(key);
}
ImGuiKey UCodeToImguiKey(InputKey  key)
{
	for (auto& Item : ImGuikeytoKeyMap)
	{
		if (Item.second == key)
		{
			return Item.first;
		}
	}
	UCodeGEUnreachable();
}
RenderAPIEnd

