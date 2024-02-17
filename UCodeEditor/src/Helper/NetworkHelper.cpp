#include "NetworkHelper.hpp"
#include "HttpHelper.hpp"

EditorStart

Optional<Version> NetworkHelper::GetNewestVersion()
{
    auto versionstr = HttpHelper::DonloadString("https://github.com","/LostbBlizzard/UCodeGameEngine/releases/latest/download/version.txt");
    return Version::Parse(versionstr.value_or(""));
}

Optional<String> NetworkHelper::GetNewestChangeLog()
{
    return HttpHelper::DonloadString("https://github.com","/LostbBlizzard/UCodeGameEngine/releases/latest/download/changelog.md"); 
}

Optional<String> NetworkHelper::GetNewestUnixInstall()
{
    return HttpHelper::DonloadString("https://github.com","/LostbBlizzard/UCodeGameEngine/releases/latest/download/install.sh");  
}

Optional<Path> NetworkHelper::GetNewestUnixInstallPath()
{
    return HttpHelper::DonloadFile("https://github.com","/LostbBlizzard/UCodeGameEngine/releases/latest/download/install.sh");  
}

Optional<Path> NetworkHelper::GetNewestWindowInstall()
{
    return HttpHelper::DonloadFile("https://github.com", "/LostbBlizzard/UCodeGameEngine/releases/latest/download/UCodeGameEngineSetup.exe");
}

EditorEnd
