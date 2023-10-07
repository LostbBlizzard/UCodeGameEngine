workspace "UCodeGameEngine"
   configurations { "Debug", "Release","Published" }
   platforms { "Win32", "Win64","linux32","linux64","macosx"}
   defines {"YAML_CPP_STATIC_DEFINE","GLEW_STATIC"}
   
   startproject "UCodeEditor"
   
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
   }
   
   
   filter { "platforms:Win32" }
    system "Windows"
    architecture "x86"
    defines { "UCode_Build_32Bit"}
   filter { "platforms:Win64" }
     system "Windows"
     architecture "x86_64"
     defines { "UCode_Build_64Bit"}
   filter { "system:Windows" }
     cppdialect "c++17"
     defines { "UCode_Build_Windows_OS","_GLFW_WIN32"}

   

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
      
      
project "UCode"
   location "UCode"
   kind "StaticLib"
   language "C++"

   dependson {
    "GLEW","GLFW","glm", 
    "box2d","yaml-cpp","stb_image","stb_image_write","Imgui","UCodeLang",
    "MinimalSocket",
    }
   
   targetdir ("Output/%{prj.name}/" .. OutDirPath)
   objdir ("Output/int/%{prj.name}/" .. OutDirPath)
   
   files { 
     "%{prj.name}/src/**.c",
     "%{prj.name}/src/**.h",
     "%{prj.name}/src/**.cpp",
     "%{prj.name}/src/**.hpp", 
   }

   includedirs{
    "%{prj.name}/src",
    "%{prj.name}/src/OtherLibrarys",
    "Dependencies/UCodeLang",
   }
project "UCodeApp"
   location "UCodeApp"
   kind "ConsoleApp"
   language "C++"
   
   targetdir ("Output/%{prj.name}/" .. OutDirPath)
   objdir ("Output/int/%{prj.name}/" .. OutDirPath)
   
   dependson { "UCode"}

   files { 
     "%{prj.name}/src/**.c",
     "%{prj.name}/src/**.h",
     "%{prj.name}/src/**.cpp",
     "%{prj.name}/src/**.hpp", 
   }
   includedirs{
    "%{prj.name}/src",
    "%{prj.name}/src/OtherLibrarys",
    "UCode/src",
    "Dependencies/UCodeLang",
   }

   
   libdirs { 
    "Output/UCode/" .. OutDirPath,

    "Output/Imgui/" .. OutDirPath,
    "Output/yaml-cpp/" .. OutDirPath,
    "Output/glm/" .. OutDirPath,
    "Output/stb_image/" .. OutDirPath,    
    "Output/stb_image_write/" .. OutDirPath,    
    "Dependencies/GLEW/Lib",
    "Output/GLFW/" .. OutDirPath, 
    "Output/UCodeLang/" .. OutDirPath, 
    "Output/SPIRV-Cross/" .. OutDirPath, 
    "Output/box2d/" .. OutDirPath, 
    "Output/MinimalSocket/" .. OutDirPath, 
   }
   links {
    "UCode",
    "Imgui",
    "yaml-cpp",
    "stb_image",
    "stb_image_write",
    "GLFW",
    "UCodeLang",
    "box2d",
    "MinimalSocket"
   }

 
   filter { "system:Windows" }
    links {"Ws2_32.lib"}

  
   filter { "system:Windows","architecture:x86"}
    links {
      "glew32s.lib","Opengl32.lib",
    }

   filter { "system:Windows","architecture:x86_64"}
    links {
     "glew64s.lib","Opengl32.lib",
    }

  
    buildmessage "Copying Output"

    postbuildcommands {
       "{COPYFILE} %{prj.location}%{cfg.targetdir}/%{prj.name}.exe %{wks.location}UCodeEditor/UFiles/bin/%{prj.name}_%{cfg.platform}.exe"
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
   }

   targetdir ("Output/%{prj.name}/" .. OutDirPath)
   objdir ("Output/int/%{prj.name}/" .. OutDirPath)
   
   files { 
     "%{prj.name}/src/**.c",
     "%{prj.name}/src/**.h",
     "%{prj.name}/src/**.cpp",
     "%{prj.name}/src/**.hpp", 
   }
   includedirs{
    "%{prj.name}/src",
    "%{prj.name}/src/OtherLibrarys",
    "UCode/src",
    "Dependencies",
   }
  
   
   libdirs { 
    "Output/UCode/" .. OutDirPath,

    "Output/Imgui/" .. OutDirPath,
    "Output/yaml-cpp/" .. OutDirPath,
    "Output/glm/" .. OutDirPath,
    "Output/stb_image/" .. OutDirPath,    
    "Output/stb_image_write/" .. OutDirPath,    
    "Dependencies/GLEW/Lib",
    "Output/GLFW/" .. OutDirPath, 
    "Output/UCodeLang/" .. OutDirPath, 
    "Output/SPIRV-Cross/" .. OutDirPath, 
    "Output/glslang/" .. OutDirPath, 
    "Output/box2d/" .. OutDirPath, 
    "Output/FileWatcher/" .. OutDirPath, 
    "Output/zip/" .. OutDirPath, 
    "Output/MinimalSocket/" .. OutDirPath, 
   }

   links {
    "UCode",
    "Imgui",
    "yaml-cpp",
    "stb_image",
    "stb_image_write",
    "GLFW",
    "UCodeLang",
    "SPIRV-Cross",
    "glslang",
    "box2d",
    "FileWatcher",
    "zip",
    "MinimalSocket"
   }
   
   filter { "system:Windows" }
    links {"Ws2_32.lib"}

  
   filter { "system:Windows","architecture:x86"}
    links {
      "glew32s.lib","Opengl32.lib",
    }

   filter { "system:Windows","architecture:x86_64"}
    links {"glew64s.lib","Opengl32.lib"}

   filter { "configurations:Published" }
      kind ("WindowedApp")
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

group "UCodeAPIs"
 project "UCodeGameEngine"
  location "UCodeAPI/GameEngine"
  kind "StaticLib"
  language "C"
  files { 
  "%{prj.name}/src/**.uc",

  "%{prj.name}/src/**.c",
  "%{prj.name}/src/**.h",
  "%{prj.name}/src/**.cpp",
  "%{prj.name}/src/**.hpp", 
  }
  targetdir ("Output/%{prj.name}/" .. OutDirPath)
  objdir ("Output/int/%{prj.name}/" .. OutDirPath)

  postbuildcommands {
    "uclang build %{prj.location}ULangModule.ucm"
   }

 project "UCodeGameEngineEditor"
  location "UCodeAPI/GameEngineEditor"
  kind "StaticLib"
  language "C"
  files { 
  "%{prj.name}/src/**.uc",

  "%{prj.name}/src/**.c",
  "%{prj.name}/src/**.h",
  "%{prj.name}/src/**.cpp",
  "%{prj.name}/src/**.hpp", 
  }
  targetdir ("Output/%{prj.name}/" .. OutDirPath)
  objdir ("Output/int/%{prj.name}/" .. OutDirPath)

  postbuildcommands {
   "uclang build %{prj.location}ULangModule.ucm"
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
    }

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
  project "GLEW"
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
    includedirs{
      "Dependencies/%{prj.name}",
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
      "Dependencies/%{prj.name}/UCodeLang/Dependencies/zycore/include",
      "Dependencies/%{prj.name}/UCodeLang/Dependencies/Zydis/include",
      "Dependencies/%{prj.name}/UCodeLang/Dependencies/Zydis/src"
      }
    removefiles{
     "Dependencies/%{prj.name}/UCodeLang/Dependencies/Zydis/**.c",
     "Dependencies/%{prj.name}/UCodeLang/Dependencies/zycore/**.c",

     "Dependencies/%{prj.name}/UCodeLang/Dependencies/Zydis/**.cpp",
     "Dependencies/%{prj.name}/UCodeLang/Dependencies/zycore/**.cpp",

     "Dependencies/%{prj.name}/UCodeLang/Dependencies/Zydis/**.h",
     "Dependencies/%{prj.name}/UCodeLang/Dependencies/zycore/**.h",
     }
    files { 
    "Dependencies/%{prj.name}/UCodeLang/Dependencies/Zydis/src/**.c",
    "Dependencies/%{prj.name}/UCodeLang/Dependencies/zycore/src/**.c",

    "Dependencies/%{prj.name}/UCodeLang/Dependencies/Zydis/src/**.inc",
    "Dependencies/%{prj.name}/UCodeLang/Dependencies/zycore/src/**.inc",

    "Dependencies/%{prj.name}/UCodeLang/Dependencies/Zydis/include/**.h",
    "Dependencies/%{prj.name}/UCodeLang/Dependencies/zycore/include/**.h",
   }
  project "glslang"
    location "Dependencies/%{prj.name}"
    kind "StaticLib"
    language "C++" 

    targetdir ("Output/%{prj.name}/" .. OutDirPath)
    objdir ("Output/int/%{prj.name}/" .. OutDirPath)

    files {
      "Dependencies/%{prj.name}/glslang/OSDependent/Windows/**.cpp",
      "Dependencies/%{prj.name}/glslang/OSDependent/Windows/**.h", 

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
    removefiles 
    {
      "Dependencies/%{prj.name}/glslang/OSDependent/Windows/main.cpp"
    }

    includedirs{
      "Dependencies/%{prj.name}",
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

    targetdir ("Output/%{prj.name}/" .. OutDirPath)
    objdir ("Output/int/%{prj.name}/" .. OutDirPath)

    files {
      "Dependencies/%{prj.name}/src/**.h",
      "Dependencies/%{prj.name}/src/**.c",
      "Dependencies/%{prj.name}/src/**.cpp",
      "Dependencies/%{prj.name}/src/**.hpp",
    }