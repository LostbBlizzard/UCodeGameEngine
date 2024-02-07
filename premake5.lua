require "vscode"

workspace "UCodeGameEngine"
   configurations { "Debug", "Release","Published" }
   platforms { "Win32", "Win64","linux32","linux64","macosx","Web","Android","IOS"}
   defines {"YAML_CPP_STATIC_DEFINE","GLEW_STATIC"}
   
   startproject "UCodeEditor"
   cppdialect "c++17"
   
   if os.host() == "windows" then
     if os.is64bit() then
       defaultplatform "Win64"
     else
       defaultplatform "Win32"
     end
   end

   if os.host() == "linux" then
      if os.is64bit() then
       defaultplatform "linux64"
      else
       defaultplatform "linux32"
      end
   end

   if os.host() == "macosx" then
    defaultplatform "macosx"
   end

   OutDirPath ="%{cfg.platform}/%{cfg.buildcfg}"
   UCPathExe = "%{wks.location}/Output/UCodeLangCl/" .. OutDirPath .. "/uclang"

   

   

   filter { "configurations:Debug" }
      defines { "DEBUG" }
      optimize "Debug"
      symbols "On"

   filter { "configurations:Release" }
      defines { "RELASE" }
      optimize "On"
      symbols "On"

   filter { "configurations:Published" }
      defines { "PUBLISHED" , "RELASE"}
      optimize "Speed"
      symbols "off"
    
   
    filter { "platforms:Win32" }
      system "Windows"
      architecture "x86"

    filter { "platforms:Win64" }
      system "Windows"
      architecture "x86_64"

    filter { "platforms:linux32" }
      system "linux"
      architecture "x86"
      toolset ("gcc")

    filter { "platforms:linux64" }
      system "Linux"
      architecture "x86_64"
      toolset ("gcc")

    filter { "platforms:macosx" }
      system "macosx"
      architecture "universal"

    filter { "platforms:Web" }
      system "Linux"
      architecture "x86"
      defines { "ZYAN_POSIX" }
      targetextension (".html")
      linkoptions { "-pthread"}
      buildoptions { "-pthread"} 


    filter { "platforms:Android" }
      system "android"
      architecture "ARM"
      androidapilevel (22)
      exceptionhandling ("On")
      rtti ("On")

    filter { "platforms:IOS" }
      system "ios"
      architecture "universal"




    filter {}


    if _ACTION == "vs2019" or _ACTION == "vs2022" then
      flags { "MultiProcessorCompile" }
    end 

function includeUCode(HasULangCompiler)


    filter {}  
     includedirs{
      "Dependencies",
      "Dependencies/glm",
      "Dependencies/GLFW/include",
      "Dependencies/GLFW/deps",
      "Dependencies/plog/include",
      "Dependencies/UCodeLang",
      "Dependencies/UCodeLang/UCodeLang",
      "Dependencies/yaml-cpp/include",
      "Dependencies/Imgui",
      "Dependencies/glslang",
      "Dependencies/box2d/include",
      "Dependencies/FileWatcher/include",
      "Dependencies/zip/src",
      "Dependencies/MinimalSocket/src/header",
      
      "UCode/src",
     }

    filter { "platforms:Win32" }
      system "Windows"
      architecture "x86"
    
    filter { "platforms:Win64" }
      system "Windows"
      architecture "x86_64"
    
    filter { "system:Windows" }
      defines {"_GLFW_WIN32"}
      files {
        "Dependencies/GLEW/GL/**.h", 
       }
      includedirs{
        "Dependencies/GLEW/%{prj.name}",
        }
      
    filter { "system:linux" }
      defines {"_GLFW_X11","GLEW_NO_GLU"}
        
    filter { "system:MacOS" }
      defines {"_GLFW_COCOA","GLEW_NO_GLU"}

    filter { "system:Android" }
     defines {"GLEW_NO_GLU"}

    filter { "not platforms:Web" }
        includedirs{"Dependencies/GLEW"}
    
    filter {}

    if not HasULangCompiler then
    defines {"UCodeLangNoCompiler"}
    end 
end
      
function linkUCode(HasULangCompiler,IsPubMode)    

    filter { "system:Windows" }
      links {"Ws2_32.lib","GlfwWin"}
      dependson {"GlfwWin"}
      includedirs{
       "Dependencies/GLFW",
      }
              
    filter { "system:Windows","architecture:x86"}
      links {"glew32s.lib","Opengl32.lib"}
          
    filter { "system:Windows","architecture:x86_64"}
      links {"glew64s.lib","Opengl32.lib"}
             
    filter { "platforms:linux64" }
      links {"GL","GLEW","glfw"}

    filter { "platforms:Web" }
      kind "ConsoleApp"   
      links {"glfw","GL"}
      linkoptions {"-s USE_PTHREADS=1","-sUSE_GLFW=3"}
     

    filter { "system:Windows" }
      defines {"_GLFW_WIN32"}
    
    filter { "system:linux" }
      defines {"_GLFW_X11","GLEW_NO_GLU"}
        
    filter { "system:MacOS" }
      defines {"_GLFW_COCOA","GLEW_NO_GLU"}

    filter {}
     libdirs { 
      "Output/Imgui/" .. OutDirPath,
      "Output/yaml-cpp/" .. OutDirPath,
      "Output/glm/" .. OutDirPath,
      "Output/stb_image/" .. OutDirPath,    
      "Output/stb_image_write/" .. OutDirPath,    
      "Dependencies/GLEW/Lib",
      "Output/GLFW/" .. OutDirPath,
      "Output/SPIRV-Cross/" .. OutDirPath, 
      "Output/box2d/" .. OutDirPath, 
      "Output/MinimalSocket/" .. OutDirPath, 
      "Output/UCode/" .. OutDirPath,
     }
     
     
     if IsPubMode then
     links {"UCodePub"}
     else
     links {"UCode"}
     end

     links {
      "Imgui",
      "yaml-cpp",
      "stb_image",
      "stb_image_write",
      "box2d",
      "MinimalSocket"
     }
     

     if HasULangCompiler then
      libdirs { "Output/UCodeLang/" .. OutDirPath}
      links { "UCodeLang" }
      dependson {"UCodeLang"}
     else
      libdirs { "Output/UCodeLangNoCompiler/" .. OutDirPath}
      links { "UCodeLangNoCompiler" }
      dependson {"UCodeLangNoCompiler"}
     end

     dependson {"glm", 
      "box2d","yaml-cpp","stb_image","stb_image_write","Imgui",
      "MinimalSocket",
     }
end
   
      
project "UCode"
   location "UCode"
   kind "StaticLib"
   language "C++"

   
   targetdir ("Output/%{prj.name}/" .. OutDirPath)
   objdir ("Output/int/%{prj.name}/" .. OutDirPath)
   
   
   files { 
     "%{prj.name}/src/**.c",
     "%{prj.name}/src/**.h",
     "%{prj.name}/src/**.cpp",
     "%{prj.name}/src/**.hpp", 
   }

   includedirs{"%{prj.name}/src"}
   
   includeUCode();

project "UCodePub"
   location "UCode"
   kind "StaticLib"
   language "C++"
   defines {"PublishMode"}
   
   targetdir ("Output/UCode/" .. OutDirPath)
   objdir ("Output/int/%{prj.name}/" .. OutDirPath)
   
   
   files { 
     "UCode/src/**.c",
     "UCode/src/**.h",
     "UCode/src/**.cpp",
     "UCode/src/**.hpp", 
   }

   includedirs{"UCode/src"}
   
   includeUCode();


project "UCodeApp"
   location "UCodeApp"
   kind "ConsoleApp"
   language "C++"
   
   targetdir ("Output/%{prj.name}/" .. OutDirPath)
   objdir ("Output/int/%{prj.name}/" .. OutDirPath)
   defines {"DebugMode"}
   
   dependson {"UCode"}

   files { 
     "%{prj.name}/src/**.c",
     "%{prj.name}/src/**.h",
     "%{prj.name}/src/**.cpp",
     "%{prj.name}/src/**.hpp", 
   }
   includedirs{"%{prj.name}/src"}

   includeUCode(false)

   linkUCode(false,false)
   
  
  
   buildmessage "Copying Output"

   filter { "system:Windows","architecture:x86_64" }
      postbuildcommands {
         "{MKDIR} %{wks.location}/UCodeEditor/UFiles/bin",
         "{COPYFILE} %{cfg.buildtarget.abspath} %{wks.location}/UCodeEditor/UFiles/bin/UCAppWinDebug86X64.exe"
      }
    
   filter { "system:Windows","architecture:x86" }
      postbuildcommands {
         "{MKDIR} %{wks.location}/UCodeEditor/UFiles/bin",
         "{COPYFILE} %{cfg.buildtarget.abspath} %{wks.location}/UCodeEditor/UFiles/bin/UCAppWinDebug86X32.exe"
      }
 
   filter { "platforms:linux","architecture:x86_64" }
      postbuildcommands {
        "{MKDIR} %{wks.location}/UCodeEditor/UFiles/bin", 
        "{COPYFILE} %{cfg.buildtarget.abspath} %{wks.location}/UCodeEditor/UFiles/bin/UCAppLinuxDebug86X64"
      }
    
   filter { "platforms:linux","architecture:x86" }
      postbuildcommands {
        "{MKDIR} %{wks.location}/UCodeEditor/UFiles/bin",
        "{COPYFILE} %{cfg.buildtarget.abspath} %{wks.location}/UCodeEditor/UFiles/bin/UCAppLinuxDebug86X32"
      }
   filter { "platforms:Web"}
      postbuildcommands {
        "{MKDIR} %{wks.location}/UCodeEditor/UFiles/bin",
        "{COPYFILE} %{cfg.buildtarget.abspath} %{wks.location}/UCodeEditor/UFiles/bin/UCApp32.wasm"
      }


   filter { "configurations:Published","system:Windows"}
    kind ("WindowedApp")
   
   filter { "configurations:Release","system:Windows"}
    kind ("WindowedApp")

project "UCodeAppPub"
   location "UCodeApp"
   kind "ConsoleApp"
   language "C++"
   
   targetdir ("Output/UCodeApp/" .. OutDirPath)
   objdir ("Output/int/UCodeAppPub/" .. OutDirPath)
   defines {"PublishMode"}
   
   dependson {"UCodePub"}

   files { 
     "UCodeApp/src/**.c",
     "UCodeApp/src/**.h",
     "UCodeApp/src/**.cpp",
     "UCodeApp/src/**.hpp", 
   }
   includedirs{"UCodeApp/src"}

   includeUCode(false)

   linkUCode(false,true)
   
  
  
   buildmessage "Copying Output"

   filter { "system:Windows","architecture:x86_64" }
      postbuildcommands {
         "{MKDIR} %{wks.location}/UCodeEditor/UFiles/bin",
         "{COPYFILE} %{cfg.buildtarget.abspath} %{wks.location}/UCodeEditor/UFiles/bin/UCAppWinPub86X64.exe"
      }
    
   filter { "system:Windows","architecture:x86" }
      postbuildcommands {
         "{MKDIR} %{wks.location}/UCodeEditor/UFiles/bin",
         "{COPYFILE} %{cfg.buildtarget.abspath} %{wks.location}/UCodeEditor/UFiles/bin/UCAppWinPub86X32.exe"
      }
 
   filter { "system:linux","architecture:x86_64" }
      postbuildcommands {
        "{MKDIR} %{wks.location}/UCodeEditor/UFiles/bin", 
        "{COPYFILE} %{cfg.buildtarget.abspath} %{wks.location}/UCodeEditor/UFiles/bin/UCAppLinuxPub86X64"
      }
    
   filter { "system:linux","architecture:x86" }
      postbuildcommands {
        "{MKDIR} %{wks.location}/UCodeEditor/UFiles/bin",
        "{COPYFILE} %{cfg.buildtarget.abspath} %{wks.location}/UCodeEditor/UFiles/bin/UCAppLinuxPub86X32"
      }
   
   filter { "configurations:Published","system:Windows"}
    kind ("WindowedApp")
   
   filter { "configurations:Release","system:Windows"}
    kind ("WindowedApp")


project "UCodeEditor"
   location "UCodeEditor"
   kind "ConsoleApp"
   language "C++"
   
   dependson {
   "UCodeApp",
   "UCodeAppPub",
   "FileWatcher",
   "glslang",
   "SPIRV-Cross",
   "UCodeGameEngine",
   "UCodeGameEngineEditor",
   }

   targetdir ("Output/%{prj.name}/" .. OutDirPath)
   objdir ("Output/int/%{prj.name}/" .. OutDirPath)
   
   files { 
     "%{prj.name}/src/**.c",
     "%{prj.name}/src/**.h",
     "%{prj.name}/src/**.cpp",
     "%{prj.name}/src/**.hpp", 
   }
   includedirs{"%{prj.name}/src"}
   removefiles{"%{prj.name}/src/OtherLibrarys/ImGuizmo/example/**"}
   
   links {
    "FileWatcher",
    "glslang",
    "SPIRV-Cross",
    "zip",
   }
   libdirs { 
      "Output/FileWatcher/" .. OutDirPath, 
      "Output/glslang/" .. OutDirPath, 
      "Output/SPIRV-Cross/" .. OutDirPath, 
      "Output/zip/" .. OutDirPath, 
   }
  
   includeUCode(true)

   linkUCode(true,false)
   

   UEditorPathExe = "%{wks.location}/Output/UCodeEditor/" .. OutDirPath .. "/UCodeEditor"

   

   buildmessage "Copying UFilesAPI"
   postbuildcommands 
   {
    "{COPYDIR} %{wks.location}/UCodeAPI/GameEngine %{prj.location}/UFiles/source/UCodeGameEngine",
    "{COPYDIR} %{wks.location}/UCodeAPI/GameEngineEditor %{prj.location}/UFiles/source/UCodeGameEngineEditor",
    "{COPYDIR} %{wks.location}/Dependencies/UCodeLang/UCodeAPI/StandardLibrary %{prj.location}/UFiles/source/StandardLibrary",
    "{COPYDIR} %{wks.location}/Dependencies/UCodeLang/UCodeAPI/NStandardLibrary %{prj.location}/UFiles/source/NStandardLibrary",

   }
   filter {"system:Windows"}
    removefiles{"%{prj.name}/src/OtherLibrarys/Nativefiledialog/nfd_gtk.c"}
   filter {"system:linux"}
    removefiles{"%{prj.name}/src/OtherLibrarys/Nativefiledialog/nfd_win.cpp"}
    linkoptions {"`pkg-config --libs gtk+-3.0`"}
    buildoptions {"`pkg-config --cflags gtk+-3.0`"}


   filter { "configurations:Published","system:Windows"}
    kind ("WindowedApp")
   
   filter { "configurations:Release","system:Windows"}
    kind ("WindowedApp")

   filter { "configurations:Published" }
      buildmessage "Packing UFilesDir"
      postbuildcommands 
      {
       UEditorPathExe.." pack %{prj.location}/UFiles %{cfg.targetdir}/UFiles.data"
      }

   filter { "configurations:Release" }
      buildmessage "Copying UFilesDir"
      postbuildcommands 
      {
       UEditorPathExe.." pack %{prj.location}/UFiles %{cfg.targetdir}/UFiles.data"
      }

project "UCodeGameEngineDoc"
   location "UCodeGameEngineDoc"
   kind "StaticLib"
   language "C++"

   targetdir ("Output/%{prj.name}/" .. OutDirPath)
   objdir ("Output/int/%{prj.name}/" .. OutDirPath)
   
   files { 
     "%{prj.name}/src/**.c",
     "%{prj.name}/src/**.h",
     "%{prj.name}/src/**.cpp",
     "%{prj.name}/src/**.hpp", 
   }
project "UCodeGameEngineWebsite"
   location "UCodeGameEngineWebsite"
   kind "StaticLib"
   language "C++"

   targetdir ("Output/%{prj.name}/" .. OutDirPath)
   objdir ("Output/int/%{prj.name}/" .. OutDirPath)
   
   files { 
     "%{prj.name}/src/**.c",
     "%{prj.name}/src/**.h",
     "%{prj.name}/src/**.cpp",
     "%{prj.name}/src/**.hpp", 
   }

group "UCodeAPIs"
 project "UCodeGameEngine"
  location "UCodeAPI/GameEngine"
  kind "StaticLib"
  language "C"


  targetdir ("Output/%{prj.name}/" .. OutDirPath)
  objdir ("Output/int/%{prj.name}/" .. OutDirPath)
  dependson { "UCodeLangCl" }
  files { 
  "%{prj.location}/src/**.uc",

  "%{prj.location}/src/**.c",
  "%{prj.location}/src/**.h",
  "%{prj.location}/src/**.cpp",
  "%{prj.location}/src/**.hpp", 

  "%{prj.location}/src/**.uc", 
  "%{prj.location}/ULangModule.ucm",
  "%{prj.location}/LICENSE.txt", 
  }

 project "UCodeGameEngineEditor"
  location "UCodeAPI/GameEngineEditor"
  kind "StaticLib"
  language "C"
  
  targetdir ("Output/%{prj.name}/" .. OutDirPath)
  objdir ("Output/int/%{prj.name}/" .. OutDirPath)

  dependson { "UCodeLangCl" }
  files { 
  "%{prj.location}/src/**.uc",

  "%{prj.location}/src/**.c",
  "%{prj.location}/src/**.h",
  "%{prj.location}/src/**.cpp",
  "%{prj.location}/src/**.hpp", 

  "%{prj.location}/src/**.uc", 
  "%{prj.location}/ULangModule.ucm",
  "%{prj.location}/LICENSE.txt", 
  }
  
  postbuildcommands {
   --"uclang build %{prj.location}ULangModule.ucm"
  }
group "Dependencies"

  project "Imgui"
    location "Dependencies/%{prj.name}"
    kind "StaticLib"
    language "C++"


    targetdir ("Output/%{prj.name}/" .. OutDirPath)
    objdir ("Output/int/%{prj.name}/" .. OutDirPath)

    files {
      "Dependencies/%{prj.name}/*.hpp",
      "Dependencies/%{prj.name}/*.cpp",
      "Dependencies/%{prj.name}/*.h", 
      "Dependencies/%{prj.name}/*.c",
      "Dependencies/%{prj.name}/misc/cpp/*.cpp",
      "Dependencies/%{prj.name}/misc/cpp/*.h",

      "Dependencies/%{prj.name}/backends/imgui_impl_opengl3.cpp",
      "Dependencies/%{prj.name}/backends/imgui_impl_opengl3.h",
      "Dependencies/%{prj.name}/backends/imgui_impl_glfw.h",
      "Dependencies/%{prj.name}/backends/imgui_impl_glfw.cpp",
      "Dependencies/%{prj.name}/backends/imgui_impl_opengl3_loader",
    }

    includedirs{
      "Dependencies",
      "Dependencies/Imgui",
      "Dependencies/GLFW/include",
      "Dependencies/GLFW/deps",
    }
    filter {  "system:android" }
      defines {"GLFW_INCLUDE_ES32"}

  project "yaml-cpp"
    location "Dependencies/%{prj.name}"
    kind "StaticLib"
    language "C++" 


    targetdir ("Output/%{prj.name}/" .. OutDirPath)
    objdir ("Output/int/%{prj.name}/" .. OutDirPath)

    files {
      "Dependencies/%{prj.name}/src/**.hpp",
      "Dependencies/%{prj.name}/src/**.cpp",
      "Dependencies/%{prj.name}/src/**.h", 
      "Dependencies/%{prj.name}/src/**.c",
    }

    includedirs{
      "Dependencies/yaml-cpp/include",
      "Dependencies/%{prj.name}",
    }

  project "glm"
    location "Dependencies/glm"
    kind "StaticLib"
    language "C++" 


    targetdir ("Output/%{prj.name}/" .. OutDirPath)
    objdir ("Output/int/%{prj.name}/" .. OutDirPath)

    files {
      "Dependencies/%{prj.name}/glm/**.hpp",
      "Dependencies/%{prj.name}/glm/**.cpp",
      "Dependencies/%{prj.name}/glm/**.h", 
      "Dependencies/%{prj.name}/glm/**.c",
    }

    includedirs{
      "Dependencies/%{prj.name}",
    }

  project "stb_image"
    location "Dependencies/%{prj.name}"
    kind "StaticLib"
    language "C++" 


    targetdir ("Output/%{prj.name}/" .. OutDirPath)
    objdir ("Output/int/%{prj.name}/" .. OutDirPath)

    files {
      "Dependencies/%{prj.name}/**.hpp",
      "Dependencies/%{prj.name}/**.cpp",
      "Dependencies/%{prj.name}/**.h", 
      "Dependencies/%{prj.name}/**.c",
    }

  project "stb_image_write"
    location "Dependencies/%{prj.name}"
    kind "StaticLib"
    language "C++" 


    targetdir ("Output/%{prj.name}/" .. OutDirPath)
    objdir ("Output/int/%{prj.name}/" .. OutDirPath)

    files {
      "Dependencies/%{prj.name}/**.hpp",
      "Dependencies/%{prj.name}/**.cpp",
      "Dependencies/%{prj.name}/**.h", 
      "Dependencies/%{prj.name}/**.c",
    }
  
 
  project "UCodeLang"
    location "Dependencies/%{prj.name}"
    kind "StaticLib"
    language "C++" 

    targetdir ("Output/%{prj.name}/" .. OutDirPath)
    objdir ("Output/int/%{prj.name}/" .. OutDirPath)

    files {
      "Dependencies/%{prj.name}/UCodeLang/**.hpp",
      "Dependencies/%{prj.name}/UCodeLang/**.cpp",
      "Dependencies/%{prj.name}/UCodeLang/**.h", 
      "Dependencies/%{prj.name}/UCodeLang/**.c",
    }

    includedirs{
      "Dependencies/%{prj.name}",
      "Dependencies/%{prj.name}/UCodeLang",
      "Dependencies/%{prj.name}/UCodeLang/Dependencies/zycore/include",
      "Dependencies/%{prj.name}/UCodeLang/Dependencies/zydis/include",
      "Dependencies/%{prj.name}/UCodeLang/Dependencies/zydis/src",
      }
    removefiles{
     "Dependencies/%{prj.name}/UCodeLang/Dependencies/zydis/**.c",
     "Dependencies/%{prj.name}/UCodeLang/Dependencies/zycore/**.c",

     "Dependencies/%{prj.name}/UCodeLang/Dependencies/zydis/**.cpp",
     "Dependencies/%{prj.name}/UCodeLang/Dependencies/zycore/**.cpp",

     "Dependencies/%{prj.name}/UCodeLang/Dependencies/zydis/**.h",
     "Dependencies/%{prj.name}/UCodeLang/Dependencies/zycore/**.h",
     }
    files { 
    "Dependencies/%{prj.name}/UCodeLang/Dependencies/zydis/src/**.c",
    "Dependencies/%{prj.name}/UCodeLang/Dependencies/zycore/src/**.c",

    "Dependencies/%{prj.name}/UCodeLang/Dependencies/zydis/src/**.inc",
    "Dependencies/%{prj.name}/UCodeLang/Dependencies/zycore/src/**.inc",

    "Dependencies/%{prj.name}/UCodeLang/Dependencies/zydis/include/**.h",
    "Dependencies/%{prj.name}/UCodeLang/Dependencies/zycore/include/**.h",
    }
  project "UCodeLangNoCompiler"
    location "Dependencies/UCodeLang"
    kind "StaticLib"
    language "C++" 

    defines {"UCodeLangNoCompiler"}

    targetdir ("Output/%{prj.name}/" .. OutDirPath)
    objdir ("Output/int/%{prj.name}/" .. OutDirPath)

    files {
      "Dependencies/UCodeLang/UCodeLang/**.hpp",
      "Dependencies/UCodeLang/UCodeLang/**.cpp",
      "Dependencies/UCodeLang/UCodeLang/**.h", 
      "Dependencies/UCodeLang/UCodeLang/**.c",
    }

    includedirs{
      "Dependencies/UCodeLang",
      "Dependencies/UCodeLang/UCodeLang",
      "Dependencies/UCodeLang/UCodeLang/Dependencies/zycore/include",
      "Dependencies/UCodeLang/UCodeLang/Dependencies/zydis/include",
      "Dependencies/UCodeLang/UCodeLang/Dependencies/zydis/src",
      }
    removefiles{
     "Dependencies/UCodeLang/UCodeLang/Dependencies/zydis/**.c",
     "Dependencies/UCodeLang/UCodeLang/Dependencies/zycore/**.c",

     "Dependencies/UCodeLang/Dependencies/zydis/**.cpp",
     "Dependencies/UCodeLang/UCodeLang/Dependencies/zycore/**.cpp",

     "Dependencies/UCodeLang/UCodeLang/Dependencies/zydis/**.h",
     "Dependencies/UCodeLang/UCodeLang/Dependencies/zycore/**.h",
     }
    files { 
    "Dependencies/UCodeLang/UCodeLang/Dependencies/zydis/src/**.c",
    "Dependencies/UCodeLang/UCodeLang/Dependencies/zycore/src/**.c",

    "Dependencies/UCodeLang/UCodeLang/Dependencies/zydis/src/**.inc",
    "Dependencies/UCodeLang/UCodeLang/Dependencies/zycore/src/**.inc",

    "Dependencies/UCodeLang/UCodeLang/Dependencies/zydis/include/**.h",
    "Dependencies/UCodeLang/UCodeLang/Dependencies/zycore/include/**.h",
    }

  project "UCodeLangCl"
    location "Dependencies/UCodeLang"
    kind "ConsoleApp"
    language "C++" 
    
    targetname ("uclang")

    targetdir ("Output/%{prj.name}/" .. OutDirPath)
    objdir ("Output/int/%{prj.name}/" .. OutDirPath)



    dependson {
       "UCodeLang",
     }

    libdirs { 
    "Output/UCodeLang/" .. OutDirPath,
    }
    
    links {
    "UCodeLang",
     }

    files {
      "Dependencies/UCodeLang/UCodelangCL/src/**.hpp",
      "Dependencies/UCodeLang/UCodelangCL/src/**.cpp",
      "Dependencies/UCodeLang/UCodelangCL/src/**.h", 
      "Dependencies/UCodeLang/UCodelangCL/src/**.c",
    }

    includedirs{
      "Dependencies/UCodeLang/UCodeLang",
      "Dependencies/UCodeLang/UCodelangCL",
      }
      
   filter { "configurations:Published","system:Windows"}
    kind ("WindowedApp")
   
   filter { "configurations:Release","system:Windows"}
    kind ("WindowedApp")



  project "glslang"
    location "Dependencies/%{prj.name}"
    kind "StaticLib"
    language "C++" 

    targetdir ("Output/%{prj.name}/" .. OutDirPath)
    objdir ("Output/int/%{prj.name}/" .. OutDirPath)

    filter { "system:Windows" }
      files {
       "Dependencies/%{prj.name}/glslang/OSDependent/Windows/**.cpp",
       "Dependencies/%{prj.name}/glslang/OSDependent/Windows/**.h", 
      } 
      removefiles {
      "Dependencies/%{prj.name}/glslang/OSDependent/Windows/main.cpp"
      }
    filter { "system:linux" }
     files {
       "Dependencies/%{prj.name}/glslang/OSDependent/Unix/**.cpp",
       "Dependencies/%{prj.name}/glslang/OSDependent/Unix/**.h", 
     }

    filter { }
    
    files {
      "Dependencies/%{prj.name}/build_info.h",
      "Dependencies/%{prj.name}/glslang/Include/**.cpp",
      "Dependencies/%{prj.name}/glslang/Include/**.h", 

      "Dependencies/%{prj.name}/glslang/Public/**.cpp",
      "Dependencies/%{prj.name}/glslang/Public/**.h", 

      "Dependencies/%{prj.name}/glslang/MachineIndependent/**.cpp",
      "Dependencies/%{prj.name}/glslang/MachineIndependent/**.h", 

      "Dependencies/%{prj.name}/glslang/GenericCodeGen/**.cpp",
      "Dependencies/%{prj.name}/glslang/GenericCodeGen/**.h", 

      "Dependencies/%{prj.name}/SPIRV/**.h",
      "Dependencies/%{prj.name}/SPIRV/**.cpp",

      "Dependencies/%{prj.name}/OGLCompilersDLL/**.h",
      "Dependencies/%{prj.name}/OGLCompilersDLL/**.cpp",
    }
   

    includedirs{
      "Dependencies/%{prj.name}",
      "Dependencies",
    }
  
  project "SPIRV-Cross"
    location "Dependencies/%{prj.name}"
    kind "StaticLib"
    language "C++" 

    targetdir ("Output/%{prj.name}/" .. OutDirPath)
    objdir ("Output/int/%{prj.name}/" .. OutDirPath)

    files {
      "Dependencies/%{prj.name}/*.hpp",
      "Dependencies/%{prj.name}/*.cpp",
      "Dependencies/%{prj.name}/*.h", 
      "Dependencies/%{prj.name}/*.c",
    }

    includedirs{
      "Dependencies/%{prj.name}",
    }
    removefiles 
    {
      "Dependencies/%{prj.name}/main.cpp"
    }
  
  project "plog"
    location "Dependencies/%{prj.name}"
    kind "StaticLib"
    language "C++" 

    targetdir ("Output/%{prj.name}/" .. OutDirPath)
    objdir ("Output/int/%{prj.name}/" .. OutDirPath)

    files {
      "Dependencies/%{prj.name}/include/**.hpp",
      "Dependencies/%{prj.name}/include/**.cpp",
      "Dependencies/%{prj.name}/include/**.h", 
      "Dependencies/%{prj.name}/include/**.c",
    }

    includedirs{
      "Dependencies/%{prj.name}",
    }
  
  project "box2d"
    location "Dependencies/%{prj.name}"
    kind "StaticLib"
    language "C++" 

    targetdir ("Output/%{prj.name}/" .. OutDirPath)
    objdir ("Output/int/%{prj.name}/" .. OutDirPath)

    files {
      "Dependencies/%{prj.name}/src/**.h",
      "Dependencies/%{prj.name}/src/**.cpp",
      "Dependencies/%{prj.name}/include/**.h",
    }

    includedirs{
      "Dependencies/%{prj.name}/include",
    }
  
  project "FileWatcher"
    location "Dependencies/%{prj.name}"
    kind "StaticLib"
    language "C++" 

    targetdir ("Output/%{prj.name}/" .. OutDirPath)
    objdir ("Output/int/%{prj.name}/" .. OutDirPath)

    files {
      "Dependencies/%{prj.name}/source/**.h",
      "Dependencies/%{prj.name}/source/**.cpp",
      "Dependencies/%{prj.name}/include/**.h",
    }

    includedirs{
      "Dependencies/%{prj.name}/include",
    }
  
  project "zip"
    location "Dependencies/%{prj.name}"
    kind "StaticLib"
    language "C++" 

    targetdir ("Output/%{prj.name}/" .. OutDirPath)
    objdir ("Output/int/%{prj.name}/" .. OutDirPath)

    files {
      "Dependencies/%{prj.name}/src/**.h",
      "Dependencies/%{prj.name}/src/**.c",
    }
  
  project "MinimalSocket"
    location "Dependencies/%{prj.name}"
    kind "StaticLib"
    language "C++" 

    includedirs {"Dependencies/MinimalSocket/src/header"}

    targetdir ("Output/%{prj.name}/" .. OutDirPath)
    objdir ("Output/int/%{prj.name}/" .. OutDirPath)

    files {
      "Dependencies/%{prj.name}/src/**.h",
      "Dependencies/%{prj.name}/src/**.c",
      "Dependencies/%{prj.name}/src/**.cpp",
      "Dependencies/%{prj.name}/src/**.hpp",
    }

  project "GlfwWin"
    location "Dependencies/GLFW"
    kind "StaticLib"
    language "C++" 

    targetdir ("Output/GLFW/" .. OutDirPath)
    objdir ("Output/int/GLFW/" .. OutDirPath)

    filter { "system:Windows"}
      defines {"_GLFW_WIN32"}
      files {
       "Dependencies/GLFW/include/**.h", 
       "Dependencies/GLFW/src/**.c",
      }

      includedirs{
       "Dependencies/GLFW",
      }

function executeorexit(str)
 exit = os.execute(str)

 if exit == nil then
  os.exit(1)
 end 

 if not exit == true then
  os.exit(1)
 end 

end 
--install
newaction {
    trigger = "install",
    description = "installs compiler tool/librarys",
    execute = function ()
        print("----installing tools for " .. os.target())
        
        if os.istarget("linux") then

          os.execute("sudo apt-get update")

          print("----downloading libx11 Packages")
          os.execute("sudo apt-get install libxcursor-dev libxrandr-dev libxinerama-dev libxi-dev")

          print("----downloading opengl Packages")
          executeorexit("sudo apt install mesa-common-dev")

          print("----downloading glew Packages")
          executeorexit("sudo apt-get install libglew-dev")

          print("----downloading glfw Packages")
          os.execute("sudo apt-get install libglfw3")
          os.execute("sudo apt-get install libglfw3-dev")

          print("----downloading gtk Packages")
          os.execute("sudo apt-get install libgtk-3-dev")
          
          print("----installing tools completed");
        end

        if os.istarget("windows") then

        end
        
        if os.istarget("macosx") then
           
          print("----downloading glfw Packages")
         
          executeorexit("brew install glfw")
          
          print("----installing tools completed");
 
        end
    end
}
newaction {
    trigger = "installwasm",
    description = "installs compiler tool/librarys for wasm",
    execute = function ()
        print("----installing emscripten for " .. os.target())
        
        if os.istarget("linux") then
          os.execute("git clone https://github.com/emscripten-core/emsdk.git")

          os.execute("cd emsdk")

          os.execute("git pull")

          os.execute("./emsdk install latest")

          os.execute("./emsdk activate latest")

          os.execute("source ./emsdk_env.sh")
        end

        if os.istarget("windows") then

        end
        
        if os.istarget("macosx") then

        end
    end
}

newaction {
    trigger = "installandroidsdk",
    description = "installs compiler tool/librarys for android",
    execute = function ()
        print("----installing android sdk tools for " .. os.target())
        
        if os.istarget("linux") then
         --executeorexit("sudo apt update && sudo apt install android-sdk")
        end

        if os.istarget("windows") then

        end
        
        if os.istarget("macosx") then

        end
    end
}

newaction {
    trigger = "installinno",
    description = "downloads inno setup and puts in output",
    execute = function ()
        print("----installing inno setup tools for " .. os.target())
        
        executeorexit("git clone https://github.com/LostbBlizzard/installed-inno Dependencies/bin/inno")
        
    end
}
--build
newaction {
    trigger = "buildeditor",
    description = "installs compiler tool/librarys for wasm",
    execute = function ()
        
        if os.istarget("linux") then
          executeorexit("make UCodeEditor -j$(nproc)")
        end

        if os.istarget("windows") then
          executeorexit("msbuild UCodeGameEngine.sln /t:Build /p:Configuration=Debug /p:Platform=Win64 -maxcpucount")
        end
        
        if os.istarget("macosx") then
          executeorexit("make UCodeEditor -j$(nproc)")
        end
    end
}
newaction {
    trigger = "buildwasm",
    description = "installs compiler tool/librarys for wasm",
    execute = function ()
       
        executeorexit("emmake make UCodeApp config=published_web -j$(nproc)")
        
    end
}
newaction {
    trigger = "buildwasmpub",
    description = "installs compiler tool/librarys for wasm",
    execute = function ()
       
        executeorexit("emmake make UCodeAppPub config=published_web -j$(nproc)")
        
    end
}

newaction {
    trigger = "buildeditor32",
    description = "installs compiler tool/librarys for wasm",
    execute = function ()
        
        if os.istarget("linux") then
          executeorexit("make UCodeEditor -j$(nproc)")
        end

        if os.istarget("windows") then
          executeorexit("msbuild UCodeGameEngine.sln /t:Build /p:Configuration=Debug /p:Platform=Win32 -maxcpucount")
        end
        
        if os.istarget("macosx") then
          executeorexit("make UCodeEditor -j$(nproc)")
        end
    end
}


newaction {
    trigger = "buildeditorpublished",
    description = "build the published editor for 64bit",
    execute = function ()
        
        if os.istarget("linux") then
          executeorexit("make UCodeEditor config=published_linux64 -j$(nproc)")
        end

        if os.istarget("windows") then
          executeorexit("msbuild UCodeGameEngine.sln /t:Build /p:Configuration=Published /p:Platform=Win64 -maxcpucount")
        end
        
        if os.istarget("macosx") then
          executeorexit("make UCodeEditor -j$(nproc)")
        end
    end
}
newaction {
    trigger = "buildeditor32published",
    description = "build the published editor for 32bit",
    execute = function ()
        
        if os.istarget("linux") then
          executeorexit("make UCodeEditor config=published_linux32 -j$(nproc)")
        end

        if os.istarget("windows") then
          executeorexit("msbuild UCodeGameEngine.sln /t:Build /p:Configuration=Published /p:Platform=Win32 -maxcpucount")
        end
        
        if os.istarget("macosx") then

        end
    end
}
--test
newaction {
    trigger = "test",
    description = "run tests",
    execute = function ()
        print("----runing tests for " .. os.target())
        
        if os.istarget("linux") then

        end

        if os.istarget("windows") then

        end
        
        if os.istarget("macosx") then

        end
    end
}
newaction {
    trigger = "test32",
    description = "run 32bit tests",
    execute = function ()
        print("----runing tests for " .. os.target())
        
        if os.istarget("linux") then

        end

        if os.istarget("windows") then

        end
        
        if os.istarget("macosx") then

        end
    end
}
--publish
newaction {
    trigger = "build_editor_publish",
    description = "installs compiler tool/librarys for wasm",
    execute = function ()
        print("----installing emscripten for " .. os.target())
        
        if os.istarget("linux") then
          executeorexit("make config=publish_linux64 -j$(nproc)")
        end

        if os.istarget("windows") then
          executeorexit("msbuild UCodeGameEngine.sln /t:Publish /p:Configuration=Debug /p:Platform=Win64 -maxcpucount")
        end
        
        if os.istarget("macosx") then

        end
    end
}
newaction {
    trigger = "package_editor",
    description = "installs compiler tool/librarys for wasm",
    execute = function ()
        
        if os.istarget("linux") then
          executeorexit("./Output/UCodeEditor/Linux64/linux64/Published/UCodeEditor pack ./UCodeEditor/UFiles ./Output/UFiles.data")
        end

        if os.istarget("windows") then
          executeorexit("Output\\UCodeEditor\\Win64\\Published\\UCodeEditor.exe pack UCodeEditor\\UFiles Output/UFiles.data")
        end
        
        if os.istarget("macosx") then
           executeorexit("./Output/UCodeEditor/macosx/linux64/Published/UCodeEditor pack ./UCodeEditor/UFiles ./Output/UFiles.data")
        end
    end
}

newaction {
    trigger = "buildinstaller",
    description = "build the installer",
    execute = function ()
        
        if os.istarget("linux") then
        end

        if os.istarget("windows") then
          executeorexit("Output\\UCodeEditor\\Win64\\Published\\UCodeEditor.exe pack UCodeEditor\\UFiles Output\\UCodeEditor\\Win64\\Published\\UFiles.data")
          executeorexit("Dependencies\\bin\\inno\\ISCC.exe install.iss")
        end
        
        if os.istarget("macosx") then
        end
    end
}

function readAll(file)
    local f = assert(io.open(file, "rb"))
    local content = f:read("*all")
    f:close()
    return content
end
function mysplit(inputstr, sep)
        if sep == nil then
                sep = "%s"
        end
        local t={}
        for str in string.gmatch(inputstr, "([^"..sep.."]+)") do
                table.insert(t, str)
        end
        return t
end

function replacetextinfile(file_path, stringtomatch, replacement)
    local file = io.open(file_path, "r")  
    if not file then
        print("Error: File not found or unable to open.")
        return
    end

    local content = file:read("*a")  
    file:close()  
    
    local modified_content = content:gsub(stringtomatch, replacement)

    local new_file = io.open(file_path, "w")
    if not new_file then
        print("Error: Unable to write to file.")
        return
    end
    new_file:write(modified_content)
    new_file:close()
end

function isdigit(char)

 if char == '0' or char == '1' or char == '2'
    or  char == '3' or char == '4' or char == '5'
    or char == '6' or char == '6' or char == '7' or char == '8'
    or char == '8' or char == '9' 
 then
    return true
 else
    return false
 end 

end

function keeponlyfirstnumberpart(str)
   local r = ""

   for i=1, string.len(str) do 
    
    local cha = str:sub(i,i)--lua cant index strings? why

    if not isdigit(cha) then
        break
    end

    r = r .. cha
   end

   return r
end 

newaction {
    trigger = "updateversion",
    description = "updates version numbers",
    execute = function ()
        local major = "0"
        local minor = "0"
        local patch = "0"

        local veriontext = readAll("version.txt")
        local verionspit = mysplit(veriontext,".");

        major = verionspit[1]

        major = verionspit[2]

        patch = keeponlyfirstnumberpart(verionspit[3])

        print("updating source files to " .. major .. "." .. minor .. "." .. patch)

        --UDefs.hpp
        replacetextinfile("UCode/src/UCodeRunTime/UDefs.hpp","#define UCodeGEMajor 0","#define UCodeGEMajor " .. major)
        replacetextinfile("UCode/src/UCodeRunTime/UDefs.hpp","#define UCodeGEMinor 0","#define UCodeGEMinor " .. minor)
        replacetextinfile("UCode/src/UCodeRunTime/UDefs.hpp","#define UCodeGEPatch 1","#define UCodeGEPatch " .. patch)

        --install.sh
        replacetextinfile("./install.sh","#VersionMajor#",major)
        replacetextinfile("./install.sh","#VersionMinor#",minor)
        replacetextinfile("./install.sh","#VersionPatch#",patch)

        --install.iss
        replacetextinfile("./install.iss","#VersionMajor#",major)
        replacetextinfile("./install.iss","#VersionMinor#",minor)
        replacetextinfile("./install.iss","#VersionPatch#",patch)


    end
}--tools
newaction {
    trigger = "updateucodelinks",
    description = "updated the ucode bindings",
    execute = function ()

      if os.istarget("windows") then
        executeorexit("Output\\UCodeLangCl\\Win64\\Debug\\uclang cppdirtoulangvm UCode/src UCode/src/UCodeRunTime/ULibrarys/UCodeLang/UCodeAPI.cpp UCodeAPI/GameEngine/src/API.uc");
        executeorexit("Output\\UCodeLangCl\\Win64\\Debug\\uclang cppdirtoulangvm UCodeEditor/src/ULang/API UCodeEditor/src/ULang/EditorLink.cpp UCodeAPI/GameEngineEditor/src/API.uc");
      end

    end
}