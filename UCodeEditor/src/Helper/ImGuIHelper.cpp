#include "ImGuIHelper.hpp"
#include "Imgui/imgui_internal.h"
#include <Imgui/misc/cpp/imgui_stdlib.h>
#include "Helper/UserSettings.hpp"
EditorStart

bool IsPopUpKey()
{	
	return ImGui::IsKeyDown(ImGuiKey_ModCtrl) && UserSettings::GetSettings().IsKeybindActive(KeyBindList::Extra);
}

bool ImGuIHelper::BeginPopupContextItem(const char* str_id, ImGuiPopupFlags popup_flags)
{
	ImGuiContext& g = *GImGui;
	ImGuiWindow* window = g.CurrentWindow;
	if (window->SkipItems)
		return false;
	ImGuiID id = str_id ? window->GetID(str_id) : g.LastItemData.ID;    // If user hasn't passed an ID, we can use the LastItemID. Using LastItemID as a Popup ID won't conflict!
	IM_ASSERT(id != 0);                                             // You cannot pass a NULL str_id if the last item has no identifier (e.g. a Text() item)
	int mouse_button = (popup_flags & ImGuiPopupFlags_MouseButtonMask_);
	if ((ImGui::IsMouseReleased(mouse_button) && ImGui::IsItemHovered(ImGuiHoveredFlags_AllowWhenBlockedByPopup)) || (ImGui::IsItemFocused() && IsPopUpKey()))
		ImGui::OpenPopupEx(id, popup_flags);
	return ImGui::BeginPopupEx(id, ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoSavedSettings);

}
const char* ImGuIHelper::ToCStr(const StringView text)
{
	thread_local String str;
	str = text;
	return str.c_str();
}
const char* ImGuIHelper::ToCStr(const String& text)
{
	return text.c_str();
}
void ImGuIHelper::ImageDontKeepAspectRatio(UCode::Texture& texture, const ImVec2& ButtionSize)
{
	texture.TryUploadTexToGPU();
	ImGui::Image((ImTextureID)(size_t)texture.Get_RendererID(), ButtionSize);
}
void ImGuIHelper::ImageDontKeepAspectRatio(UCode::Sprite& Sprite, const ImVec2& ButtionSize)
{
	ImVec2 uvo;
	ImVec2 uv1;
	ImTextureID texid = (ImTextureID)(size_t)Sprite.Get_texture()->Get_RendererID();
	GetSpriteUV(&Sprite, uvo, uv1);

	ImGui::Image(texid, ButtionSize, uvo, uv1);
}
bool ImGuIHelper::ImageButtonDontKeepAspectRatio(const void* id, UCode::Texture* Sprite, const ImVec2& ButtionSize)
{
	Sprite->TryUploadTexToGPU();
	return ImGui::ImageButtonEx(ImGui::GetID(id), (ImTextureID)(size_t)Sprite->Get_RendererID(), ButtionSize, { 0,0 }, { 1,1 }, ImVec4(0, 0, 0, 0), ImVec4(1, 1, 1, 1));
}
bool ImGuIHelper::ImageButtonDontKeepAspectRatio(const void* id, UCode::Sprite* Sprite, const ImVec2& ButtionSize)
{
	ImVec2 uvo;
	ImVec2 uv1;
	ImTextureID texid = (ImTextureID)(size_t)Sprite->Get_texture()->Get_RendererID();
	GetSpriteUV(Sprite, uvo, uv1);
	return ImGui::ImageButtonEx(ImGui::GetID(id), texid, ButtionSize, uvo, uv1, ImVec4(0, 0, 0, 0), ImVec4(1, 1, 1, 1));
}


Vec2 ImageSize(Vec2 spritesize, Vec2 buttionsize)
{
	Vec2 r;
	r.X = buttionsize.X;
	r.Y = buttionsize.Y;

	//r = buttionsize /2;

	UCodeGEAssert(r.X > buttionsize.X)
		UCodeGEAssert(r.Y > buttionsize.Y)
		return r;
}

void ImGuIHelper::Image(UCode::Texture* texture, const ImVec2& ButtionSize)
{
	texture->TryUploadTexToGPU();

	auto p = ImageSize({ (float)texture->Get_Width(),(float)texture->Get_Height() }, { ButtionSize.x,ButtionSize.y });
	ImVec2 v;
	v.x = p.X;
	v.y = p.Y;


	auto c = ImGui::GetCursorPos();

	ImGui::Dummy(ButtionSize);

	auto d = ImGui::GetCursorPos();

	ImGui::SetCursorPos(c);

	ImGui::Image((ImTextureID)(size_t)texture->Get_RendererID(), v);

	ImGui::SetCursorPos(d);
}
void ImGuIHelper::Image(UCode::Sprite* Sprite, const ImVec2& ButtionSize)
{
	ImVec2 uvo;
	ImVec2 uv1;
	ImTextureID texid = (ImTextureID)(size_t)Sprite->Get_texture()->Get_RendererID();
	GetSpriteUV(Sprite, uvo, uv1);

	auto p = ImageSize({ (float)Sprite->Get_Width(),(float)Sprite->Get_Height() }, { ButtionSize.x,ButtionSize.y });
	ImVec2 v;
	v.x = p.X;
	v.y = p.Y;

	auto c = ImGui::GetCursorPos();

	ImGui::Dummy(ButtionSize);

	auto d = ImGui::GetCursorPos();

	ImGui::SetCursorPos(c);

	ImGui::Image(texid, v, uvo, uv1);

	ImGui::SetCursorPos(d);
}
bool ImGuIHelper::ImageButton(const void* id, UCode::Texture* Sprite, const ImVec2& ButtionSize)
{
	Sprite->TryUploadTexToGPU();

	auto p = ImageSize({ (float)Sprite->Get_Width(),(float)Sprite->Get_Height() }, { ButtionSize.x,ButtionSize.y });
	ImVec2 v;
	v.x = p.X;
	v.y = p.Y;

	auto c = ImGui::GetCursorPos();

	bool hovered;
	bool held;
	bool r;
	//ImGui::PushID(id); 
	{
		using namespace ImGui;

		ImGuiContext& g = *GImGui;
		ImGuiWindow* window = GetCurrentWindow();

		const ImGuiID id2 = window->GetID(id);
		ImVec2 size = CalcItemSize(ButtionSize, 0.0f, 0.0f);
		auto tep = (*(Vec2*)&window->DC.CursorPos + *(Vec2*)&size);
		const ImRect bb(window->DC.CursorPos, *(ImVec2*)&tep);
		ItemSize(size);
		if (!ItemAdd(bb, id2))
			return false;

		r = ButtonBehavior(bb, id2, &hovered, &held);
	}
	//ImGui::PopID();


	auto d = ImGui::GetCursorPos();

	ImGui::SetCursorPos(c);

	{
		using namespace ImGui;
		ImGuiContext& g = *GImGui;
		ImGuiWindow* window = GetCurrentWindow();
		if (window->SkipItems)
			return false;

		const ImVec2 padding = g.Style.FramePadding;
		auto tep = (*(Vec2*)&window->DC.CursorPos + *(Vec2*)&ButtionSize + *(Vec2*)&padding * 2.0f);
		const ImRect bb(window->DC.CursorPos, *(ImVec2*)&tep);

		const ImU32 col = GetColorU32((held && hovered) ? ImGuiCol_ButtonActive : hovered ? ImGuiCol_ButtonHovered : ImGuiCol_Button);
		ImGui::RenderNavHighlight(bb, ImGui::GetID(id));
		ImGui::RenderFrame(bb.Min, bb.Max, col, true, ImClamp((float)ImMin(padding.x, padding.y), 0.0f, g.Style.FrameRounding));

		{
			auto tep = (*(Vec2*)&window->DC.CursorPos + *(Vec2*)&v + *(Vec2*)&padding * 2.0f);
			const ImRect bb(window->DC.CursorPos, *(ImVec2*)&tep);

			auto tep2 = (*(Vec2*)&bb.Min + *(Vec2*)&padding);
			auto tep3 = (*(Vec2*)&bb.Max - *(Vec2*)&padding);
			window->DrawList->AddImage((ImTextureID)(size_t)Sprite->Get_RendererID(),
				*(ImVec2*)&tep2, *(ImVec2*)&tep3, { 0,0, }, { 1,1 });
		}
	}
	//ImGui::Image((ImTextureID)(size_t)Sprite->Get_RendererID(), v, { 0,0 }, { 1,1 });

	ImGui::SetCursorPos(d);

	return r;
}

bool ImGuIHelper::ImageButton(const void* id, UCode::Sprite* Sprite, const ImVec2& ButtionSize)
{
	ImVec2 uvo;
	ImVec2 uv1;
	ImTextureID texid = (ImTextureID)(size_t)Sprite->Get_texture()->Get_RendererID();
	GetSpriteUV(Sprite, uvo, uv1);

	auto p = ImageSize({ (float)Sprite->Get_Width(),(float)Sprite->Get_Height() }, { ButtionSize.x,ButtionSize.y });
	ImVec2 v;
	v.x = p.X;
	v.y = p.Y;

	auto c = ImGui::GetCursorPos();

	bool hovered;
	bool held;
	bool r;
	//ImGui::PushID(id); 
	{
		using namespace ImGui;

		ImGuiContext& g = *GImGui;
		ImGuiWindow* window = GetCurrentWindow();

		const ImGuiID id2 = window->GetID(id);
		ImVec2 size = CalcItemSize(ButtionSize, 0.0f, 0.0f);
		auto tep = (*(Vec2*)&window->DC.CursorPos + *(Vec2*)&size);
		const ImRect bb(window->DC.CursorPos, *(ImVec2*)&tep);
		ItemSize(size);
		if (!ItemAdd(bb, id2))
			return false;

		r = ButtonBehavior(bb, id2, &hovered, &held);
	}
	//ImGui::PopID();


	auto d = ImGui::GetCursorPos();

	ImGui::SetCursorPos(c);

	{
		using namespace ImGui;
		ImGuiContext& g = *GImGui;
		ImGuiWindow* window = GetCurrentWindow();
		if (window->SkipItems)
			return false;

		const ImVec2 padding = g.Style.FramePadding;
		auto tep4 = (*(Vec2*)&window->DC.CursorPos + *(Vec2*)&ButtionSize + *(Vec2*)&padding * 2.0f);
		const ImRect bb(window->DC.CursorPos, *(ImVec2*)&tep4);

		const ImU32 col = GetColorU32((held && hovered) ? ImGuiCol_ButtonActive : hovered ? ImGuiCol_ButtonHovered : ImGuiCol_Button);
		ImGui::RenderNavHighlight(bb, ImGui::GetID(id));
		ImGui::RenderFrame(bb.Min, bb.Max, col, true, ImClamp((float)ImMin(padding.x, padding.y), 0.0f, g.Style.FrameRounding));

		{
			auto tep3 = (*(Vec2*)&window->DC.CursorPos + *(Vec2*)&v + *(Vec2*)&padding * 2.0f);
			const ImRect bb(window->DC.CursorPos, *(ImVec2*)&tep3);
			auto tep = (*(Vec2*)&bb.Min + *(Vec2*)&padding);
			auto tep2 = (*(Vec2*)&bb.Max - *(Vec2*)&padding);

			window->DrawList->AddImage(texid,
				*(ImVec2*)&tep, *(ImVec2*)&tep2, { 0,0, }, { 1,1 });
		}
	}

	ImGui::SetCursorPos(d);

	return r;
}
void ImGuIHelper::GetSpriteUV(UCode::Sprite* Sprite, ImVec2& uvo, ImVec2& uv1)
{
	auto tex = Sprite->Get_texture();
	if (tex->IsUploadedToGPU())
	{


		float w = (float)tex->Get_Width();
		float h = (float)tex->Get_Height();

		float sx = (float)Sprite->Get_Xoffset();
		float sy = (float)Sprite->Get_Yoffset();
		float sw;
		float sh;

		if (Sprite->Get_Width() == UCode::Sprite::GetTexureSize)
		{
			sw = (float)tex->Get_Width();
		}
		else
		{
			sw = (float)Sprite->Get_Width();
		}

		if (Sprite->Get_Height() == UCode::Sprite::GetTexureSize)
		{
			sh = (float)tex->Get_Height();
		}
		else
		{
			sh = (float)Sprite->Get_Height();
		}



		uvo = { sx / w      ,sy / h };
		uv1 = { (sx + sw) / w,(sy + sh) / h };
	}
	else
	{
		tex->TryUploadTexToGPU();


		uvo = { 0,0 };
		uv1 = { 1,1 };
	}
}

ImVec2 ImGuIHelper::CheckBoxSize()
{
	const float square_sz = ImGui::GetFrameHeight();
	return ImVec2(square_sz, square_sz);
}
ImVec2 ImGuIHelper::CheckBoxSizeWithPadding()
{
	const float square_sz = ImGui::GetFrameHeight();

    const float pad = ImMax(1.0f, IM_TRUNC(square_sz / 6.0f));
	return ImVec2(square_sz + 2 + pad * 2, square_sz + 2 + pad * 2);
}

//From https://github.com/ocornut/imgui/issues/3469
void ImGuIHelper::ItemLabel(StringView title, ItemLabelFlag flags)
{
    ImGuiWindow* window = ImGui::GetCurrentWindow();
    const ImVec2 lineStart = ImGui::GetCursorScreenPos();
    const ImGuiStyle& style = ImGui::GetStyle();
    float fullWidth = ImGui::GetContentRegionAvail().x;
    float itemWidth = ImGui::CalcItemWidth() + style.ItemSpacing.x;
    ImVec2 textSize = ImGui::CalcTextSize(title.data(), title.data() + title.size());
    ImRect textRect;
    textRect.Min = ImGui::GetCursorScreenPos();
    if (flags & ItemLabelFlag::Right)
        textRect.Min.x = textRect.Min.x + itemWidth;
    textRect.Max = textRect.Min;
    textRect.Max.x += fullWidth - itemWidth;
    textRect.Max.y += textSize.y;

    ImGui::SetCursorScreenPos(textRect.Min);

    ImGui::AlignTextToFramePadding();
    // Adjust text rect manually because we render it directly into a drawlist instead of using public functions.
    textRect.Min.y += window->DC.CurrLineTextBaseOffset;
    textRect.Max.y += window->DC.CurrLineTextBaseOffset;

    ImGui::ItemSize(textRect);
    if (ImGui::ItemAdd(textRect, window->GetID(title.data(), title.data() + title.size())))
    {
        ImGui::RenderTextEllipsis(ImGui::GetWindowDrawList(), textRect.Min, textRect.Max, textRect.Max.x,
            textRect.Max.x, title.data(), title.data() + title.size(), &textSize);

        if (textRect.GetWidth() < textSize.x && ImGui::IsItemHovered())
            ImGui::SetTooltip("%.*s", (int)title.size(), title.data());
    }
    if (flags & ItemLabelFlag::Left)
    {
		auto v = ImVec2{ 0, textSize.y + window->DC.CurrLineTextBaseOffset };
		auto n = textRect.Max;
		n.x -= v.x;
		n.x -= v.x;
        ImGui::SetCursorScreenPos(n);
        ImGui::SameLine();
    }
    else if (flags & ItemLabelFlag::Right)
        ImGui::SetCursorScreenPos(lineStart);
}

bool ImGuIHelper::InputText(const char* label, String& buffer, ImGuiInputTextFlags flags)
{
	ItemLabel(StringView(label), ItemLabelFlag::Left);

	//ImGui::SameLine();


	ImGui::PushID(&buffer);
	//ImGui::PushItemWidth(ImGui::GetContentRegionAvail().x);
	auto V = ImGui::InputText("", &buffer, flags);
	//ImGui::PopItemWidth();
	ImGui::PopID();

	return V;
}
bool ImGuIHelper::MultLineText(const char* label, String& buffer, ImVec2 Size, ImGuiInputTextFlags flags)
{
	return  ImGui::InputTextMultiline(label, &buffer, Size, flags);
}
bool ImGuIHelper::InputPath(const char* label, String& buffer, ImGuiInputTextFlags flags, InputPathType PathType, Span<String> Extension)
{
	bool R = InputText(label, buffer, flags);

	ImGui::SameLine();

	if (ImGui::Button("+", { 20,20 }))
	{
		FileHelper::OpenFileData _Data;
		if (PathType == InputPathType::Dir)
		{
			_Data = FileHelper::OpenDirFromUser(buffer);
		}
		else
		{
			Path ExtWithNoDot = {};
			for (size_t i = 0; i < Extension.Size(); i++)
			{
				auto& Item = Extension[i];

				if (Item[0] == '.')
				{
					ExtWithNoDot += Item.substr(1);
				}
				else
				{
					ExtWithNoDot = Item;
				}
				ExtWithNoDot += ",";

			}
			_Data = FileHelper::OpenFileFromUser(ExtWithNoDot, buffer);
		}


		if (_Data.Result == FileHelper::OpenFileResult::OKAY)
		{
			buffer = _Data.Path;
			R = true;
		}
	}

	return R;
}
bool ImGuIHelper::InputText(const StringView label, String& buffer, ImGuiInputTextFlags flags)
{
	return InputText(ToCStr(label), buffer, flags);
}
bool ImGuIHelper::MultLineText(const StringView label, String& buffer, ImVec2 Size, ImGuiInputTextFlags flags)
{
	return MultLineText(ToCStr(label), buffer, Size, flags);
}
bool ImGuIHelper::TreeNode(const void* id, const StringView label, UCode::Sprite* Sprite)
{
	return TreeNode(id, ToCStr(label), Sprite);
}

bool ImGuIHelper::InputPath(const StringView label, String& buffer, ImGuiInputTextFlags flags, InputPathType PathType, Span<String> Extension)
{
	return InputPath(ToCStr(label), buffer, flags, PathType, Extension);
}
bool ImGuIHelper::InputPath(const StringView label, Path& buffer, ImGuiInputTextFlags flags, InputPathType PathType, Span<String> Extension)
{
	return InputPath(ToCStr(label), buffer, flags, PathType, Extension);
}


bool ImGuIHelper::InputPath(const char* label, Path& buffer, ImGuiInputTextFlags flags, InputPathType PathType, Span<String> Extension)
{
	String TepStr = buffer.generic_string();

	if (InputPath(label, TepStr, flags, PathType, Extension))
	{
		buffer = TepStr;

		return true;
	}

	return false;
}
ImGuIHelper::TreeNodeText ImGuIHelper::TreeNode(const void* id, String& buffer)
{
	ImGuIHelper::TreeNodeText Data;

	Data.TextUpdate = InputText("##", buffer);
	ImGui::SameLine();

	Data.NodeOpen = ImGui::TreeNode(id, "");

	return Data;
}
bool ImGuIHelper::TreeNode(const void* id, const char* label, UCode::Sprite* Sprite)
{
	auto& g = *GImGui;
	auto SizeY = g.FontSize + g.Style.FramePadding.y * 2;
	ImGuIHelper::Image(Sprite, ImVec2(SizeY, SizeY)); ImGui::SameLine();
	bool r = ImGui::TreeNode(id, label);
	return r;
}
ImGuIHelper::TreeNodeText ImGuIHelper::TreeNode(const void* id, String& buffer, UCode::Sprite* Sprite)
{
	auto& g = *GImGui;


	ImGuIHelper::Image(Sprite, ImVec2(20, g.FontSize + g.Style.FramePadding.y * 2)); ImGui::SameLine();
	return TreeNode(id, buffer);
}

bool ImGuIHelper::TreeNode(const void* id, StringView label, AppFiles::sprite Sprite)
{
	return ImGuIHelper::TreeNode(id, ToCStr(label), Sprite);
}

bool ImGuIHelper::ToggleField(const char* FieldName, bool& Value)
{
	ItemLabel(StringView(FieldName), ItemLabelFlag::Left);	

	ImGui::PushID(&Value);
	auto V = ImGui::Checkbox("", &Value);
	ImGui::PopID();

	return V;
}

bool ImGuIHelper::Vec2Field(const char* FieldName, UCode::Vec2& Value)
{	
	ItemLabel(StringView(FieldName), ItemLabelFlag::Left);	


	ImGui::PushID(&Value);
	auto V = ImGui::DragFloat2("", &Value.X);
	ImGui::PopID();

	return V;
}

bool ImGuIHelper::Vec2Field(const char* FieldName, UCode::Vec2i& Value)
{
	ItemLabel(StringView(FieldName), ItemLabelFlag::Left);	
	
	ImGui::PushID(&Value);
	auto V = ImGui::DragInt2("", &Value.X);
	ImGui::PopID();

	return V;
}

bool ImGuIHelper::Vec3Field(const char* FieldName, UCode::Vec3& Value)
{
	ItemLabel(StringView(FieldName), ItemLabelFlag::Left);	
	
	ImGui::PushID(&Value);
	auto V = ImGui::DragFloat3("", &Value.X);
	ImGui::PopID();

	return V;
}

bool ImGuIHelper::Vec3Field(const char* FieldName, UCode::Vec3i& Value)
{
	ItemLabel(StringView(FieldName), ItemLabelFlag::Left);	
	
	ImGui::PushID(&Value);
	auto V = ImGui::DragInt3("", &Value.X);
	ImGui::PopID();

	return V;
}

bool ImGuIHelper::ColorField(const char* FieldName, UCode::ColorRGBA& Value)
{
	ItemLabel(StringView(FieldName), ItemLabelFlag::Left);	
	
	ImGui::PushID(&Value);
	auto V = ImGui::ColorEdit4("", &Value.R);
	ImGui::PopID();

	return V;
}

bool ImGuIHelper::ColorField(const char* FieldName, UCode::ColorRGB& Value)
{
	ItemLabel(StringView(FieldName), ItemLabelFlag::Left);	
	
	ImGui::PushID(&Value);
	auto V = ImGui::ColorEdit3("", &Value.R);
	ImGui::PopID();

	return V;
}

bool ImGuIHelper::ColorField(const char* FieldName, UCode::Color32& Value)
{
	ItemLabel(StringView(FieldName), ItemLabelFlag::Left);	
	
	ImGui::PushID(&Value);
	auto V = ImGui::ColorEdit4("", (float*)&Value.R, ImGuiColorEditFlags_::ImGuiColorEditFlags_Uint8);
	ImGui::PopID();

	return V;
}

bool ImGuIHelper::ColorField(const char* FieldName, UCode::Color24& Value)
{
	ItemLabel(StringView(FieldName), ItemLabelFlag::Left);	
	
	ImGui::PushID(&Value);
	auto V = ImGui::ColorEdit3("", (float*)&Value.R, ImGuiColorEditFlags_::ImGuiColorEditFlags_Uint8);

	ImGui::PopID();

	return V;
}

bool ImGuIHelper::uInt64Field(const char* FieldName, u64& Value)
{
	ItemLabel(StringView(FieldName), ItemLabelFlag::Left);	
	
	ImGui::PushID(&Value);
	auto V = ImGui::DragScalar("", ImGuiDataType_U64, (void*)&Value);
	ImGui::PopID();

	return V;
}

bool ImGuIHelper::uInt32Field(const char* FieldName, u32& Value)
{
	ItemLabel(StringView(FieldName), ItemLabelFlag::Left);	
	
	ImGui::PushID(&Value);
	auto V = ImGui::DragScalar("", ImGuiDataType_U32, (void*)&Value);
	ImGui::PopID();

	return V;
}

bool ImGuIHelper::uInt16Field(const char* FieldName, u16& Value)
{
	ItemLabel(StringView(FieldName), ItemLabelFlag::Left);	
	
	ImGui::PushID(&Value);
	auto V = ImGui::DragScalar("", ImGuiDataType_U16, (void*)&Value);
	ImGui::PopID();

	return V;
}

bool ImGuIHelper::uInt8Field(const char* FieldName, u8& Value)
{
	ItemLabel(StringView(FieldName), ItemLabelFlag::Left);	
	
	ImGui::PushID(&Value);
	auto V = ImGui::DragScalar("", ImGuiDataType_U8, (void*)&Value);
	ImGui::PopID();

	return V;
}

void ImGuIHelper::LoadingIndicatorCircle(const char* label, const float indicator_radius, const ImVec4& main_color, const ImVec4& backdrop_color, const int circle_count, const float speed) {
	ImGuiWindow* window = ImGui::GetCurrentWindow();
	if (window->SkipItems) {
		return;
	}

	ImGuiContext& g = *GImGui;
	const ImGuiID id = window->GetID(label);
	const ImGuiStyle& style = g.Style;

	const ImVec2 pos = window->DC.CursorPos;
	const float circle_radius = indicator_radius / 10.0f;
	const ImRect bb(pos, ImVec2(pos.x + indicator_radius * 2.0f,
		pos.y + indicator_radius * 2.0f));
	ImGui::ItemSize(bb, style.FramePadding.y);
	if (!ImGui::ItemAdd(bb, id)) {
		return;
	}
	const float t = g.Time;
	const auto degree_offset = 2.0f * IM_PI / circle_count;
	for (int i = 0; i < circle_count; ++i) {
		const auto x = indicator_radius * std::sin(degree_offset * i);
		const auto y = indicator_radius * std::cos(degree_offset * i);
		const auto growth = std::max(0.0f, std::sin(t * speed - i * degree_offset));
		ImVec4 color;
		color.x = main_color.x * growth + backdrop_color.x * (1.0f - growth);
		color.y = main_color.y * growth + backdrop_color.y * (1.0f - growth);
		color.z = main_color.z * growth + backdrop_color.z * (1.0f - growth);
		color.w = 1.0f;
		window->DrawList->AddCircleFilled(ImVec2(pos.x + indicator_radius + x,
			pos.y + indicator_radius - y),
			circle_radius + growth * circle_radius,
			ImGui::GetColorU32(color));
	}
}
void ImGuIHelper::LoadingIndicatorCircle(const char* label, const float indicator_radius, const int circle_count, const float speed)
{
	auto& main_color = ImGui::GetStyleColorVec4(ImGuiCol_TitleBg);
	auto& backdrop_color = ImGui::GetStyleColorVec4(ImGuiCol_ChildBg);
	ImGuIHelper::LoadingIndicatorCircle(label, indicator_radius, main_color, backdrop_color, circle_count, speed);
}
void ImGuIHelper::ShowOnMiddleLoadingIndicatorCircle(const char* label, const int circle_count, const float speed)
{

	auto Size = ImGui::GetContentRegionAvail();
	ImVec2 NewSize;
	if (Size.x < Size.y)
	{
		NewSize = { Size.x,Size.x };
	}
	else
	{
		NewSize = { Size.y,Size.y };
	}
	f32 radius = NewSize.x / 2;

	auto tep = ImGui::GetItemRectMax();
	auto tep2 = ImGui::GetItemRectMin();
	auto pos = (*(Vec2*)&tep + *(Vec2*)&tep2) / 2;
	ImVec2 Pos = *(ImVec2*)&pos;
	ImGui::SetCursorPos(Pos);

	ImGuIHelper::LoadingIndicatorCircle("if", radius - 50.0f, circle_count, speed);
}
bool ImGuIHelper::DrawRenameTree(String& label, bool TreeIsOpened, bool& IsRenameing)
{
	return DrawRenameName(label, IsRenameing);
}
bool ImGuIHelper::DrawRenameTree(String& label, bool TreeIsOpened, bool& IsRenameing, UCode::Sprite* sprite)
{
	ImGuiContext& g = *GImGui;
	Image(sprite, ImVec2(20, g.FontSize + g.Style.FramePadding.y * 2));
	ImGui::SameLine();

	ImGui::SetKeyboardFocusHere();
	ImGui::PushID(&label);
	auto V = ImGui::InputText("", &label, ImGuiInputTextFlags_EnterReturnsTrue);
	ImGui::PopID();
	if (V)
	{
		IsRenameing = false;
		return true;
	}
	if (ImGui::IsKeyPressed(ImGuiKey::ImGuiKey_Escape)
		|| ImGui::IsMouseClicked(ImGuiMouseButton_::ImGuiMouseButton_Left)
		|| ImGui::IsMouseClicked(ImGuiMouseButton_::ImGuiMouseButton_Middle)
		|| ImGui::IsMouseClicked(ImGuiMouseButton_::ImGuiMouseButton_Right))
	{
		IsRenameing = false;
		return true;
	}

	return false;
}
bool ImGuIHelper::DrawRenameName(String& label, bool& IsRenameing)
{
	ImGui::SetKeyboardFocusHere();
	ImGui::PushID(&label);
	auto V = ImGui::InputText("", &label, ImGuiInputTextFlags_EnterReturnsTrue);
	ImGui::PopID();
	if (V)
	{
		IsRenameing = false;
		return true;
	}
	if (ImGui::IsKeyPressed(ImGuiKey::ImGuiKey_Escape)
		|| ImGui::IsMouseClicked(ImGuiMouseButton_::ImGuiMouseButton_Left)
		|| ImGui::IsMouseClicked(ImGuiMouseButton_::ImGuiMouseButton_Middle)
		|| ImGui::IsMouseClicked(ImGuiMouseButton_::ImGuiMouseButton_Right))
	{
		IsRenameing = false;
		return true;
	}

	return false;
}
bool ImGuIHelper::DrawVector(const char* label, void* Object, void* Buffer, size_t Size, const DrawVectorInfo& Item)
{
	bool WasUpdated = false;

	ItemLabel(StringView(label), ItemLabelFlag::Left);
	
	const ImVec2 imagesize = { 20,20 };	
	auto& spaceing = ImGui::GetStyle().ItemSpacing;
	
	
	ImGui::PushID(label);
	//ImGui::PushItemWidth(ImGui::CalcItemWidth() - (imagesize.x + spaceing.x));
	bool Value = ImGui::TreeNode("");
	//ImGui::PopItemWidth();
	ImGui::PopID();

	ImGui::SameLine();
	size_t NewSize = Size;


	ImGui::BeginDisabled(!Item._ResizeVector.has_value());
	{
		float mult = Value ? 3 : 1;

		ImGui::PushID(&Item);
		ImGui::PushItemWidth(ImGui::CalcItemWidth() + 5 - ( (imagesize.x +spaceing.x) * mult));
		bool ResizeWasUpdated = InputSize_t("", &NewSize, 1, 100, ImGuiInputTextFlags_EnterReturnsTrue);
		ImGui::PopItemWidth();
		ImGui::PopID();

		if (ResizeWasUpdated && Item._ResizeVector.has_value())
		{
			WasUpdated = true;
			Item._ResizeVector.value()(Object, NewSize);
		}

	}
	ImGui::EndDisabled();

	if (Value)
	{


		{
			ImGui::SameLine();

			ImVec2 ButtionSize = { 20,20 };

			ImGui::BeginDisabled(!Item._AddNewValue.has_value());
			if (ImGui::Button("+", ButtionSize))
			{
				WasUpdated = true;
				Item._AddNewValue.value()(Object, Size);
			}
			ImGui::EndDisabled();

			ImGui::SameLine();

			ImGui::BeginDisabled(!Item._AddNewRemove.has_value() || Size == 0);
			if (ImGui::Button("-", ButtionSize))
			{
				WasUpdated = true;
				Item._AddNewRemove.value()(Object, Size - 1);
			}
			ImGui::EndDisabled();
		}

		//ImGui::Separator();
		if (!WasUpdated) 
		{
			String Label;
			for (size_t i = 0; i < Size; i++)
			{
				Label = "Item:" + std::to_string(i);

				ItemLabel(Label,ItemLabelFlag::Left);

				bool RemoveItem = false;
				{
					void* ItemPtr = (Byte*)Buffer + (i * Item.ItemSize);
					ImGui::PushID(Label.c_str());
					if (ImGuIHelper::BeginPopupContextItem("????"))
					{
						if (ImGui::MenuItem("Remove Item"))
						{
							RemoveItem = true;
						}
						ImGui::EndPopup();
					}
					ImGui::PopID();
				}
				ImGui::SameLine();


				Item._OnDrawItem(Object, i);





				if (RemoveItem)
				{
					WasUpdated = true;
					Item._AddNewRemove.value()(Object, i);
					break;
				}
			}
		}

		ImGui::TreePop();
	}



	return  WasUpdated;
}
bool ImGuIHelper::InputSize_t(const char* label, size_t* v, int step, int step_fast, ImGuiInputTextFlags flags)
{
	if constexpr (sizeof(size_t) == sizeof(u64))
	{
		return ImGui::InputScalar(label, ImGuiDataType_::ImGuiDataType_U64, v, nullptr, nullptr, nullptr, flags);
	}
	else
	{
		return ImGui::InputScalar(label, ImGuiDataType_::ImGuiDataType_U32, v, nullptr, nullptr, nullptr, flags);
	}
}

#define ObjectNotNull "[value]"
#define ObjectNullStr "[empty]"
bool ImGuIHelper::Draw_UniquePtr(const char* label, void* Uniqueptr, const DrawUniquePtrInfo& Info)
{
	ItemLabel(StringView(label), ItemLabelFlag::Left);


	Unique_ptr<Byte>& Ptr = *(Unique_ptr<Byte>*)Uniqueptr;
	bool WasUpdated = false;
	if (Ptr.get())
	{
		bool Value = ImGui::TreeNode(ObjectNotNull);

		ImGui::SameLine();
		if (Info._FreeItem.has_value())
		{
			ImGui::SameLine();
			if (ImGui::Button("-", { 20,20 }))
			{
				WasUpdated = true;
				Info._FreeItem.value()(Uniqueptr);
			}
		}

		if (Value)
		{
			if (WasUpdated == false) {
				Info._OnDrawItem(Uniqueptr);
			}

			ImGui::TreePop();
		}


	}
	else
	{
		bool Value = ImGui::TreeNode(ObjectNullStr);
		if (Value)
		{
			ImGui::TreePop();
		}
		ImGui::SameLine();

		if (Info._AllocateItem.has_value())
		{
			ImGui::SameLine();
			if (ImGui::Button("+", { 20,20 }))
			{
				WasUpdated = true;
				Info._AllocateItem.value()(Uniqueptr);
			}
		}
	}

	return WasUpdated;
}

bool ImGuIHelper::Draw_Optional(const char* label, void* Optionalptr, const DrawOptionalInfo& Info)
{
	ItemLabel(StringView(label), ItemLabelFlag::Left);
	
	bool HasValue = Info._HasValue(Optionalptr);

	ImGui::BeginDisabled(!HasValue);

	Info._OnDrawItem(Optionalptr);

	ImGui::EndDisabled();

	ImGui::SameLine();

	ImGui::PushID((Byte*)Optionalptr + 1);

	bool Updated = ImGuIHelper::ToggleField("", HasValue);
	if (Updated)
	{
		if (HasValue)
		{
			Info._SetValue(Optionalptr);
		}
		else
		{
			Info._FreeValue(Optionalptr);
		}
	}

	ImGui::PopID();

	return HasValue;
}

bool ImGuIHelper::CharField(const char* FieldName, char& Value)
{



	thread_local String V;
	V.resize(1);
	V[0] = Value;

	ImGui::PushID(&Value);
	bool R = InputText("", V);
	ImGui::PopID();

	if (R)
	{
		Value = V[0];
	}

	ImGui::SameLine();

	ImGui::BeginDisabled(true);

	ImGui::SetNextItemWidth(ImGui::GetTextLineHeightWithSpacing());

	ImGui::PushID(&Value + 1);

	ImGui::DragScalar("", ImGuiDataType_S8, (void*)&Value);

	ImGui::PopID();

	ImGui::EndDisabled();

	return R;
}
bool ImGuIHelper::Int64Field(const char* FieldName, i64& Value)
{
	ItemLabel(StringView(FieldName), ItemLabelFlag::Left);	
	
	ImGui::PushID(&Value);
	auto V = ImGui::DragScalar("", ImGuiDataType_S64, (void*)&Value);
	ImGui::PopID();

	return V;
}
bool ImGuIHelper::Int32Field(const char* FieldName, i32& Value)
{
	ItemLabel(StringView(FieldName), ItemLabelFlag::Left);	
	
	ImGui::PushID(&Value);
	auto V = ImGui::DragScalar("", ImGuiDataType_S32, (void*)&Value);
	ImGui::PopID();

	return V;
}
bool ImGuIHelper::Int16Field(const char* FieldName, i16& Value)
{
	ItemLabel(StringView(FieldName), ItemLabelFlag::Left);	
	
	ImGui::PushID(&Value);
	auto V = ImGui::DragScalar("", ImGuiDataType_S16, (void*)&Value);
	ImGui::PopID();

	return V;
}
bool ImGuIHelper::Int8Field(const char* FieldName, i8& Value)
{
	ItemLabel(StringView(FieldName), ItemLabelFlag::Left);	
	
	ImGui::PushID(&Value);
	auto V = ImGui::DragScalar("", ImGuiDataType_S8, (void*)&Value);
	ImGui::PopID();

	return V;
}
bool ImGuIHelper::f32Field(const char* FieldName, f32& Value)
{
	ItemLabel(StringView(FieldName), ItemLabelFlag::Left);	
	
	ImGui::PushID(&Value);
	auto V = ImGui::DragScalar("", ImGuiDataType_Float, (void*)&Value);
	ImGui::PopID();

	return V;
}
bool ImGuIHelper::f64Field(const char* FieldName, f64& Value)
{
	ItemLabel(StringView(FieldName), ItemLabelFlag::Left);	
	
	ImGui::PushID(&Value);
	auto V = ImGui::DragScalar("", ImGuiDataType_Double, (void*)&Value);
	ImGui::PopID();

	return V;
}
bool ImGuIHelper::DrawOrdeField(const char* FieldName, unsigned char& Value)
{
	ItemLabel(StringView(FieldName), ItemLabelFlag::Left);	
	
	ImGui::PushID(&Value);
	auto V = uInt8Field("", Value);
	ImGui::PopID();

	return V;
}
bool ImGuIHelper::EnumField(const char* label, void* Value, const EnumValue2* Values, size_t ValuesSize, size_t EnumBaseSize)
{
	bool Updated = false;
	const EnumValue2* current_item = nullptr;
	for (size_t i = 0; i < ValuesSize; i++)
	{
		const EnumValue2& Item = Values[i];

		bool IsSame = !memcmp(Item.Value, Value, EnumBaseSize);
		if (IsSame)
		{
			current_item = &Item;
		}
	}

	if (current_item == nullptr)
	{
		if (ValuesSize == 0)
		{
			ItemLabel(StringView(label), ItemLabelFlag::Left);	
			ImGuIHelper::Text(StringView("No EnumValues"));
			return false;
		}
		current_item = &Values[0];

	}


	ItemLabel(StringView(label), ItemLabelFlag::Left);	

	ImGui::PushID(Value);
	if (ImGui::BeginCombo("", current_item->label, ImGuiComboFlags_NoArrowButton))
	{
		for (size_t i = 0; i < ValuesSize; i++)
		{
			const EnumValue2& Item = Values[i];

			bool is_selected = (current_item == &Item);
			if (ImGui::Selectable(Item.label, is_selected))
			{
				memcpy(Value, Item.Value, EnumBaseSize);
				Updated = true;
			}
			if (is_selected)
			{
				ImGui::SetItemDefaultFocus();
			}
		}
		ImGui::EndCombo();
	}
	ImGui::PopID();

	return Updated;
}
bool ImGuIHelper::Draw_StringView(const char* label, StringView& Item)
{
	thread_local String V;
	V = Item;

	bool R = false;
	bool Value = ImGui::TreeNode(label);
	if (Value)
	{
		R = InputText("AsString", V);
		if (R)
		{
			memcpy((void*)Item.data(), (void*)V.c_str(), Item.size());
		}

		ImGui::PushID(&Item);
		Span<char> spanvalue = { (char*)Item.data(), Item.size() };
		DrawSpan("AsSpan", spanvalue);
		ImGui::PopID();

		ImGui::TreePop();
	}
	return R;
}
bool ImGuIHelper::Draw_Dictionary(const char* label, void* Dictionaryptr, size_t DictionarySize, const DrawDictionaryInfo& Info)
{
	bool WasUpdated = false;
	bool Value = ImGui::TreeNode(label);

	ImGui::SameLine();


	ImGui::BeginDisabled(true);
	{
		size_t NewSize = DictionarySize;

		ImGui::PushID(&Info);
		bool ResizeWasUpdated = InputSize_t("", &NewSize, 1, 100, ImGuiInputTextFlags_EnterReturnsTrue);
		ImGui::PopID();
	}
	ImGui::EndDisabled();

	thread_local void* TepValue = nullptr;
	if (TepValue == nullptr)
	{
		ImGui::SameLine();
		if (ImGui::Button("+", { 20,20 }))
		{
			TepValue = Info._OnMakeTepItem(Dictionaryptr);
		}
	}

	if (TepValue)
	{
		Info._OnDrawTepItem(Dictionaryptr, TepValue);
		if (ImGui::Button("Add Value"))
		{
			Info._AddTepItemTODictionary(TepValue, Dictionaryptr);
			Info._OnFreeTepItem(TepValue);
			TepValue = nullptr;
			WasUpdated = true;
		}

		ImGui::SameLine();

		if (ImGui::Button("Cancel"))
		{
			Info._OnFreeTepItem(TepValue);
			TepValue = nullptr;
		}
	}


	if (Value)
	{
		Info._LoopOverObjects(Dictionaryptr, Info);

		ImGui::TreePop();
	}
	return Value;
}
bool ImGuIHelper::DrawObjectField(UCode::Sprite* Sprite, void* object,
	const void* ObjectList, size_t ObjectListSize, size_t ItemObjectSize,
	ObjectFieldFuncPtr DrawObject, const String& Name)
{
	const float square_sz = ImGui::GetFrameHeight();
	const ImVec2 imagesize = { square_sz,square_sz };
	auto winwidth = ImGui::GetWindowWidth();


	bool ok = false;
	ImGui::SameLine();

	static bool someid = false;

	auto& spaceing = ImGui::GetStyle().ItemSpacing;
	ImGui::PushItemWidth(ImGui::CalcItemWidth() - (imagesize.x + spaceing.x));
	bool r = ImGui::BeginCombo("##oiwj", Name.c_str(),ImGuiComboFlags_::ImGuiComboFlags_NoArrowButton);
	if (r)
	{
		static String V;
		static bool t = false;


		ImGui::PushID(&t);
		ImGui::PushItemWidth(ImGui::GetContentRegionAvail().x * 0.96);
		ImGuIHelper::InputText("", V);
		ImGui::PopItemWidth();
		ImGui::PopID();

		//ImGui::SameLine();
		static bool ListMode = true;
		//ImGuIHelper::BoolEnumField("List",ListMode);


		ImGui::Separator();
		//ImGui::Columns(1, 0, false);

		uintptr_t ptr = (uintptr_t)ObjectList;


		for (size_t i = 0; i < ObjectListSize; i++)
		{

			ok = DrawObject(object, (void*)ptr, ListMode, V);

			ptr += (uintptr_t)ItemObjectSize;

			if (ok)
			{
				break;
			}
		}


		ImGui::EndCombo();
	}
	ImGui::PopItemWidth();

	ImGui::SameLine();
	ImGuIHelper::Image(Sprite, imagesize);

	return ok;
}
EditorEnd


