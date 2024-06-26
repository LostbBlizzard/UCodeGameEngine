#include "AppFiles.hpp"
EditorStart

#if UCodeGEDebug
    const char *nullTexPath = "_NullTex";
#else
    const char *nullTexPath = "";
#endif // DEBUG
constexpr i32 SetTodefault = UC::Sprite::GetTexureSize;
struct SpriteInfo
{
    AppFiles::texture tex;

    i32 X, Y, SizeX, SizeY;
    constexpr SpriteInfo(AppFiles::texture texure) : tex(texure), X(SetTodefault), Y(SetTodefault), SizeX(SetTodefault), SizeY(SetTodefault)
    {
    }
    constexpr SpriteInfo(AppFiles::texture texure, i32 x, i32 y, i32 sizeX, i32 sizey)
        : tex(texure), X(x), Y(y), SizeX(sizeX), SizeY(sizey)
    {
    }
};
const String TexturePaths[]{
    nullTexPath,
    "art/folder-outline.png",               // folder
    "art/reader-outline.png",               // file
    "art/build-outline.png",                // Uility_image
    "art/cog-outline.png",                  // DLL_image
    "art/UCodeLangIcon.png",                // ULangScript
    "art/cog-outline.png",                  // ULangLib
    "art/cube-outline.png",                 // Entity
    "art/prism-outline.png",                // Scene
    "art/cube-outline.png",                 // RawEntityData 10
    "art/camera-outline.png",               // Cam
    "art/image-outline.png",                // SpriteRenderer
    "art/caret-forward-outline.png",        // Play_NotPlaying
    "art/caret-forward-circle-outline.png", // Play_Play

    "art/caret-forward-outline.png",        // Paused_Blue
    "art/caret-forward-circle-outline.png", // Paused_Green
    "art/UCodeEngineLogo.png",              // AppIcon
    "art/windows-icon.png",
    "art/web-icon.png",
    "art/android-icon.png" // 20
};

const SpriteInfo SpriteInfos[]{
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
    SpriteInfo(AppFiles::texture::Cam), // 10
    SpriteInfo(AppFiles::texture::SpriteRenderer),

    SpriteInfo(AppFiles::texture::Play_NotPlaying),
    SpriteInfo(AppFiles::texture::Play_Play),

    SpriteInfo(AppFiles::texture::Paused_Blue),
    SpriteInfo(AppFiles::texture::Paused_Green),
    SpriteInfo(AppFiles::texture::AppIcon),

    SpriteInfo(AppFiles::texture::Windows_Platform),
    SpriteInfo(AppFiles::texture::Web_Platform),
    SpriteInfo(AppFiles::texture::Android_Platform), // 20
};

constexpr size_t TexturePaths_Size = sizeof(TexturePaths) / sizeof(TexturePaths[0]);
constexpr size_t SpriteInfos_Size = sizeof(SpriteInfos) / sizeof(SpriteInfos[0]);

#if PUBLISHED || RELASE
const char *AppFilesPath = "UFiles.data";
const char *AppFilesPathDir = "UFiles/";
#else
const char *AppFilesPath = UCodeEditor_VS_PROJECTPATH "UFiles.data";
const char *AppFilesPathDir = UCodeEditor_VS_PROJECTPATH "UFiles/";
#endif

void AppFiles::Init(UC::Gamelibrary *lib)
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
    UC::GameFilesData f;
    if (std::filesystem::is_directory(path))
    {
        f._Type = UC::GameFilesData::Type::Redirect;
        f._RedirectDir = path;
        GetGameFiles()->Init(lib, f);
    }
    else
    {
        UC::FileBuffer Buffer;
        if (UC::GameFilesData::ReadFileKeepOpen(path, Buffer, f))
        {
            UC::GameFiles *gamefiles = UC::GameFiles::Init(lib, f);
            gamefiles->SetFileBuffer(std::move(Buffer));
        }
    }
}

bool AppFiles::IsLoaded(texture tex)
{
    return _loadtextures.GetValue((texture_t)tex);
}
UC::Texture *AppFiles::GetTexture(texture tex)
{
    texture_t id = (texture_t)tex;
    if (id > TexturePaths_Size)
    {
        id = (texture_t)texture::Null;
    }
    if (_textures.HasValue(id))
    {
        return _textures.GetValue(id);
    }
    else
    {

        const String &Path = TexturePaths[id];

        Unique_ptr<UC::Texture> newtext;

        if (Path == nullTexPath)
        {
            newtext = std::make_unique<UC::Texture>(std::move(UC::Texture::MakeNewPlaceHolderTexture()));
        }
        else
        {
            newtext = std::make_unique<UC::Texture>(std::move(UC::Texture::MakeNewPlaceHolderTexture()));
            UC::GameFiles* gamefiles = UC::GameFiles::Get(_GameLib);

            UCodeGEAssert(std::filesystem::exists(AppFilesPathDir + Path));

            gamefiles->AsynReadGameFileFullBytes(Path)
                .ContinueOnAnyThread(
                    [id](Unique_Bytes&& Bytes)
                    {
                        return UC::AssetRendering::LoadTextureAsync(_GameLib, Bytes.AsView())
                            .ContinueOnMainThread(
                                [id](Unique_ptr<UC::Texture>&& Val)
                                {
                                    *_textures.GetValue(id) = std::move(*Val.release());
                                    _loadtextures.GetOrAdd(id, true);

                                    return std::move(Val);
                                }).Start();
                    }).Start();
        }

        auto Tex = newtext.get();

        _textures.AddValue(id,Tex);

        _LoadedTextures.push_back(std::move(newtext));
        return Tex;
    }
}

AsynTask_t<UC::Texture *> AppFiles::AsynGetTexture(texture tex)
{
    texture_t id = (texture_t)tex;
    if (id > TexturePaths_Size)
    {
        id = (texture_t)texture::Null;
    }
    if (_textures.HasValue(id))
    {
        AsynTask_t<UC::Texture *> R;
        R.SetValue(std::move(_textures.GetValue(id)));
        return R;
    }
    else
    {

        const String &Path = TexturePaths[id];

        Unique_ptr<UC::Texture> newtext;

        if (Path == nullTexPath)
        {
            auto Tex = std::make_unique<UC::Texture>(UC::Texture::MakeNewNullTexture());
            AsynTask_t<UC::Texture*> R;
            R.SetValue(Tex.get());
            _LoadedTextures.push_back(std::move(Tex));
            return R;
        }
        else
        {
            UCodeGEAssert(std::filesystem::exists(AppFilesPathDir + Path));

            UC::GameFiles* gamefiles = UC::GameFiles::Get(_GameLib);


            Delegate<UC::Texture*, Unique_ptr<UC::Texture>&&> Func2 = [id](Unique_ptr<UC::Texture>&& Val)
                {
                    auto RVal = Val.get();
                    _textures.GetValue(id) = Val.release();
                    _loadtextures.GetOrAdd(id,true);

                    if (id != (texture_t)texture::AppIcon)
                    {
                        RVal->FreeFromCPU();
                    }

                    return RVal;
                };
            auto Task = UC::AssetRendering::LoadTextureAsync(_GameLib, Path)
                .ContinueOnMainThread(std::move(Func2)).Start();


            Unique_ptr<UC::Texture> newtext;

            auto Tex = newtext.get();

            _textures.AddValue(id, Tex);
            _LoadedTextures.push_back(std::move(newtext));

            return Task;
        }
    }
}
UC::Sprite *AppFiles::GetSprite(sprite tex)
{
    Sprite_t id = (Sprite_t)tex;
    if (id > SpriteInfos_Size)
    {
        id = (Sprite_t)sprite::Null;
    }

    if (_sprites.HasValue(id))
    {
        const SpriteInfo &info = SpriteInfos[id];
        if (_loadtextures.GetOrAdd((texture_t)info.tex,false))
        {
            auto tex = GetTexture(info.tex);

            i32 _X, _Y, _SizeX, _SizeY;

            if (info.X == SetTodefault)
            {
                _X = 0;
            }
            else
            {
                _X = info.X;
            }

            if (info.Y == SetTodefault)
            {
                _Y = 0;
            }
            else
            {
                _Y = info.Y;
            }

            if (info.SizeX == SetTodefault)
            {
                _SizeX = tex->Get_Width();
            }
            else
            {
                _SizeX = info.SizeX;
            }

            if (info.SizeY == SetTodefault)
            {
                _SizeY = tex->Get_Height();
            }
            else
            {
                _SizeY = info.SizeY;
            }

            auto &R = _sprites.GetValue(id);
            R->Set_Xoffset(_X);
            R->Set_Yoffset(_Y);

            R->Set_Width(_SizeX);
            R->Set_Height(_SizeY);
            R->Set_texture(tex);

            return R;
        }
        else
        {
            return _sprites.GetValue(id);
        }
    }
    else
    {
        const SpriteInfo &info = SpriteInfos[id];

        auto tex = GetTexture(info.tex);
        i32 _X, _Y, _SizeX, _SizeY;

        if (info.X == SetTodefault)
        {
            _X = 0;
        }
        else
        {
            _X = info.X;
        }

        if (info.Y == SetTodefault)
        {
            _Y = 0;
        }
        else
        {
            _Y = info.Y;
        }

        if (info.SizeX == SetTodefault)
        {
            _SizeX = UC::Sprite::GetTexureSize;
        }
        else
        {
            _SizeX = info.SizeX;
        }

        if (info.SizeY == SetTodefault)
        {
            _SizeY = UC::Sprite::GetTexureSize;
        }
        else
        {
            _SizeY = info.SizeY;
        }

        Unique_ptr<UC::Sprite> newSpr = std::make_unique<UC::Sprite>(tex, _X, _Y, _SizeX, _SizeY);
        UC::Sprite *newSprite = newSpr.get();
        _sprites.AddValue(id,newSprite);

        _LoadedSprite.push_back(std::move(newSpr));
        return newSprite;
    }
}

String AppFiles::ReadFileAsString(const Path &path)
{
    return GetGameFiles()->ReadFileAsString(path);
}

Unique_Bytes AppFiles::ReadFileAsBytes(const Path &path)
{
    return GetGameFiles()->ReadGameFileAsBytes(path);
}

Unique_Bytes AppFiles::ReadFileAsBytes(const Path &path, size_t Offset, size_t Size)
{
    return GetGameFiles()->ReadFileAsBytes(path, Offset, Size);
}

AsynTask_t<Path> AppFiles::AsynGetFilePathByMove(const Path &path)
{
    return GetGameFiles()->AsynGetGameFilePathByMove(path);
}

Path AppFiles::GetFilePathByMove(const Path &path)
{
    return GetGameFiles()->GetGameFilePathByMove(path);
}

bool AppFiles::CopyFile(const Path& path, const Path& outpath)
{
    return GetGameFiles()->CopyGameFileTo(path,outpath);
}

AsynTask_t<String> AppFiles::AsynReadFileString(const Path &path)
{
    return GetGameFiles()->AsynReadFileString(path);
}

AsynTask_t<Unique_Bytes> AppFiles::AsynReadFileAsBytes(const Path &path)
{
    return GetGameFiles()->AsynReadGameFileFullBytes(path);
}

AsynTask_t<Unique_Bytes> AppFiles::AsynReadFileAsBytes(const Path &path, size_t Offset, size_t Size)
{
    return GetGameFiles()->AsynReadGameFileBytes(path, Offset, Size);
}

AppFiles::sprite AppFiles::GetSprite(FileHelper::FileType type)
{
    switch (type)
    {
    case UCodeEditor::FileHelper::FileType::Null:
        return AppFiles::sprite::File_image;
    case UCodeEditor::FileHelper::FileType::File:
        return AppFiles::sprite::File_image;
    case UCodeEditor::FileHelper::FileType::Dir:
        return AppFiles::sprite::Dir_folder_image;
    case UCodeEditor::FileHelper::FileType::File_Image_Type:
        return AppFiles::sprite::File_image;
    case UCodeEditor::FileHelper::FileType::SceneData:
        return AppFiles::sprite::Scene2dData;
    case UCodeEditor::FileHelper::FileType::RawEntityData:
        return AppFiles::sprite::RawEntityData;
    case UCodeEditor::FileHelper::FileType::ULangFile:
        return AppFiles::sprite::ULangScript;
    case UCodeEditor::FileHelper::FileType::ULangLib:
        return AppFiles::sprite::ULangLib;
    case UCodeEditor::FileHelper::FileType::UPugin:
        return AppFiles::sprite::UPugin;
    case UCodeEditor::FileHelper::FileType::Dll_File:
        return AppFiles::sprite::DLL_image;
    case UCodeEditor::FileHelper::FileType::metaFile:
        return AppFiles::sprite::File_image;
    case UCodeEditor::FileHelper::FileType::Color:
        return AppFiles::sprite::File_image;
    case UCodeEditor::FileHelper::FileType::TxtFile:
        return AppFiles::sprite::TxtFile;
    case UCodeEditor::FileHelper::FileType::glslFile:
        return AppFiles::sprite::GlslFile;
    default:
        return AppFiles::sprite::File_image;
    }
}

EditorEnd