#include "UCodeDrawer.hpp"
#include <memory>
EditorStart
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
		R = ImGuIHelper::uInt8Field("", *(UInt8*)Pointer);
		ImGui::PopID();
	}
	break;
	case UCodeLang::ReflectionTypes::sInt8:
	{
		ImGui::PushID(Pointer);
		R = ImGuIHelper::Int8Field("", *(SInt8*)Pointer);
		ImGui::PopID();
	}
	break;
	case UCodeLang::ReflectionTypes::uInt16:
	{
		ImGui::PushID(Pointer);
		R = ImGuIHelper::uInt16Field("", *(UInt16*)Pointer);
		ImGui::PopID();
	}
	break;
	case UCodeLang::ReflectionTypes::sInt16:
	{
		ImGui::PushID(Pointer);
		R = ImGuIHelper::Int16Field("", *(SInt16*)Pointer);
		ImGui::PopID();
	}
	break;

	uint32:
	case UCodeLang::ReflectionTypes::uInt32:
	{
		ImGui::PushID(Pointer);
		R = ImGuIHelper::uInt32Field("", *(UInt32*)Pointer);
		ImGui::PopID();
	}
	break;

	sint32:
	case UCodeLang::ReflectionTypes::sInt32:
	{
		ImGui::PushID(Pointer);
		R = ImGuIHelper::Int32Field("", *(SInt32*)Pointer);
		ImGui::PopID();
	}
	break;

	uint64:
	case UCodeLang::ReflectionTypes::uInt64:
	{
		ImGui::PushID(Pointer);
		R = ImGuIHelper::uInt64Field("", *(UInt64*)Pointer);
		ImGui::PopID();
	}
	break;
	sint64:
	case UCodeLang::ReflectionTypes::sInt64:
	{
		ImGui::PushID(Pointer);
		R = ImGuIHelper::Int64Field("", *(SInt64*)Pointer);
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
		R = ImGuIHelper::float32Field("", *(float32*)Pointer);
		ImGui::PopID();
	}
	break;
	case UCodeLang::ReflectionTypes::float64:
	{
		ImGui::PushID(Pointer);
		R = ImGuIHelper::float64Field("", *(float64*)Pointer);
		ImGui::PopID();
	}
	break;
	case UCodeLang::ReflectionTypes::CustomType:
	{
		auto Node = Assembly.Find_Node(Type._CustomTypeID);
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
	ImGui::Text(FieldName);
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
EditorEnd