#include "CoreModule.hpp"
#include "../ModuleNames.hpp"


#include "Assets/Core/EntityAssetFile.hpp"
#include "Assets/Art/PNGAssetFile.hpp"
#include "Assets/Audio/MP3AssetFile.hpp"
#include "Assets/Core/SceneAssetFile.hpp"
#include "Assets/Tilemap/TilePalette.hpp"

#include "Componets/Camera.hpp"
#include "Componets/SpriteRenderer.hpp"
#include "Componets/TileMapRenderer.hpp"
EditorStart

CoreModule::CoreModule()
{
	Init();
}

CoreModule::~CoreModule()
{

}

void CoreModule::Init()
{
	ID = UCODEEDITOR_MODULENAME_CORE;

	{
		Assets[0] = Unique_ptr<UEditorAssetFileData>(new ScencAssetFile());
		Assets[1] = std::move(Unique_ptr<UEditorAssetFileData>(new PNGAssetFile()));
		Assets[2] = std::move(Unique_ptr<UEditorAssetFileData>(new EntityAssetFile()));
		Assets[3] = std::move(Unique_ptr<UEditorAssetFileData>(new MP3AssetFile()));
		Assets[4] = std::move(Unique_ptr<UEditorAssetFileData>(new TilePaletteAssetFile()));
	}

	{
		Components[0] = std::move(Unique_ptr<UEditorComponentData>(new SpriteRendererUEditorData()));
		Components[1] = std::move(Unique_ptr<UEditorComponentData>(new CameraUEditorData()));
		Components[2] = std::move(Unique_ptr<UEditorComponentData>(new TileMapRenderUEditorData()));
	}
}
EditorEnd