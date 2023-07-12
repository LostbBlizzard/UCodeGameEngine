#pragma once

#include <UCodeRunTime/ULibrarys/Rendering/RenderingNamespace.hpp>
#include <UCodeRunTime/RunTimeBasicTypes/IntTypes.hpp>
RenderingStart
class VertexBuffer
{
public:
	VertexBuffer(const void* data, size_t size);
	~VertexBuffer();

	void UpdateData(const void* data, size_t size,SInt32 DataOffset = 0);
	void Bind() const;
	void UnBind() const;
private:
	UInt32 _BufferId;
};
RenderingEnd

