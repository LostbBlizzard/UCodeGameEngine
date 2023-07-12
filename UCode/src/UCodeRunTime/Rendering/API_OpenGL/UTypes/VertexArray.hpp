#pragma once

#include <UCodeRunTime/ULibrarys/Rendering/RenderingNamespace.hpp>
#include "VertexBuffer.hpp"
#include <UCodeRunTime/RunTimeBasicTypes/Vector.hpp>
#include <UCodeRunTime/RunTimeBasicTypes/IntTypes.hpp>
RenderingStart

class VertexBufferLayout
{
private:
	UInt32 _Stride;

	

	
public:
	struct Element
	{	
		UInt32 type;
		UInt32 Count;
		
		unsigned char nromalized;
		static UInt32 GetSizeOfType(UInt32 GlType);
	};
	Vector<Element> _Element;
	VertexBufferLayout();

	template<typename T>
	void Push(UInt32 count)
	{
		S_ASSERT(NAMEOF(VertexBufferLayout) " Has no implementation Push for Type "NAMEOF(T));
	};
	
	void Push_float(UInt32 count);
	void Push_Uint(UInt32 count);
	void Push_int(UInt32 count);
	void Push_Uchar(UInt32 count);
	inline const auto& Get_Elements()const { return _Element; }
	inline UInt32 GetStride() const {
		return _Stride;
	}
	
};

class VertexArray
{
private:
	UInt32 _BufferId;

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
