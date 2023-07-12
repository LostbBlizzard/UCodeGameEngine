#include <iostream>
#include <fstream>
#include <filesystem>
#include <functional>

//Steps
//1.Build UCodeEditor
//2.Batch Build UCodeApp


namespace fs = std::filesystem;
fs::path::string_type ProjectDir = fs::current_path().parent_path().native();

// Recursively copies those files and folders from src to target which matches
// predicate, and overwrites existing files in target.
void CopyRecursive(const fs::path& src, const fs::path& target,
    const std::function<bool(fs::path)>& predicate /* or use template */) noexcept
{
    try
    {
        for (const auto& dirEntry : fs::recursive_directory_iterator(src))
        {
            const auto& p = dirEntry.path();
            if (predicate(p))
            {
                // Create path in target, if not existing.
                const auto relativeSrc = fs::relative(p, src);
                const auto targetParentPath = target / relativeSrc.parent_path();
                fs::create_directories(targetParentPath);

                // Copy to the targetParentPath which we just created.
                fs::copy(p, targetParentPath, fs::copy_options::overwrite_existing);
            }
        }
    }
    catch (std::exception& e)
    {
        std::cout << e.what() << std::endl;
    }
}

void CopyRecursive(const fs::path& src, const fs::path& target)
{
    return CopyRecursive(src, target,[](fs::path p) {return true; });
}
int main()
{
    std::function<bool(fs::path)> predicate = [](fs::path p) {return true; };

    CopyRecursive(ProjectDir + fs::path("/UCodeAPI/GameEngine").native(), ProjectDir + fs::path("/UCodeEditor/UFiles/source/UCodeGameEngine").native(),predicate);
    CopyRecursive(ProjectDir + fs::path("/UCodeAPI/GameEngineEditor").native(), ProjectDir + fs::path("/UCodeEditor/UFiles/source/UCodeGameEngineEditor").native(), predicate);
    
    

	std::cout << "Done" << std::endl;
	char Tep;
	std::cin >> Tep;
}