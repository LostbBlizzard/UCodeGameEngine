#include "ImGuIHelper.hpp"
#include "Imgui/imgui_internal.h"
#include <Imgui/misc/cpp/imgui_stdlib.h>
EditorStart
void ImGuIHelper::Image(UCode::Texture* texture, const ImVec2& ButtionSize)
{
	texture->TryUploadTexToGPU();
	ImGui::Image((ImTextureID)(size_t)texture->Get_RendererID(), ButtionSize);
}
void ImGuIHelper::Image(UCode::Sprite* Sprite, const ImVec2& ButtionSize)
{
	ImVec2 uvo;
	ImVec2 uv1;
	ImTextureID texid = (ImTextureID)(size_t)Sprite->Get_texture()->Get_RendererID();
	GetSpriteUV(Sprite, uvo, uv1);

	ImGui::Image(texid, ButtionSize, uvo, uv1);
}

bool ImGuIHelper::ImageButton(const void* id, UCode::Texture* Sprite, const ImVec2& ButtionSize)
{
	Sprite->TryUploadTexToGPU();
	return ImGui::ImageButtonEx(ImGui::GetID(id), (ImTextureID)(size_t)Sprite->Get_RendererID(), ButtionSize, { 0,0 }, {1,1}, ImVec4(0, 0, 0, 0), ImVec4(1, 1, 1, 1));
}

bool ImGuIHelper::ImageButton(const void* id, UCode::Sprite* Sprite, const ImVec2& ButtionSize)
{
	ImVec2 uvo;
	ImVec2 uv1;
	ImTextureID texid = (ImTextureID)(size_t)Sprite->Get_texture()->Get_RendererID();
	GetSpriteUV( Sprite, uvo, uv1);
	return ImGui::ImageButtonEx(ImGui::GetID(id), texid, ButtionSize, uvo, uv1, ImVec4(0, 0, 0, 0), ImVec4(1, 1, 1, 1));
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
bool ImGuIHelper::InputText(const char* label, String&  buffer,ImGuiInputTextFlags flags)
{
	ImGui::Text(label);
	ImGui::SameLine();

	ImGui::PushID(&buffer);
	auto V = ImGui::InputText("", &buffer, flags);
	ImGui::PopID();

	return V;
}
bool ImGuIHelper::MultLineText(const char* label, String&  buffer, ImVec2 Size, ImGuiInputTextFlags flags)
{
	return  ImGui::InputTextMultiline(label,&buffer,Size, flags);
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
			_Data = FileHelper::OpenFileFromUser(ExtWithNoDot,buffer);
		}
		

		if (_Data.Result == FileHelper::OpenFileResult::OKAY)
		{
			buffer = _Data.Path;
			R = true;
		}
	}

	return R;
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
ImGuIHelper::TreeNodeText ImGuIHelper::TreeNode(const void* id, String&  buffer)
{
	ImGuIHelper::TreeNodeText Data;
	
	Data.TextUpdate = InputText("##", buffer);
	ImGui::SameLine();
	
	Data.NodeOpen = ImGui::TreeNode(id,"");
	
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
ImGuIHelper::TreeNodeText ImGuIHelper::TreeNode(const void* id, String&  buffer, UCode::Sprite* Sprite)
{
	auto& g = *GImGui;


	ImGuIHelper::Image(Sprite, ImVec2(20, g.FontSize + g.Style.FramePadding.y * 2)); ImGui::SameLine();
	return TreeNode(id,buffer);
}

 bool ImGuIHelper::ToggleField(const char* FieldName, bool& Value)
{
	ImGui::Text(FieldName);
	ImGui::SameLine();
	
	ImGui::PushID(&Value);
	auto V = ImGui::Checkbox("", &Value);
	ImGui::PopID();

	return V;
}

 bool ImGuIHelper::Vec2Field(const char* FieldName, UCode::Vec2& Value)
{
	ImGui::Text(FieldName);
	ImGui::SameLine();

	ImGui::PushID(&Value);
	auto V = ImGui::DragFloat2("", &Value.X);
	ImGui::PopID();

	return V;
}

 bool ImGuIHelper::Vec2Field(const char* FieldName, UCode::Vec2i& Value)
{
	ImGui::Text(FieldName);
	ImGui::SameLine();

	ImGui::PushID(&Value);
	auto V = ImGui::DragInt2("", &Value.X);
	ImGui::PopID();

	return V;
}

 bool ImGuIHelper::Vec3Field(const char* FieldName, UCode::Vec3& Value)
{
	ImGui::Text(FieldName);
	ImGui::SameLine();

	ImGui::PushID(&Value);
	auto V = ImGui::DragFloat3("", &Value.X);
	ImGui::PopID();

	return V;
}

 bool ImGuIHelper::Vec3Field(const char* FieldName, UCode::Vec3i& Value)
{
	ImGui::Text(FieldName);
	ImGui::SameLine();

	ImGui::PushID(&Value);
	auto V = ImGui::DragInt3("", &Value.X);
	ImGui::PopID();

	return V;
}

 bool ImGuIHelper::ColorField(const char* FieldName, UCode::ColorRGBA& Value)
{
	ImGui::Text(FieldName);
	ImGui::SameLine();

	ImGui::PushID(&Value);
	auto V = ImGui::ColorEdit4("", &Value.R);
	ImGui::PopID();

	return V;
}

 bool ImGuIHelper::ColorField(const char* FieldName, UCode::ColorRGB& Value)
{
	ImGui::Text(FieldName);
	ImGui::SameLine();

	ImGui::PushID(&Value);
	auto V = ImGui::ColorEdit3("", &Value.R);
	ImGui::PopID();

	return V;
}

 bool ImGuIHelper::ColorField(const char* FieldName, UCode::Color32& Value)
{
	ImGui::Text(FieldName);
	ImGui::SameLine();

	ImGui::PushID(&Value);
	auto V = ImGui::ColorEdit4("", (float*)&Value.R, ImGuiColorEditFlags_::ImGuiColorEditFlags_Uint8);
	ImGui::PopID();

	return V;
}

 bool ImGuIHelper::ColorField(const char* FieldName, UCode::Color24& Value)
{
	ImGui::Text(FieldName);
	ImGui::SameLine();

	ImGui::PushID(&Value);
	auto V = ImGui::ColorEdit3("", (float*)&Value.R, ImGuiColorEditFlags_::ImGuiColorEditFlags_Uint8);

	ImGui::PopID();

	return V;
}

 bool ImGuIHelper::uInt64Field(const char* FieldName, u64& Value)
{
	ImGui::Text(FieldName);
	ImGui::SameLine();

	ImGui::PushID(&Value);
	auto V = ImGui::DragScalar("", ImGuiDataType_U64, (void*)&Value);
	ImGui::PopID();

	return V;
}

 bool ImGuIHelper::uInt32Field(const char* FieldName, u32& Value)
{
	ImGui::Text(FieldName);
	ImGui::SameLine();

	ImGui::PushID(&Value);
	auto V = ImGui::DragScalar("", ImGuiDataType_U32, (void*)&Value);
	ImGui::PopID();

	return V;
}

 bool ImGuIHelper::uInt16Field(const char* FieldName,u16& Value)
{
	ImGui::Text(FieldName);
	ImGui::SameLine();

	ImGui::PushID(&Value);
	auto V = ImGui::DragScalar("", ImGuiDataType_U16, (void*)&Value);
	ImGui::PopID();

	return V;
}

 bool ImGuIHelper::uInt8Field(const char* FieldName,u8& Value)
{
	ImGui::Text(FieldName);
	ImGui::SameLine();

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
	ImGui::ItemSize(bb,style.FramePadding.y);
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

    auto tep =ImGui::GetItemRectMax();
	auto tep2 =ImGui::GetItemRectMin();
	auto pos = (*(Vec2*)&tep + *(Vec2*)&tep2) / 2;
	ImVec2 Pos = *(ImVec2*)&pos;
	ImGui::SetCursorPos(Pos);

	ImGuIHelper::LoadingIndicatorCircle("if", radius - 50.0f, circle_count, speed);
}
bool ImGuIHelper::DrawRenameTree(String& label, bool TreeIsOpened, bool& IsRenameing)
{
	return DrawRenameName(label, IsRenameing);
}
bool ImGuIHelper::DrawRenameName(String& label, bool& IsRenameing)
{
	ImGui::SetKeyboardFocusHere();
	if (ImGuIHelper::InputText("##rename", label, ImGuiInputTextFlags_EnterReturnsTrue))
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
	bool Value = ImGui::TreeNode(label);
	
	size_t NewSize = Size;

	ImGui::SameLine();

	ImGui::BeginDisabled(!Item._ResizeVector.has_value());
	{
		ImGui::PushID(&Item);
		bool ResizeWasUpdated = InputSize_t("",&NewSize,1,100, ImGuiInputTextFlags_EnterReturnsTrue);
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

			ImGui::BeginDisabled(!Item._AddNewRemove.has_value());
			if (ImGui::Button("-", ButtionSize))
			{
				WasUpdated = true;
				Item._AddNewRemove.value()(Object, Size - 1);
			}
			ImGui::EndDisabled();
		}

		//ImGui::Separator();
		if (!WasUpdated) {

			for (size_t i = 0; i < Size; i++)
			{
				String Lable = "Item:" + std::to_string(i);
				ImGui::Text(Lable.c_str());

				
				bool RemoveItem = false;
				{
					void* ItemPtr = (Byte*)Buffer + (i * Item.ItemSize);
					ImGui::PushID(Lable.c_str());
					if (ImGui::BeginPopupContextItem("????"))
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
					Item._AddNewRemove.value()(Object,i);
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
	ImGui::Text(label);
	ImGui::SameLine();

	
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
			if (ImGui::Button("+", {20,20}))
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

	ImGui::Text(label);
	ImGui::SameLine();

	bool HasValue = Info._HasValue(Optionalptr);
	
	ImGui::BeginDisabled(!HasValue);

	Info._OnDrawItem(Optionalptr);

	ImGui::EndDisabled();

	ImGui::SameLine();

	ImGui::PushID((Byte*)Optionalptr + 1);

	bool Updated = ImGuIHelper::ToggleField("",HasValue);
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
	ImGui::Text(FieldName);
	ImGui::SameLine();

	ImGui::PushID(&Value);
	auto V = ImGui::DragScalar("", ImGuiDataType_S64, (void*)&Value);
	ImGui::PopID();

	return V;
}
 bool ImGuIHelper::Int32Field(const char* FieldName, i32& Value)
{
	ImGui::Text(FieldName);
	ImGui::SameLine();

	ImGui::PushID(&Value);
	auto V = ImGui::DragScalar("", ImGuiDataType_S32, (void*)&Value);
	ImGui::PopID();

	return V;
}
 bool ImGuIHelper::Int16Field(const char* FieldName, i16& Value)
{
	ImGui::Text(FieldName);
	ImGui::SameLine();

	ImGui::PushID(&Value);
	auto V = ImGui::DragScalar("", ImGuiDataType_S16, (void*)&Value);
	ImGui::PopID();

	return V;
}
 bool ImGuIHelper::Int8Field(const char* FieldName, i8& Value)
{
	ImGui::Text(FieldName);
	ImGui::SameLine();

	ImGui::PushID(&Value);
	auto V = ImGui::DragScalar("", ImGuiDataType_S8, (void*)&Value);
	ImGui::PopID();

	return V;
}
 bool ImGuIHelper::f32Field(const char* FieldName, f32& Value)
{
	ImGui::Text(FieldName);
	ImGui::SameLine();

	ImGui::PushID(&Value);
	auto V = ImGui::DragScalar("", ImGuiDataType_Float, (void*)&Value);
	ImGui::PopID();

	return V;
}
 bool ImGuIHelper::f64Field(const char* FieldName, f64& Value)
{
	ImGui::Text(FieldName);
	ImGui::SameLine();

	ImGui::PushID(&Value);
	auto V = ImGui::DragScalar("", ImGuiDataType_Double, (void*)&Value);
	ImGui::PopID();

	return V;
}
 bool ImGuIHelper::DrawOrdeField(const char* FieldName, unsigned char& Value)
{
	ImGui::Text(FieldName);
	ImGui::SameLine();

	ImGui::PushID(&Value);
	auto V = uInt8Field("",Value);
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
			 ImGui::Text("Cant show EnumField because there is no EnumValues");
			 return false;
		 }
		 current_item = &Values[0];

	 }

	
	 ImGui::Text(label); ImGui::SameLine();

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
	ObjectFieldFuncPtr DrawObject,const String& Name)
{
	ImGuIHelper::Image(Sprite, { 20,20 });
	auto winwidth = ImGui::GetWindowWidth();

	ImGui::SameLine();

	ImGui::Text(Name.c_str());
	
	ImGui::SameLine();
	
	if (ImGui::Button("..."))
	{
		ImGui::OpenPopup("Object Finder");
	}
	
	bool open = true;
	bool ok = false;
	if (ImGui::BeginPopupModal("Object Finder", &open))
	{
		static String V;
		ImGuIHelper::InputText("Find", V);

		
		static bool ListMode = false;
		ImGuIHelper::BoolEnumField("List Mode",ListMode);

		ImGui::Separator();
		ImGui::Columns(1, 0, false);

		uintptr_t ptr = (uintptr_t)ObjectList;


		for (size_t i = 0; i < ObjectListSize; i++)
		{
			//ImGui::PushID(ptr);
			
			ok = DrawObject(object,(void*)ptr, ListMode,V);

			//ImGui::PopID();

			ptr += (uintptr_t)ItemObjectSize;

			if (ok)
			{
				break;
			}
		}


		
		ImGui::EndPopup();
	}

	return ok;
}
EditorEnd


