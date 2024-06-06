#include "EntityAssetFile.hpp"
#include "Editor/EditorAppCompoent.hpp"
#include "Helper/ImGuIHelper.hpp"
#include <EditorWindows/DragAndDropNames.hpp>
#include "stb_image_write.h"
#include "UCodeRunTime/ULibrarys/AssetManagement/EntityPlaceHolder.hpp"
#include "Imgui/imgui_internal.h"
#include "UCodeRunTime/ULibrarys/AssetManagement/AssetRendering.hpp"
#include "UCodeRunTime/Rendering/Render.hpp"
#include "EditorWindows/BasicWindows/GameEditorWindow.hpp"
#include "EditorWindows/InspectTypes/Inspect_Entity2d.hpp"
EditorStart
UC::ImageData RenderFrame(RenderFrameData& Data, UC::RenderRunTime2d::DrawData& drawdata, UC::Entity* newentity)
{
	UC::Camera2d* Cam = newentity->AddCompoent<UC::Camera2d>();
	Cam->Set_Ortho_size(Data.CamOrth);
	Cam->API_Set_WindowSize(Data.CamWidth, Data.CamHeight);
	Cam->Set_BackRoundClearColor(Color(0, 0, 0, 0));

	auto scene = Cam->NativeEntity()->NativeScene();
	auto ren = UC::RenderRunTime2d::GetRenderRunTime(scene->Get_RunTime());
	UC::RenderAPI::Render render;

	UC::RenderAPI::WindowData window;
	window.height = Data.CamWidth;
	window.width = Data.CamHeight;
	window.shared_window = glfwGetCurrentContext();
	window.ImGui_Init = false;
	window.GenNewWindow = false;

	render.Init(window, scene->Get_RunTime());

	Cam->Get_Buffer().UpdateBufferSize(window.height, window.width);

	render.Draw(drawdata, Cam);

	auto v = Cam->Get_Buffer().GetGPUImageData();

	render.EndRender();

	UC::Compoent::Destroy(Cam);
	scene->Get_RunTime()->DestroyNullScenes();

	return v;
}

EntityAssetFile::EntityAssetFile()
{
	FileExtWithDot = UC::RawEntityData::FileExtDot;
	CanHaveLiveingAssets = true;
	CallLiveingAssetsWhenUpdated = true;
}
Optional<GetUIDInfo> EntityAssetFile::GetFileUID(UEditorGetUIDContext& context)
{
	UC::RawEntityData val;
	if (UC::RawEntityData::ReadFromFile(context.AssetPath, val))
	{
		GetUIDInfo r;
		r._MainAssetID = val._UID;

		return r;
	}
	return {};
}
EntityAssetFile::Liveing::Liveing()
{

}
EntityAssetFile::Liveing::~Liveing()
{
	if (_entity.Has_Value())
	{
		auto e = _entity.Get_Value();
		UC::Entity::Destroy(e);
	}
}
UC::RunTimeScene* EntityAssetFile::Liveing::GetTepScene()
{
	static UC::GameRunTime _tepRunTime = UC::GameRunTime(EditorAppCompoent::GetCurrentEditorAppCompoent()->GetGameRunTime()->Get_LibraryRef());
	static UC::RunTimeScene* _tepScene = _tepRunTime.Add_NewScene();

	return _tepScene;
}
UC::Entity* EntityAssetFile::Liveing::GetAsEntity()
{
	if (!_entity.Has_Value())
	{
		auto e = GetTepScene()->NewEntity();

		UC::Scene2dData::LoadEntity(e, _asset._Base._Data);

		_entity = e->NativeManagedPtr();
	}
	return _entity.Get_Value();
}
Vector<Byte> EntityAssetFile::Liveing::GetAssetAsBytes(UC::Scene2dData::Entity_Data& data)
{
	USerializer node(UC::USerializerType::Fastest);
	node.Write("_", data);

	Vector<Byte> r;
	node.ToBytes(r, false);
	return r;
}
void EntityAssetFile::Liveing::Init(const UEditorAssetFileInitContext& Context)
{
	if (UC::RawEntityData::ReadFromFile(FileFullPath, _asset._Base))
	{
		_assetasbytes = GetAssetAsBytes(_asset._Base._Data);
	}
	else
	{
		UCodeGEError("Unable to Open/Parse " << FileFullPath);
	}
}
Path EntityAssetFile::Liveing::ThumbnailPath()
{
	return TemporaryPath.native() + Path(".png").native();
}
void EntityAssetFile::Liveing::LoadThumbnail()
{
	const Path thumbnailpath = ThumbnailPath();
	bool isoutdataed = true;
	bool cancach = false;
	{
		if (cancach)
		{
			if (std::filesystem::exists(thumbnailpath))
			{

			}
		}
	}

	auto lib = EditorAppCompoent::GetCurrentEditorAppCompoent()->GetGameRunTime()->Get_Library_Edit();

	UCode::Threads* threads = UCode::Threads::Get(lib);

	if (isoutdataed)
	{
		std::shared_ptr<Delegate<void>> LoopFunc = std::make_shared<Delegate<void>>();


		auto _tepRunTime = std::make_shared<UC::GameRunTime>(EditorAppCompoent::GetCurrentEditorAppCompoent()->GetGameRunTime()->Get_LibraryRef());
		UC::RunTimeScene* _tepScene = _tepRunTime->Add_NewScene();
		UC::Entity* myentity = _tepScene->NewEntity();
		UC::Scene2dData::LoadEntity(myentity, _asset._Base._Data);


		auto ren = UC::RenderRunTime2d::GetRenderRunTime(_tepRunTime.get());

		
		RenderFrameData data;
		data.CamHeight = 500;
		data.CamWidth = 500;
		data.CamPos = myentity->WorldPosition();
		data.CamOrth = 1;

		UC::Camera2d* Cam = myentity->AddCompoent<UC::Camera2d>();
		Cam->Set_Ortho_size(data.CamOrth);
		Cam->API_Set_WindowSize(data.CamWidth, data.CamHeight);
		Cam->Set_BackRoundClearColor(Color(0, 0, 0, 0));


		ren->UpdateDrawData(Cam);
		auto renderdata = ren->Get_DrawData();



		*LoopFunc = [LoopFunc, this, renderdata = std::move(renderdata),data, _tepRunTime = std::move(_tepRunTime),Cam, _tepScene, myentity]()
			{

				bool issceneloading = renderdata.HasAnyPlaceHolders();
				auto lib = EditorAppCompoent::GetCurrentEditorAppCompoent()->GetGameRunTime()->Get_Library_Edit();
				UCode::Threads* threads = UCode::Threads::Get(lib);

				if (issceneloading)
				{
					Delegate<void> Func = [LoopFunc]() -> void
						{
							(*LoopFunc)();
						};
					threads->AddTask_t(UCode::TaskType::Main, Func, {}).Start();
					return;
				}

				Delegate<UC::ImageData> RenderFunc = [this, _tepScene, myentity, _tepRunTime,data,Cam]() mutable -> UC::ImageData
					{
						auto ren = UC::RenderRunTime2d::GetRenderRunTime(_tepRunTime.get());
						ren->UpdateDrawData(Cam);//We need UpdateDrawData because of how SpriteRenderer2d PixelsPerUnit Work Right now.
						auto renderdata = ren->Get_DrawData();

						return RenderFrame(data, renderdata,myentity);
					};
				Delegate<UC::ImageData, UC::ImageData&&> WriteToOutput = [this](UC::ImageData&& val) -> UC::ImageData
					{
						const Path thumbnailpath = ThumbnailPath();
						auto pathstring = thumbnailpath.generic_string();

						constexpr auto CHANNEL_NUM = 4;

#if UCodeGEOpenGL
						//OpenGl Textures are unside down.
						UC::Texture::FlipImageVertically(val.Width, val.Height, val._ColorData.data());
#endif

						if (false)
						{
							std::filesystem::create_directories(thumbnailpath.parent_path());
							stbi_write_png(pathstring.c_str(), val.Width, val.Height, CHANNEL_NUM, val._ColorData.data(), val.Width * CHANNEL_NUM);
						}
						return val;
					};
				Delegate<void, UC::ImageData&&> SetFile = [this](UC::ImageData&& val) -> void
					{
						_Thumbnail = UC::Texture(val.Width, val.Height, val._ColorData.data());
						auto tex = &_Thumbnail.value();
						_ThumbnailSprite = UC::Sprite(tex, 0, 0, tex->Get_Width(), tex->Get_Height());

						tex->FreeFromCPU();
					};

				threads->AddTask_t(UCode::TaskType::Rendering, std::move(RenderFunc), {})
					.ContinueOnThread(UCode::TaskType::File_Output, std::move(WriteToOutput))
					.ContinueOnMainThread(std::move(SetFile)).Start();
			};

		Delegate<void> Func = [LoopFunc]() -> void
			{
				(*LoopFunc)();
			};
		threads->AddTask_t(UCode::TaskType::Main, Func, {}).Start();
	}
	else
	{
		Delegate<Vector<Byte>> Func = [lib, path = this->FileFullPath]()
			{
				UCode::GameFiles* f = UCode::GameFiles::Get(lib);
				auto bytes = f->ReadFileAsBytes(path);

				return bytes.MoveToVector();
			};
		Delegate<Unique_ptr<UCode::Texture>, Vector<Byte>&&> Func2 = [](Vector<Byte>&& Bits) mutable
			{

				auto teptex = new UCode::Texture();
				teptex->SetTexture(UC::PngDataSpan(BytesView::Make(Bits.data(), Bits.size())));


				return Unique_ptr<UCode::Texture>(teptex);
			};
		Delegate<Unique_ptr<UCode::Texture>, Unique_ptr<UCode::Texture>> Func3 = [](Unique_ptr<UCode::Texture>&& Tex) mutable
			{
				Tex->InitTexture();

				return std::move(Tex);
			};
		Delegate<void, Unique_ptr<UC::Texture>&&> Func4 = [this](Unique_ptr<UC::Texture>&& text)
			{
				this->_Thumbnail = std::move(*text.get());

				auto tex = &_Thumbnail.value();

				_ThumbnailSprite = UC::Sprite(tex, 0, 0, tex->Get_Width(), tex->Get_Height());

				tex->FreeFromCPU();
			};

		threads->AddTask_t(UCode::TaskType::File_Input, std::move(Func), {})
			.ContinueOnThread<Unique_ptr<UCode::Texture>>(UCode::TaskType::DataProcessing, std::move(Func2))
			.ContinueOnThread<Unique_ptr<UCode::Texture>>(UCode::TaskType::Rendering, std::move(Func3))
			.OnCompletedOnThread(Func4, UC::TaskType::Main).Start();
	}
}
void EntityAssetFile::Liveing::SaveFile(const UEditorAssetFileSaveFileContext& Context)
{

}
bool EntityAssetFile::Liveing::DrawButtion(const UEditorAssetDrawButtionContext& Item)
{
	UCode::Sprite* thumbnail = nullptr;

	if (hascalledloadthumnail == false)
	{
		LoadThumbnail();
		hascalledloadthumnail = true;
	}

	if (_ThumbnailSprite.has_value())
	{
		thumbnail = &_ThumbnailSprite.value();
	}
	else
	{
		thumbnail = AppFiles::GetSprite(AppFiles::sprite::RawEntityData);
	}

	auto predrawpos = ImGui::GetCursorPos();

	if (_Thumbnail.has_value())
	{
		auto imagescale = 3;
		auto minimagesize = Item.ButtionSize / imagescale;
		auto predrawasucode = Vec2(predrawpos.x, predrawpos.y);

		auto newcursorpos = predrawasucode;
		newcursorpos += {Item.ButtionSize.X - (minimagesize.X / 2), 0};

		ImGui::SetCursorPos(ImVec2(newcursorpos.X, newcursorpos.Y));

		ImGuIHelper::Image(AppFiles::sprite::RawEntityData, *(ImVec2*)&minimagesize);

		ImGui::SetCursorPos(predrawpos);
	}

	bool r = ImGuIHelper::ImageButton(Item.ObjectPtr, thumbnail, *(ImVec2*)&Item.ButtionSize);


	if (ImGui::BeginDragDropSource())
	{
		static UID tepAssetPath;
		tepAssetPath = _asset._Base._UID;
		auto p = &tepAssetPath;
		bool OnDropable = ImGui::SetDragDropPayload(DragAndDropType_RawEntity2dPathType, &p, sizeof(UID*));

		auto& g = *GImGui;
		auto ImageHight = ImGui::GetFrameHeight();


		String EntityName = FileFullPath.filename().replace_extension().generic_string();

		Vec2 imagesize = { ImageHight,ImageHight };
		if (OnDropable)
		{
			String Text = "Drop " + EntityName + "Here?";

			ImGuIHelper::Text(Text);
			ImGui::SameLine();
			ImGuIHelper::Image(thumbnail, *(ImVec2*)&imagesize);
		}
		else
		{
			ImGuIHelper::Text(EntityName);
			ImGui::SameLine();
			ImGuIHelper::Image(thumbnail, *(ImVec2*)&imagesize);
		}

		ImGui::EndDragDropSource();
	}

	

	

	return r;
}
USerializerType EntityAssetFile::Liveing::GetSerializerTypeForAsset()
{
	USerializerType type;
	{
		//type is wrong but its not that important if is wrong 
		// it should be based on _assetasbyte SerializeType

		auto runtime = EditorAppCompoent::GetCurrentEditorAppCompoent()->Get_RunTimeProjectData();
		type = runtime->Get_ProjData()._SerializeType;
	}
	return type;
}
bool EntityAssetFile::Liveing::WasEntityUpdated(UC::Entity* e)
{
	bool wasupdated = false;
	USerializerType type = GetSerializerTypeForAsset();

	UC::Scene2dData::Entity_Data data;
	UC::Scene2dData::SaveEntityData(e, data, type);
	auto tep = GetAssetAsBytes(data);

	if (tep.size() == _assetasbytes.size())
	{
		for (size_t i = 0; i < tep.size(); i++)
		{
			if (tep[i] != _assetasbytes[i])
			{
				int a = 0;
			}

		}
		bool issame = memcmp(tep.data(), _assetasbytes.data(), _assetasbytes.size()) == 0;
		return !issame;
	}
	return true;
}
void EntityAssetFile::Liveing::SaveEntity(UC::Entity* e)
{
	auto runtime = EditorAppCompoent::GetCurrentEditorAppCompoent()->Get_RunTimeProjectData();
	auto type = runtime->Get_ProjData()._SerializeType;

	UC::RawEntityData data;
	UC::Scene2dData::SaveEntityData(e, data._Data, type);

	data._UID = _asset._Base._UID;

	if (!UC::RawEntityData::WriteToFile(FileFullPath, data, type))
	{
		UCodeGEError("Unable to SaveFile " << FileHelper::ToRelativePath(runtime->GetAssetsDir(), FileFullPath));
	}

}
void EntityAssetFile::Liveing::DrawInspect(const UEditorAssetDrawInspectContext& Item)
{
	auto entity = GetAsEntity();
	{
		auto imageh = ImGui::GetFrameHeight();
		ImGuIHelper::Image(AppFiles::sprite::RawEntityData, { imageh,imageh });
		ImGui::SameLine();

		ImGuIHelper::ItemLabel("Name", ImGuIHelper::ItemLabelFlag::Left);

		ImGui::PushItemWidth(ImGui::CalcItemWidth() - (ImGuIHelper::CheckBoxSizeWithPadding().x));
		ImGuIHelper::InputText(entity->NativeName());
		ImGui::PopItemWidth();

		ImGui::SameLine();


		bool V = entity->GetActive();
		ImGui::PushID(&V);
		ImGui::Checkbox("", &V);
		ImGui::PopID();
		entity->SetActive(V);
	}

	ImGuIHelper::ItemLabel("Prefab", ImGuIHelper::ItemLabelFlag::Left);
	{
		auto buttioncount = 3;

		auto h = ImGui::GetFrameHeight();
		auto fieldwidthleft = ImGui::GetCurrentWindow()->Size.x - ImGui::GetCursorPosX();
		auto w = (fieldwidthleft / buttioncount) - ImGui::GetStyle().ItemSpacing.x;

		if (ImGui::Button("Open Prefab", { w,h }))
		{
			if (auto val = EditorAppCompoent::GetCurrentEditorAppCompoent()->Get_Window<GameEditorWindow>())
			{
				GameEditorWindow::PrefabModeData mod;
				mod.WasUpdated = [&](UC::Entity* e) -> bool
					{
						return WasEntityUpdated(e);
					};
				mod.OnSave = [&](UC::Entity* e)
					{
						return SaveEntity(e);
					};

				val->SetPrefabMode(entity->NativeManagedPtr(), std::move(mod));
			}
		}

		ImGui::SameLine();

		const char* makevarinatpopupid = "Make Variant Popup";

		auto wasentityupdated = WasEntityUpdated(entity);

		static String NewAssetName;
		ImGui::SetNextWindowSize({ 350,100 });
		if (ImGui::BeginPopup(makevarinatpopupid))
		{
			String str = "Make Variant of ";
			str += FileFullPath.filename().replace_extension().generic_string();

			ImGui::Text(str.c_str());
			ImGui::Separator();

			ImGuIHelper::InputText("FileName", NewAssetName);

			auto buttioncount = 2;
			auto h = ImGui::GetFrameHeight();
			auto fieldwidthleft = ImGui::GetCurrentWindow()->Size.x - ImGui::GetCursorPosX();
			auto w = (fieldwidthleft / buttioncount) - ImGui::GetStyle().ItemSpacing.x;

			if (ImGui::Button("Make", { w,h }))
			{
				auto newfilename = FileHelper::GetNewFileName(FileFullPath.parent_path() / Path(NewAssetName).native(), UC::RawEntityData::FileExtDot);

				auto runprj = EditorAppCompoent::GetCurrentEditorAppCompoent()->Get_RunTimeProjectData();
				auto outtype = runprj->Get_ProjData()._SerializeType;

				UC::Entity* e = GetTepScene()->NewEntity();
				UC::Entity::Destroy(e);

				auto thisassetid = _asset._Base._UID;

				auto placeholder = e->AddCompoent<UC::EntityPlaceHolder>();
				placeholder->_id = thisassetid;

				UC::Scene2dData::LoadEntity(e, _asset._Base._Data);

				UC::RawEntityData raw = UC::RawEntityData(runprj->GetNewUID(), e, outtype);
				if (!UC::RawEntityData::WriteToFile(newfilename, raw, outtype))
				{
					auto relpath = FileHelper::ToRelativePath(runprj->GetAssetsDir(), newfilename);
					UCodeGEError("Unable to Write to " << relpath);
				}

				ImGui::CloseCurrentPopup();
			}
			ImGui::SameLine();
			if (ImGui::Button("Close", { w,h }))
			{
				ImGui::CloseCurrentPopup();
			}

			ImGui::EndPopup();
		}

		ImGui::BeginDisabled(wasentityupdated);
		auto str = wasentityupdated ? "Save First" : "Make Variant";
		if (ImGui::Button(str, { w,h }))
		{
			NewAssetName = FileFullPath.filename().replace_extension().generic_string();
			ImGui::OpenPopup(makevarinatpopupid);
		}
		ImGui::EndDisabled();

		ImGui::SameLine();

		ImGui::BeginDisabled(!wasentityupdated);
		if (ImGui::Button("Save Prefab", { w,h }))
		{
			SaveEntity(entity);
		}
		ImGui::EndDisabled();
	}
	{

		//this is 2d GameEngine for now
		bool Is2D = true;
		auto Draw = *Item.Drawer;
		if (Is2D)
		{
			Vec2 Tep = entity->WorldPosition2D();
			Vec2 Tep1 = entity->WorldRotation2D();
			Vec2 Tep2 = entity->WorldScale2D();

			if (ImGuIHelper::Vec2Field("Position", Tep))
			{
				entity->WorldPosition(Tep);
			}

			if (ImGuIHelper::Vec2Field("Rotation", Tep1))
			{
				entity->WorldRotation(Tep1);
			}

			if (ImGuIHelper::Vec2Field("Scale", Tep2))
			{
				entity->WorldScale(Tep2);
			}
		}
		else
		{
			Vec3 Tep = entity->WorldPosition();
			Vec3 Tep1 = entity->WorldRotation();
			Vec3 Tep2 = entity->WorldScale();

			if (ImGuIHelper::Vec3Field("Position", Tep))
			{
				entity->WorldPosition(Tep);
			}

			if (ImGuIHelper::Vec3Field("Rotation", Tep1))
			{
				entity->WorldRotation(Tep2);
			}

			if (ImGuIHelper::Vec3Field("Scale", Tep2))
			{
				entity->WorldScale(Tep2);
			}
		}
	}
	{
		ImGui::Separator();

		bool isonedestroyed = false;
		for (auto& compent : entity->NativeCompoents())
		{
			if (compent->Get_CompoentTypeData() == &UC::EntityPlaceHolder::type_Data)
			{
				return;
			}
			if (compent->Get_IsDestroyed())
			{
				isonedestroyed = true;
			}
			Inspect_Compoent2d::Insp_(compent.get(), *Item.Drawer);
			ImGui::Separator();
		}
		ImGui::Separator();
		if (ImGui::BeginMenu("Add Compoent"))
		{
			Inspect_Compoent2d::ShowAddCompoenList(entity);
			ImGui::EndMenu();
		}

		if (isonedestroyed)
		{
			_entity.Get_Value()->NativeScene()->Get_RunTime()->DestroyNullScenes();
		}
	}

}
void EntityAssetFile::Liveing::FileUpdated()
{
	UC::RawEntityData _val;
	if (UC::RawEntityData::ReadFromFile(FileFullPath, _val))
	{
		hascalledloadthumnail = false;
		if (_entity.Has_Value())
		{
			auto e = _entity.Get_Value();
			for (auto& Copoent : e->NativeCompoents())
			{
				UC::Compoent::Destroy(Copoent.get());
			}
			for (auto& entity : e->NativeGetEntitys())
			{
				UC::Entity::Destroy(entity.get());

			}
			_entity.Get_Value()->NativeScene()->Get_RunTime()->DestroyNullScenes();

			UC::Editor_Only_CallAssetUpdatedPre(_asset._Base._UID);

			_asset._Base = std::move(_val);

			UC::Scene2dData::LoadEntity(e, _asset._Base._Data);

			UC::Editor_Only_CallAssetUpdated(_asset._Base._UID);
		}
	}
	else
	{
		auto assetdir = EditorAppCompoent::GetCurrentEditorAppCompoent()->Get_RunTimeProjectData()->GetAssetsDir();
		auto relpath = FileHelper::ToRelativePath(assetdir, FileFullPath);
		UCodeGEError("Unable to Read/Parse " << relpath);
	}
}
NullablePtr<UCode::Asset> EntityAssetFile::Liveing::LoadAsset(const LoadAssetContext& Item)
{
	return _asset.GetAsset();
}
EditorEnd

