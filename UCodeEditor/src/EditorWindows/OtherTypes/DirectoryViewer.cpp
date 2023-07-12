#include "DirectoryViewer.hpp"

#include "Helper/ImGuIHelper.hpp"
#include "ImGui.hpp"
#include "Imgui/imgui_internal.h"
#include "Helper/StringHelper.hpp"
#include "Helper/FileHelper.hpp"
EditorStart
namespace fs = std::filesystem;
DirectoryViewer::DirectoryViewer()
{
}

DirectoryViewer::~DirectoryViewer()
{
}

void DirectoryViewer::Draw()
{
	
	if (_ShowFindFiles) {
		ImGuIHelper::InputText("Find file", FindString);
	}
auto style = ImGui::GetStyleColorVec4(ImGuiCol_::ImGuiCol_WindowBg);
	ImGui::PushStyleColor(ImGuiCol_::ImGuiCol_FrameBg, style);
	DrawDir(LookingAtPath);
	
	ImGui::PopStyleColor();
}

void DirectoryViewer::DrawDir(const Path& Dir)
{
	fs::directory_options opt = fs::directory_options::skip_permission_denied;
	size_t Index =0;
	
	for (auto& Item : fs::directory_iterator(Dir, opt))
	{
		const auto& path = Item.path();
		const auto& FileName = Item.path().filename();
		if (Item.is_regular_file())
		{
			bool Show = false;	
			auto Ext = FileName.extension().generic_string();
			if (OnlyExt.size())
			{
			
				for (const auto& Item : OnlyExt)
				{
					if (Ext == Item)
					{
						Show = true;
						break;
					}
				}
			}
			else
			{
				Show = true;
			}


			auto PathStr = FileHelper::ToRelativePath(LookingAtPath, path).generic_string();
			if (Show && StringHelper::Fllter(FindString, PathStr))
			{
				ImGuIHelper::Image(AppFiles::GetSprite(FileHelper::GetFileType(Ext)), {20,20});
				ImGui::SameLine();
				if (ImGui::Selectable(PathStr.c_str(), PathSelected == PathStr))
				{
					if (OpenPathCallback)
					{
						OpenPathCallback(*this, CallBackPtr, path);
					}
				}
			}
		}
		else if (Item.is_directory())
		{
			String Tep = FileName.generic_string(); 
			auto Treeret = ImGuIHelper::TreeNode(Tep.c_str() + (uintptr_t)Index, Tep, AppFiles::sprite::Dir_folder_image);
			Index++;
			if (Treeret.NodeOpen)
			{
				DrawDir(path / ""); 
				ImGui::TreePop();
			}
			if (Treeret.TextUpdate)
			{
				//FileHelper::RenameFile(path, (path.parent_path() / Path(Tep)));
			}

		}
	}

	
}

EditorEnd

