#include "Texture.hpp"

#include <UCodeRunTime/Rendering/API_OpenGL/OpenGlHelper.hpp>

#include <stb_image/stb_image.h>

RenderingStart


constexpr const char* nullstring = "";
constexpr const char* MadewithColor = "C32";
constexpr const char* PlaceHolderTexture = "_PlaceHolder_";


void Texture::InitTexture()
{
	u32 newid = 0;
	GlCall(glGenTextures(1, &newid));
	GlCall(glBindTexture(GL_TEXTURE_2D, newid));

	GlCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST));
	GlCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST));



	GlCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));
	GlCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));

	GlCall(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, _Width, _Height, 0, GL_RGBA, GL_UNSIGNED_BYTE, _Buffer.get()));
	_BufferIsInGPU = true;

	_RendererID = newid;
}

void Texture::FreeFromGPU()
{
	_BufferIsInGPU = false;
	GlCall(glDeleteTextures(1, &_RendererID.value()));
	_RendererID = {};
}

Texture::Texture()
{

}

Texture::Texture(const Path& filePath)
	: _RendererID(),_Width(0), _Height(0), _BPP(0), _Buffer(nullptr), _BufferIsInGPU(false),_FilePath(filePath)
{
	stbi_set_flip_vertically_on_load(0);
	_Buffer = Unique_array<Byte>(stbi_load(filePath.generic_string().c_str(), &_Width, &_Height, &_BPP, 4));

	InitTexture();
}

Texture::Texture(i32 width, i32 height, const Color32* color)
	: _RendererID(), _Width(width), _Height(height), _BPP(4), _Buffer(nullptr), _BufferIsInGPU(false), _FilePath(MadewithColor)
{
	size_t DataSize = (size_t)width * (size_t)height* sizeof(Color32);
	_Buffer = std::make_unique<Byte[]>(DataSize);
	memcpy(_Buffer.get(),color, DataSize);

	InitTexture();
}

Texture::Texture(const PngDataSpan PngData)
	: _RendererID(), _Width(0), _Height(0), _BPP(0), _Buffer(nullptr), _BufferIsInGPU(false), _FilePath(MadewithColor)
{
	_Buffer = Unique_array<Byte>(stbi_load_from_memory(PngData.Data.Data(), PngData.Data.Size(), &_Width, &_Height, &_BPP, 4));

	InitTexture();
}

void Texture::FreeFromCPU()
{
	if (_FilePath == MadewithColor)
	{
		free(_Buffer.release());
	}
	else if (_FilePath != nullstring)
	{
		stbi_image_free(_Buffer.release());
	}
}

Texture::~Texture()
{	
	if (_Buffer.get()) {
		FreeFromCPU();
	}
	if (_RendererID.has_value()) {
		FreeFromGPU();
	}
}

Texture Texture::MakeNewNullTexture()
{
	const Color32 NullTexureColorData[]
	{
		Color32(),
		Color32(75,0,130),
		Color32(75,0,130),
		Color32(),
	};
	auto tex = Texture(2,2,NullTexureColorData);	 
	return tex;
}
Texture Texture::MakeNewPlaceHolderTexture()
{
	const Color32 NullTexureColorData[]
	{
		Color32(0,0,0,0),
	};
	auto tex = Texture(1, 1,NullTexureColorData);
	tex._FilePath = PlaceHolderTexture;
	return tex;
}
void Texture::RemovePlaceHolderFlag() 
{
	_FilePath = "";
}

void Texture::SetTexture(const PngDataSpan PngData)
{
	stbi_set_flip_vertically_on_load(0);
	_Buffer = Unique_array<Byte>(stbi_load_from_memory(PngData.Data.Data(), PngData.Data.Size(), &_Width, &_Height, &_BPP, 4));
}

void Texture::UpdateDataToGPU()
{
#if UCodeGEDebug
	if (_FilePath != MadewithColor)
	{
		UCodeGEThrow("This texture is read only cannot be Updated");
	}
#endif // DEBUG
	GlCall(glBindTexture(GL_TEXTURE_2D, _RendererID.value()));
	GlCall(glTexSubImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, _Width, _Height, 0, GL_RGBA, GL_UNSIGNED_BYTE, _Buffer.get()))
}

void Texture::MultThread_UpdateTextureFromPath(const Path Path)
{
	Set_FilePath(Path);
	if (_Buffer) {
		free(_Buffer.release()); 
	}

	stbi_set_flip_vertically_on_load(0);
	_Buffer = Unique_array<Byte>(
		stbi_load(_FilePath.generic_string().c_str(), &_Width, &_Height, &_BPP, 4)
		);
	_BufferIsInGPU = false;
}

void Texture::TryUploadTexToGPU()
{
	if (_Buffer && _BufferIsInGPU == false)
	{
		FreeFromGPU();
		InitTexture();
	}
}

bool Texture::IsPlaceHolder() const
{
	return _FilePath == PlaceHolderTexture;
}
void Texture::FlipImageVertically(i32 width, i32 height,Color32* color)
{
	for (size_t x = 0; x < width; x++)
	{
		for (size_t y = 0; y < height/2; y++)
		{
			auto& pixel = color[GetPixelIndex(x, y, width, height)];
			auto& otherpixel = color[GetPixelIndex(x,height - y - 1, width, height)];

			std::swap(pixel, otherpixel);
		}
	}
}
void Texture::FlipImageHorizontally(i32 width, i32 height,Color32* color)
{
	for (size_t x = 0; x < width/2; x++)
	{
		for (size_t y = 0; y < height; y++)
		{
			auto& pixel = color[GetPixelIndex(x, y, width, height)];
			auto& otherpixel = color[GetPixelIndex(width - x - 1, y, width, height)];

			std::swap(pixel, otherpixel);
		}
	}

}

size_t Texture::GetPixelIndex(i32 x, i32 y, i32 width, i32 height)
{
	return (y * width) + x;
}


Texture::Texture(Texture&& source)
{
	new (this) Texture();
	*this = std::move(source);
}
Texture& Texture::operator=(Texture&& source)
{
	if (_Buffer.get()) {
		FreeFromCPU();
	}
	if (_RendererID.has_value()) {
		FreeFromGPU();
	}
	PixelsPerUnit = source.PixelsPerUnit;
	_RendererID = std::move(source._RendererID);
	_Width = source._Width;
	_Height = source._Height;
	_BPP = source._BPP;
	_BufferIsInGPU = std::move(source._BufferIsInGPU);
	_FilePath = std::move(source._FilePath);
	_Buffer = std::move(source._Buffer);


	source._RendererID = {};

	source.~Texture();
	new (&source) Texture;

	return *this;
}
ImageData Texture::GetGPUImageData(i32 Width,i32 Height,GPUTextureID RendererID)
{
	ImageData data;
	data.Width = Width;
	data.Height = Height;
	data._ColorData.resize(Width * Height);

	GlCall(glBindTexture(GL_TEXTURE_2D, RendererID));
	GlCall(glGetTexImage(GL_TEXTURE_2D, 0, GL_RGBA, GL_UNSIGNED_BYTE,data._ColorData.data()))

	return data;
}





RenderingEnd