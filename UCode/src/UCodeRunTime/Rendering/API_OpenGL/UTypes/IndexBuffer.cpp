#include "IndexBuffer.hpp"
#include <UCodeRunTime/Rendering/API_OpenGL/OpenGlHelper.hpp>
RenderingStart
IndexBuffer::IndexBuffer(const unsigned int* data, u32 count)
{
	GlCall(glGenBuffers(1, &_BufferId));																					//Binding the VAO so we can manipulate it
	GlCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _BufferId));

	GlCall(glBufferData(GL_ELEMENT_ARRAY_BUFFER,
		count * sizeof(unsigned int),
		data,
		GL_DYNAMIC_DRAW));
}
IndexBuffer::~IndexBuffer()
{
	GlCall(glDeleteBuffers(1, &_BufferId));
}
void IndexBuffer::Bind() const
{
	GlCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,_BufferId));
}
void IndexBuffer::UnBind() const
{
	GlCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));
}
RenderingEnd