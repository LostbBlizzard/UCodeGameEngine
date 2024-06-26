#include "FrameBuffer.hpp"
#include <UCodeRunTime/Rendering/API_OpenGL/OpenGlHelper.hpp>
#include "Texture.hpp"
#include <UCodeRunTime/ULibrarys/HeadlessContext.hpp>
RenderingStart
FrameBuffer::FrameBuffer(FrameBufferData data):_BufferId(0), _Texture(0),_Data(data)
{
	if (IsHeadlessMode()){	return;}

	UpdateBuffer();
}

FrameBuffer::~FrameBuffer()
{
	if (IsHeadlessMode()){	return;}
	
	GlCall(glDeleteFramebuffers(1, &_BufferId));
	GlCall(glDeleteTextures(1, &_Texture));
}
void FrameBuffer::Bind() const
{
	GlCall(glBindFramebuffer(GL_FRAMEBUFFER, _BufferId));
}
void FrameBuffer::UnBind() const
{
	GlCall(glBindFramebuffer(GL_FRAMEBUFFER, 0));
}
void FrameBuffer::UpdateBufferSize(i32 width, i32 heigth)
{
	_Data.Width = width;
	_Data.Height = heigth;
	UpdateBuffer();
}

void FrameBuffer::UpdateBuffer()
{
	if (_BufferId)
	{
		GlCall(glDeleteFramebuffers(1, &_BufferId));
		GlCall(glDeleteTextures(1, &_Texture));
	}

	GlCall(glGenFramebuffers(1, &_BufferId));
	GlCall(glBindFramebuffer(GL_FRAMEBUFFER, _BufferId));

	GlCall(glGenTextures(1, &_Texture));
	GlCall(glBindTexture(GL_TEXTURE_2D, _Texture));


	GlCall(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, _Data.Width, _Data.Height, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr))
	GlCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
	GlCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));

	GlCall(glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, _Texture, 0));

	auto V = glCheckFramebufferStatus(GL_FRAMEBUFFER);
	if (V != GL_FRAMEBUFFER_COMPLETE)
	{
		
		switch (V)
		{
		case GL_FRAMEBUFFER_UNDEFINED:UCodeGEError("Frame buff is UNDEFINED") break;
		case GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT:UCodeGEError("Frame buff is INCOMPLETE_ATTACHMENT"); break;
		case GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT:UCodeGEError("Frame buff is INCOMPLETE_MISSING_ATTACHMENT"); break;
		case GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER:UCodeGEError("Frame buff is GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER ");  break;
		case GL_FRAMEBUFFER_INCOMPLETE_READ_BUFFER:UCodeGEError("Frame buff is GL_FRAMEBUFFER_INCOMPLETE_READ_BUFFER ");  break;
		case GL_FRAMEBUFFER_UNSUPPORTED:UCodeGEError("Frame buff is GL_FRAMEBUFFER_UNSUPPORTED "); break;
		case GL_FRAMEBUFFER_INCOMPLETE_MULTISAMPLE:UCodeGEError("Frame buff is GL_FRAMEBUFFER_INCOMPLETE_MULTISAMPLE ");  break;
		case GL_FRAMEBUFFER_INCOMPLETE_LAYER_TARGETS:UCodeGEError("Frame buff is GL_FRAMEBUFFER_INCOMPLETE_LAYER_TARGETS ");  break;
		default:
			UCodeGEError("default case for incomplete FrameBuffer");
			break;
		}
		UCodeGEError(RenderAPIErrorStartTag " Frame buff is GL_FRAMEBUFFER_InCOMPLETE");
	}
}
ImageData FrameBuffer::GetGPUImageData() const
{
	return Texture::GetGPUImageData(Get_Width(), Get_Height(), _Texture);
}
RenderingEnd