#include "UniformBuffer.hpp"
#include <UCodeRunTime/Rendering/API_OpenGL/OpenGlHelper.hpp>

RenderingStart
UniformBuffer::UniformBuffer(size_t BufferSize)
{
	glGenBuffers(1, &id);
	glBindBuffer(GL_UNIFORM_BUFFER, id);
	glBufferData(GL_UNIFORM_BUFFER, BufferSize, NULL, GL_STATIC_DRAW);
	glBindBuffer(GL_UNIFORM_BUFFER, 0);
}

void UniformBuffer::UpdateData(void* Data, size_t Size)
{
	glBindBuffer(GL_UNIFORM_BUFFER, id);
	glBufferSubData(GL_UNIFORM_BUFFER, 0, Size, Data);
	glBindBuffer(GL_UNIFORM_BUFFER, 0);
}

UniformBuffer::~UniformBuffer()
{

}
RenderingEnd