#include "UCodeDrawer.hpp"
#include <memory>
#include "UCodeRunTime/ULibrarys/UCodeLang/ULangRunTime.hpp"
#include "UCodeLang/RunTime/ReflectionStl.hpp"
#include "UCodeRunTime/ULibrarys/UCodeLang/API/Object.hpp"
#include "UCodeRunTime/ULibrarys/UCodeLang/ScirptableObject.hpp"
#include "Editor/EditorAppCompoent.hpp"
#include "Helper/StringHelper.hpp"
#include "EditorWindows/ProjectManagement/ProjectFilesWindow.hpp"
#include "Helper/UserSettings.hpp"
#include "UCodeRunTime/ULibrarys/AssetManagement/UCodeLangAssets.hpp"
EditorStart

bool DrawString(void* Pointer, UCodeLang::ReflectionString& Type,
	ImGuiInputTextFlags flags)
{
	if (Type.GetElementType()._Type == UCodeLang::ReflectionTypes::Char) {
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
	return false;
}
bool DrawObjectAsset(void* Pointer,const UCodeLang::ReflectionTypeInfo& AssetType, const UCodeLang::ClassAssembly& Assembly)
{
	using Base = UC::ULangAPI::ObjectAPI::Base;
	Base& Value = *(Base*)Pointer;

	struct ObjectSceneAssetInfo
	{
		Path _RelativePath;
		UID _UID;
	};


	Vector<ObjectSceneAssetInfo> List;
	{
		ObjectSceneAssetInfo P;
		P._RelativePath = "None";

		List.push_back(std::move(P));
	}

	auto sprite = AppFiles::GetSprite(AppFiles::sprite::ScirptableObjectFile);
	

	auto app = EditorAppCompoent::GetCurrentEditorAppCompoent();
	auto assetmanager = app->Get_AssetManager();
	auto ProjectData = app->Get_RunTimeProjectData();

	auto tyoe_classdata = &Assembly.Find_Node(AssetType)->Get_ClassData();
	for (auto& Item : ProjectData->Get_AssetIndex()._Files)
	{
		if (!Item.UserID.has_value())
		{
			continue;
		}

		if (Path(Item.RelativePath).extension() != UCode::ScirptableObjectData::FileExtDot)
		{
			continue;
		}

		bool issametype = false;
		{
			//may be this should be Chached and not load all the ScirptableObjects
			auto opasset = assetmanager->FindOrLoad_t<UC::ScirptableObjectAsset>(Item.UserID.value());

			if (opasset.has_value())
			{
				auto value = opasset.value();

				if (value->_Base.Get_ClassData() == tyoe_classdata)
				{
					issametype = true;
				}
			}
		}

		if (issametype)
		{
			ObjectSceneAssetInfo P;
			P._RelativePath = Item.RelativePath.substr(0,Item.RelativePath.size() - strlen(UCode::ScirptableObjectData::FileExtDot));
			P._UID = Item.UserID.value();
			List.push_back(std::move(P));
		}
	}

	String MyName = "None";

	if (Value.Has_UID()) {
		for (auto& Item : List)
		{
			if (Item._UID == Value.Get_UID())
			{
				MyName = Item._RelativePath.generic_string();
				break;
			}
		}
	}

	ImGuIHelper::ObjectFieldData data;
	data.OnFileDroped = [ProjectData](const Path& fullpath, void* object, ImGuIHelper::ObjectDropState state) -> bool
		{
			Base& objectas =*(Base*)object;

			if (state == ImGuIHelper::ObjectDropState::CanBeDroped)
			{
				return (fullpath.extension() == UCode::ScirptableObjectData::FileExtDot);	
			}
			else if (state == ImGuIHelper::ObjectDropState::OnDroped)
			{
				auto& editorindex = ProjectData->Get_AssetIndex();
				auto assetpath = ProjectData->GetAssetsDir();
				Path relativepath = fullpath.native().substr(assetpath.native().size());

				auto opasset = editorindex.FindFileRelativeAssetName(relativepath.generic_string());
				if (opasset.has_value())
				{
					auto& asset = opasset.value();
					if (asset.UserID.has_value())
					{
						objectas = asset.UserID.value();
						return true;
					}
				}
				return false;
			}

		};
	data.OnObjectInList = [sprite](void* Ptr, void* Object, bool Listmode, const String& Find)
		{
			Base& Value = *(Base*)Ptr;
			ObjectSceneAssetInfo* obj = (ObjectSceneAssetInfo*)Object;

			auto Name = obj->_RelativePath.generic_string();
			bool r = false;


			if (StringHelper::Fllter(Find, Name))
			{

				if (Listmode)
				{
					auto imagesize = ImGui::GetFrameHeight();
					ImGuIHelper::Image(sprite, { imagesize,imagesize });
					ImGui::SameLine();

					//ImGui::PushID(Object);
					r = ImGui::Selectable(Name.c_str(),Value.Has_UID() ? Value.Get_UID() == obj->_UID : false);
					//ImGui::PopID();

					if (r)
					{
						Value = obj->_UID;
						r = true;
					}
				}
				else
				{
					auto NewName = Name;
					ImGuIHelper::Text(NewName);
					auto imagesize = ImGui::GetFrameHeight();
					if (ImGuIHelper::ImageButton(obj + NewName.size(), AppFiles::sprite::Scene2dData, { imagesize,imagesize }))
					{
						Value = obj->_UID;
						r = true;
					}
				}
			}

			return r;
		};
	data.OnInspect = [](void* object) -> bool
		{
			Base& objectas = *(Base*)object;

			auto p = EditorAppCompoent::GetCurrentEditorAppCompoent();


			bool r = false;
			if (auto win = p->Get_Window<ProjectFilesWindow>())
			{
				if (objectas.Has_UID()) 
				{
					win->OpenAndFocusOnAsset(objectas.Get_UID());
					r = true;
				}
			}
			return r;
		};
	data.OnCopy = [](void* object) -> String
		{
			Base& objectas = *(Base*)object;

			UID id = objectas.Has_UID() ? objectas.Get_UID() : UID();
			return UserSettings::SetCopyBufferAsValueStr("_AssetUID",id);
		};
	data.OnPatse = [ProjectData](void* object, const String& Paste) -> bool
		{
			bool isgood = false;
			Base& objectas = *(Base*)object;
			
			auto idop = UserSettings::ReadCopyBufferAs<UID>("_AssetUID",Paste);
		
			if (idop.has_value())
			{
				UID id = idop.value();

				auto& editorindex = ProjectData->Get_AssetIndex();
				auto opasset = editorindex.FindFileUsingID(id);

				if (opasset.has_value())
				{
					if (Path(opasset.value().RelativeAssetName).extension() == Path(UCode::ScirptableObjectData::FileExtDot))
					{
						objectas = id;
						isgood = true;
					}
				}
			}
			else 
			{
				auto pathop = UserSettings::ReadCopyBufferAs<Path>("AssetPath",Paste);

				if (pathop.has_value())
				{
					auto& editorindex = ProjectData->Get_AssetIndex();
					auto rel = FileHelper::ToRelativePath(ProjectData->GetAssetsDir(), pathop.value().generic_string()).generic_string();
					auto opasset = editorindex.FindFileRelativeAssetName(rel);
				
					
					if (opasset.has_value())
					{
						auto asset = opasset.value();
						if (Path(opasset.value().RelativeAssetName).extension() == Path(UCode::ScirptableObjectData::FileExtDot))
						{
							if (asset.UserID.has_value())
							{
								objectas = asset.UserID.value();
								isgood = true;
							}
						}
					}
				}
			}
			return isgood;
		};
	data.OnDestory = [](void* object)
		{
			Base& objectas = *(Base*)object;
			objectas = UID();
		};

	return ImGuIHelper::DrawObjectField(&Value, List.data(), List.size(), sizeof(ObjectSceneAssetInfo),
		data,AppFiles::sprite::ScirptableObjectFile,MyName);

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
		{
			auto typeop = UCode::UCodeRunTimeState::IsTypeUCodeObjectAndAsset(Type, Assembly);

			if (typeop.has_value())
			{
				auto& type = typeop.value();
				return DrawObjectAsset(Pointer, type, Assembly);
			}
		}

		//built-in
		{
			auto typeop = UCode::UCodeRunTimeState::IsTypeUCodeObject(Type, Assembly);

			if (typeop.has_value())
			{
				auto& type = typeop.value();

				if (UCode::UCodeRunTimeState::IsTile(type,Assembly))
				{
					auto& ptr = *(TileDataPtr*)Pointer;
					return ImGuIHelper_Asset::AssetField(ptr);
				}
				else if (UCode::UCodeRunTimeState::IsSprite(type,Assembly))
				{
					auto& ptr = *(UC::SpritePtr*)Pointer;
					return ImGuIHelper_Asset::AssetField(ptr);
				}
				else if (UCode::UCodeRunTimeState::IsTexture(type,Assembly))
				{
					auto& ptr = *(UC::TexturePtr*)Pointer;
					return ImGuIHelper_Asset::AssetField(ptr);
				}
				else if (UCode::UCodeRunTimeState::IsAudio(type,Assembly))
				{
					auto& ptr = *(UC::AudioPtr*)Pointer;
					return ImGuIHelper_Asset::AssetField(ptr);
				}
				else if (UCode::UCodeRunTimeState::IsSceneAsset(type,Assembly))
				{
					auto& ptr = *(UC::ScencPtr*)Pointer;
					return ImGuIHelper_Asset::AssetField(ptr);
				}
				else if (UCode::UCodeRunTimeState::IsEntityAsset(type,Assembly))
				{
					auto& ptr = *(UC::RawEntityDataPtr*)Pointer;
					return ImGuIHelper_Asset::AssetField(ptr);
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
	ImGuIHelper::ItemLabel(StringView(FieldName), ImGuIHelper::Left);
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

			R = ImGuIHelper::EnumField(Pointer, _Eunm, *Size);
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

			std::function<bool(void* Tag, void* Union, bool UpdatedEnum, bool Draw)> OnDraw = [UnionTypeID,&Assembly, Class, TagSize,LastTag,Is32Mode](void* Tag, void* Union, bool UpdatedEnum, bool Draw)
			{
				bool Updated = false;
				auto state = UCode::UCodeRunTimeState::Get_Current();
				UCodeLang::AnyInterpreterPtr anyptr = &state->GetCurrentInterpreter();


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

								auto op = Assembly.CallDestructor(TypeToInit, Ptr, Is32Mode);
								if (op.has_value())
								{
									if (op.value().has_value())
									{
										for (auto& Func : op.value().value())
										{
											anyptr.ThisCall(Func.MethodToCall, Func.ThisPtr);
										}
									}
								}
								else
								{
									String name = "[PutTypeNameHere]";
									UCodeGEError("Unable to Call Destructor for type '" << name << "' we may have leak memory")
								}
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
								auto op = Assembly.CallDefaultConstructor(TypeToInit, Ptr, Is32Mode);
								if (op.has_value())
								{
									if (op.value().has_value())
									{
										for (auto& Func : op.value().value())
										{
											anyptr.ThisCall(Func.MethodToCall, Func.ThisPtr);
										}
									}
								}
								else
								{
									String name = "[PutTypeNameHere]";
									UCodeGEError("Unable to Call DefaultConstructor for type '" << name << "' we may have uninitialized object in memory")
								}
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

			auto Ret = ImGuIHelper::EnumVariantField(_Info,OnDraw,_Eunm.data(),_Eunm.size(), TagSize);
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