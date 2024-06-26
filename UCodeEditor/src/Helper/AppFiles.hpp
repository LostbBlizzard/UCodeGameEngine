#pragma once
#include <Editor/EditorNamespace.hpp>
#include <UCodeRunTime/BasicTypes.hpp>
#include <UCodeRunTime/ULibrarys/Rendering/GpuTypes/Sprite.hpp>
#include <UCodeRunTime/ULibrarys/AssetManagement/AssetRendering.hpp>
#include <Typedefs.hpp>
#include <Asyn.hpp>
#include "FileHelper.hpp"
EditorStart
class AppFiles
{
public:
	typedef size_t texture_t;
	enum class texture : texture_t
	{
		Null,
		folder,
		file,
		Uility_image,
		DLL_image,
		ULangScript,
		ULangLib,
		Entity,
		Scene,
		RawEntityData,
		Cam,
		SpriteRenderer,
		Play_NotPlaying,
		Play_Play,
		Paused_Blue,
		Paused_Green,
		AppIcon,
		Windows_Platform,
		Web_Platform,
		Android_Platform,
		Max_Value,
	};
	typedef size_t Sprite_t;
	enum class sprite : Sprite_t
	{
		Null,
		Dir_folder_image,
		File_image,
		Uility_image,
		DLL_image,
		ULangScript,
		ULangLib,
		Entity,
		Scene2dData,
		RawEntityData,
		Camera2d,
		SpriteRenderer,
		Play_Green,
		Play_Blue,
		Paused_Blue,
		Paused_Green,
		AppIcon,

		Windows_Platform,
		Web_Platform,
		Android_Platform,

		//

		Sever_Platform = Uility_image,
		UPugin = Uility_image,
		Property = Uility_image,
		MoreData = Null,
		TxtFile = Uility_image,
		GlslFile = Uility_image,
		ScirptableObjectFile = Uility_image,
		ULangModule = Uility_image,

		UCodeEditor_Platform = UPugin,
		UCodeAssetIcon =RawEntityData,

		TileMap = Uility_image,
		TilePalette = Uility_image,
		TileAsset = Uility_image,
		TilePack = Uility_image,
		SpriteAsset = AppIcon,
		TextureAsset =AppIcon,

		AudioIcon = Uility_image,
		Linux_Platform,
		Mac_Platform,
		IOS_Platform,

		EditorOnlyDir_folder_image =AppIcon,

		//
		Max_Value,
	};
	static void Init(UCode::Gamelibrary* lib);
	static bool IsLoaded(texture tex);
	static UCode::Texture* GetTexture(texture tex);
	static AsynTask_t<UCode::Texture*> AsynGetTexture(texture tex);
	static UCode::Sprite* GetSprite(sprite tex);

	static String ReadFileAsString(const Path& Path);
	static Unique_Bytes ReadFileAsBytes(const Path& Path);
	static Unique_Bytes ReadFileAsBytes(const Path& Path, size_t Offset, size_t Size);


	static AsynTask_t<String> AsynReadFileString(const Path& Path);
	static AsynTask_t<Unique_Bytes> AsynReadFileAsBytes(const Path& Path);
	static AsynTask_t<Unique_Bytes> AsynReadFileAsBytes(const Path& Path, size_t Offset, size_t Size);
	static AsynTask_t<Path> AsynGetFilePathByMove(const Path& Path);


	static Path GetFilePathByMove(const Path& Path);
	static bool CopyFile(const Path& path, const Path& outpath);


	static AppFiles::sprite GetSprite(FileHelper::FileType type);
private:
	inline static Unordered_map<texture_t, UCode::Texture*> _textures;
	inline static Unordered_map<Sprite_t, UCode::Sprite*> _sprites;
	inline static Unordered_map<texture_t, bool> _loadtextures;
	inline static UCode::Gamelibrary* _GameLib;
	inline static String ULang_uc;
	
	inline static Vector<Unique_ptr<UCode::Texture>> _LoadedTextures;
	inline static Vector<Unique_ptr<UCode::Sprite>> _LoadedSprite;
	inline static UCode::GameFiles* GetGameFiles()
	{
		return UCode::GameFiles::Get(_GameLib);
	}
};
EditorEnd
