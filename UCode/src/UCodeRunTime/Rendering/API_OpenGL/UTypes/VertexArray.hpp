#pragma once

#include <UCodeRunTime/ULibrarys/Rendering/RenderingNamespace.hpp>
#include "VertexBuffer.hpp"
#include <UCodeRunTime/BasicTypes.hpp>
RenderingStart

class VertexBufferLayout
{
private:
	u32 _Stride;

	

	
public:
	struct Element
	{	
		u32 type;
		u32 Count;
		
		unsigned char nromalized;
		static u32 GetSizeOfType(u32 GlType);
	};
	Vector<Element> _Element;
	VertexBufferLayout();

	template<typename T>
	void Push(u32 count)
	{
	 static_assert(true,"VertexBufferLayout Has no implementation Push for Type 'T'");
	};
	
	void Push_float(u32 count);
	void Push_Uint(u32 count);
	void Push_int(u32 count);
	void Push_Uchar(u32 count);
	inline const auto& Get_Elements()const { return _Element; }
	inline u32 GetStride() const {
		return _Stride;
	}
	
};

class VertexArray
{
private:
	u32 _BufferId;

public:
	VertexArray();
	~VertexArray();
	void Bind()  const;
	void UnBind() const;
	
	inline auto Get_ID(){ return _BufferId; }

	void AddBuffer(const VertexBuffer& buffer, const VertexBufferLayout& layout);
	inline void AddBuffer(const VertexBuffer* buffer, const VertexBufferLayout& layout)
	{
		AddBuffer(buffer[0], layout);
	}
};
RenderingEnd
