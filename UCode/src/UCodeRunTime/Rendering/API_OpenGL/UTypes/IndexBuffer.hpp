#pragma once

#include <UCodeRunTime/ULibrarys/Rendering/RenderingNamespace.hpp>
#include <UCodeRunTime/RunTimeBasicTypes/IntTypes.hpp>
RenderingStart
class IndexBuffer
{
public:
	IndexBuffer(const unsigned int* data, UInt32 count);
	~IndexBuffer();

	//void UpdateData(const unsigned int* data, unsigned int count);
	void Bind() const;
	void UnBind() const;
private:
	UInt32 _BufferId;
};
RenderingEnd

