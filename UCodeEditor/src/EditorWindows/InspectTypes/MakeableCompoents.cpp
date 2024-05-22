#include "MakeableCompoents.hpp"
#include "UCodeRunTime/ULibrarys/UCodeLang/ULangRunTime.hpp"
#include "UCodeRunTime/ULibrarys/Rendering_Library.hpp"
#include "UCodeRunTime/ULibrarys/Physics_Library.hpp"
#include "UCodeRunTime/ULibrarys/UI_Library.hpp"
EditorStart

#define _NewMenu(Name) { Name,  MakeableCompoents::NewMenuName,nullptr },
#define _EndMenu() {"",  MakeableCompoents::EndMenuName,nullptr },

MakeableCompoents::Lib Data[] =
{


_NewMenu("Rendering")

{"SpriteRenderer", UCode::SpriteRenderer::Get_TypeID(),&MakeableCompoents::Make_Compoent<UCode::SpriteRenderer>},
{"TileMap", UCode::TileMapRenderer::Get_TypeID(),&MakeableCompoents::Make_Compoent<UCode::TileMapRenderer>},
{"Camera",UCode::Camera2d::Get_TypeID(), &MakeableCompoents::Make_Compoent<UCode::Camera2d>},


_EndMenu()

_NewMenu("Physics")

//_NewMenu("2D")
{"Rigidbody2d", UCode::ULangScript::Get_TypeID(),& MakeableCompoents::Make_Compoent<UCode::Rigidbody2d>},
{"BoxCollider2d", UCode::ULangScript::Get_TypeID(),&MakeableCompoents::Make_Compoent<UCode::BoxCollider2d>},
//_EndMenu()

_EndMenu()

_NewMenu("UI")

{"Image", UCode::UIImage::Get_TypeID(),& MakeableCompoents::Make_Compoent<UCode::UIImage>},
{"Buttion", UCode::Buttion::Get_TypeID(),&MakeableCompoents::Make_Compoent<UCode::Buttion>},
_EndMenu()

_NewMenu(CustomMenuName)

{"Script", UCode::ULangScript::Get_TypeID(),& MakeableCompoents::Make_Compoent<UCode::ULangScript> },

_EndMenu()

};

const MakeableCompoents::Lib* MakeableCompoents::Compoents = Data;
const size_t MakeableCompoents::Compoents_Size = sizeof(Data) / sizeof(MakeableCompoents::Lib);

EditorEnd