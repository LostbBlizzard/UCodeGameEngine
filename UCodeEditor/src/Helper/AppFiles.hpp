#pragma once
#include <UCodeRunTime/RunTimeBasicTypes/String.hpp>
#include <UCodeRunTime/RunTimeBasicTypes/unordered_map.hpp>
#include <UCodeRunTime/ULibrarys/Rendering/GpuTypes/Sprite.hpp>
#include <UCodeRunTime/ULibrarys/AssetManagement/AssetRendering.hpp>
#include <Editor/EditorNamespace.hpp>
#include <Typedefs.hpp>
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
		MoreData =Null,
		TxtFile = Uility_image,
		GlslFile = Uility_image,

		UCodeEditor_Platform = UPugin,
		//
		Max_Value,
	};
	
	static UCode::Texture* GetTexture(texture tex);
	static UCode::Sprite* GetSprite(sprite tex);

	static Path GetFilesDir();
	static AppFiles::sprite GetSprite(FileHelper::FileType type);
	static UCODE_EDITOR_FORCEINLINE void Set_GameLib(UCode::Gamelibrary* lib) { _GameLib = lib; }
private:
	inline static Unordered_map<texture_t, UCode::Texture*> _textures;
	inline static Unordered_map<Sprite_t, UCode::Sprite*> _sprites;
	inline static UCode::Gamelibrary* _GameLib;
	inline static String ULang_uc;
	
	inline static Vector<Unique_ptr<UCode::Texture>> _LoadedTextures;
	inline static Vector<Unique_ptr<UCode::Sprite>> _LoadedSprite;
};
EditorEnd
