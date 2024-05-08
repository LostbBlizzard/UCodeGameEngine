#include "ImGuIHelper_Asset.hpp"
#include "EditorWindows/OtherTypes/DirectoryViewer.hpp"
#include "UEditorModules/UEditorModule.hpp"
#include "ImGuIHelper.hpp"
#include "StringHelper.hpp"
#include "Helper/Fuzzhelper.hpp"
#include "EditorWindows/DragAndDropNames.hpp"
#include "Imgui/imgui_internal.h"
EditorStart



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
	ImGuIHelper::ObjectFieldData data;
	data.OnObjectInList = [](void* Ptr, void* Object, bool Listmode, const String& Find)
		{
			bool r = false;
			return r;
		};


	return ImGuIHelper::DrawObjectField(FieldName, &Value, List.data(), List.size(), sizeof(ObjectTextureAssetInfo),
		data);
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
	ImGuIHelper::ObjectFieldData data;
	data.OnFileDroped = [](const Path& fullpath, void* object, ImGuIHelper::ObjectDropState state) -> bool
		{
			UCode::SpritePtr& objectas =*(UCode::SpritePtr*)object;

			if (state == ImGuIHelper::ObjectDropState::CanBeDroped)
			{
				return (fullpath.extension() == UCode::SpriteData::FileExtDot);	
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
				FindWasUpdated = true;
				OldFind = Find;
			}

			UCode::SpritePtr& Value = *(UCode::SpritePtr*)Ptr;
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
		};

	return ImGuIHelper::DrawObjectField(spr, FieldName, &Value, List.data(), List.size(), sizeof(ObjectSpriteAssetInfo),
			data, MyName);
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

	ImGuIHelper::ObjectFieldData data;
	data.OnFileDroped = [](const Path& fullpath, void* object, ImGuIHelper::ObjectDropState state) -> bool
		{
			UCode::ScencPtr& objectas =*(UCode::ScencPtr*)object;

			if (state == ImGuIHelper::ObjectDropState::CanBeDroped)
			{
				return (fullpath.extension() == UCode::Scene2dData::FileExtDot);	
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
			UCode::ScencPtr& Value = *(UCode::ScencPtr*)Ptr;
			ObjectSceneAssetInfo* obj = (ObjectSceneAssetInfo*)Object;

			auto Name = obj->_RelativePath.generic_string();
			bool r = false;


			if (StringHelper::Fllter(Find, Name))
			{

				if (Listmode)
				{
					auto imagesize = ImGui::GetFrameHeight();
					ImGuIHelper::Image(AppFiles::sprite::Scene2dData, { imagesize,imagesize });
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

	return ImGuIHelper::DrawObjectField(FieldName, &Value, List.data(), List.size(), sizeof(ObjectSceneAssetInfo),
		data, AppFiles::sprite::Scene2dData, MyName);
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
	ImGuIHelper::ObjectFieldData data;
	data.OnObjectInList = [](void* Ptr, void* Object, bool Listmode, const String& Find)
		{
			bool r = false;
			return r;
		};


	return ImGuIHelper::DrawObjectField(FieldName, &Value, List.data(), List.size(), sizeof(ObjectSceneAssetInfo),
	data);
}

UCode::Sprite* GetAssetSpriteFromUID(const UID& value)
{
	return AppFiles::GetSprite(AppFiles::sprite::UCodeAssetIcon);
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
					ImGuIHelper::Image(AppFiles::sprite::UCodeAssetIcon, { imagesize,imagesize });
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
					if (ImGuIHelper::ImageButton(obj + NewName.size(), GetAssetSpriteFromUID(obj->_UID), { imagesize,imagesize }))
					{
						Value = obj->_UID;
						r = true;
					}
				}
			}

			return r;
		};

		return ImGuIHelper::DrawObjectField(GetAssetSpriteFromUID(Value), &Value, List.data(), List.size(), sizeof(ObjectSceneAssetInfo),
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

