#pragma once
#include "UCodeRunTime/Core/Entity.hpp"
#include <EditorWindows/BasicWindows/InspectWindow.hpp>
#include "Helper/AppFiles.hpp"
#include "UCodeLang/LangCore/ReflectionData.hpp"
EditorStart
class Inspect_Entity2d
{
public:
   
    static InspectWindow::InspectData Get(UCode::EntityPtr Item);
    static InspectWindow::InspectData Get(UCode::Entity* Item);
    static void Insp_(InspectWindow::InspectDrawer& Draw);
    
};
class Inspect_Compoent2d
{
public:
    static void DrawCompoentButtion(UCode::Compoent* Item, EditorAppCompoent* App);

    static constexpr const char* unknownCompoentName = "[Compoent type is unknown]";
    static const char* Get_CompoentName(UCode::Compoent* Item);
    static const char* Get_CompoentName(const UCode::UComponentsID& Type);

    static AppFiles::sprite GetCompoentSprite(UCode::UComponentsID Type);
    static InspectWindow::InspectData Get(UCode::Compoent* Item);
    static void Insp_(InspectWindow::InspectDrawer& Draw);
    static void Insp_(UCode::Compoent* item, InspectWindow::InspectDrawer& Draw);
    static bool ShowAddCompoenList(UCode::Entity* item);
};
EditorEnd


