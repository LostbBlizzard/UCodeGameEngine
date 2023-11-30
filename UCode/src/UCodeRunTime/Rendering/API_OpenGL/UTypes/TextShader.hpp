#pragma once
#include "UCodeRunTime/ULibrarys/Rendering/RenderingNamespace.hpp"
#include "UCodeRunTime/BasicTypes.hpp"
RenderingStart
class TextShader
{
public:
	String Vertex, fragment;
	TextShader(const String& vertex,const String& frag) : Vertex(vertex), fragment(frag) {}
	static TextShader Get_ShaderFromPath(const String& FullShader);
};
RenderingEnd
