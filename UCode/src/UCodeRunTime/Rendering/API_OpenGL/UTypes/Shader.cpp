#include "Shader.hpp"

#include <UCodeRunTime/Rendering/API_OpenGL/OpenGlHelper.hpp>
#include "TextShader.hpp"
#include <UCodeRunTime/BasicTypes.hpp>
#include <UCodeRunTime/ULibrarys/Loger.hpp>
#include <UCodeRunTime/CoreSystems/GameFiles.hpp>
RenderingStart

    /*

    "layout(std140) uniform Matrices"
    "{"
        "Mat4 _ViewProj;"
        "Mat4 _Transfrom;"
    "};"

    */

    /*
    const char* Shader::dafalult_Vertex_shader_text = R"(
    #version 450 core


    layout(location = 0) in vec3 V_Pos;
    layout(location = 1) in vec4 V_Color;
    layout(location = 2) in vec2 V_texCord;
    layout(location = 3) in float V_TexIndex;

    layout(location = 0) out vec4 F_Color;
    layout(location = 1) out vec2 F_texCord;
    layout(location = 2) out float F_TexIndex;

    uniform Mat4 _ViewProj;
    uniform Mat4 _Transfrom;

    void main()
    {
    F_Color = V_Color;
    F_texCord = V_texCord;
    F_TexIndex = V_TexIndex;
    gl_Position = (_ViewProj * _Transfrom) * vec4(V_Pos, 1.0);
    }

    )";

    const char* Shader::dafalult_fragment_shader_text = R"(
    #version 450 core
    layout(location = 0) in vec4 F_Color;
    layout(location = 1) in vec2 F_texCord;
    layout(location = 2) in float F_TexIndex;

    layout(set=0, binding = 0) uniform sampler2D F_Textures[32];
    layout(location = 0) out vec4 P_Color;
    void main()
    {
    int Index = int(F_TexIndex);
    vec4 texturerColor = texture(F_Textures[Index], F_texCord);
    P_Color = texturerColor * F_Color;
    }

    )";
    */

    const char *Default_Vertex_shader_v330 = R"(


layout(location = 0) in vec3 V_Pos;
layout(location = 1) in vec4 V_Color;
layout(location = 2) in vec2 V_texCord;
layout(location = 3) in float V_TexIndex;

layout(location = 0) out vec4 F_Color;
layout(location = 1) out vec2 F_texCord;
layout(location = 2) out float F_TexIndex;

uniform mat4 _ViewProj;
uniform mat4 _Transfrom;

void main()
{
F_Color = V_Color;
F_texCord = V_texCord;
F_TexIndex = V_TexIndex;
gl_Position = (_ViewProj * _Transfrom) * vec4(V_Pos, 1.0);
}

)";

const char *Default_fragment_shader_v330 = R"(

layout(location = 0) in vec4 F_Color;
layout(location = 1) in vec2 F_texCord;
layout(location = 2) in float F_TexIndex;

uniform sampler2D F_Textures[32];
layout(location = 0) out vec4 P_Color;
void main()
{
int Index = int(F_TexIndex);
vec4 texturerColor = texture(F_Textures[Index], F_texCord);
P_Color = texturerColor * F_Color;
}

)";

const char *Default_Vertex_shader_v410 = R"(

layout(location = 0) in vec3 V_Pos;
layout(location = 1) in vec4 V_Color;
layout(location = 2) in vec2 V_texCord;
layout(location = 3) in float V_TexIndex;

layout(location = 0) out vec4 F_Color;
layout(location = 1) out vec2 F_texCord;
layout(location = 2) out float F_TexIndex;

uniform mat4 _ViewProj;
uniform mat4 _Transfrom;

void main()
{
F_Color = V_Color;
F_texCord = V_texCord;
F_TexIndex = V_TexIndex;
gl_Position = (_ViewProj * _Transfrom) * vec4(V_Pos, 1.0);
}

)";

const char *Default_fragment_shader_v410 = R"(

layout(location = 0) in vec4 F_Color;
layout(location = 1) in vec2 F_texCord;
layout(location = 2) in float F_TexIndex;

uniform sampler2D F_Textures[32];
layout(location = 0) out vec4 P_Color;
void main()
{
int Index = int(F_TexIndex);
vec4 texturerColor;
switch (Index){
 case 0:
  texturerColor = texture(F_Textures[0], F_texCord);
  break;
  case 1:
  texturerColor = texture(F_Textures[1], F_texCord);
  break;
  case 2:
  texturerColor = texture(F_Textures[2], F_texCord);
  break;
  case 3:
  texturerColor = texture(F_Textures[3], F_texCord);
  break;
  case 4:
  texturerColor = texture(F_Textures[4], F_texCord);
  break;
  case 5:
  texturerColor = texture(F_Textures[5], F_texCord);
  break;
  case 6:
  texturerColor = texture(F_Textures[6], F_texCord);
  break;
  case 7:
  texturerColor = texture(F_Textures[7], F_texCord);
  break;
  case 8:
  texturerColor = texture(F_Textures[8], F_texCord);
  break;
  case 9:
  texturerColor = texture(F_Textures[9], F_texCord);
  break;
  case 10:
  texturerColor = texture(F_Textures[10], F_texCord);
  break;
  case 11:
  texturerColor = texture(F_Textures[11], F_texCord);
  break;
  case 12:
  texturerColor = texture(F_Textures[12], F_texCord);
  break;
  case 13:
  texturerColor = texture(F_Textures[13], F_texCord);
  break;
  case 14:
  texturerColor = texture(F_Textures[14], F_texCord);
  break;
  case 15:
  texturerColor = texture(F_Textures[15], F_texCord);
  break;
  case 16:
  texturerColor = texture(F_Textures[16], F_texCord);
  break;
  case 17:
  texturerColor = texture(F_Textures[17], F_texCord);
  break;
  case 18:
  texturerColor = texture(F_Textures[18], F_texCord);
  break;
  case 19:
  texturerColor = texture(F_Textures[19], F_texCord);
  break;
  case 20:
  texturerColor = texture(F_Textures[20], F_texCord);
  break;
  case 21:
  texturerColor = texture(F_Textures[21], F_texCord);
  break;
  case 22:
  texturerColor = texture(F_Textures[22], F_texCord);
  break;
  case 23:
  texturerColor = texture(F_Textures[23], F_texCord);
  break;
  case 24:
  texturerColor = texture(F_Textures[24], F_texCord);
  break;
  case 25:
  texturerColor = texture(F_Textures[25], F_texCord);
  break;
  case 26:
  texturerColor = texture(F_Textures[26], F_texCord);
  break;
  case 27:
  texturerColor = texture(F_Textures[27], F_texCord);
  break;
  case 28:
  texturerColor = texture(F_Textures[28], F_texCord);
  break;
  case 29:
  texturerColor = texture(F_Textures[29], F_texCord);
  break;
  case 30:
  texturerColor = texture(F_Textures[30], F_texCord);
  break;
  case 31:
  texturerColor = texture(F_Textures[31], F_texCord);
  break;
}
P_Color = texturerColor * F_Color;
}

)";

Shader::Shader(const String &Vex, const String &Feg)
{
    InitShader(Vex, Feg);
}
Shader::Shader(const String &filepath)
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

    GLint program_linked;
    glGetProgramiv(program, GL_LINK_STATUS, &program_linked);
    if (program_linked != GL_TRUE)
    {
        GLint logSize;
        GlCall(glGetProgramiv(program, GL_INFO_LOG_LENGTH, &logSize));

        auto Text = std::make_unique<char[]>(logSize);
        GlCall(glGetProgramInfoLog(program, logSize, &logSize, &Text[0]));
        UCodeGELog("program linking  failed:" << StringView(Text.get(), logSize) << '\n');
    }

    GlCall(glDeleteShader(vertex_shader));
    GlCall(glDeleteShader(fragment_shader));
    ShaderId = program;
}
void Shader::InitShader(const String &Vex, const String &Feg)
{
    const char *vex = Vex.c_str();
    const char *frg = Feg.c_str();

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
        GLint logSize;
        GlCall(glGetShaderiv(vertex_shader, GL_INFO_LOG_LENGTH, &logSize));

        auto Text = std::make_unique<char[]>(logSize);

        GlCall(glGetShaderInfoLog(vertex_shader, logSize, &logSize, &Text[0]));

        UCodeGELog("Vertex shader failed:" << StringView(Text.get(), logSize) << std::endl);
    }
    if (success2 == GL_FALSE)
    {
        GLint logSize;
        GlCall(glGetShaderiv(fragment_shader, GL_INFO_LOG_LENGTH, &logSize));

        auto Text = std::make_unique<char[]>(logSize);

        GlCall(glGetShaderInfoLog(fragment_shader, logSize, &logSize, &Text[0]));

        UCodeGELog("Fragment shader failed:" << StringView(Text.get(), logSize) << std::endl);
    }
    GLint program_linked;
    glGetProgramiv(program, GL_LINK_STATUS, &program_linked);
    if (program_linked != GL_TRUE)
    {
        GLint logSize;
        GlCall(glGetProgramiv(program, GL_INFO_LOG_LENGTH, &logSize));

        auto Text = std::make_unique<char[]>(logSize);
        GlCall(glGetProgramInfoLog(program, logSize, &logSize, &Text[0]));
        UCodeGELog("program linking  failed:" << StringView(Text.get(), logSize) << std::endl);
    }

    GlCall(glDeleteShader(vertex_shader));
    GlCall(glDeleteShader(fragment_shader));
    ShaderId = program;
}

Shader::~Shader()
{
    glDeleteProgram(ShaderId);
}

Shader *Shader::Default_Shader(Gamelibrary *lib) { return GameFiles::Get(lib)->Get_DefaultShader(); }

void Shader::Bind() const
{
    glUseProgram(ShaderId);
}
void Shader::UnBind() const
{
    glUseProgram(0);
}
void Shader::SetUniform1i(const String &name, int V) const
{
    Bind();
    auto Location = GetUniformLocation(name);
    GlCall(glUniform1i(Location, V));
}
void Shader::SetUniform1iv(const String &name, i32 count, int *V) const
{
    Bind();
    auto Location = GetUniformLocation(name);
    GlCall(glUniform1iv(Location, count, V));
}
void Shader::SetUniform4f(const String &name, f32 V, f32 V2, f32 V3, f32 V4) const
{
    Bind();
    auto Location = GetUniformLocation(name);
    GlCall(glUniform4f(Location, V, V2, V3, V4));
}
void Shader::SetUniformMat4f(const String &name, const Mat4 &V) const
{
    Bind();
    auto Location = GetUniformLocation(name);
    GlCall(glUniformMatrix4fv(Location, 1, GL_FALSE, &V[0][0]))
}

GLuint GLVersion()
{
    GLint major = 0;
    GLint minor = 0;
    glGetIntegerv(GL_MAJOR_VERSION, &major);
    glGetIntegerv(GL_MINOR_VERSION, &minor);
    if (major == 0 && minor == 0)
    {
        // Query GL_VERSION in desktop GL 2.x, the string will start with "<major>.<minor>"
        const char *gl_version = (const char *)glGetString(GL_VERSION);
        sscanf(gl_version, "%d.%d", &major, &minor);
    }
    return (GLuint)(major * 100 + minor * 10);
}

String defaultver;
const char *Shader::GetDefaultVertexShader()
{
    if (defaultver.size() == 0)
    {
        auto v = GLVersion();

        defaultver += "#version ";
        defaultver += std::to_string(v);
        defaultver += " core";

        if (v == 330)
        {
            defaultver += Default_Vertex_shader_v330;
        }
        else if (v >= 410)
        {
            defaultver += Default_Vertex_shader_v410;
        }
        else
        {
            UCodeGEUnreachable();
        }
    }

    return defaultver.c_str();
}

String defaultfra;
const char *Shader::GetDefaultFragmentShader()
{
    if (defaultfra.size() == 0)
    {
        auto v = GLVersion();

        defaultfra += "#version ";
        defaultfra += std::to_string(v);
        defaultfra += " core";

        if (v == 330)
        {
            defaultfra += Default_fragment_shader_v330;
        }
        else if (v >= 410)
        {
            defaultfra += Default_fragment_shader_v410;
        }
        else
        {
            UCodeGEUnreachable();
        }
    }

    return defaultfra.c_str();
}
i32 Shader::GetUniformLocation(const String &Name) const
{
    //
    GlCall(auto r = glGetUniformLocation(ShaderId, Name.c_str()));
    if (r == -1)
    {
        UCodeGEThrow("Uniform does not exist");
    }
    return r;
}
RenderingEnd
