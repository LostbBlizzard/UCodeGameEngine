#pragma once
#include <UCodeRunTime/ULibrarys/Rendering/RenderingNamespace.hpp>
#include <UCodeRunTime/BasicTypes.hpp>
RenderingStart


class Texture
{
public:
	Texture();
	Texture(const Path& filePath);
	Texture(i32 width, i32 height,const Color32* color);
	Texture(const BytesView PngData);
	~Texture();

	static Unique_ptr<Texture> MakeNewNullTexture();

	Texture(Texture&& source) = default;
	Texture& operator=(Texture&& source) = default;

	Texture(const Texture& source) = delete;
	Texture& operator=(const Texture& source) = delete;
	
	void SetTexture(const BytesView PngData);
	void InitTexture();
	void UpdateDataToGPU();


	UCodeGameEngineForceinlne auto Get_RendererID() const {return _RendererID;}
	UCodeGameEngineForceinlne const Color32* Get_ColorData() const { return (const Color32*)_Buffer.get(); }
	UCodeGameEngineForceinlne i32 Get_Width() const { return _Width; }
	UCodeGameEngineForceinlne i32 Get_Height() const { return _Height; }

	UCodeGameEngineForceinlne const Path& Get_FilePath() const { return _FilePath; }

	//MultThreading Stuff
	void MultThread_UpdateTextureFromPath(const Path Path);
	void TryUploadTexToGPU();
	UCodeGameEngineForceinlne void Set_FilePath(const Path& path) { _FilePath = path; }//This is used with MultThread_UpdateTextureFromPath;
	UCodeGameEngineForceinlne bool IsUploadedToGPU() { return _BufferIsInGPU; }
private:
	u32 _RendererID;
	i32 _Width, _Height, _BPP;
	Unique_array<Byte> _Buffer;
	bool _BufferIsInGPU;
	Path _FilePath;

	
	void FreeFromGPU();
};
RenderingEnd
