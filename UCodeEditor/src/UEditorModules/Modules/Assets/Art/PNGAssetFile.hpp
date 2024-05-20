#pragma once
#include "UEditorModules/Modules/CoreModule.hpp" 
#include "UCodeRunTime/ULibrarys/Serialization/SerlizeableType.hpp"

EditorStart
struct SpriteItem
{
	UID uid;
	String spritename;

	Vec2i_t<u32> offset;
	Vec2i_t<u32> size;

	std::shared_ptr<UCode::SpriteAsset> _Asset;
};
EditorEnd

MakeSerlizeType(UCodeEditor::SpriteItem,
	Field("_UID", _this->uid);
Field("_SpriteName", _this->spritename);
Field("offset", _this->offset);
Field("size", _this->size);
)


EditorStart
class PNGAssetFile :public UEditorAssetFileData
{
public:
	PNGAssetFile();
	using Compression_t = int;
	enum class Compression :Compression_t
	{
		None,
		LowQuality,
		NormalQuality,
		HighQuality,
	};

	using Filter_t = UCode::Texture::Filter_t;
	using Filter = UCode::Texture::Filiter;
	struct TextureSettings
	{
		UID uid;
		bool ReadAndWrite = false;
		Compression compression = Compression::None;
		Filter filter = Filter::Point;
		u32 pixelperunit = 16;

		Vector<SpriteItem> sprites;
	};
	class LiveingPng :public UEditorAssetFile
	{
	public:

		LiveingPng();

		TextureSettings setting;
		Optional<UCode::TextureAsset> asset;
		bool IsLoadingTexture = false;

		bool isbroken = false;

		inline static const ImGuIHelper::EnumValue<Compression> CompressionEnumValues[] =
		{
			{"None",Compression::None},
			{"LowQuality",Compression::LowQuality},
			{"NormalQuality",Compression::NormalQuality},
			{"HighQuality",Compression::HighQuality},
		};
		static constexpr size_t CompressionEnumValuesSize = sizeof(CompressionEnumValues) / sizeof(CompressionEnumValues[0]);


		inline static const ImGuIHelper::EnumValue<Filter> FilterEnumValues[] =
		{
			{"Point",Filter::Point},
			{"Bilinear",Filter::bilinear},
			{"Trilinear",Filter::trilinear},
		};
		static constexpr size_t FilterEnumValuesSize = sizeof(FilterEnumValues) / sizeof(FilterEnumValues[0]);

		void LoadAssetIfNeeded(SpriteItem& spr);

		Optional<UID> OpenSpriteEditor;
		void DrawSubAssets(const UEditorDrawSubAssetContext& Item) override;
		void Init(const UEditorAssetFileInitContext& Context) override;
		void SaveFile(const UEditorAssetFileSaveFileContext& Context) override;

		static  bool fromfile(const Path& settings, TextureSettings& Out);
		static  bool tofile(const Path& settings, const TextureSettings& in);
		void RemoveSubAssets(const Path& AssetDir, EditorIndex& index);


		bool DrawButtion(const UEditorAssetDrawButtionContext& Item) override;
		void RemoveSprite(UID id);
		void SetupTexture(UCode::Texture* tex);
		NullablePtr<UC::Asset> LoadAsset(const LoadAssetContext& Item)  override;

		bool ShouldBeUnloaded(const UEditorAssetShouldUnloadContext& Context) override;
		void DrawInspect(const UEditorAssetDrawInspectContext& Item) override;

	};

	Unique_ptr<UEditorAssetFile> GetMakeNewAssetFile() override
	{
		return Unique_ptr< UEditorAssetFile>(new LiveingPng());
	}
	ExportFileRet ExportFile(const Path& path, const ExportFileContext& Item) override;


	Optional<GetUIDInfo> GetFileUID(UEditorGetUIDContext& context) override;
};

EditorEnd
