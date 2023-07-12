#pragma once
#include <map>
#include <UCodeRunTime/RunTimeBasicTypes/String.hpp>
#include <UCodeRunTime/ULibrarys/Rendering/RenderingNamespace.hpp>
#include <UCodeRunTime/RunTimeBasicTypes/mat4.hpp>
#include <UCodeRunTime/Core/GameRunTime.hpp>
RenderingStart
struct Shader
{
public:
	Shader(const String& Vex,const String& Feg);
	Shader(const String& filepath);
	Shader(const Span<UInt32> SPRVB_Vex, const Span<UInt32> SPRVB_Feg);
	~Shader();
	
	Shader(const Shader& toCopyfrom) = delete;
	Shader& operator=(const Shader& ToCopy) = delete;
	
	
	static Shader* Default_Shader(Gamelibrary* lib);
	
	//Uniform

	void Bind() const;
	void UnBind() const;
	//SetUnifom
	inline void SetUniform1i_bool(const String& name, bool V) const
	{
		SetUniform1i(name, V);
	}
	void SetUniform1i(const String& name, SInt32 V) const;
	void SetUniform1iv(const String& name, SInt32 count, int* V) const;
	inline void SetUniform4f(const String& name, float* Values) const
	{
		SetUniform4f(name, Values[0], Values[1], Values[2], Values[3]);
	}
	void SetUniform4f(const String& name, float32 V, float32 V2, float32 V3, float32 V4) const;
	void SetUniformMat4f(const String& name,const mat4& V) const;


	
	static const char* dafalult_Vertex_shader_text;
	static const char* dafalult_fragment_shader_text;
private:
	UInt32 ShaderId;
	void InitShader(const String& Vex, const String& Feg);
	SInt32 GetUniformLocation(const String& name) const;

};
RenderingEnd