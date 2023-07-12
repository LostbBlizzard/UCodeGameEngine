#pragma once

#include <UCodeRunTime/ULibrarys/Rendering/RenderingNamespace.hpp>
#include <UCodeRunTime/RunTimeBasicTypes/IntTypes.hpp>
RenderingStart

struct FrameBufferData
{
	SInt32 Width=1, Height=1;
	SInt32 Samples = 1;
	bool SwapChinTarget = false;
};

class FrameBuffer
{
public:
	FrameBuffer(FrameBufferData data);
	~FrameBuffer();


	void Bind() const;
	void UnBind() const;

	void UpdateBufferSize(SInt32 width,SInt32 heigth);
	void UpdateBuffer();
	inline UInt32 Get_TextureId() const { return _Texture; }
	inline SInt32 Get_Width() { return _Data.Width; }
	inline SInt32 Get_Height() { return _Data.Height; }
	inline bool Is_SwapChinTarget() { return _Data.SwapChinTarget; }
	inline void Set_SwapChinTarget(bool V) { _Data.SwapChinTarget =V; }
private:
	UInt32 _BufferId;
	UInt32 _Texture;
	FrameBufferData _Data;
};
RenderingEnd
