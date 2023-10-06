#include "VertexArray.hpp"

#include <UCodeRunTime/Rendering/API_OpenGL/OpenGlHelper.hpp>
RenderingStart
VertexArray::VertexArray()
{
	GlCall(glGenVertexArrays(1, &_BufferId));//Generating VAO
	GlCall(glBindVertexArray(_BufferId));
}
void VertexArray::Bind() const
{
	GlCall(glBindVertexArray(_BufferId));
}
void VertexArray::UnBind() const
{
	GlCall(glBindVertexArray(0));
}
VertexArray::~VertexArray()
{
	GlCall(glDeleteVertexArrays(1,&_BufferId));
}
void VertexArray::AddBuffer(const VertexBuffer& buffer, const VertexBufferLayout& layout)
{
	Bind();
	buffer.Bind();
	const auto& elements = layout.Get_Elements();


	size_t offset = 0;
	for (GLuint i = 0; i < elements.size(); i++)
	{
		const auto& element = elements[i];
		GlCall(glEnableVertexAttribArray(i));
		GlCall(glVertexAttribPointer(i, element.Count, element.type, element.nromalized
			,layout.GetStride(), (const void*)offset)); //No need to make changes here as our data type (GLfloat) is the same. In later examples we will be changing it.
		
		offset += (size_t)element.Count * (size_t)VertexBufferLayout::Element::GetSizeOfType(element.type);
	}
}

VertexBufferLayout::VertexBufferLayout():_Stride(0)
{
}
u32 VertexBufferLayout::Element::GetSizeOfType(u32 GlType)
{
	switch (GlType)
	{
	case GL_FLOAT:return sizeof(GLfloat);
	case GL_INT:return sizeof(GLint);
	case GL_UNSIGNED_INT:return sizeof(GLuint);
	case GL_UNSIGNED_BYTE:return sizeof(GLubyte);
	case GL_UNSIGNED_SHORT:return sizeof(GLushort);
	default:
		UCodeGameEngineUnreachable();
		return -1;
	}
}
void VertexBufferLayout::Push_float(u32 count)
{
	_Element.push_back({ GL_FLOAT,count ,GL_FALSE });
	_Stride += count * Element::GetSizeOfType(GL_FLOAT);
}
void VertexBufferLayout::Push_Uint(u32 count)
{
	_Element.push_back({ GL_UNSIGNED_INT,count ,GL_FALSE });
	_Stride += count * Element::GetSizeOfType(GL_UNSIGNED_INT);
}
void VertexBufferLayout::Push_int(u32 count)
{
	_Element.push_back({ GL_INT,count ,GL_FALSE });
	_Stride += count * Element::GetSizeOfType(GL_INT);
}
void VertexBufferLayout::Push_Uchar(u32 count)
{
	_Element.push_back({ GL_UNSIGNED_BYTE,count ,GL_FALSE });
	_Stride += count * Element::GetSizeOfType(GL_UNSIGNED_BYTE);
}

RenderingEnd


