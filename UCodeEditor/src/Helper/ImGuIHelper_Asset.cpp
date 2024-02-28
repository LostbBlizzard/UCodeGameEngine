#include "ImGuIHelper_Asset.hpp"
#include "EditorWindows/OtherTypes/DirectoryViewer.hpp"
#include "UEditorModules/UEditorModule.hpp"
#include "ImGuIHelper.hpp"
#include "StringHelper.hpp"
EditorStart
bool ImGuIHelper_Asset::DrawLayerField(const char* FieldName, UCode::RenderRunTime2d::DrawLayer_t& Value)
{
	return ImGuIHelper::uInt8Field(FieldName, Value);
}
bool ImGuIHelper_Asset::AsssetField(const char* FieldName, UCode::TexturePtr& Value) 
{ 
	struct ObjectTextureAssetInfo
	{
		UID _UID;
	};
	Vector<ObjectTextureAssetInfo> List;
	for (auto& Item : ProjectData->Get_AssetIndex()._Files)
	{
		ObjectTextureAssetInfo P;

		List.push_back(std::move(P));
	}



	return ImGuIHelper::DrawObjectField(FieldName, &Value, List.data(), List.size(), sizeof(ObjectTextureAssetInfo),
		[](void* Ptr, void* Object, bool Listmode, const String& Find)
		{
			bool r = false;
			return r;
		});
}
bool ImGuIHelper_Asset::AsssetField(const char* FieldName, UCode::TextureAssetPtr& Value)
{
	struct ObjectTextureAssetInfo
	{
		UID _UID;
	};
	Vector<ObjectTextureAssetInfo> List;
	for (auto& Item : ProjectData->Get_AssetIndex()._Files)
	{
		ObjectTextureAssetInfo P;

		List.push_back(std::move(P));
	}



	return ImGuIHelper::DrawObjectField(FieldName, &Value, List.data(), List.size(), sizeof(ObjectTextureAssetInfo),
		[](void* Ptr, void* Object, bool Listmode, const String& Find)
		{
			bool r = false;
			return r;
		});
}
bool ImGuIHelper_Asset::AsssetField(const char* FieldName, UCode::SpritePtr& Value) 
{ 
	struct ObjectSpriteAssetInfo
	{
		Path _RelativePath;
		UID _UID;
	};
	Vector<ObjectSpriteAssetInfo> List;
	for (auto& Item : ProjectData->Get_AssetIndex()._Files)
	{
		ObjectSpriteAssetInfo P;

		List.push_back(std::move(P));
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


	return ImGuIHelper::DrawObjectField(FieldName, &Value, List.data(), List.size(), sizeof(ObjectSpriteAssetInfo),
		[](void* Ptr, void* Object, bool Listmode, const String& Find)
		{
			bool r = false;
			return r;
		});
}
bool ImGuIHelper_Asset::AsssetField(const char* FieldName, UCode::SpriteAssetPtr& Value)
{
	struct ObjectSpriteAssetInfo
	{
		UID _UID;
	};
	Vector<ObjectSpriteAssetInfo> List;
	for (auto& Item : ProjectData->Get_AssetIndex()._Files)
	{
		ObjectSpriteAssetInfo P;

		List.push_back(std::move(P));
	}

	


	return ImGuIHelper::DrawObjectField(FieldName, &Value, List.data(), List.size(), sizeof(ObjectSpriteAssetInfo),
		[](void* Ptr, void* Object, bool Listmode, const String& Find)
		{
			bool r = false;
			return r;
		});
}
bool ImGuIHelper_Asset::AsssetField(const char* FieldName, UCode::ShaderAssetPtr& Value)
{
	struct ObjectShaderAssetInfo
	{
		UID _UID;
	};
	Vector<ObjectShaderAssetInfo> List;
	for (auto& Item : ProjectData->Get_AssetIndex()._Files)
	{
		ObjectShaderAssetInfo P;

		List.push_back(std::move(P));
	}



	return ImGuIHelper::DrawObjectField(FieldName, &Value, List.data(), List.size(), sizeof(ObjectShaderAssetInfo),
		[](void* Ptr, void* Object, bool Listmode, const String& Find)
		{
			bool r = false;
			return r;
		});
}
bool ImGuIHelper_Asset::AsssetField(const char* FieldName, UCode::ShaderPtr& Value) 
{ 
	struct ObjectShaderAssetInfo
	{
		UID _UID;
	};
	Vector<ObjectShaderAssetInfo> List;
	for (auto& Item : ProjectData->Get_AssetIndex()._Files)
	{
		ObjectShaderAssetInfo P;

		List.push_back(std::move(P));
	}



	return ImGuIHelper::DrawObjectField(FieldName, &Value, List.data(), List.size(), sizeof(ObjectShaderAssetInfo),
		[](void* Ptr, void* Object, bool Listmode, const String& Find)
		{
			bool r = false;
			return r;
		});
}
bool ImGuIHelper_Asset::AsssetField(const char* FieldName, UCode::ScencPtr& Value) 
{
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
	for (auto& Item : ProjectData->Get_AssetIndex()._Files)
	{
		if (Path(Item.RelativePath).extension() != Path(UCode::Scene2dData::FileExtDot))
		{
			continue;
		}

		ObjectSceneAssetInfo P;
		P._RelativePath = Item.RelativePath;
		P._UID = Item.UserID;

		List.push_back(std::move(P));
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

	return ImGuIHelper::DrawObjectField(FieldName, &Value, List.data(), List.size(), sizeof(ObjectSceneAssetInfo),
		[](void* Ptr, void* Object, bool Listmode,const String& Find)
		{
			UCode::ScencPtr& Value = *(UCode::ScencPtr*)Ptr;
			ObjectSceneAssetInfo* obj = (ObjectSceneAssetInfo*)Object;

			auto Name = obj->_RelativePath.generic_string();
			bool r = false;


			if (StringHelper::Fllter(Find,Name))
			{
				
				if (Listmode)
				{
					ImGuIHelper::Image(AppFiles::sprite::Scene2dData, { 20,20 });
					ImGui::SameLine();
				
					//ImGui::PushID(Object);
					r = ImGui::Selectable(Name.c_str(), Value.Get_UID() == obj->_UID);
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
					if (ImGuIHelper::ImageButton(obj +NewName.size(), AppFiles::sprite::Scene2dData, {30,30}))
					{
						Value = obj->_UID;
						r = true;
					}
				}	
			}

			return r;
		}, AppFiles::sprite::Scene2dData, MyName);
}
bool ImGuIHelper_Asset::AsssetField(const char* FieldName, UCode::ScencAssetPtr& Value)
{
	struct ObjectSceneAssetInfo
	{
		UID _UID;
	};
	Vector<ObjectSceneAssetInfo> List;
	for (auto& Item : ProjectData->Get_AssetIndex()._Files)
	{
		ObjectSceneAssetInfo P;

		List.push_back(std::move(P));
	}



	return ImGuIHelper::DrawObjectField(FieldName, &Value, List.data(), List.size(), sizeof(ObjectSceneAssetInfo),
		[](void* Ptr, void* Object, bool Listmode, const String& Find)
		{
			bool r = false;
			return r;
		});
}
bool ImGuIHelper_Asset::AnyAsssetField(UID& Value)
{
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
	for (auto& Item : ProjectData->Get_AssetIndex()._Files)
	{
		if (Item.UserID == UID())
		{
			continue;
		}
		
		ObjectSceneAssetInfo P;
		P._RelativePath = Item.RelativePath;
		
		P._UID = Item.UserID;

		List.push_back(std::move(P));
	}

	String MyName = "None";

	for (auto& Item : List)
	{
		if (Item._UID == Value)
		{
			MyName = Item._RelativePath.generic_string();
			break;
		}
	}
	return ImGuIHelper::DrawObjectField(&Value, List.data(), List.size(), sizeof(ObjectSceneAssetInfo),
		[](void* Ptr, void* Object, bool Listmode,const String& Find)
		{
			UCode::UID& Value = *(UCode::UID*)Ptr;
			ObjectSceneAssetInfo* obj = (ObjectSceneAssetInfo*)Object;

			auto Name = obj->_RelativePath.generic_string();
			bool r = false;


			if (StringHelper::Fllter(Find,Name))
			{
				
				if (Listmode)
				{
					ImGuIHelper::Image(AppFiles::sprite::UCodeAssetIcon, { 20,20 });
					ImGui::SameLine();
				
					//ImGui::PushID(Object);
					r = ImGui::Selectable(Name.c_str(), Value == obj->_UID);
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
					if (ImGuIHelper::ImageButton(obj +NewName.size(), AppFiles::sprite::UCodeAssetIcon, {30,30}))
					{
						Value = obj->_UID;
						r = true;
					}
				}	
			}

			return r;
		}, AppFiles::sprite::UCodeAssetIcon, MyName);
}
bool ImGuIHelper_Asset::AnyAsssetsField(StringView FieldName, Vector<UID>& Value)
{
	return ImGuIHelper::DrawVector<UID>(ImGuIHelper::ToCStr(FieldName), Value, [](UID& Value) -> bool
		{
			return AnyAsssetField(Value);
		});
}
EditorEnd

