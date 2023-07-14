#include "Texture.hpp"

#include <UCodeRunTime/Rendering/API_OpenGL/OpenGlHelper.hpp>

#include <stb_image/stb_image.h>

#include <UCodeRunTime/CMem.hpp>
RenderingStart


#ifdef DEBUG
const static std::string nullstring = "null";
const static std::string MadewithColor = "madewithColor32";
#else
const static std::string nullstring = "";
const static std::string MadewithColor = "C32";
#endif // DEBUG


void Texture::InitTexture()
{
	GlCall(glGenTextures(1, &_RendererID));
	GlCall(glBindTexture(GL_TEXTURE_2D, _RendererID));

	GlCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST));
	GlCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST));



	GlCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));
	GlCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));

	GlCall(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, _Width, _Height, 0, GL_RGBA, GL_UNSIGNED_BYTE, _Buffer.get()));
	_BufferIsInGPU = true;
}

void Texture::FreeFromGPU()
{
	_BufferIsInGPU = false;
	GlCall(glDeleteTextures(1, &_RendererID));
	_RendererID = 0;
}

Texture::Texture(const Path& filePath)
	: _RendererID(0),_Width(0), _Height(0), _BPP(0), _Buffer(nullptr), _BufferIsInGPU(false),_FilePath(filePath)
{
	stbi_set_flip_vertically_on_load(0);
	_Buffer = Unique_array<Byte>(stbi_load(filePath.generic_string().c_str(), &_Width, &_Height, &_BPP, 4));

	InitTexture();
}

Texture::Texture(SInt32 width, SInt32 height, const Color32* color)
	: _RendererID(0), _Width(width), _Height(height), _BPP(4), _Buffer(nullptr), _BufferIsInGPU(false), _FilePath(MadewithColor)
{
	size_t DataSize = (size_t)width * (size_t)height* sizeof(Color32);
	_Buffer = std::make_unique<Byte[]>(DataSize);
	memcpy(_Buffer.get(),color, DataSize);

	InitTexture();
}

Texture::Texture(const BytesView PngData)
	: _RendererID(0), _Width(0), _Height(0), _BPP(0), _Buffer(nullptr), _BufferIsInGPU(false), _FilePath(MadewithColor)
{
	stbi_set_flip_vertically_on_load(0);
	_Buffer = Unique_array<Byte>(stbi_load_from_memory(PngData.Data(),PngData.Size(), &_Width, &_Height, &_BPP, 4));

	InitTexture();
}

Texture::~Texture()
{	
	
	if (_FilePath == MadewithColor)
	{
		Mem::free(_Buffer.release());
	}
	else if (_FilePath != nullstring)
	{
		stbi_image_free(_Buffer.release());
	}
	
	FreeFromGPU();
}

Unique_ptr<Texture> Texture::MakeNewNullTexture()
{
	#ifdef DEBUG
	const Color32 NullTexureColorData[]
	{
		Color32(),
		Color32(75,0,130),
		Color32(),
		Color32(75,0,130),
	};
	auto tex =std::make_unique<Texture>(2, 2, NullTexureColorData);
	#else
	const Color32 NullTexureColorData[]
	{
		Color32(0,0,0,0),
	};
	Texture* tex = std::make_unique<Texture>(1, 1, NullTexureColorData);
	#endif // DEBUG

	Mem::free(tex->_Buffer.release());
	tex->_Buffer = nullptr;
	return tex;
}

void Texture::UpdateDataToGPU()
{
#ifdef DEBUG
	if (_FilePath != MadewithColor)
	{
		throw std::exception("This texture is read only cannot be Updated");
	}
#endif // DEBUG
	GlCall(glBindTexture(GL_TEXTURE_2D, _RendererID));
	GlCall(glTexSubImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, _Width, _Height, 0, GL_RGBA, GL_UNSIGNED_BYTE, _Buffer.get()))
}

void Texture::MultThread_UpdateTextureFromPath(const Path Path)
{
	Set_FilePath(Path);
	if (_Buffer) {
		Mem::free(_Buffer.release()); _Buffer = nullptr; }

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






RenderingEnd