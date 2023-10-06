#pragma once

#include "GL/glew.h"
#include "GLFW/glfw3.h"

#include <iostream>
#include "UCodeRunTime/UDefs.hpp"
#include "UCodeRunTime/ULibrarys/Loger.hpp"

#include "../RenderAPINamespace.h"


#if UCodeGameEngineDEBUG
#define GlCall(x) _RenderAPI::GLClearError();x;UCodeGameEngineAssert(_RenderAPI::GLCheckError(#x,__FILE__,__LINE__));
#else
#define GlCall(x) x;
#endif // DEBUG

#define RenderAPIErrorStartTag "[OpenGL Error]"

RenderAPIStart
static void GLClearError()
{
	while (glGetError() != GL_NO_ERROR)
	{

	}
}
static bool GLCheckError(const char* funk, const char* file,i32 line)
{
	while (auto errorcode =	glGetError())
	{
		const char* errortypeName;
		switch (errorcode)
		{
		case GL_INVALID_ENUM:
			errortypeName = "GL_INVALID_ENUM";
			break;
		case GL_INVALID_VALUE:
			errortypeName = "GL_INVALID_VALUE";
			break;
		case GL_INVALID_OPERATION:
			errortypeName = "GL_INVALID_OPERATION";
			break;
		case GL_STACK_OVERFLOW:
			errortypeName = "GL_STACK_OVERFLOW";
			break;
		case GL_STACK_UNDERFLOW:
			errortypeName = "GL_STACK_UNDERFLOW";
			break;
		case GL_OUT_OF_MEMORY:
			errortypeName = "GL_OUT_OF_MEMORY";
			break;
		case GL_INVALID_FRAMEBUFFER_OPERATION:
			errortypeName = "GL_INVALID_FRAMEBUFFER_OPERATION";
			break;
		case GL_CONTEXT_LOST:
			errortypeName = "GL_CONTEXT_LOST";
			break;
		case GL_TABLE_TOO_LARGE:
			errortypeName = "GL_CONTEXT_LOST";
			break;
		default:
			errortypeName = nullptr;
			UCodeGameEngineUnreachable();
			break;
		}
		UCODE_ENGINE_LOG(RenderAPIErrorStartTag "(" << errorcode << ")" << errortypeName <<'\n'
			<< " On Line:" << line << " At file " << file
			<< '\n' << " Called:" << funk);
		return true;
	}
	return false;
}
RenderAPIEnd