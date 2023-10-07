#pragma once

#include <UCodeRunTime/ULibrarys/Rendering/RenderingNamespace.hpp>
#include <UCodeRunTime/BasicTypes.hpp>
RenderingStart
class VertexBuffer
{
public:
	VertexBuffer(const void* data, size_t size);
	~VertexBuffer();

	void UpdateData(const void* data, size_t size,i32 DataOffset = 0);
	void Bind() const;
	void UnBind() const;
private:
	u32 _BufferId;
};
RenderingEnd

