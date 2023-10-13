+++
title = 'My First Post'
date = 2023-10-13T14:55:23-04:00
draft = true
+++
## Introduction

This is **bold** text, and this is *emphasized* text.

Visit the [Hugo](https://gohugo.io) website!

libdirs { 
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
     "Output/UCode/" .. OutDirPath,
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