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
InputKey ImguiKeyToUCodeKey(ImGuiKey key)
{
	switch (key)
	{
	case ImGuiKey::ImGuiKey_MouseLeft: return InputKey::LeftMouseButtion;
	case ImGuiKey::ImGuiKey_MouseRight: return InputKey::RightMouseButtion;
	case ImGuiKey::ImGuiKey_MouseMiddle: return InputKey::MiddleMouseButtion;

	case ImGuiKey::ImGuiKey_Space: return InputKey::Space;
	case ImGuiKey::ImGuiKey_Tab: return InputKey::Tab;
	case ImGuiKey::ImGuiKey_Escape: return InputKey::Escape;
	case ImGuiKey::ImGuiKey_Enter: return InputKey::Enter;
	case ImGuiKey::ImGuiKey_Backspace: return InputKey::BackSpace;
	case ImGuiKey::ImGuiKey_Delete: return InputKey::Delete;

	case ImGuiKey::ImGuiKey_LeftArrow: return InputKey::LeftArrow;
	case ImGuiKey::ImGuiKey_RightArrow: return InputKey::RightArrow;
	case ImGuiKey::ImGuiKey_DownArrow: return InputKey::DownArrow;
	case ImGuiKey::ImGuiKey_UpArrow:return InputKey::UpArrow;

	case ImGuiKey::ImGuiKey_0:return InputKey::Key_0;
	case ImGuiKey::ImGuiKey_1:return InputKey::Key_1;
	case ImGuiKey::ImGuiKey_2:return InputKey::Key_2;
	case ImGuiKey::ImGuiKey_3:return InputKey::Key_3;
	case ImGuiKey::ImGuiKey_4:return InputKey::Key_4;
	case ImGuiKey::ImGuiKey_5:return InputKey::Key_5;
	case ImGuiKey::ImGuiKey_6:return InputKey::Key_6;
	case ImGuiKey::ImGuiKey_7:return InputKey::Key_7;
	case ImGuiKey::ImGuiKey_8:return InputKey::Key_8;
	case ImGuiKey::ImGuiKey_9:return InputKey::Key_9;
		//leters
	case ImGuiKey::ImGuiKey_A:return InputKey::A;
	case ImGuiKey::ImGuiKey_B:return InputKey::B;
	case ImGuiKey::ImGuiKey_C:return InputKey::C;
	case ImGuiKey::ImGuiKey_D:return InputKey::D;
	case ImGuiKey::ImGuiKey_E:return InputKey::E;
	case ImGuiKey::ImGuiKey_F:return InputKey::F;
	case ImGuiKey::ImGuiKey_G:return InputKey::G;
	case ImGuiKey::ImGuiKey_H:return InputKey::H;
	case ImGuiKey::ImGuiKey_I:return InputKey::I;
	case ImGuiKey::ImGuiKey_J:return InputKey::J;
	case ImGuiKey::ImGuiKey_K:return InputKey::K;
	case ImGuiKey::ImGuiKey_L:return InputKey::L;
	case ImGuiKey::ImGuiKey_N:return InputKey::N;
	case ImGuiKey::ImGuiKey_O:return InputKey::O;
	case ImGuiKey::ImGuiKey_P:return InputKey::P;
	case ImGuiKey::ImGuiKey_Q:return InputKey::Q;
	case ImGuiKey::ImGuiKey_R:return InputKey::R;
	case ImGuiKey::ImGuiKey_S:return InputKey::S;
	case ImGuiKey::ImGuiKey_T:return InputKey::T;
	case ImGuiKey::ImGuiKey_U:return InputKey::U;
	case ImGuiKey::ImGuiKey_W:return InputKey::W;
	case ImGuiKey::ImGuiKey_X:return InputKey::X;
	case ImGuiKey::ImGuiKey_Z:return InputKey::Y;



	default:return  InputKey::Null;
	}


}
RenderAPIEnd

