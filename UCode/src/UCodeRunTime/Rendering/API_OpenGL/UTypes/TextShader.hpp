#pragma once
#include <string>
#include <UCodeRunTime/ULibrarys/Rendering/RenderingNamespace.hpp>
#include <UCodeRunTime/CoreBooks/GameFiles.hpp>
RenderingStart
class TextShader
{
public:
	std::string Vertex, fragment;
	TextShader(std::string vertex, std::string frag) : Vertex(vertex), fragment(frag) {}
	static TextShader Get_ShaderFromPath(const std::string& FullShader);
};
RenderingEnd
