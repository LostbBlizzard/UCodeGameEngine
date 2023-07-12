#include "UGlobalState.hpp"

#if UCode_Build_Windows_OS
#include <Windows.h>
#include <shlobj.h>
#endif // Windows

EditorStart
Path UGlobalState::GetGlobalDirectory()
{
    #if UCode_Build_Windows_OS

    WCHAR my_documents[MAX_PATH];
    HRESULT result = SHGetFolderPathW(0, CSIDL_PROFILE, 0, 0, my_documents);

    if (SUCCEEDED(result))
    {
        Path DocPath = my_documents;

        DocPath /= ".ucodeeditor";

        if (!std::filesystem::exists(DocPath)) {
            std::filesystem::create_directory(DocPath);
        }

        return DocPath;
    }
    else
    {
        return "";
    }

	#else
	UCODE_ENGINE_STATIC_ASSERT("GetGlobalDirectory Has no Body");
	#endif // Windows
}
Path UGlobalState::GetPluginDirectory()
{
    auto Path = GetGlobalDirectory() / "Plugin";

    if (!std::filesystem::exists(Path)) {
        std::filesystem::create_directory(Path);
    }
    return Path;
}
EditorEnd