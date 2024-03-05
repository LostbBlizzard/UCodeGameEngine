#include "ImGuIHelper_Asset.hpp"
#include "EditorWindows/OtherTypes/DirectoryViewer.hpp"
#include "UEditorModules/UEditorModule.hpp"
#include "ImGuIHelper.hpp"
#include "StringHelper.hpp"
#include "UCodeRunTime/ULibrarys/AssetManagement/AssetRendering.hpp"
#include "OtherLibrarys/rapidfuzz/fuzz.hpp"
EditorStart

String ToUper(StringView Base)
{
	String r;
	for (auto& Item : Base)
	{
		r.push_back(std::toupper(Item));
	}
	return r;
}

float getfuzzrotio(StringView Base,StringView Other)
{
	static String upbase;
	static String upOther;
	upbase= ToUper(Base);
	upOther = ToUper(Other);

	return rapidfuzz::fuzz::ratio(upbase, upOther);
}

float minscorebefordontshow()
{
	return 0;
}

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
		String _ShowAbleName;
		Path _RelativePath;
		Optional<UID> _UID;
		float score=0;
	};
	static Vector<ObjectSpriteAssetInfo> List = {};
	static String OldFind;
	static bool FindWasUpdated = false;

	bool AssetIndexUpdated = true;

	if (AssetIndexUpdated)
	{
		List.clear();
		{
			ObjectSpriteAssetInfo P;
			P._RelativePath = "None";
			P._ShowAbleName = "None";

			List.push_back(std::move(P));
		}
		for (auto& Item : ProjectData->Get_AssetIndex()._Files)
		{
			ObjectSpriteAssetInfo P;

			auto ext = Path(Item.RelativeAssetName).extension().generic_string();

			if (Item.UserID.has_value() && ext == UCode::SpriteData::FileExtDot)
			{
				P._UID = Item.UserID.value();
				P._RelativePath = Item.RelativeAssetName;
				P._ShowAbleName = Item.RelativeAssetName;
				P._ShowAbleName = P._ShowAbleName.substr(0, P._ShowAbleName.size() - strlen(UCode::SpriteData::FileExtDot));
				List.push_back(std::move(P));
			}
		}

		for (auto& Item : List)
		{
			Item.score = getfuzzrotio(OldFind, Item._ShowAbleName);
		}

		std::sort(List.begin(), List.end(), [](const ObjectSpriteAssetInfo& A,const ObjectSpriteAssetInfo& B)
			{
				return A.score > B.score;
			});
	}

	if (FindWasUpdated)
	{
		FindWasUpdated = false;

		for (auto& Item : List)
		{
			Item.score = getfuzzrotio(OldFind, Item._ShowAbleName);
		}

		std::sort(List.begin(), List.end(), [](const ObjectSpriteAssetInfo& A,const ObjectSpriteAssetInfo& B)
			{
				return A.score > B.score;
			});	
	}

	String MyName = "None";
	UCode::Sprite* spr = nullptr;
	if (Value.Has_UID()) 
	{
		auto id = Value.Get_UID();
		for (auto& Item : List)
		{
			if (Item._UID == id)
			{
				MyName = Item._ShowAbleName;

				auto v = AssetManager->FindOrLoad(id);
				if (v.has_value())
				{
					if (v.value().Has_Value()) {
						UCode::SpriteAsset* asset = dynamic_cast<UCode::SpriteAsset*>(v.value().Get_Value());
						if (asset)
						{
							spr = &asset->_Base;
						}
					}
				}

				break;
			}
		}
	}
	if (spr == nullptr)
	{
		spr = AppFiles::GetSprite(AppFiles::sprite::AppIcon);
	}

	return ImGuIHelper::DrawObjectField(spr,FieldName, &Value, List.data(), List.size(), sizeof(ObjectSpriteAssetInfo),
		[](void* Ptr, void* Object, bool Listmode, const String& Find)
		{
			if (Find != OldFind)
			{
				FindWasUpdated = true;
				OldFind = Find;
			}

			UCode::SpritePtr& Value = *(UCode::SpritePtr*)Ptr;
			ObjectSpriteAssetInfo* obj = (ObjectSpriteAssetInfo*)Object;

			auto Name = obj->_ShowAbleName;
			bool r = false;
			Optional<UID> _idval = Value.Has_UID() ? Value.Get_UID() : Optional<UID>();

			if (Find.size() ? obj->score >= minscorebefordontshow() : true)
			{
				NullablePtr<UCode::Sprite> sp;
			
				if (_idval.has_value()) 
				{
					auto v = AssetManager->FindOrLoad(_idval.value());
					if (v.has_value())
					{
						if (v.value().Has_Value()) {
							UCode::SpriteAsset* asset = dynamic_cast<UCode::SpriteAsset*>(v.value().Get_Value());
							if (asset)
							{
								sp = &asset->_Base;
							}
						}
					}
				}

				if (Listmode)
				{
					if (sp.has_value()) {
						ImGuIHelper::Image(sp.value().value(), {20,20});
						ImGui::SameLine();
					}
				
					//ImGui::PushID(Object);
					r = ImGui::Selectable(Name.c_str(), _idval.has_value() ? _idval.value() == obj->_UID : false);
					//ImGui::PopID();
					
					if (r)
					{
						if (obj->_UID.has_value()) 
						{
							Value = obj->_UID.value();
						}
						else
						{
							Value = {};
						}
						r = true;
					}
				}
				else
				{
					r = false;

					auto NewName = Name;
					ImGuIHelper::Text(NewName);
					//if (ImGuIHelper::ImageButton(obj +NewName.size(), sp, {30,30}))
					{
						//Value = obj->_UID;
						//r = true;
					}
				}	
			}

			return r;
		},MyName);
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

		auto ext = Path(Item.RelativeAssetName).extension().generic_string();

		if (Item.UserID.has_value() && ext == UCode::SpriteData::FileExtDot)
		{
			P._UID = Item.UserID.value();

			List.push_back(std::move(P));
		}
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
	return false;
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

	Path ext = UCode::Scene2dData::FileExtDot;

	for (auto& Item : ProjectData->Get_AssetIndex()._Files)
	{
		if (!Item.UserID.has_value())
		{
			continue;
		}
			
		if (Path(Item.RelativePath).extension() != Path(ext))
		{
			continue;
		}

		ObjectSceneAssetInfo P;
		P._RelativePath = Item.RelativePath;
		P._UID = Item.UserID.value();

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
		if (!Item.UserID.has_value())
		{
			continue;
		}
		
		ObjectSceneAssetInfo P;
		P._RelativePath = Item.RelativePath;	
		P._UID =Item.UserID.value();

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

