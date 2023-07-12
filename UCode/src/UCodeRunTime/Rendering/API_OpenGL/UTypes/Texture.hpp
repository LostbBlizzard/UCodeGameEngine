#pragma once
#include <UCodeRunTime/ULibrarys/Rendering/RenderingNamespace.hpp>
#include <UCodeRunTime/RunTimeBasicTypes/String.hpp>
#include <UCodeRunTime/RunTimeBasicTypes/Color.hpp>
#include <UCodeRunTime/RunTimeBasicTypes/Ref.hpp>
RenderingStart


class Texture
{
public:
	Texture(const Path& filePath);
	Texture(SInt32 width, SInt32 height,const Color32* color);
	~Texture();

	static Unique_ptr<Texture> MakeNewNullTexture();

	Texture(Texture&& source) = default;
	Texture& operator=(Texture&& source) = default;

	Texture(const Texture& source) = delete;
	Texture& operator=(const Texture& source) = delete;
	
	void UpdateDataToGPU();


	UCODE_ENGINE_FORCE_INLINE auto Get_RendererID() const {return _RendererID;}
	UCODE_ENGINE_FORCE_INLINE const Color32* Get_ColorData() const { return (const Color32*)_Buffer.get(); }
	UCODE_ENGINE_FORCE_INLINE SInt32 Get_Width() const { return _Width; }
	UCODE_ENGINE_FORCE_INLINE SInt32 Get_Height() const { return _Height; }

	UCODE_ENGINE_FORCE_INLINE const Path& Get_FilePath() const { return _FilePath; }

	//MultThreading Stuff
	void MultThread_UpdateTextureFromPath(const Path Path);
	void TryUploadTexToGPU();
	UCODE_ENGINE_FORCE_INLINE void Set_FilePath(const Path& path) { _FilePath = path; }//This is used with MultThread_UpdateTextureFromPath;
	UCODE_ENGINE_FORCE_INLINE bool IsUploadedToGPU() { return _BufferIsInGPU; }
private:
	UInt32 _RendererID;
	SInt32 _Width, _Height, _BPP;
	Unique_array<Byte> _Buffer;
	bool _BufferIsInGPU;
	Path _FilePath;

	void InitTexture();
	void FreeFromGPU();
};
RenderingEnd
