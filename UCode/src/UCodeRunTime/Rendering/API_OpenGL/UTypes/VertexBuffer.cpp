#include "VertexBuffer.hpp"
#include <UCodeRunTime/Rendering/API_OpenGL/OpenGlHelper.hpp>
RenderingStart

static_assert(sizeof(size_t) == sizeof(GLsizeiptr), "GLsizeiptr and size_t are not the sametype");

VertexBuffer::VertexBuffer(const void* data, size_t size)
{
	GlCall(glGenBuffers(1, &_BufferId));																					//Binding the VAO so we can manipulate it
	GlCall(glBindBuffer(GL_ARRAY_BUFFER, _BufferId));

	GlCall(glBufferData(GL_ARRAY_BUFFER,
		(GLsizeiptr)size,
		data,
		GL_DYNAMIC_DRAW));
}


VertexBuffer::~VertexBuffer()
{
	GlCall(glDeleteBuffers(1, &_BufferId));
}
void VertexBuffer::UpdateData(const void* data, size_t  size,SInt32 DataOffset)
{
	Bind();
	GlCall(glBufferSubData(GL_ARRAY_BUFFER, DataOffset, (GLsizeiptr)size,data));
}
void VertexBuffer::Bind() const
{
	GlCall(glBindBuffer(GL_ARRAY_BUFFER,_BufferId));
}
void VertexBuffer::UnBind() const
{
	GlCall(glBindBuffer(GL_ARRAY_BUFFER, 0));
}
RenderingEnd