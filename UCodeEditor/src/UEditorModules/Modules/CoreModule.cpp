#include "CoreModule.hpp"
#include "../ModuleNames.hpp"


#include "Assets/Core/EntityAssetFile.hpp"
#include "Assets/Art/PNGAssetFile.hpp"
#include "Assets/Audio/MP3AssetFile.hpp"
#include "Assets/Core/SceneAssetFile.hpp"
#include "Assets/Tilemap/TilePalette.hpp"
#include "Assets/Tilemap/Tile.hpp"
#include "Assets/Tilemap/TilePack.hpp"

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
		Assets.push_back(Unique_ptr<UEditorAssetFileData>(new ScencAssetFile()));
		Assets.push_back(Unique_ptr<UEditorAssetFileData>(new PNGAssetFile()));
		Assets.push_back(Unique_ptr<UEditorAssetFileData>(new EntityAssetFile()));
		Assets.push_back(Unique_ptr<UEditorAssetFileData>(new MP3AssetFile()));
		Assets.push_back(Unique_ptr<UEditorAssetFileData>(new TilePaletteAssetFile()));
		Assets.push_back(Unique_ptr<UEditorAssetFileData>(new TileAssetFile()));
		Assets.push_back(Unique_ptr<UEditorAssetFileData>(new TilePackAssetFile()));
	}

	{
		Components.push_back(Unique_ptr<UEditorComponentData>(new SpriteRendererUEditorData()));
		Components.push_back(Unique_ptr<UEditorComponentData>(new CameraUEditorData()));
		Components.push_back(Unique_ptr<UEditorComponentData>(new TileMapRenderUEditorData()));
	}
}
EditorEnd