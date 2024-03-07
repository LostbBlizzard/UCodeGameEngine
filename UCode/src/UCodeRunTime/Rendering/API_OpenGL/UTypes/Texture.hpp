#pragma once
#include <UCodeRunTime/ULibrarys/Rendering/RenderingNamespace.hpp>
#include <UCodeRunTime/BasicTypes.hpp>
RenderingStart


class Texture
{
public:
	using Filter_t = Byte;
	enum class Filiter : Filter_t
	{
		Point,
		bilinear,
		trilinear,
	};
	Texture();
	Texture(const Path& filePath);
	Texture(i32 width, i32 height,const Color32* color);
	Texture(const BytesView PngData);
	~Texture();

	static Unique_ptr<Texture> MakeNewNullTexture();

	Texture(Texture&& source);
	Texture& operator=(Texture&& source);

	Texture(const Texture& source) = delete;
	Texture& operator=(const Texture& source) = delete;
	
	void SetTexture(const BytesView PngData);
	void InitTexture();
	void UpdateDataToGPU();


	UCodeGEForceinlne auto Get_RendererID() const {return _RendererID.value();}
	UCodeGEForceinlne const Color32* Get_ColorData() const { return (const Color32*)_Buffer.get(); }
	UCodeGEForceinlne i32 Get_Width() const { return _Width; }
	UCodeGEForceinlne i32 Get_Height() const { return _Height; }

	UCodeGEForceinlne const Path& Get_FilePath() const { return _FilePath; }

	//MultThreading Stuff
	void MultThread_UpdateTextureFromPath(const Path Path);
	void TryUploadTexToGPU();
	UCodeGEForceinlne void Set_FilePath(const Path& path) { _FilePath = path; }//This is used with MultThread_UpdateTextureFromPath;
	UCodeGEForceinlne bool IsUploadedToGPU() { return _BufferIsInGPU; }

	void FreeFromCPU();
	
	u32 PixelsPerUnit = 1;
private:
	Optional<u32> _RendererID;
	i32 _Width = 0, _Height = 0, _BPP = 0;
	Unique_array<Byte> _Buffer;
	bool _BufferIsInGPU =false;
	Path _FilePath;

	
	void FreeFromGPU();
};
RenderingEnd
