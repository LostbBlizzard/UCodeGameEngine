#pragma once

#include <UCodeRunTime/ULibrarys/Rendering/RenderingNamespace.hpp>
#include <UCodeRunTime/BasicTypes.hpp>
RenderingStart
class IndexBuffer
{
public:
	IndexBuffer(const unsigned int* data, u32 count);
	~IndexBuffer();

	//void UpdateData(const unsigned int* data, unsigned int count);
	void Bind() const;
	void UnBind() const;
private:
	u32 _BufferId;
};
RenderingEnd

