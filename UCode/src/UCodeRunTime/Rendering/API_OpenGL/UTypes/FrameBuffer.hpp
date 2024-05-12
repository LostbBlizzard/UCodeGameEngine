#pragma once

#include <UCodeRunTime/ULibrarys/Rendering/RenderingNamespace.hpp>
#include <UCodeRunTime/BasicTypes.hpp>
#include "GPUTypeDef.hpp"
#include "UCodeRunTime/ULibrarys/Rendering/ImageData.hpp"
RenderingStart

struct FrameBufferData
{
	i32 Width=1, Height=1;
	i32 Samples = 1;
	bool SwapChinTarget = false;
};

class FrameBuffer
{
public:
	FrameBuffer(FrameBufferData data);
	~FrameBuffer();


	void Bind() const;
	void UnBind() const;

	void UpdateBufferSize(i32 width,i32 heigth);
	void UpdateBuffer();

	inline GPUTextureID Get_TextureId() const { return _Texture; }
	inline i32 Get_Width() const { return _Data.Width; }
	inline i32 Get_Height() const  { return _Data.Height; }
	inline bool Is_SwapChinTarget() const { return _Data.SwapChinTarget; }
	
	inline void Set_SwapChinTarget(bool V) { _Data.SwapChinTarget =V; }

	ImageData GetGPUImageData() const;	
private:
	u32 _BufferId;
	GPUTextureID _Texture;
	FrameBufferData _Data;
};
RenderingEnd
