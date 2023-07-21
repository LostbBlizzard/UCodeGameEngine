#include "AppFiles.hpp"
EditorStart


#ifdef DEBUG
const char* nullTexPath = "_NullTex";
#else
const char* nullTexPath = "";
#endif // DEBUG
constexpr SInt32 SetTodefault = UCode::Sprite::GetTexureSize;
struct SpriteInfo
{   
    AppFiles::texture tex;
    
    SInt32 X, Y, SizeX, SizeY;
    constexpr SpriteInfo(AppFiles::texture texure) : tex(texure)
        , X(SetTodefault), Y(SetTodefault), SizeX(SetTodefault), SizeY(SetTodefault)
    {

    }
    constexpr SpriteInfo(AppFiles::texture texure, SInt32 x, SInt32 y, SInt32 sizeX, SInt32 sizey)
        : tex(texure)
        , X(x), Y(y), SizeX(sizeX), SizeY(sizey)
    {

    }
};
const String TexturePaths[]
{
    nullTexPath,
    "art/folder-outline.png",//folder
    "art/reader-outline.png",//file
    "art/build-outline.png",//Uility_image
    "art/cog-outline.png",//DLL_image
    "art/UCodeLangIcon.png", //ULangScript
    "art/cog-outline.png",//ULangLib
    "art/cube-outline.png",//Entity
    "art/prism-outline.png",//Scene
    "art/cube-outline.png",//RawEntityData 10
    "art/camera-outline.png",//Cam
    "art/image-outline.png",//SpriteRenderer
    "art/caret-forward-outline.png",//Play_NotPlaying
    "art/caret-forward-circle-outline.png",//Play_Play

    "art/21 - UCodeIcons.png",//Paused_Blue
    "art/22 - UCodeIcons.png",//Paused_Green
    "art/UCodeEngineLogo.png",//AppIcon
    "art/windows-icon.png",
    "art/web-icon.png",
    "art/android-icon.png" //20
};

const SpriteInfo SpriteInfos[]
{
    SpriteInfo(AppFiles::texture::Null),
    SpriteInfo(AppFiles::texture::folder),
    SpriteInfo(AppFiles::texture::file),
    SpriteInfo(AppFiles::texture::Uility_image),
    SpriteInfo(AppFiles::texture::DLL_image),
    SpriteInfo(AppFiles::texture::ULangScript),
    SpriteInfo(AppFiles::texture::ULangLib),
    SpriteInfo(AppFiles::texture::Entity),
    SpriteInfo(AppFiles::texture::Scene),
    SpriteInfo(AppFiles::texture::RawEntityData),
    SpriteInfo(AppFiles::texture::Cam),//10
    SpriteInfo(AppFiles::texture::SpriteRenderer),

    SpriteInfo(AppFiles::texture::Play_NotPlaying),
    SpriteInfo(AppFiles::texture::Play_Play),

    SpriteInfo(AppFiles::texture::Paused_Blue),
    SpriteInfo(AppFiles::texture::Paused_Green),
    SpriteInfo(AppFiles::texture::AppIcon),

    SpriteInfo(AppFiles::texture::Windows_Platform),
    SpriteInfo(AppFiles::texture::Web_Platform),
    SpriteInfo(AppFiles::texture::Android_Platform),//20
};

constexpr size_t TexturePaths_Size = sizeof(TexturePaths) / sizeof(TexturePaths[0]);
constexpr size_t SpriteInfos_Size = sizeof(SpriteInfos) / sizeof(SpriteInfos[0]);

#ifdef PUBLISHED
const char* AppFilesPath = "UFiles.data";
const char* AppFilesPathDir = "UFiles//";
#else
const char* AppFilesPath = UCodeEditor_VS_PROJECTPATH  "UFiles.data";
const char* AppFilesPathDir = UCodeEditor_VS_PROJECTPATH "UFiles//";
#endif


void AppFiles::Init(UCode::Gamelibrary* lib)
{
    Path path;
    if (std::filesystem::exists(AppFilesPath))
    {
        path = AppFilesPath;
    }
    else
    {
        path = AppFilesPathDir;
    }

    _GameLib = lib;
    UCode::GameFilesData f;
    if (std::filesystem::is_directory(path))
    {
        f._Type = UCode::GameFilesData::Type::Redirect;
        f._RedirectDir = path;
        GetGameFiles()->Init(lib, f);
    }
    else
    {
        UCode::FileBuffer Buffer;
        if (UCode::GameFilesData::ReadFileKeepOpen(path, Buffer, f))
        {
            UCode::GameFiles* gamefiles = UCode::GameFiles::Init(lib, f);
            gamefiles->SetFileBuffer(std::move(Buffer));

        }
    }
}

bool AppFiles::IsLoaded(texture tex)
{
    return _loadtextures[(texture_t)tex];
}
UCode::Texture* AppFiles::GetTexture(texture tex)
{
    texture_t id = (texture_t)tex;
    if (id > TexturePaths_Size) {id = (texture_t)texture::Null;}
    if (_textures.count(id))
    {
        return _textures[id];
    }
    else
    {


        const String& Path = TexturePaths[id];



        Unique_ptr<UCode::Texture> newtext;

        if (Path == nullTexPath)
        {
            newtext = std::move(UCode::Texture::MakeNewNullTexture());
        }
        else
        {
            newtext = std::move(UCode::Texture::MakeNewNullTexture());
            UCode::GameFiles* gamefiles = UCode::GameFiles::Get(_GameLib);

            auto Func2 = [id](Unique_Bytes Bytes)
            {
                return UCode::AssetRendering::LoadTextureAsync(_GameLib, Bytes.AsView())
                    .ContinueOnMainThread(
                        [id](Unique_ptr<UCode::Texture>&& Val)
                        {
                            _textures[id] = Val.release();
                            _loadtextures[id] = true;

                            return std::move(Val);
                        });
            };

            gamefiles->AsynReadGameFileFullBytes(Path)
                .ContinueOnAnyThread(
                    [id](Unique_Bytes Bytes)
                    {
                        return UCode::AssetRendering::LoadTextureAsync(_GameLib, Bytes.AsView())
                            .ContinueOnMainThread(
                                [id](Unique_ptr<UCode::Texture>&& Val)
                                {
                                    _textures[id] = Val.release();
                                    _loadtextures[id] = true;

                                    return std::move(Val);
                                }
                        );
                    }
            );
        }
        
        auto Tex = newtext.get();

        _textures[id] = Tex;

        _LoadedTextures.push_back(std::move(newtext));
        return  Tex;
    }
}

AsynTask_t<UCode::Texture*> AppFiles::AsynGetTexture(texture tex)
{
    texture_t id = (texture_t)tex;
    if (id > TexturePaths_Size) { id = (texture_t)texture::Null; }
    if (_textures.count(id))
    {
        AsynTask_t<UCode::Texture*> R;
        R.SetValue(std::move(_textures[id]));
        return R;
    }
    else
    {


        const String& Path = TexturePaths[id];



        Unique_ptr<UCode::Texture> newtext;

        if (Path == nullTexPath)
        {
            auto Tex =UCode::Texture::MakeNewNullTexture();
            AsynTask_t<UCode::Texture*> R;
            R.SetValue(Tex.get());
            _LoadedTextures.push_back(std::move(Tex));
            return R;
        }
        else
        {
            UCode::GameFiles* gamefiles = UCode::GameFiles::Get(_GameLib);


            auto Task = std::move(gamefiles->AsynReadGameFileFullBytes(Path)
                .OnCompletedOnAnyThread(
                    [id](Unique_Bytes Bytes)
                    {
                       UCode::AssetRendering::LoadTextureAsync(_GameLib, Bytes.AsView())
                            .OnCompletedOnMainThread(
                                [id](Unique_ptr<UCode::Texture>&& Val)
                                {
                                    auto RVal = Val.get();
                                    _textures[id] = Val.release();
                                    _loadtextures[id] = true;
                                    return RVal;
                                });
                    }));

            AsynTask_t<UCode::Texture*> R;
            memcpy(&R, &Task, sizeof(AsynTask_t<UCode::Texture*>));
            
            return R;
        }
    }
}
UCode::Sprite* AppFiles::GetSprite(sprite tex)
{
    Sprite_t id = (Sprite_t)tex;
    if (id > SpriteInfos_Size) { id = (Sprite_t)sprite::Null; }
    
    if (_sprites.count(id))
    {
        const SpriteInfo& info = SpriteInfos[id];
        if (_loadtextures[(texture_t)info.tex]) 
        {
            auto tex = GetTexture(info.tex);

            SInt32 _X, _Y, _SizeX, _SizeY;

            if (info.X == SetTodefault) { _X = 0; }
            else { _X = info.X; }

            if (info.Y == SetTodefault) { _Y = 0; }
            else { _Y = info.Y; }

            if (info.SizeX == SetTodefault) { _SizeX = tex->Get_Width(); }
            else { _SizeX = info.SizeX; }

            if (info.SizeY == SetTodefault)
            {
                _SizeY = tex->Get_Height();
            }
            else { _SizeY = info.SizeY; }

            auto& R = _sprites[id];
            R->Set_Xoffset(_X);
            R->Set_Yoffset(_Y);

            R->Set_Width(_SizeX);
            R->Set_Height(_SizeY);
            R->Set_texture(tex);

            return R;
        }
        else
        {
            return  _sprites[id];
        }
    }
    else
    {
        const SpriteInfo& info = SpriteInfos[id];

        auto tex = GetTexture(info.tex);
        SInt32 _X, _Y, _SizeX, _SizeY;

        if (info.X == SetTodefault) { _X = 0; }
        else { _X = info.X; }

        if (info.Y == SetTodefault) { _Y = 0; }
        else { _Y = info.Y; }

        if (info.SizeX == SetTodefault) { _SizeX = UCode::Sprite::GetTexureSize; }
        else { _SizeX = info.SizeX; }

        if (info.SizeY == SetTodefault) 
        { _SizeY = UCode::Sprite::GetTexureSize; }
        else { _SizeY = info.SizeY; }

        Unique_ptr<UCode::Sprite> newSpr = std::make_unique<UCode::Sprite>(tex, _X, _Y, _SizeX, _SizeY);
        UCode::Sprite* newSprite = newSpr.get();
        _sprites[id] = newSprite;

        _LoadedSprite.push_back(std::move(newSpr));
        return newSprite;
    }
}

String AppFiles::ReadFileAsString(const Path& path)
{
    return GetGameFiles()->ReadFileAsString(path);
}

Unique_Bytes AppFiles::ReadFileAsBytes(const Path& path)
{
    return  GetGameFiles()->ReadGameFileAsBytes(path);
}

Unique_Bytes AppFiles::ReadFileAsBytes(const Path& path, size_t Offset, size_t Size)
{
    return  GetGameFiles()->ReadFileAsBytes(path,Offset,Size);
}



AsynTask_t<Path> AppFiles::AsynGetFilePathByMove(const Path& path)
{
    return GetGameFiles()->AsynGetGameFilePathByMove(path);
}

Path AppFiles::GetFilePathByMove(const Path& path)
{
    return  GetGameFiles()->GetGameFilePathByMove(path);
}

AsynTask_t<String> AppFiles::AsynReadFileString(const Path& path)
{
    return GetGameFiles()->AsynReadFileString(path);
}

AsynTask_t<Unique_Bytes> AppFiles::AsynReadFileAsBytes(const Path& path)
{
    return GetGameFiles()->AsynReadGameFileFullBytes(path);
}

AsynTask_t<Unique_Bytes> AppFiles::AsynReadFileAsBytes(const Path& path, size_t Offset, size_t Size)
{
    return GetGameFiles()->AsynReadGameFileBytes(path,Offset,Size);
}

AppFiles::sprite AppFiles::GetSprite(FileHelper::FileType type)
{
    switch (type)
    {
    case UCodeEditor::FileHelper::FileType::Null:return AppFiles::sprite::File_image;
    case UCodeEditor::FileHelper::FileType::File:return AppFiles::sprite::File_image;
    case UCodeEditor::FileHelper::FileType::Dir:return AppFiles::sprite::Dir_folder_image;
    case UCodeEditor::FileHelper::FileType::File_Image_Type:return AppFiles::sprite::File_image;
    case UCodeEditor::FileHelper::FileType::SceneData:return AppFiles::sprite::Scene2dData;
    case UCodeEditor::FileHelper::FileType::RawEntityData:return AppFiles::sprite::RawEntityData;
    case UCodeEditor::FileHelper::FileType::ULangFile:return AppFiles::sprite::ULangScript;
    case UCodeEditor::FileHelper::FileType::ULangLib:return AppFiles::sprite::ULangLib;
    case UCodeEditor::FileHelper::FileType::UPugin:return AppFiles::sprite::UPugin;
    case UCodeEditor::FileHelper::FileType::Dll_File:return AppFiles::sprite::DLL_image;
    case UCodeEditor::FileHelper::FileType::metaFile:return AppFiles::sprite::File_image;
    case UCodeEditor::FileHelper::FileType::Color:return AppFiles::sprite::File_image;
    case UCodeEditor::FileHelper::FileType::TxtFile:return AppFiles::sprite::TxtFile;
    case UCodeEditor::FileHelper::FileType::glslFile:return AppFiles::sprite::GlslFile;
    default:return AppFiles::sprite::File_image;
    }
}

EditorEnd