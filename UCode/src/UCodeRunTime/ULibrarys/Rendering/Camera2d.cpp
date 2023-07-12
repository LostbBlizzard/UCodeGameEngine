#include "Camera2d.hpp"
#include <glm/ext/matrix_clip_space.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <cmath>

#include <UCodeRunTime/RunTimeBasicTypes/Vector.hpp>
#include "RenderRunTime2d.hpp"
RenderingStart


UComponentsID Camera2d::Get_TypeID()
{
	return "Camera";
}
UComponentData Camera2d::type_Data = { Get_TypeID(),[](Entity* E) {return (Compoent*)new Camera2d(E); } };

Vector<Camera2d*> Cameras = Vector<Camera2d*>();//Not thread safe
//Move This to RenderRunTime2d;
Camera2d::Camera2d(Entity* entity) : Compoent(entity,&type_Data)
	,ProjectionMatrix(1.0f)
	,ViewMatrix(1.0f)
	,ViewProjectionMatrix(1.0f)
	,_Buffer(FrameBufferData())
{
	SetAsMainCam();
	Cameras.push_back(this);
}

Camera2d::~Camera2d()
{
	for (auto it = Cameras.begin(); it != Cameras.end();)
	{
		auto Item = *it;



		if (Item == this)
		{
			it = Cameras.erase(it);
			break;
		}
		++it;
	}
	UnSetAsMainCam();
	
}

void Camera2d::Serialize(USerializer& Serializer) const
{
	Serializer.Write("Type", (Camera2d::CamType_t)Get_CamType());
	Serializer.Write("ortho_size", Get_Ortho_size());
	Serializer.Write("Fov", Get_Fov());
}

void Camera2d::Deserialize(UDeserializer& Serializer)
{
	Camera2d::CamType_t& CamT = *(Camera2d::CamType_t*)&Type;
	Serializer.ReadType("Type", CamT, CamT);
	Serializer.ReadType("ortho_size", ortho_size, ortho_size);
	Serializer.ReadType("Fov", Fov, Fov);
}

void Camera2d::UpdateCamData()
{
	if (Type == CamType::Orthographic) 
	{
		float32 _AspectRatio = Get_windowAspect();
		float32 _ortho_size = ortho_size;

		float32 ProjW = _AspectRatio * ortho_size;

		ProjectionMatrix = glm::ortho(-ProjW, ProjW, -ortho_size, ortho_size);
	}
	else
	{
		float32 _AspectRatio = Get_windowAspect();
		float32 _ortho_size = ortho_size;

		float32 ProjW = _AspectRatio * ortho_size;

		float nearF = 0.1f;
		ProjectionMatrix = glm::perspective(glm::radians(Fov), Get_windowAspect(), nearF, 10.0f);
	}


	const auto MyPOs = GetMyEntity()->Get_WorldPosition();
	glm::mat4 transform = glm::translate(glm::mat4(1.0f), glm::vec3(MyPOs.X, MyPOs.Y, 0));

	ViewMatrix = glm::inverse(transform);

	ViewProjectionMatrix = ProjectionMatrix * ViewMatrix;

	if (Width != _Buffer.Get_Width() || Height != _Buffer.Get_Height())
	{
		_Buffer.UpdateBufferSize(Width, Height);
	}
}

Camera2d* Camera2d::FindMainCam(const GameRunTime* e)
{
	return (Camera2d*)e->Get_StaticComponent((size_t)StaticRuntimeComponentsIndex::Camera2d);
}


void Camera2d::SetAsMainCam()
{
	GetGameRunTime()->SetStaticComponent((size_t)StaticRuntimeComponentsIndex::Camera2d, this);
}

void Camera2d::UnSetAsMainCam()
{
	if (GetGameRunTime()->Get_StaticComponent((size_t)StaticRuntimeComponentsIndex::Camera2d) == this)
	{
		GetGameRunTime()->ReMoveStaticComponent((size_t)StaticRuntimeComponentsIndex::Camera2d);


		for (auto Item : Cameras)
		{
			if (Item->GetGameRunTime() == GetGameRunTime())
			{
				GetGameRunTime()->SetStaticComponent((size_t)StaticRuntimeComponentsIndex::Camera2d, Item);
				break;
			}
		}
	}
}

Bounds2d Camera2d::GetCam_Bounds() const
{
	float32 screenAspect = Get_windowAspect();
	float32 cameraHeight = ortho_size * 2.0f;

	Vec2 CamPos = GetMyEntity()->Get_WorldPosition2D();
	Vec2 CamSize = { cameraHeight * screenAspect, cameraHeight };
	
	
	return { CamPos,CamSize };
}

Vec2 Camera2d::ScreenToWorldPoint(const Camera2d* cam, const Vec2 CursorPosition)
{
	if (cam) //add Code
	{
		const RenderRunTime2d* r = RenderRunTime2d::GetRenderRunTime(cam->GetGameRunTime());
		
		const Bounds2d cambounds = cam->GetCam_Bounds();

		const Vec2 WInSize = { (float32)cam->Get_WindowWidth(),(float32)cam->Get_WindowHeight() };
		const Vec2 Ncords = { (CursorPosition.X / WInSize.X) - 0.5f ,(CursorPosition.Y / WInSize.Y) - 0.5f };


		const float32 cameraWidth = cambounds.extents.X;
		const float32 cameraHeight = cambounds.extents.Y;
		const float32 x = cambounds.center.X + (cameraWidth * Ncords.X);
		const float32 y = cambounds.center.Y + (cameraHeight * Ncords.Y);

		return Vec2(x, y);
	}

	return Vec2();
}



RenderingEnd
