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
      if os.is64bit() then
       defaultplatform "MacOS"
      else
       defaultplatform "MacOS"
      end
   end

   OutDirPath ="%{cfg.platform}/%{cfg.buildcfg}"
   UCPathExe = "%{wks.location}Output/UCodelangCL/" .. OutDirPath .. "/uclang"

   

   

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
      "Dependencies/GLEW",
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

    filter {}

    if not HasULangCompiler then
    defines {"UCodeLangNoCompiler"}
    end 
end
      
function linkUCode(HasULangCompiler)    

    filter { "system:Windows" }
      links {"Ws2_32.lib"}
      
              
    filter { "system:Windows","architecture:x86"}
      links {"glew32s.lib","Opengl32.lib"}
          
    filter { "system:Windows","architecture:x86_64"}
      links {"glew64s.lib","Opengl32.lib"}

             
    filter { "system:linux" }
      links {"GL"}
      links {"GLEW"}
    
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
     links {
      "UCode",
      "Imgui",
      "yaml-cpp",
      "stb_image",
      "stb_image_write",
      "GLFW",
      "box2d",
      "MinimalSocket"
     }

     if HasULangCompiler then
      libdirs { "Output/UCodeLang/" .. OutDirPath}
      links { "UCodeLang" }
     else
      libdirs { "Output/UCodeLangNoCompiler/" .. OutDirPath}
      links { "UCodeLangNoCompiler" }
     end
end
   
      
project "UCode"
   location "UCode"
   kind "StaticLib"
   language "C++"

   dependson {
    "GLEW","GLFW","glm", 
    "box2d","yaml-cpp","stb_image","stb_image_write","Imgui","UCodeLangCl","UCodeLangNoCompiler",
    "plog","MinimalSocket",
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
   
   includeUCode();


project "UCodeApp"
   location "UCodeApp"
   kind "ConsoleApp"
   language "C++"
   
   targetdir ("Output/%{prj.name}/" .. OutDirPath)
   objdir ("Output/int/%{prj.name}/" .. OutDirPath)
   
   dependson {"UCode"}

   files { 
     "%{prj.name}/src/**.c",
     "%{prj.name}/src/**.h",
     "%{prj.name}/src/**.cpp",
     "%{prj.name}/src/**.hpp", 
   }
   includedirs{"%{prj.name}/src"}

   includeUCode(false)

   linkUCode(false)
   

  
   buildmessage "Copying Output"

   filter { "system:Windows","architecture:x86_64" }
      postbuildcommands {
         "{COPYFILE} %{cfg.buildtarget.abspath} %{wks.location}UCodeEditor/UFiles/bin/UCAppWinDebug86X64.exe"
      }
    
   filter { "system:Windows","architecture:x86" }
      postbuildcommands {
         "{COPYFILE} %{cfg.buildtarget.abspath} %{wks.location}UCodeEditor/UFiles/bin/UCAppWinDebug86X32.exe"
      }
 
   filter { "system:linux","architecture:x86_64" }
      postbuildcommands {
        "{COPYFILE} %{cfg.buildtarget.abspath} %{wks.location}UCodeEditor/UFiles/bin/UCAppLinuxDebug86X64"
      }
    
   filter { "system:linux","architecture:x86" }
      postbuildcommands {
        "{COPYFILE} %{cfg.buildtarget.abspath} %{wks.location}UCodeEditor/UFiles/bin/UCAppLinuxDebug86X32"
      }


project "UCodeEditor"
   location "UCodeEditor"
   kind "ConsoleApp"
   language "C++"
   
   dependson {
   "UCodeApp",
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

   linkUCode(true)
   
   

   buildmessage "Copying UFilesAPI"
   postbuildcommands 
   {
    "{COPYDIR} %{wks.location}/UCodeAPI/GameEngine %{prj.location}/UFiles/source/UCodeGameEngine",
    "{COPYDIR} %{wks.location}/UCodeAPI/GameEngineEditor %{prj.location}/UFiles/source/UCodeGameEngineEditor",

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
      buildmessage "Copying UFilesDir"
      postbuildcommands 
      {
      "{COPYDIR} %{prj.location}/UFiles %{prj.location}%{cfg.targetdir}/UFiles"
      }--Make into GameFile.data file.

   filter { "configurations:Release" }
      buildmessage "Copying UFilesDir"
      postbuildcommands 
      {
      "{COPYDIR} %{prj.location}/UFiles %{prj.location}%{cfg.targetdir}/UFiles"
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
  
  prebuildcommands
  {
     UCPathExe.." cpptoulangvm %{wks.location}UCode/src/UCodeRunTime/ULibrarys/UCodeLang/API/API.hpp %{wks.location}UCode/src/UCodeRunTime/ULibrarys/UCodeLang/UCodeAPI.cpp %{prj.location}src/API.uc",
  }
  postbuildcommands {
    --"uclang build %{prj.location}ULangModule.ucm"
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
  
  project "GLFW"
    location "Dependencies/%{prj.name}"
    kind "StaticLib"
    language "C++" 

    targetdir ("Output/%{prj.name}/" .. OutDirPath)
    objdir ("Output/int/%{prj.name}/" .. OutDirPath)

    files {
      "Dependencies/%{prj.name}/include/**.h", 
      "Dependencies/%{prj.name}/src/**.c",
    }

    includedirs{
      "Dependencies/%{prj.name}",
      
    }
    filter { "system:Windows" }
      defines {"_GLFW_WIN32"}
      
    
    filter { "system:linux" }

    filter { "system:linux" }
      defines {"_GLFW_X11","GLEW_NO_GLU"}
        
    filter { "system:MacOS" }
      defines {"_GLFW_COCOA","GLEW_NO_GLU"}
    
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


