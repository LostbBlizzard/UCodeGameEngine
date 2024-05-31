use ULang;



//Exporter.hpp
UCodeGameEngineEditor:
  $Exporter trait export:
      uintptr _Handle = 0;
    



//OpenedProject.hpp
UCodeGameEngine:
  $AssetPath export extern "c":
   export extern dynamic |ID[this&] -> uint32;


  $OpenedProject export extern "c":
   export extern dynamic |GetProjectDirPath[] -> Path;




//Window.hpp
UCodeGameEngineEditor:
  $Window trait:
      uintptr _Handle = 0;
    

  $OverWriteTypeDraw trait;
    


