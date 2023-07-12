#pragma once
#include <UCodeRunTime/includeULibrayCompoent.hpp>
#include <UCodeRunTime/RunTimeBasicTypes/mat4.hpp>
#include <UCodeRunTime/RunTimeBasicTypes/Bounds.hpp>
#include "RenderingNamespace.hpp"
#include "GpuTypes/FrameBuffer.hpp"
RenderingStart

class CoreModule;
class Camera2d final :public Compoent
{
	friend CoreModule;
public:
	typedef UInt8 CamType_t;
	enum class CamType : CamType_t
	{
		Orthographic,
		Perspective,
	};
	Camera2d(Entity* entity);
	~Camera2d();

	

	void Serialize(USerializer& Serializer)  const override;
	void Deserialize(UDeserializer& Serializer) override;


	static Camera2d* FindMainCam(const GameRunTime* runtime);
	

	UCODE_ENGINE_FORCE_INLINE mat4 Get_ProjectionMatrix() const
	{
		return ProjectionMatrix;
	}
	UCODE_ENGINE_FORCE_INLINE mat4 Get_ViewMatrix() const
	{
		return ViewMatrix;
	}
	UCODE_ENGINE_FORCE_INLINE mat4 Get_ViewProjectionMatrix() const
	{
		return ViewProjectionMatrix;
	}

	UCODE_ENGINE_FORCE_INLINE const mat4 Get_ProjectionMatrixRead() const
	{
		return ProjectionMatrix;
	}
	UCODE_ENGINE_FORCE_INLINE const mat4 Get_ViewMatrixRead() const
	{
		return ViewMatrix;
	}
	UCODE_ENGINE_FORCE_INLINE const mat4 Get_ViewProjectionMatrixRead() const
	{
		return ViewProjectionMatrix;
	}

	void SetAsMainCam();
	void UnSetAsMainCam();
	Bounds2d GetCam_Bounds() const;
	static Vec2 ScreenToWorldPoint(const Camera2d* cam, const Vec2 CursorPosition);

	void UpdateCamData();

	UCODE_ENGINE_FORCE_INLINE FrameBuffer& Get_Buffer()
	{
		return _Buffer;
	}
	UCODE_ENGINE_FORCE_INLINE float32 Get_windowAspect() const
	{
		return (float32)Width / (float32)Height;
	}
	UCODE_ENGINE_FORCE_INLINE void API_Set_WindowSize(SInt32 X, SInt32 Y)
	{
		Width = X;
		Height = Y;
	}
	UCODE_ENGINE_FORCE_INLINE SInt32 Get_WindowWidth() const
	{
		return Width;
	}
	UCODE_ENGINE_FORCE_INLINE SInt32 Get_WindowHeight()const
	{
		return Height;
	}
	UCODE_ENGINE_FORCE_INLINE CamType Get_CamType()const
	{
		return Type;
	}
	UCODE_ENGINE_FORCE_INLINE void Set_CamType(CamType Value)
	{
		Type = Value;
	}
	UCODE_ENGINE_FORCE_INLINE float32 Get_Ortho_size() const { return ortho_size; }
	UCODE_ENGINE_FORCE_INLINE void Set_Ortho_size(float32 Value) { ortho_size = Value; }

	UCODE_ENGINE_FORCE_INLINE float32 Get_Fov() const { return Fov; }
	UCODE_ENGINE_FORCE_INLINE void Set_Fov(float32 Value) { Fov = Value; }


	static UComponentsID Get_TypeID();
private:
	mat4 ProjectionMatrix;
	mat4 ViewMatrix;
	mat4 ViewProjectionMatrix;
	FrameBuffer _Buffer;
	SInt32 Width =-1, Height =-1;
	
	struct
	{
		float32 ortho_size = 5;
		float32 Fov = 90;
	};
	CamType Type = CamType::Orthographic;
	
	static UComponentData type_Data;
};
RenderingEnd

