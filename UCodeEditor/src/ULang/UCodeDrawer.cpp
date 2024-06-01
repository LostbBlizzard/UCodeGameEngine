#include "UCodeDrawer.hpp"
#include <memory>
#include "UCodeRunTime/ULibrarys/UCodeLang/ULangRunTime.hpp"
#include "UCodeLang/RunTime/ReflectionStl.hpp"
EditorStart

struct InputTextCallback_UserData
{
    UCodeLang::ReflectionString*            Str;
    ImGuiInputTextCallback  ChainCallback;
    void*                   ChainCallbackUserData;
};
static int InputTextCallback(ImGuiInputTextCallbackData* data)
{
    InputTextCallback_UserData* user_data = (InputTextCallback_UserData*)data->UserData;
    if (data->EventFlag == ImGuiInputTextFlags_CallbackResize)
    {
        // Resize string callback
        // If for some reason we refuse the new length (BufTextLen) and/or capacity (BufSize) we need to set them back to what we want.
        UCodeLang::ReflectionString* str = user_data->Str;
        IM_ASSERT(data->Buf == str->data());
        str->resize(data->BufTextLen);
        data->Buf = (char*)str->data();
    }
    else if (user_data->ChainCallback)
    {
        // Forward to user callback, if any
        data->UserData = user_data->ChainCallbackUserData;
        return user_data->ChainCallback(data);
    }
    return 0;
}
bool DrawString(void* Pointer, UCodeLang::ReflectionString& Type,
	ImGuiInputTextFlags flags)
{
	if (Type.GetElementType()._Type == UCodeLang::ReflectionTypes::Char) 
	{
		String copy = String(Type.AsCharView());

		bool r = ImGuIHelper::InputText(copy, flags);

		if (r)
		{
			Type = copy;
		}
		return r;
	}
	else
	{
		return false;
	}
}
bool DrawStringMultiline(void* Pointer, UCodeLang::ReflectionString& Type,
	ImVec2 size)
{
	/*
	String copy = String(Type.AsCharView());

	bool r = ImGuIHelper::MultLineText(copy, flags);

	if (r)
	{
		Type = copy;
	}
	return r;
	*/
	return false;
}
bool UCodeDrawer::DrawType(void* Pointer, const UCodeLang::ReflectionTypeInfo& Type, const UCodeLang::ClassAssembly& Assembly, bool IfClassRemoveFlags)
{
	const bool Is32Mode = sizeof(void*) == 4;

	bool R = false;
	switch (Type._Type)
	{
	case UCodeLang::ReflectionTypes::Bool:
	{
		ImGui::PushID(Pointer);
		R = ImGuIHelper::BoolEnumField("", *(bool*)Pointer);
		ImGui::PopID();
	}
	break;
	case UCodeLang::ReflectionTypes::Char:
	{
		ImGui::PushID(Pointer);
		R = ImGuIHelper::CharField("", *(char*)Pointer);
		ImGui::PopID();
	}
	break;
	case UCodeLang::ReflectionTypes::uInt8:
	{
		ImGui::PushID(Pointer);
		R = ImGuIHelper::uInt8Field("", *(u8*)Pointer);
		ImGui::PopID();
	}
	break;
	case UCodeLang::ReflectionTypes::sInt8:
	{
		ImGui::PushID(Pointer);
		R = ImGuIHelper::Int8Field("", *(i8*)Pointer);
		ImGui::PopID();
	}
	break;
	case UCodeLang::ReflectionTypes::uInt16:
	{
		ImGui::PushID(Pointer);
		R = ImGuIHelper::uInt16Field("", *(u16*)Pointer);
		ImGui::PopID();
	}
	break;
	case UCodeLang::ReflectionTypes::sInt16:
	{
		ImGui::PushID(Pointer);
		R = ImGuIHelper::Int16Field("", *(i16*)Pointer);
		ImGui::PopID();
	}
	break;

	uint32:
	case UCodeLang::ReflectionTypes::uInt32:
	{
		ImGui::PushID(Pointer);
		R = ImGuIHelper::uInt32Field("", *(u32*)Pointer);
		ImGui::PopID();
	}
	break;

	sint32:
	case UCodeLang::ReflectionTypes::sInt32:
	{
		ImGui::PushID(Pointer);
		R = ImGuIHelper::Int32Field("", *(i32*)Pointer);
		ImGui::PopID();
	}
	break;

	uint64:
	case UCodeLang::ReflectionTypes::uInt64:
	{
		ImGui::PushID(Pointer);
		R = ImGuIHelper::uInt64Field("", *(u64*)Pointer);
		ImGui::PopID();
	}
	break;
	sint64:
	case UCodeLang::ReflectionTypes::sInt64:
	{
		ImGui::PushID(Pointer);
		R = ImGuIHelper::Int64Field("", *(i64*)Pointer);
		ImGui::PopID();
	}
	break;
	
	case UCodeLang::ReflectionTypes::uIntPtr:
	{
		if (Is32Mode)
		{
			goto uint32;
		}
		else
		{
			goto uint64;
		}
	}
	break;
	case UCodeLang::ReflectionTypes::sIntPtr:
	{
		if (Is32Mode)
		{
			goto sint32;
		}
		else
		{
			goto sint64;
		}
	}
	break;
	case UCodeLang::ReflectionTypes::float32:
	{
		ImGui::PushID(Pointer);
		R = ImGuIHelper::f32Field("", *(f32*)Pointer);
		ImGui::PopID();
	}
	break;
	case UCodeLang::ReflectionTypes::float64:
	{
		ImGui::PushID(Pointer);
		R = ImGuIHelper::f64Field("", *(f64*)Pointer);
		ImGui::PopID();
	}
	break;
	case UCodeLang::ReflectionTypes::CustomType:
	{
		auto Node = Assembly.Find_Node(Type._CustomTypeID);

		auto state = UCode::UCodeRunTimeState::Get_Current();
		UCodeLang::AnyInterpreterPtr anyptr = &state->GetCurrentInterpreter();
		{
			auto typeop = Assembly.IsVector_t(Type);
			if (typeop.has_value())
			{
				auto& type = typeop.value();

				UCodeLang::ReflectionVector data;
				data.Set(Pointer, &type, anyptr, Assembly, Is32Mode);
				
				
				ImGuIHelper::DrawVectorInfo info;
				info.ItemSize = data.GetElementTypeSize();
				info._AddNewValue = [state, &anyptr, &Assembly, Is32Mode](void* Object, size_t index)
					{
						UCodeLang::ReflectionVector* info = (UCodeLang::ReflectionVector*)Object;
						void* val = state->Lang_Malloc(info->GetElementTypeSize());

						auto functocallop = Assembly.CallDefaultConstructor(info->GetElementType(), val, Is32Mode);
						if (functocallop.has_value())
						{
							auto& func = functocallop.value();
							
							if (func.has_value()) 
							{
								auto& funclist = func.value();
								for (auto& Item : funclist)
								{
									anyptr.ThisCall(Item.MethodToCall, Item.ThisPtr);
								}
							}
							info->insert(index, val);
						}
						state->Lang_Free(val);
					};
				info._AddNewRemove =  [](void* Object, size_t index)
					{
						UCodeLang::ReflectionVector* info = (UCodeLang::ReflectionVector*)Object;
						info->remove(index);
					};
				info._ResizeVector = [](void* Object, size_t newsize)
					{
						UCodeLang::ReflectionVector* info = (UCodeLang::ReflectionVector*)Object;
						info->resize(newsize);
					};

				info._OnDrawItem = [&Assembly](void* Object, size_t index)
					{
						UCodeLang::ReflectionVector* info = (UCodeLang::ReflectionVector*)Object;
						
						void* objitem = info->at(index);
						DrawType(objitem, info->GetElementType(), Assembly);
					};

				return ImGuIHelper::DrawVector(&data, data.data(), data.size(), info);				
			}
		}
		{
			auto typeop = Assembly.IsString_t(Type);

			if (typeop.has_value())
			{
				auto& type = typeop.value();

				UCodeLang::ReflectionString data;
				data.Set(Pointer, &type, anyptr, Assembly, Is32Mode);

				bool useMultiline = false;
				if (useMultiline)
				{
					return DrawStringMultiline(Pointer, data,{20,20});
				}
				else
				{
					return DrawString(Pointer, data,0);
				}
			}
		}
		if (Node)
		{
			switch (Node->Get_Type())
			{
			case UCodeLang::ClassType::Class:
			{
				auto& ClassData = Node->Get_ClassData();


				


				if (IfClassRemoveFlags) 
				{
					R = DrawClass(Pointer, ClassData, Assembly);
				}
				else 
				{
					ImGui::PushID(ClassData.TypeID);
					bool IsOpen = ImGui::TreeNode("");
					if (IsOpen)
					{
						R = DrawClass(Pointer, ClassData, Assembly);
						ImGui::TreePop();
					}	
					ImGui::PopID();
				}
			
			}
			break;
			case UCodeLang::ClassType::Alias:
			{
				auto& AliasData = Node->Get_AliasData();
				R = DrawType(Pointer, AliasData.Type, Assembly);
			}
			break;
			case UCodeLang::ClassType::Enum:
			{
				auto& EnumData = Node->Get_EnumData();
				R = DrawEnum(Pointer, EnumData, Assembly);
			}
			break;
			default:
				break;
			}
		}
		else
		{

		}
	}
	break;
	default:
		break;
	}
	return R;
}
bool UCodeDrawer::DrawClass(void* Pointer, const UCodeLang::Class_Data& Class, const UCodeLang::ClassAssembly& Assembly)
{
	bool R = false;
	for (const auto& Item : Class.Fields)
	{
		if (Item.Name == "_Handle") { continue; }

		uintptr_t newFieldPtr = (uintptr_t)Pointer + (uintptr_t)Item.offset;
		if (DrawField((void*)newFieldPtr, Item.Name.c_str(), Item.Type, Assembly))
		{
			R = true;
		}
	}
	return R;
}
bool UCodeDrawer::DrawField(void* Pointer, const char* FieldName, const UCodeLang::ReflectionTypeInfo& Type, const UCodeLang::ClassAssembly& Assembly)
{
	ImGuIHelper::Text(StringView(FieldName));
	ImGui::SameLine();
	return DrawType(Pointer, Type, Assembly);
}
bool UCodeDrawer::DrawEnum(void* Pointer, const UCodeLang::Enum_Data& Class, const UCodeLang::ClassAssembly& Assembly)
{
	const bool Is32Mode = sizeof(void*) == 4;

	bool R = false;
	if (!Class.EnumVariantUnion.has_value()) 
	{
		ImGui::PushID(Pointer);

		auto Size = Assembly.GetSize(Class.BaseType, Is32Mode);
		if (Size.has_value())
		{
			Vector< ImGuIHelper::EnumValue2> _Eunm;
			_Eunm.resize(Class.Values.size());
			for (size_t i = 0; i < Class.Values.size(); i++)
			{
				auto& ImguiEnum = _Eunm[i];
				const auto& ClassItem = Class.Values[i];

				ImguiEnum.label = ClassItem.Name.c_str();
				ImguiEnum.Value = ClassItem._Data.Get_Data();
			}

			R = ImGuIHelper::EnumField("", Pointer, _Eunm, *Size);
		}
		ImGui::PopID();
	}
	else
	{
		ImGui::PushID(Pointer);

		auto Size = Assembly.GetSize(Class.BaseType, Is32Mode);
		if (Size.has_value())
		{
			size_t TagSize = *Size;

			ImGuIHelper::VariantInfo _Info;
			_Info.Tag = Pointer;
			_Info.Union = (void*)(TagSize + (uintptr_t)Pointer);

			Vector< ImGuIHelper::EnumValue2> _Eunm;
			_Eunm.resize(Class.Values.size());
			for (size_t i = 0; i < Class.Values.size(); i++)
			{
				auto& ImguiEnum = _Eunm[i];
				const auto& ClassItem = Class.Values[i];

				ImguiEnum.label = ClassItem.Name.c_str();
				ImguiEnum.Value = ClassItem._Data.Get_Data();
			}
			size_t UnionTypeID = Class.EnumVariantUnion.value();

			std::shared_ptr<Byte> LastTag = std::shared_ptr<Byte>(new Byte[TagSize]);
			memcpy(LastTag.get(), _Info.Tag, TagSize);

			std::function<bool(void* Tag, void* Union, bool UpdatedEnum, bool Draw)> OnDraw = [UnionTypeID,&Assembly, Class, TagSize,LastTag](void* Tag, void* Union, bool UpdatedEnum, bool Draw)
			{
				bool Updated = false;

				if (UpdatedEnum)
				{

					{//drop old object.
						const UCodeLang::EnumValues* _UseingValue = nullptr;
						for (size_t i = 0; i < Class.Values.size(); i++)
						{
							auto& Item = Class.Values[i];
							if (!memcmp(LastTag.get(), Item._Data.Get_Data(), TagSize))
							{
								_UseingValue = &Item;
							}
						}
						if (_UseingValue)
						{
							if (_UseingValue->EnumVariantType.has_value())
							{
								auto TypeToInit = _UseingValue->EnumVariantType.value();
								auto Ptr = Union;
								//To-DO drop object
							}
						}
					}

					{//init object
						const UCodeLang::EnumValues* _UseingValue = nullptr;
						for (size_t i = 0; i < Class.Values.size(); i++)
						{
							auto& Item = Class.Values[i];
							if (!memcmp(Tag, Item._Data.Get_Data(), TagSize))
							{
								_UseingValue = &Item;
							}
						}
						if (_UseingValue)
						{
							if (_UseingValue->EnumVariantType.has_value())
							{
								auto TypeToInit = _UseingValue->EnumVariantType.value();
								auto Ptr = Union;
								//init new object
							}
						}
					}
					
				}

				if (Draw)
				{
					const UCodeLang::EnumValues* _UseingValue = nullptr;
					for (size_t i = 0; i < Class.Values.size(); i++)
					{
						auto& Item = Class.Values[i];
						if (!memcmp(Tag, Item._Data.Get_Data(), TagSize))
						{
							_UseingValue = &Item;
						}
					}

					if (_UseingValue)
					{
						if (_UseingValue->EnumVariantType.has_value()) 
						{
							Updated = DrawType(Union, _UseingValue->EnumVariantType.value(), Assembly,true);
						}
					}
				}

				return Updated;
			};

			auto Ret = ImGuIHelper::EnumVariantField("",_Info,OnDraw,_Eunm.data(),_Eunm.size(), TagSize);
			R = Ret.EnumUpdated || Ret.VariantUpdated;
		}
		ImGui::PopID();
	}
	return R;
}

bool UCodeDrawer::DrawModuleFileDeps(StringView FieldName, Vector<UCodeLang::ModuleFile::ModuleDependencie>& Dependencies)
{
	return false;
}
EditorEnd