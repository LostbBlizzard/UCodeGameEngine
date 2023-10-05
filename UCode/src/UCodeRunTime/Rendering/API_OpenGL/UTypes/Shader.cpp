#include "Shader.hpp"

#include <UCodeRunTime/Rendering/API_OpenGL/OpenGlHelper.hpp>
#include "TextShader.hpp"
#include <UCodeRunTime/RunTimeBasicTypes/Ref.hpp>
#include <UCodeRunTime/ULibrarys/Loger.hpp>
RenderingStart

/*

"layout(std140) uniform Matrices"
"{"
    "Mat4 _ViewProj;"
    "Mat4 _Transfrom;"
"};"

*/

const char* Shader::dafalult_Vertex_shader_text =
"#version 450 core\n"



"layout(location = 0) in vec3 V_Pos;\n"
"layout(location = 1) in vec4 V_Color;\n"
"layout(location = 2) in vec2 V_texCord;\n"
"layout(location = 3) in float V_TexIndex;\n"

"layout(location = 0) out vec4 F_Color;\n"
"layout(location = 1) out vec2 F_texCord;\n"
"layout(location = 2) out float F_TexIndex;\n"

"uniform Mat4 _ViewProj;\n"
"uniform Mat4 _Transfrom;\n"

"void main()\n"
"{\n"
"F_Color = V_Color;\n"
"F_texCord = V_texCord;\n"
"F_TexIndex = V_TexIndex;\n"
"gl_Position = (_ViewProj * _Transfrom) * vec4(V_Pos, 1.0);\n"
"}";

const char* Shader::dafalult_fragment_shader_text =
"#version 450 core\n"
"layout(location = 0) in vec4 F_Color;\n"
"layout(location = 1) in vec2 F_texCord;\n"
"layout(location = 2) in float F_TexIndex;\n"

"layout(set=0, binding = 0) uniform sampler2D F_Textures[32];\n"
"layout(location = 0) out vec4 P_Color;\n"
"void main()\n"
"{\n"
"int Index = int(F_TexIndex);\n"
"vec4 texturerColor = texture(F_Textures[Index], F_texCord);\n"
"P_Color = texturerColor * F_Color;\n"
"}\n";
Shader::Shader(const String& Vex,const String& Feg)
{
    InitShader(Vex, Feg);
}
Shader::Shader(const String& filepath)
{
    auto s = TextShader::Get_ShaderFromPath(filepath);
    InitShader(s.Vertex, s.fragment);
}
Shader::Shader(const Span<u32> SPRVB_Vex, const Span<u32> SPRVB_Feg)
{


    GlCall(GLuint vertex_shader = glCreateShader(GL_VERTEX_SHADER));

    GlCall(glShaderBinary(1, &vertex_shader, GL_SHADER_BINARY_FORMAT_SPIR_V, SPRVB_Vex.Data(), SPRVB_Vex.Size()));

    GlCall(glSpecializeShader(vertex_shader, "main", 0, 0, 0));


    GlCall(GLuint fragment_shader = glCreateShader(GL_VERTEX_SHADER));

    GlCall(glShaderBinary(1, &fragment_shader, GL_SHADER_BINARY_FORMAT_SPIR_V, SPRVB_Vex.Data(), SPRVB_Vex.Size()));

    GlCall(glSpecializeShader(fragment_shader, "main", 0, 0, 0));


    GlCall(GLuint program = glCreateProgram());
    GlCall(glAttachShader(program, vertex_shader));
    GlCall(glAttachShader(program, fragment_shader));
    GlCall(glLinkProgram(program));

    GLint program_linked; glGetProgramiv(program, GL_LINK_STATUS, &program_linked);
    if (program_linked != GL_TRUE)
    {
        GLint logSize; GlCall(glGetProgramiv(program, GL_INFO_LOG_LENGTH, &logSize));

        auto Text = std::make_unique<char[]>(logSize);
        GlCall(glGetProgramInfoLog(program, logSize, &logSize, &Text[0]));
        UCODE_ENGINE_LOG("program linking  failed:" << Text << std::endl);
    }


    GlCall(glDeleteShader(vertex_shader));
    GlCall(glDeleteShader(fragment_shader));
    ShaderId = program;
}
void Shader::InitShader(const String& Vex, const String& Feg)
{
    const char* vex = Vex.c_str();
    const char* frg = Feg.c_str();

    GlCall(GLuint vertex_shader = glCreateShader(GL_VERTEX_SHADER))
        GlCall(glShaderSource(vertex_shader, 1, &vex, NULL))
        GlCall(glCompileShader(vertex_shader))

        GlCall(GLuint fragment_shader = glCreateShader(GL_FRAGMENT_SHADER));
    GlCall(glShaderSource(fragment_shader, 1, &frg, NULL));
    GlCall(glCompileShader(fragment_shader));

    GlCall(GLuint program = glCreateProgram());
    GlCall(glAttachShader(program, vertex_shader));
    GlCall(glAttachShader(program, fragment_shader));
    GlCall(glLinkProgram(program));


    GLint success = 0;
    GlCall(glGetShaderiv(vertex_shader, GL_COMPILE_STATUS, &success));
    GLint success2 = 0;
    GlCall(glGetShaderiv(fragment_shader, GL_COMPILE_STATUS, &success2));



    if (success == GL_FALSE)
    {
        GLint logSize; GlCall(glGetShaderiv(vertex_shader, GL_INFO_LOG_LENGTH, &logSize));

         auto Text =std::make_unique<char[]>(logSize);

        GlCall(glGetShaderInfoLog(vertex_shader, logSize, &logSize, &Text[0]));

        UCODE_ENGINE_LOG("Vertex shader failed:" << Text<<std::endl);
      
    }
    if (success2 == GL_FALSE)
    {
        GLint logSize; GlCall(glGetShaderiv(fragment_shader, GL_INFO_LOG_LENGTH, &logSize));

        auto Text = std::make_unique<char[]>(logSize);

        GlCall(glGetShaderInfoLog(fragment_shader, logSize, &logSize, &Text[0]));

        UCODE_ENGINE_LOG("Fragment shader failed:" << Text <<std::endl);
       
    }
    GLint program_linked; glGetProgramiv(program, GL_LINK_STATUS, &program_linked);
    if (program_linked != GL_TRUE)
    {
        GLint logSize; GlCall(glGetProgramiv(program, GL_INFO_LOG_LENGTH, &logSize));

        auto Text = std::make_unique<char[]>(logSize);
        GlCall(glGetProgramInfoLog(program, logSize, &logSize, &Text[0]));
        UCODE_ENGINE_LOG("program linking  failed:" << Text << std::endl);
    }

  
    GlCall(glDeleteShader(vertex_shader));
    GlCall(glDeleteShader(fragment_shader));
    ShaderId = program;
}

Shader::~Shader()
{
    glDeleteProgram(ShaderId);
}




Shader* Shader::Default_Shader(Gamelibrary* lib) { return GameFiles::Get(lib)->Get_DefaultShader(); }

void Shader::Bind() const
{
    glUseProgram(ShaderId);
}
void Shader::UnBind() const
{
    glUseProgram(0);
}
void Shader::SetUniform1i(const String& name, int V) const
{
    Bind();
    auto Location = GetUniformLocation(name);
    GlCall(glUniform1i(Location, V));
}
void Shader::SetUniform1iv(const String& name,i32 count, int* V) const
{
    Bind();
    auto Location = GetUniformLocation(name);
    GlCall(glUniform1iv(Location, count, V));
}
void Shader::SetUniform4f(const String& name, f32 V, f32 V2, f32 V3, f32 V4) const
{
    Bind();
    auto Location = GetUniformLocation(name);
    GlCall(glUniform4f(Location, V, V2, V3, V4));
}
void Shader::SetUniformMat4f(const String& name, const Mat4& V) const
{
    Bind();
    auto Location = GetUniformLocation(name);
    GlCall(glUniformMatrix4fv(Location,1,GL_FALSE,&V[0][0]))
}
i32 Shader::GetUniformLocation(const String& Name) const
{
    //
    GlCall(auto r = glGetUniformLocation(ShaderId, Name.c_str()));
    if (r == -1) 
    {
        UCodeGameEngineThrowException("Uniform does not exist");
    }
    return r;
}
RenderingEnd


