#pragma once
#include <UCodeRunTime/ULibrarys/Rendering/RenderingNamespace.hpp>
#include <UCodeRunTime/BasicTypes.hpp>
#include <UCodeRunTime/Core/GameRunTime.hpp>
RenderingStart
struct Shader
{
public:
	Shader(const String& Vex,const String& Feg);
	Shader(const String& filepath);
	Shader(const Span<u32> SPRVB_Vex, const Span<u32> SPRVB_Feg);
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
	void SetUniform1i(const String& name, i32 V) const;
	void SetUniform1iv(const String& name, i32 count, int* V) const;
	inline void SetUniform4f(const String& name, float* Values) const
	{
		SetUniform4f(name, Values[0], Values[1], Values[2], Values[3]);
	}
	void SetUniform4f(const String& name, f32 V, f32 V2, f32 V3, f32 V4) const;
	void SetUniformMat4f(const String& name,const Mat4& V) const;


	
	static const char* dafalult_Vertex_shader_text;
	static const char* dafalult_fragment_shader_text;
private:
	u32 ShaderId;
	void InitShader(const String& Vex, const String& Feg);
	i32 GetUniformLocation(const String& name) const;

};
RenderingEnd