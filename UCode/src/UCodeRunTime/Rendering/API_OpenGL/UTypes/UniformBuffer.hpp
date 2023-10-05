#pragma once

#include <UCodeRunTime/ULibrarys/Rendering/RenderingNamespace.hpp>
#include <UCodeRunTime/RunTimeBasicTypes/IntTypes.hpp>
RenderingStart
class UniformBuffer
{
public:
	UniformBuffer(size_t BufferSize);
	void UpdateData(void* Data, size_t Size);
	~UniformBuffer();
private:
	u32 id;
};
RenderingEnd
