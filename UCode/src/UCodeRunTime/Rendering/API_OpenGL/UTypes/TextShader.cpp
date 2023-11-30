#include "TextShader.hpp"
#include <sstream>
#include <fstream>
#include <UCodeRunTime/CoreSystems/GameFiles.hpp>

RenderingStart
TextShader TextShader::Get_ShaderFromPath(const String& ShaderPath)
{
    enum class Type
    {
        Null = -1,
        Vex = 0,
        Fra = 1,
    };
    Type Type_ = Type::Null;
    std::ifstream stream(ShaderPath);
    String Line;
    std::stringstream ss[2];
    while (getline(stream,Line))
    {
        if (Line.find("#shader") != std::string::npos)
        {

            if (Line.find("vertex") != std::string::npos)
            {
                Type_ = Type::Vex;
            }
            else if (Line.find("fragment") != std::string::npos)
            {
                Type_ = Type::Fra;
            }

        }
        else
        {
            if (Type_ != Type::Null)
            {
                ss[(i32)Type_] << Line << '\n';
            }
        }
    }

    return TextShader(ss[0].str(), ss[1].str());
}
RenderingEnd