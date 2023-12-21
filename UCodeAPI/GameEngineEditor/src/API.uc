use ULang;



//OpenedProject.hpp
UCodeGameEngine:
  $AssetDir export extern "c":


  $AssetPath export extern "c":


  $OpenedProject export extern "c":
   export |GetProjectDirPath[] => Internal::OpenedProject__GetProjectDirPath0();
   export extern dynamic |AsAssetPath[imut Path& path, UCodeLangOutPartype] -> bool;
   export |AsAssetPath[imut Path& path, AssetPath] => Internal::OpenedProject__AsAssetPath1(path,AssetPath);
   export extern dynamic |AsAssetDir[imut Path& path, UCodeLangOutPartype] -> bool;
   export |AsAssetDir[imut Path& path, AssetDir] => Internal::OpenedProject__AsAssetDir0(path,AssetDir);


UCodeGameEngine::Internal:
  extern dynamic |OpenedProject__GetProjectDirPath0[] -> Path;
  extern dynamic |OpenedProject__AsAssetPath1[imut Path& path, AssetPath] -> bool;
  extern dynamic |OpenedProject__AsAssetDir0[imut Path& path, AssetDir] -> bool;

