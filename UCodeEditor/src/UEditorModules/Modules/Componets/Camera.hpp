#pragma once
#include "../CoreModule.hpp"
EditorStart

static const ImGuIHelper::EnumValue<UCode::Camera2d::CamType> CamTypes[] =
{
	{"Orthographic",UCode::Camera2d::CamType::Orthographic},
	{"Perspective",UCode::Camera2d::CamType::Perspective},
};
constexpr size_t CamTypesCount = sizeof(CamTypes) / sizeof(CamTypes[0]);
class CameraUEditorData : public UEditorComponentData
{
public:
	CameraUEditorData();


	void DrawAsTree(const UEditorComponentDrawTreeData& Data, UCode::Compoent* Value) const override;

	void DrawInspect(const UEditorComponentDrawData& Data, UCode::Compoent* Value) const override;
};

EditorEnd
