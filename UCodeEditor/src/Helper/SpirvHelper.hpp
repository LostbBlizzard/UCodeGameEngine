#pragma once
#include "SPIRV-Cross/spirv_glsl.hpp"
#include "glslang/SPIRV/GlslangToSpv.h"
#include "Typedefs.hpp"

EditorStart

struct SpirvHelper
{
	using SpirvByte = UCode::u32;
	static void Init();

	static void Finalize();

	static void InitResources(TBuiltInResource& Resources);
	
	inline thread_local static Vector<String> Errors;

	static bool GLSLtoSPV(const EShLanguage shader_type, const char* pshader, Vector<SpirvByte>& spirv);


	static bool SPVToGLSL(const Vector<SpirvByte>& spirv_binary, String& Out);
};

EditorEnd