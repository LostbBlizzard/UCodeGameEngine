#include "ImGuIHelper_Asset.hpp"
#include "EditorWindows/OtherTypes/DirectoryViewer.hpp"
#include "UEditorModules/UEditorModule.hpp"
#include "ImGuIHelper.hpp"
#include "StringHelper.hpp"
#include "Helper/Fuzzhelper.hpp"
#include "EditorWindows/DragAndDropNames.hpp"
#include "Imgui/imgui_internal.h"
#include "Helper/UserSettings.hpp"
#include "Editor/EditorAppCompoent.hpp"
#include "EditorWindows/ProjectManagement/ProjectFilesWindow.hpp"
#include "UCodeRunTime/ULibrarys/AssetManagement/RawEntityData.hpp"
EditorStart


template<typename T>
bool AsssetField_t(RunTimeProjectData* ProjectData,const char* FieldName, T& Value,const char* FileExtDot,AppFiles::sprite sprite)
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

	Path ext = FileExtDot;

	for (auto& Item : ProjectData->Get_AssetIndex()._Files)
	{
		if (!Item.UserID.has_value())
		{
			continue;
		}

		if (Path(Item.RelativePath).extension() != ext)
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

	ImGuIHelper::ObjectFieldData data;
	data.OnFileDroped = [ProjectData,FileExtDot](const Path& fullpath, void* object, ImGuIHelper::ObjectDropState state) -> bool
		{
			T& objectas =*(T*)object;

			if (state == ImGuIHelper::ObjectDropState::CanBeDroped)
			{
				return (fullpath.extension() == FileExtDot);	
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
			T& Value = *(T*)Ptr;
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
			T& objectas = *(T*)object;

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
			T& objectas = *(T*)object;

			UID id = objectas.Has_UID() ? objectas.Get_UID() : UID();
			return UserSettings::SetCopyBufferAsValueStr("_AssetUID",id);
		};
	data.OnPatse = [ProjectData,FileExtDot](void* object, const String& Paste) -> bool
		{
			bool isgood = false;
			T& objectas = *(T*)object;
			
			auto idop = UserSettings::ReadCopyBufferAs<UID>("_AssetUID",Paste);
		
			if (idop.has_value())
			{
				UID id = idop.value();

				auto& editorindex = ProjectData->Get_AssetIndex();
				auto opasset = editorindex.FindFileUsingID(id);

				if (opasset.has_value())
				{
					if (Path(opasset.value().RelativeAssetName).extension() == Path(FileExtDot))
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
						if (Path(opasset.value().RelativeAssetName).extension() == Path(FileExtDot))
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
			T& objectas = *(T*)object;
			objectas = UID();
		};

	return ImGuIHelper::DrawObjectField(FieldName, &Value, List.data(), List.size(), sizeof(ObjectSceneAssetInfo),
		data, sprite, MyName);
}

template<typename T,typename AssetType>
bool AssetField_tSprite(UCode::AssetManager* AssetManager,RunTimeProjectData* ProjectData,const char* FieldName, T& Value,
	const char* FileExtDot,UC::Sprite* defailtsprite,std::function<UC::Sprite*(AssetType& Type)> GetSprite)
{
	struct ObjectSpriteAssetInfo
	{
		String _ShowAbleName;
		Path _RelativePath;
		Optional<UID> _UID;
		float score = 0;
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

			if (Item.UserID.has_value() && ext == FileExtDot)
			{
				P._UID = Item.UserID.value();
				P._RelativePath = Item.RelativeAssetName;
				P._ShowAbleName = Item.RelativeAssetName;
				P._ShowAbleName = P._ShowAbleName.substr(0, P._ShowAbleName.size() - strlen(FileExtDot));
				List.push_back(std::move(P));
			}
		}

		for (auto& Item : List)
		{
			Item.score = OldFind.size() ? FuzzHelper::GetFuzzRotio(OldFind, Item._ShowAbleName) : minscorebefordontshow();
		}

		std::sort(List.begin(), List.end(), [](const ObjectSpriteAssetInfo& A, const ObjectSpriteAssetInfo& B)
			{
				return A.score > B.score;
			});
	}

	if (FindWasUpdated)
	{
		FindWasUpdated = false;

		for (auto& Item : List)
		{
			Item.score = FuzzHelper::GetFuzzRotio(OldFind, Item._ShowAbleName);
		}

		std::sort(List.begin(), List.end(), [](const ObjectSpriteAssetInfo& A, const ObjectSpriteAssetInfo& B)
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
						AssetType* asset = dynamic_cast<AssetType*>(v.value().Get_Value());
						if (asset)
						{
							spr =GetSprite(*asset);
						}
					}
				}

				break;
			}
		}
	}
	if (spr == nullptr)
	{
		spr = defailtsprite;
	}
	ImGuIHelper::ObjectFieldData data;
	data.OnFileDroped = [ProjectData,FileExtDot](const Path& fullpath, void* object, ImGuIHelper::ObjectDropState state) -> bool
		{
			T& objectas = *(T*)object;

			if (state == ImGuIHelper::ObjectDropState::CanBeDroped)
			{
				return (fullpath.extension() == FileExtDot);
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
	data.OnObjectInList = [&GetSprite,AssetManager,FileExtDot](void* Ptr, void* Object, bool Listmode, const String& Find)
		{
			if (Find != OldFind)
			{
				FindWasUpdated = true;
				OldFind = Find;
			}

			T& Value = *(T*)Ptr;
			ObjectSpriteAssetInfo* obj = (ObjectSpriteAssetInfo*)Object;

			auto Name = obj->_ShowAbleName;
			bool r = false;
			Optional<UID> _idval = Value.Has_UID() ? Value.Get_UID() : Optional<UID>();

			if (obj->score >= minscorebefordontshow())
			{
				NullablePtr<UCode::Sprite> sp;

				if (obj->_UID.has_value())
				{
					auto v = AssetManager->FindOrLoad(obj->_UID.value());
					if (v.has_value())
					{
						if (v.value().Has_Value()) 
						{
							AssetType* asset = dynamic_cast<AssetType*>(v.value().Get_Value());
							if (asset)
							{
								sp = GetSprite(*asset);
							}
						}
					}
				}

				if (Listmode)
				{
					if (sp.has_value())
					{
						auto imagesize = ImGui::GetFrameHeight();
						ImGuIHelper::Image(sp.value().value(), { imagesize,imagesize });
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
							Value = UID();
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
		};

	data.OnInspect = [](void* object) -> bool
		{
			T& objectas = *(T*)object;

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
			T& objectas = *(T*)object;

			UID id = objectas.Has_UID() ? objectas.Get_UID() : UID();

			return UserSettings::SetCopyBufferAsValueStr("_AssetUID",id);
		};
	data.OnPatse = [ProjectData,FileExtDot](void* object, const String& Paste) -> bool
		{
			T& objectas = *(T*)object;
			auto clipboard = Paste;
			bool isgood = false;

			auto idop = UserSettings::ReadCopyBufferAs<UID>("_AssetUID",Paste);
		
			if (idop.has_value())
			{
				UID id = idop.value();
		
				auto& editorindex = ProjectData->Get_AssetIndex();
				auto opasset = editorindex.FindFileUsingID(id);

				if (opasset.has_value())
				{
					if (Path(opasset.value().RelativeAssetName).extension() == Path(FileExtDot))
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
						auto& asset = opasset.value();
						if (Path(opasset.value().RelativeAssetName).extension() == Path(FileExtDot)) 
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
			T& objectas = *(T*)object;
			objectas = UID();
		};


	return ImGuIHelper::DrawObjectField(spr, FieldName, &Value, List.data(), List.size(), sizeof(ObjectSpriteAssetInfo),
			data, MyName);

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
	return AssetField_tSprite<UCode::TexturePtr, UCode::TextureAsset>(
		AssetManager, ProjectData, FieldName, Value, ".png",
		AppFiles::GetSprite(AppFiles::sprite::TextureAsset),
		[](UCode::TextureAsset& value) -> UC::Sprite*
		{
			static UC::Sprite tep = UC::Sprite(&value._Base, 0, 0, value._Base.Get_Width(), value._Base.Get_Height());;
			tep = UC::Sprite(&value._Base, 0, 0, value._Base.Get_Width(), value._Base.Get_Height());
			return &tep;
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
	ImGuIHelper::ObjectFieldData data;
	data.OnObjectInList = [](void* Ptr, void* Object, bool Listmode, const String& Find)
		{
			bool r = false;
			return r;
		};


	return ImGuIHelper::DrawObjectField(FieldName, &Value, List.data(), List.size(), sizeof(ObjectTextureAssetInfo),
	data);
}
bool ImGuIHelper_Asset::AsssetField(const char* FieldName, UCode::SpritePtr& Value)
{
	return AssetField_tSprite<UCode::SpritePtr, UCode::SpriteAsset>(
		AssetManager,ProjectData, FieldName,Value, UC::SpriteData::FileExtDot,
		AppFiles::GetSprite(AppFiles::sprite::SpriteAsset),
		[](UCode::SpriteAsset& value) -> UC::Sprite*
		{
			return &value._Base;
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

		auto ext = Path(Item.RelativeAssetName).extension().generic_string();

		if (Item.UserID.has_value() && ext == UCode::SpriteData::FileExtDot)
		{
			P._UID = Item.UserID.value();

			List.push_back(std::move(P));
		}
	}
	ImGuIHelper::ObjectFieldData data;
	data.OnObjectInList = [](void* Ptr, void* Object, bool Listmode, const String& Find)
		{
			bool r = false;
			return r;
		};



	return ImGuIHelper::DrawObjectField(FieldName, &Value, List.data(), List.size(), sizeof(ObjectSpriteAssetInfo),
		data);
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
	ImGuIHelper::ObjectFieldData data;
	data.OnObjectInList = [](void* Ptr, void* Object, bool Listmode, const String& Find)
		{
			bool r = false;
			return r;
		};


	return ImGuIHelper::DrawObjectField(FieldName, &Value, List.data(), List.size(), sizeof(ObjectShaderAssetInfo),
		data);
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
	ImGuIHelper::ObjectFieldData data;
	data.OnObjectInList = [](void* Ptr, void* Object, bool Listmode, const String& Find)
		{
			bool r = false;
			return r;
		};


	return ImGuIHelper::DrawObjectField(FieldName, &Value, List.data(), List.size(), sizeof(ObjectShaderAssetInfo),
	data);
}
bool ImGuIHelper_Asset::AsssetField(const char* FieldName, UCode::ScencPtr& Value)
{	
	return AsssetField_t<UCode::ScencPtr>(ProjectData,FieldName, Value, UC::Scene2dData::FileExtDot, AppFiles::sprite::Scene2dData);
}

bool ImGuIHelper_Asset::AsssetField(const char* FieldName, UCode::ScencAssetPtr& Value)
{
	return false;
}

bool ImGuIHelper_Asset::AsssetField(const char* FieldName, TilePaletteAssetPtr& Value)
{
	return false;
}


bool ImGuIHelper_Asset::AsssetField(const char* FieldName, TilePalettePtr& Value)
{
	return AsssetField_t<TilePalettePtr>(ProjectData,FieldName, Value, TilePalette::FileExtDot, AppFiles::sprite::TilePalette);
}

bool ImGuIHelper_Asset::AsssetField(const char* FieldName, TileDataPtr& Value)
{
	
	return AssetField_tSprite<TileDataPtr, UCode::TileAsset>(
		AssetManager,ProjectData, FieldName,Value, UC::TileData::FileExtDot,
		AppFiles::GetSprite(AppFiles::sprite::TileAsset),
		[](UCode::TileAsset& value) -> UC::Sprite*
		{
			auto& assetptr = value._Base.Sprite;
			UC::Sprite* r = nullptr;

			if (assetptr.Has_UID() && !assetptr.Get_Asset())
			{
				auto val = AssetManager->FindOrLoad_t<UC::SpriteAsset>(assetptr.Get_UID());
				if (val.has_value())
				{
					r = &val.value()->_Base;
					assetptr = val.value()->GetManaged();
				}
			}
			
			if (assetptr.Get_Asset())
			{
				r = assetptr.Get_Asset();
			}


			if (r == nullptr)
			{
				r = AppFiles::GetSprite(AppFiles::sprite::TileAsset);
			}

			return r;
		});
}

bool ImGuIHelper_Asset::AsssetField(const char* FieldName, TileDataAssetPtr& Value)
{
	return false;
}

UCode::Sprite* ImGuIHelper_Asset::GetAssetSpriteFromUID(const UID& value, AssetSpriteType Type)
{
	auto& index = ProjectData->Get_AssetIndex();
	auto file = index.FindFileUsingID(value);
	if (!file.has_value())
	{
		return AppFiles::GetSprite(AppFiles::sprite::UCodeAssetIcon);
	}


	auto& asset = file.value();
	auto ext = Path(asset.RelativeAssetName).extension().generic_string();


	if (ext == UC::Scene2dData::FileExtDot)	{ return AppFiles::GetSprite(AppFiles::sprite::Scene2dData);}
	else if (ext == UC::RawEntityData::FileExtDot){ return AppFiles::GetSprite(AppFiles::sprite::RawEntityData);}

	if (Type == AssetSpriteType::Icon)
	{
		if (ext == UC::SpriteData::FileExtDot)
		{
			auto val = AssetManager->FindOrLoad_t<UC::SpriteAsset>(value);
			if (val.has_value())
			{
				return &val.value()->_Base;
			}
		}
		if (ext == ".png")
		{
			auto val = AssetManager->FindOrLoad_t<UC::TextureAsset>(value);
			if (val.has_value())
			{
				auto& tex = val.value()->_Base;

				static UC::Sprite tep = UC::Sprite(&tex, 0, 0, tex.Get_Height(), tex.Get_Width());
				tep = UC::Sprite(&tex, 0, 0, tex.Get_Height(), tex.Get_Width());

				return &tep;
			}
		}
		if (ext == UC::TileData::FileExtDot)
		{
			auto val = AssetManager->FindOrLoad(value);
			if (val.has_value())
			{
				auto val = AssetManager->FindOrLoad_t<TileDataAsset>(value);
				if (val.has_value())
				{
					auto& SpriteUID = val.value()->_Base._Data.Sprite;
					if (SpriteUID.Has_UID())
					{
						return GetAssetSpriteFromUID(SpriteUID.Get_UID(), Type);
					}
				}
			}
		}
	}
	
	
	{
		if (ext == UC::TileData::FileExtDot) { return AppFiles::GetSprite(AppFiles::sprite::TileAsset); }
		if (ext == TileDataPack::FileExtDot) { return AppFiles::GetSprite(AppFiles::sprite::TilePack); }
		if (ext == TilePalette::FileExtDot) { return AppFiles::GetSprite(AppFiles::sprite::TilePalette); }
		if (ext == UC::SpriteData::FileExtDot) { return AppFiles::GetSprite(AppFiles::sprite::SpriteAsset); }
		if (ext == UC::TextureData::FileExtDot) { return AppFiles::GetSprite(AppFiles::sprite::TextureAsset); }

		return AppFiles::GetSprite(AppFiles::sprite::UCodeAssetIcon);
	}
}

bool ImGuIHelper_Asset::AnyAsssetField(UID& Value)
{
	struct ObjectSceneAssetInfo
	{
		Path _RelativePath;
		UID _UID;
		float score =0;
		String ShowableName;
	};


	static Vector<ObjectSceneAssetInfo> List;
	static String OldFind;
	static bool FindWasUpdated = false;
	bool AssetIndexUpdated = true;

	if (AssetIndexUpdated)
	{
		List.clear();
		{
			ObjectSceneAssetInfo P;
			P._RelativePath = "None";
			P.ShowableName = "None";

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
			P._UID = Item.UserID.value();
			P.ShowableName = Item.RelativeAssetName;
			List.push_back(std::move(P));
		}
		
		for (auto& Item : List)
		{
			Item.score = OldFind.size() ? FuzzHelper::GetFuzzRotio(OldFind, Item.ShowableName) : minscorebefordontshow();
		}

		std::sort(List.begin(), List.end(), [](const ObjectSceneAssetInfo& A,const ObjectSceneAssetInfo& B)
			{
				return A.score > B.score;
			});
	}
	if (FindWasUpdated)
	{
		FindWasUpdated = false;

		for (auto& Item : List)
		{
			Item.score = FuzzHelper::GetFuzzRotio(OldFind, Item.ShowableName);
		}

		std::sort(List.begin(), List.end(), [](const ObjectSceneAssetInfo& A,const ObjectSceneAssetInfo& B)
			{
				return A.score > B.score;
			});	
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
	
	ImGuIHelper::ObjectFieldData data;
	data.OnFileDroped = [](const Path& fullpath, void* object, ImGuIHelper::ObjectDropState state) -> bool
		{
			UCode::UID& objectas = *(UCode::UID*)object;

			if (state == ImGuIHelper::ObjectDropState::CanBeDroped)
			{
				auto& editorindex = ProjectData->Get_AssetIndex();
				auto assetpath = ProjectData->GetAssetsDir();
				Path relativepath = fullpath.native().substr(assetpath.native().size());

				auto opasset = editorindex.FindFileRelativeAssetName(relativepath.generic_string());
				if (opasset.has_value())
				{
					auto& asset = opasset.value();
					return asset.UserID.has_value();
				}
				return false;
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
	data.OnObjectInList = [](void* Ptr, void* Object, bool Listmode, const String& Find)
		{
			if (Find != OldFind)
			{
				OldFind = Find;
				FindWasUpdated = true;
			}

			UCode::UID& Value = *(UCode::UID*)Ptr;
			ObjectSceneAssetInfo* obj = (ObjectSceneAssetInfo*)Object;

			auto Name = obj->ShowableName;
			bool r = false;


			if (obj->score >= minscorebefordontshow())
			{

				if (Listmode)
				{
					auto imagesize = ImGui::GetFrameHeight();
					ImGuIHelper::Image(GetAssetSpriteFromUID(obj->_UID,AssetSpriteType::Icon), { imagesize,imagesize });
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
					auto imagesize = ImGui::GetFrameHeight();
					if (ImGuIHelper::ImageButton(obj + NewName.size(), GetAssetSpriteFromUID(obj->_UID,AssetSpriteType::Icon), { imagesize,imagesize }))
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
			UID& objectas = *(UID*)object;

			auto p = EditorAppCompoent::GetCurrentEditorAppCompoent();

			bool r = false;
			if (auto win = p->Get_Window<ProjectFilesWindow>())
			{
				win->OpenAndFocusOnAsset(objectas);
				r = true;
			}
			return r;
		};
	data.OnCopy = [](void* object) -> String
		{
			UID& objectas = *(UID*)object;
			return UserSettings::SetCopyBufferAsValueStr("_AssetUID",objectas);
		};
	data.OnPatse = [](void* object,const String& Paste)
		{	
			bool isgood = false;
			
			UID& objectas = *(UID*)object;
			auto idop = UserSettings::ReadCopyBufferAs<UID>("_AssetUID",Paste);
		
			if (idop.has_value())
			{
				UID id = idop.value();
		

				auto& editorindex = ProjectData->Get_AssetIndex();
				auto opasset = editorindex.FindFileUsingID(id);

				if (opasset.has_value())
				{
					objectas = id;
					isgood = true;
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

						if (asset.UserID.has_value())
						{
							objectas = asset.UserID.value();
							isgood = true;
						}
					}
				}
			}
			return isgood;
		};
	data.OnDestory = [](void* object)
		{
			UID& objectas = *(UID*)object;
			objectas = UID();
		};
		return ImGuIHelper::DrawObjectField(GetAssetSpriteFromUID(Value,AssetSpriteType::Icon), &Value, List.data(), List.size(), sizeof(ObjectSceneAssetInfo),
			data, MyName);
}
bool ImGuIHelper_Asset::AnyAsssetsField(StringView FieldName, Vector<UID>& Value)
{
	return ImGuIHelper::DrawVector<UID>(ImGuIHelper::ToCStr(FieldName), Value, [](UID& Value) -> bool
		{
			return AnyAsssetField(Value);
		});
}

bool ImGuIHelper_Asset::IconField(StringView FieldName, UCode::SpritePtr& Value, ImVec2 IconSize)
{
	bool changed = false;
	UCode::Sprite* spr = nullptr;
	if (Value.Has_UID())
	{
		auto id = Value.Get_UID();
		for (auto& Item : ProjectData->Get_AssetIndex()._Files)
		{
			if (Item.UserID.has_value())
			{
				if (Item.UserID.value() == id)
				{
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
	}
	if (spr == nullptr)
	{
		spr = AppFiles::GetSprite(AppFiles::sprite::AppIcon);
	}
	ImGui::PushStyleColor(ImGuiCol_::ImGuiCol_Button, { 0,0,0,0 });
	ImGui::PushStyleColor(ImGuiCol_::ImGuiCol_ButtonActive, { 0,0,0,0 });
	ImGui::PushStyleColor(ImGuiCol_::ImGuiCol_ButtonHovered, { 1,1,1,0.1 });

	auto popupid = "IconFeildPopup";
	if (ImGuIHelper::ImageButton(&Value, spr, IconSize))
	{
		ImGui::OpenPopup(popupid);
	}
	if (ImGui::IsItemHovered() || ImGui::IsItemFocused())
	{
		String v = String(FieldName);
		ImGui::SetTooltip(v.c_str());
	}

	if (ImGui::BeginPopup(popupid))
	{
		auto start = ImGuIHelper::DrawStarOfMenuObjects();

		if (ImGui::Selectable("None", Value.Has_UID() ? Value.Get_UID() == UID() : false))
		{
			Value = UID();
			changed = true;
		}

		for (auto& Item : ProjectData->Get_AssetIndex()._Files)
		{
			auto ext = Path(Item.RelativeAssetName).extension().generic_string();

			if (Item.UserID.has_value() && ext == UCode::SpriteData::FileExtDot)
			{
				NullablePtr<UCode::Sprite> sp;
				auto v = AssetManager->FindOrLoad(Item.UserID.value());
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

				if (start.ListMode)
				{
					if (sp.has_value())
					{
						auto imagesize = ImGui::GetFrameHeight();
						ImGuIHelper::Image(sp.value().value(), { imagesize,imagesize });
						ImGui::SameLine();
					}

					//ImGui::PushID(Object);
					String Name = Item.RelativeAssetName;
					bool r = ImGui::Selectable(Name.c_str(), Value.Has_UID() ? Value.Get_UID() == Item.UserID.value() : false);
					if (r)
					{
						Value = Item.UserID.value();
						changed = true;
					}
				}
				else
				{

				}
			}
		}
		ImGui::EndPopup();
	}

	bool isspect = false;
	bool iscopy = false;
	bool ispaste = false;
	bool isdelete = false;
	if (ImGui::IsItemFocused())
	{
		auto& Settings = UserSettings::GetSettings();

		if (Settings.IsKeybindActive(KeyBindList::Inspect))
		{
			isspect = true;
		}

		if (Settings.IsKeybindActive(KeyBindList::Copy))
		{
			iscopy = true;
		}

		if (Settings.IsKeybindActive(KeyBindList::Paste))
		{
			ispaste = true;
		}	
		
		if (Settings.IsKeybindActive(KeyBindList::Delete))
		{
			isdelete = true;
		}
	}
	if (ImGuIHelper::BeginPopupContextItem("IconFieldPopup"))
	{
		auto& Settings = UserSettings::GetSettings();

		auto str = Settings.KeyBinds[(size_t)KeyBindList::Inspect].ToString();


		if (ImGui::MenuItem("Show Location", str.c_str()) || Settings.IsKeybindActive(KeyBindList::Inspect))
		{
			isspect = true;
			ImGui::CloseCurrentPopup();
		}

		str = Settings.KeyBinds[(size_t)KeyBindList::Copy].ToString();
		if (ImGui::MenuItem("Copy", str.c_str()) || Settings.IsKeybindActive(KeyBindList::Copy))
		{
			iscopy = true;
			ImGui::CloseCurrentPopup();
		}

		str = Settings.KeyBinds[(size_t)KeyBindList::Paste].ToString();
		if (ImGui::MenuItem("Paste", str.c_str()) || Settings.IsKeybindActive(KeyBindList::Paste))
		{
			ispaste = true;
			ImGui::CloseCurrentPopup();
		}

		str = Settings.KeyBinds[(size_t)KeyBindList::Delete].ToString();
		if (ImGui::MenuItem("Set to None",str.c_str()) || Settings.IsKeybindActive(KeyBindList::Delete))
		{	
			isdelete = true;
			ImGui::CloseCurrentPopup();
		}

		ImGui::EndPopup();
	}

	if (isspect)
	{
		auto p = EditorAppCompoent::GetCurrentEditorAppCompoent();

		if (auto win = p->Get_Window<ProjectFilesWindow>())
		{
			if (Value.Has_UID())
			{
				win->OpenAndFocusOnAsset(Value.Get_UID());
			}
		}
	}
	if (iscopy)
	{
		iscopy = true;
		UID id = Value.Has_UID() ? Value.Get_UID() : UID();

		UserSettings::SetCopyBufferAsValue("_AssetUID", id);
	}
	if (ispaste)
	{
		auto idop = UserSettings::ReadCopyBufferAs<UID>("_AssetUID");

		if (idop.has_value())
		{
			UID id = idop.value();

			auto& editorindex = ProjectData->Get_AssetIndex();
			auto opasset = editorindex.FindFileUsingID(id);

			if (opasset.has_value())
			{
				if (Path(opasset.value().RelativeAssetName).extension() == Path(UCode::SpriteData::FileExtDot))
				{
					Value = id;
					changed = true;
				}
			}
		}
		else
		{
			auto pathop = UserSettings::ReadCopyBufferAs<Path>("AssetPath");

			if (pathop.has_value())
			{
				auto& editorindex = ProjectData->Get_AssetIndex();
				auto rel = FileHelper::ToRelativePath(ProjectData->GetAssetsDir(), pathop.value().generic_string()).generic_string();
				auto opasset = editorindex.FindFileRelativeAssetName(rel);

				if (opasset.has_value())
				{
					auto asset = opasset.value();

					if (asset.UserID.has_value())
					{
						Value = asset.UserID.value();
						changed = true;
					}
				}
			}
		}
	}
	if (isdelete)
	{
		Value = UID();
		changed = true;
	}

	if (ImGui::BeginDragDropTarget())
	{
		ImGuiDragDropFlags target_flags = 0;
		target_flags |= ImGuiDragDropFlags_AcceptBeforeDelivery;
		target_flags |= ImGuiDragDropFlags_AcceptNoDrawDefaultRect;

		if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload(DragAndDropType_AssetPath, target_flags))
		{
			const Path* fullpath = *(Path**)payload->Data;


			auto ext = Path(*fullpath).extension().generic_string();
			bool canbedroped = ext == UCode::SpriteData::FileExtDot;
			if (payload->IsDelivery())
			{
				if (canbedroped)
				{
					auto& editorindex = ProjectData->Get_AssetIndex();
					auto assetpath = ProjectData->GetAssetsDir();
					Path relativepath = (*fullpath).native().substr(assetpath.native().size());

					auto opasset = editorindex.FindFileRelativeAssetName(relativepath.generic_string());
					if (opasset.has_value())
					{
						auto& asset = opasset.value();
						if (asset.UserID.has_value())
						{
							Value = asset.UserID.value();
							changed = true;
						}
					}
				}
			}

			if (canbedroped)
			{
				ImGuiContext& g = *GImGui;
				ImGui::RenderDragDropTargetRect(g.DragDropTargetRect, g.DragDropTargetClipRect);
			}
		}
		ImGui::EndDragDropTarget();
	}

	ImGui::PopStyleColor();
	ImGui::PopStyleColor();
	ImGui::PopStyleColor();


	return changed;
}
EditorEnd

