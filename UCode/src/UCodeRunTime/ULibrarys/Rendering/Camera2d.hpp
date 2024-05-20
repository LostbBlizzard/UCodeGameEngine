#pragma once
#include <UCodeRunTime/includeULibrayCompoent.hpp>
#include <UCodeRunTime/BasicTypes.hpp>
#include "RenderingNamespace.hpp"
#include "GpuTypes/FrameBuffer.hpp"
RenderingStart

class CoreModule;
class Camera2d final :public Compoent
{
	friend CoreModule;
public:
	typedef u8 CamType_t;
	enum class CamType : CamType_t
	{
		Orthographic,
		Perspective,
	};
	Camera2d(Entity* entity);
	~Camera2d();



	void Serialize(USerializer& Serializer)  const override;
	void Deserialize(UDeserializer& Serializer) override;
	rttr::instance Get_Rttr_Instance() override { return rttr::instance(this); }

	static Camera2d* FindMainCam(const GameRunTime* runtime);


	UCodeGEForceinlne Mat4 Get_ProjectionMatrix() const
	{
		return ProjectionMatrix;
	}
	UCodeGEForceinlne Mat4 Get_ViewMatrix() const
	{
		return ViewMatrix;
	}
	UCodeGEForceinlne Mat4 Get_ViewProjectionMatrix() const
	{
		return ViewProjectionMatrix;
	}

	UCodeGEForceinlne const Mat4 Get_ProjectionMatrixRead() const
	{
		return ProjectionMatrix;
	}
	UCodeGEForceinlne const Mat4 Get_ViewMatrixRead() const
	{
		return ViewMatrix;
	}
	UCodeGEForceinlne const Mat4 Get_ViewProjectionMatrixRead() const
	{
		return ViewProjectionMatrix;
	}

	void SetAsMainCam();
	void UnSetAsMainCam();
	Bounds2d GetCam_Bounds() const;
	static Vec2 ScreenToWorldPoint(const Camera2d* cam, const Vec2 CursorPosition);

	void UpdateCamData();

	UCodeGEForceinlne FrameBuffer& Get_Buffer()
	{
		return _Buffer;
	}
	UCodeGEForceinlne f32 Get_windowAspect() const
	{
		return (f32)Width / (f32)Height;
	}
	UCodeGEForceinlne void API_Set_WindowSize(i32 X, i32 Y)
	{
		Width = X;
		Height = Y;
	}
	UCodeGEForceinlne i32 Get_WindowWidth() const
	{
		return Width;
	}
	UCodeGEForceinlne i32 Get_WindowHeight()const
	{
		return Height;
	}
	UCodeGEForceinlne CamType Get_CamType()const
	{
		return Type;
	}
	UCodeGEForceinlne void Set_CamType(CamType Value)
	{
		Type = Value;
	}
	UCodeGEForceinlne f32 Get_Ortho_size() const { return ortho_size; }
	UCodeGEForceinlne void Set_Ortho_size(f32 Value) { ortho_size = Value; }

	UCodeGEForceinlne f32 Get_Fov() const { return Fov; }
	UCodeGEForceinlne void Set_Fov(f32 Value) { Fov = Value; }

	UCodeGEForceinlne Color Get_BackRoundClearColor() const { return BackRoundClearColor; }
	UCodeGEForceinlne void Set_BackRoundClearColor(Color Value) { BackRoundClearColor = Value; }

	static UComponentsID Get_TypeID();
private:
	Mat4 ProjectionMatrix;
	Mat4 ViewMatrix;
	Mat4 ViewProjectionMatrix;
	FrameBuffer _Buffer;
	i32 Width = -1, Height = -1;
	Color BackRoundClearColor = { 0.1f,0.1f,0.1f,1.0f };
	struct
	{
		f32 ortho_size = 5;
		f32 Fov = 90;
	};
	CamType Type = CamType::Orthographic;

	static UComponentData type_Data;
};
RenderingEnd

